#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <functional>
#include <ctime>
#include <chrono>

#include "scenes.h"
#include "parallel.h"
#include "Parser.h"

void threaded_render(int idx);
std::string render_v2();
Vector3f color2(const Ray& r, Hitable *world, Hitable *lightShape, int &depth);
void parseSceneFile(const std::vector<std::string> &,
                    std::vector<Triangle>&,
                    std::vector<Light> &,
                    Camera**);


Vector3f *output_img;
float *depths;
int *workedon;
int nx;
int ny;
int ns;
Camera* render_cam;
Hitable *world;
HitableList hlist;

int main(int argc, char **argv) {

    std::string filename(argv[argc - 1]);
    std::ifstream infile;
    infile.open(filename);
    std::stringstream buffer;
    buffer << infile.rdbuf();
    infile.close();
    nx = 500;
    ny = 500;
    ns = 100;

    Camera *c;
    Hitable *w;
    Hitable *l;

    //parse_pbrt_scene(buffer.str(), &l, &c, &w);


    std::vector<std::string> lines;
    std::string line;


    output_img = new Vector3f[nx*ny];
    depths = new float[nx*ny];
    workedon = new int[12];
    for (int i = 0; i < 12; ++i){
        workedon[i] = 0;
    }
    while(std::getline(infile, line)){
        lines.push_back(line);
    }
    infile.close();
    std::vector<Triangle> geoList;
    std::vector<Light> lightList;
    Camera *cam;
    parseSceneFile(lines, geoList, lightList, &cam);

    std::string ppmImg;
    auto startTime = std::chrono::high_resolution_clock::now();
    ppmImg = render_v2();
    auto endTime = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    int mint = int(dur) / 60;
    int sect = int(dur) % 60;
    std::cout << "time: " << mint << "m " << sect << "s" << std::endl;
    std::ofstream outfile(filename);
    std::cout << "writing file" << std::endl;
    outfile << ppmImg << std::endl;
    std::cout << "done writing" << std::endl;
    outfile.close();
    for (int i = 0; i < 12; ++i){
        std::cout << "thread " << i << " worked on " << workedon[i] << " chunks" << std::endl;
    }
    float sum = 0;
    for (int i = 0; i < nx*ny; ++i){
        sum += depths[i];
    }
    std::cout << "avg bounce depth: " << sum / (nx*ny) << std::endl;
    std::cout << "stddev: " << calculateSD(depths, nx*ny) << std::endl;
    return 0;
}

Vector3f reflect(const Vector3f& v, const Vector3f& n){
    return v - 2*dot(v,n)*n;
}

bool refract(const Vector3f& v, const Vector3f& n, float niOverNt, Vector3f& refracted){
    Vector3f uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0f - niOverNt*niOverNt*(1-dt*dt);
    if (discriminant > 0){
        refracted = niOverNt*(uv - n*dt)- n*sqrt(discriminant);
        return true;
    }
    return false;
}

float schlick(float cosine, float refIdx){
    float r0 = (1-refIdx) / (1+refIdx);
    r0 = r0*r0;
    return r0 + (1-r0)*powf((1 - cosine), 5);
}

Vector3f randomInUnitDisk(){

    Vector3f p;
    do {
        float r1 = gen.UniformFloat(0,1);
        float r2 = gen.UniformFloat(0,1);
        p = 2.0*Vector3f(r1,r2,0) - Vector3f(1,1,0);
    } while (dot(p,p) >= 1.0);

    return p;
}

Vector3f randomInUnitSphere(){

    Vector3f p;

    do {
        float r1 = gen.UniformFloat(0,1);
        float r2 = gen.UniformFloat(0,1);
        float r3 = gen.UniformFloat(0,1);
        p = 2.0f * Vector3f(r1, r2, r3) - Vector3f(1.0, 1.0, 1.0);
    } while (dot(p,p) >= 1.0f);

    return unit_vector(p);
}

Vector3f color(const Ray& r, Hitable *world, Hitable *lightShape, int depth) {
    HitRecord rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ScatterRecord srec;
        Vector3f emitted = rec.matPtr->emitted(r, rec, rec.u, rec.v, rec.p);

        if (depth >= 50){
            return emitted;
        }

        if (rec.matPtr->scatter(r, rec, srec)){
            if (srec.isSpecular){
                return srec.attenuation * color(srec.SpecularRay, world, lightShape, depth+1);
            }else{
                HitablePDF p0(*lightShape, rec.p);
                MixturePDF p(p0, srec.cospdf);
                Ray scattered = Ray(rec.p, p.generate(), r.time);
                float pdf_val = p.value(scattered.direction());
                float spdf = rec.matPtr->scattering_pdf(r, rec, scattered);
                if (pdf_val == 0 && spdf == 0){
                    return emitted;
                }
                Vector3f tcolor = color(scattered, world, lightShape, depth+1);
                return emitted + srec.attenuation*spdf*tcolor / pdf_val;
            }
        }
        else
            return emitted;
    }
    else
        return Vector3f(0,0,0);
}

Vector3f color2(const Ray& r, Hitable *world, Hitable *lightShape, int &depth) {


    Vector3f incoming_color(0,0,0);
    Ray iter_ray = r;
    int ptr = 0;
    float threshhold = 0.25f;
    int threshold_depth = 2;
    Vector3f cumulative_val(std::nextafter(1.0f, 0.0f),std::nextafter(1.0f, 0.0f),std::nextafter(1.0f, 0.0f));
    bool once = true;
    while (depth < 50){
        ++depth;

        HitRecord rec;
        if (!world->hit(iter_ray, 0.001, MAXFLOAT, rec)){
            break;
        }

        ScatterRecord srec;
        Vector3f emitted = rec.matPtr->emitted(iter_ray, rec, rec.u, rec.v, rec.p);
        if (!(emitted ==  Vector3f(0,0,0))){
            incoming_color += emitted * cumulative_val;
        }
        if(!rec.matPtr->scatter(iter_ray, rec, srec)){
            //incoming_color = emitted;
            break;
        }


        if(srec.isSpecular){
            cumulative_val *= srec.attenuation;
            iter_ray = srec.SpecularRay;
            if (once){
                threshold_depth = depth + 4;
                once = false;
            }
//            float prob = std::max(cumulative_val.x(), std::max(cumulative_val.y(), cumulative_val.z()));

//            if (depth > threshold_depth && gen.UniformFloat(0,1) < (1-prob)* threshhold ){
//                break;
//            }
//          cumulative_val *= 1 / (1-prob);
        }else{
            HitablePDF p0(*lightShape, rec.p);
            MixturePDF p(p0, srec.cospdf);
            Ray scattered = Ray(rec.p, p.generate(), iter_ray.time);
            float pdf_val = p.value(scattered.direction());


            float spdf = rec.matPtr->scattering_pdf(iter_ray, rec, scattered);
            if (pdf_val == 0 && spdf == 0){
                pdf_val = 1;
                spdf = 1;
            }

            cumulative_val *= srec.attenuation*spdf / pdf_val;
            float max_val = std::max(cumulative_val.x(), std::max(cumulative_val.y(), cumulative_val.z()));
            if (max_val < threshhold){
                float prob = std::max(0.05f, (1-max_val));
                if (depth > threshold_depth && gen.UniformFloat(0,1) < prob){
                    break;
                }

                if ( depth > threshold_depth)
                    cumulative_val /= prob;
            }
            iter_ray = scattered;
        }
    }

    if (incoming_color == Vector3f(0,0,0)){
        return Vector3f(0,0,0);
    }

//    for(int i = ptr-1; i >= 0; --i){
//        emitted_val = incoming_col[i] * emitted_val;
//    }

    return incoming_color;
}

std::string render(){

    std::stringstream ss;
    ss << "P3\n" << nx << " " << ny << "\n255\n";
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    cornell_box_v2(&world, &render_cam, float(nx)/float(ny));

    Hitable* lightShape = new XZRectangle(213, 343, 227, 332, 554, nullptr);
    Hitable* glassSphere = new Sphere(Vector3f(190, 90, 190), 90, nullptr);
    Hitable *a[2];
    a[0] = lightShape;
    a[1] = glassSphere;
    hlist = HitableList(a,2);

    srand48(10);
    for (int j = ny - 1, count = 0; j >= 0; j--, count++) {
        std::cout << (count / float(ny))*100 << "%" << std::endl;
        for (int i = 0; i < nx; i++) {

            Vector3f col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float r1 = gen.UniformFloat(0, 1);
                float r2 = gen.UniformFloat(0, 1);
                float u = (i + r1) / float(nx);
                float v = (j + r2) / float(ny);
                Ray r = render_cam->getRay(u, v);
                int depth = 0;
                Vector3f tcol = deNan(color2(r, world, &hlist, depth));
                col += tcol;
            }
            col /= float(ns);
            col = Vector3f(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            ss << ir << " " << ig << " " << ib << "\n";
        }
    }
    return ss.str();
}

std::string render_v2(){

    std::stringstream ss;
    ss << "P3\n" << nx << " " << ny << "\n255\n";

    cornell_box_v2(&world, &render_cam, float(nx)/float(ny));

    Hitable* lightShape = new XZRectangle(213, 343, 227, 332, 554, nullptr);
    Hitable* glassSphere = new Sphere(Vector3f(190, 90, 190), 90, nullptr);
    Hitable *a[2];
    a[0] = lightShape;
    a[1] = glassSphere;
    hlist = HitableList(a,2);

    int chunkSize =  ((nx * ny) / 11) / 8;
    ParallelInit();
    ParallelFor(threaded_render, nx*ny, chunkSize);
    ParallelCleanup();
    for(int i = 0; i < nx*ny; ++i){
        Vector3f rgbv = output_img[i];
        int ir = int(255.99 * rgbv[0]);
        int ig = int(255.99 * rgbv[1]);
        int ib = int(255.99 * rgbv[2]);
        ss << ir << " " << ig << " " << ib << "\n";
    }
    return ss.str();
}
void threaded_render(int idx){
    int row = idx / ny;
    row = (ny-1) - row;
    int col = idx % ny;

    Vector3f rgbv(0, 0, 0);

    float total_depth = 0;
    for (int s = 0; s < ns; s++) {
        float r1 = gen.UniformFloat(0,1);
        float r2 = gen.UniformFloat(0,1);
        float u = (col + r1) / float(nx);
        float v = (row + r2) / float(ny);
        Ray r = render_cam->getRay(u, v);
        int depth = 0;
        Vector3f tcol = deNan(color2(r, world, &hlist, depth));
        total_depth += depth;
        //Vector3f tcol = deNan(color(r, world, &hlist, 0));
        rgbv += tcol;
    }
    depths[idx] =  total_depth / ns;
    rgbv /= float(ns);
    rgbv = Vector3f(sqrt(rgbv[0]), sqrt(rgbv[1]), sqrt(rgbv[2]));
    output_img[idx] = rgbv;
}


float calculateSD(float data[], int num)
{
    float sum = 0.0, mean, standardDeviation = 0.0;

    int i;

    for(i = 0; i < num; ++i)
    {
        sum += data[i];
    }

    mean = sum/num;
    for(i = 0; i < num; ++i)
        standardDeviation += pow(data[i] - mean, 2);

    return sqrt(standardDeviation / num);
}
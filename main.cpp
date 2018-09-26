#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <functional>
#include <ctime>
#include <chrono>

#include "scenes.h"
#include "parallel.h"

void threaded_render(int idx);
std::string render_v2();

void parseSceneFile(const std::vector<std::string> &,
                    std::vector<Triangle>&,
                    std::vector<Light> &,
                    Camera**);


Vector3f *output_img;
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
    std::vector<std::string> lines;
    std::string line;

    nx = 500;
    ny = 500;
    ns = 500;

    output_img = new Vector3f[nx*ny];
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
    //ppmImg = render();
    ppmImg = render_v2();
    auto endTime = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    int mint = int(dur) / 60;
    int sect = int(dur) % 60;
    std::cout << "time: " << mint << "m " << sect << "s" << std::endl;
    std::ofstream outfile("E:/scenes/renderer/img.ppm");
    std::cout << "writing file" << std::endl;
    outfile << ppmImg << std::endl;
    std::cout << "done writing" << std::endl;
    outfile.close();
    for (int i = 0; i < 12; ++i){
        std::cout << "thread " << i << " worked on " << workedon[i] << " chunks" << std::endl;
    }
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
        float r1 = drand48();
        float r2 = drand48();
        p = 2.0*Vector3f(r1,r2,0) - Vector3f(1,1,0);
    } while (dot(p,p) >= 1.0);

    return p;
}

Vector3f randomInUnitSphere(){

    Vector3f p;

    do {
        p = 2.0f * Vector3f(float(drand48()),float(drand48()),float(drand48())) - Vector3f(1.0, 1.0, 1.0);
    } while (dot(p,p) >= 1.0f);

    return unit_vector(p);
}

Vector3f color(const Ray& r, Hitable *world, Hitable *lightShape, int depth) {
    HitRecord rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ScatterRecord srec;
        Vector3f emitted = rec.matPtr->emitted(r, rec, rec.u, rec.v, rec.p);
        if (depth < 50 && rec.matPtr->scatter(r, rec, srec)){
            if (srec.isSpecular){
                return srec.attenuation * color(srec.SpecularRay, world, lightShape, depth+1);
            }else{
                HitablePDF p0(lightShape, rec.p);
                MixturePDF p(&p0, srec.pdfPtr);
                Ray scattered = Ray(rec.p, p.generate(), r.time);
                float pdf_val = p.value(scattered.direction());
                float spdf = rec.matPtr->scattering_pdf(r, rec, scattered);
                if (pdf_val == 0 && spdf == 0){
                    return emitted;
                }
                Vector3f zeroVec(0, 0, 0);
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
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                Ray r = render_cam->getRay(u, v);
                Vector3f tcol = deNan(color(r, world, &hlist, 0));
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
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    cornell_box_v2(&world, &render_cam, float(nx)/float(ny));

    Hitable* lightShape = new XZRectangle(213, 343, 227, 332, 554, nullptr);
    Hitable* glassSphere = new Sphere(Vector3f(190, 90, 190), 90, nullptr);
    Hitable *a[2];
    a[0] = lightShape;
    a[1] = glassSphere;
    hlist = HitableList(a,2);

    srand48(10);
    //ParallelInit();
    int chunkSize = ((nx * ny) / 11) / 8;
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

    for (int s = 0; s < ns; s++) {
        float u = float(col + drand48()) / float(nx);
        float v = float(row + drand48()) / float(ny);
        Ray r = render_cam->getRay(u, v);
        Vector3f tcol = deNan(color(r, world, &hlist, 0));
        rgbv += tcol;
    }
    rgbv /= float(ns);
    rgbv = Vector3f(sqrt(rgbv[0]), sqrt(rgbv[1]), sqrt(rgbv[2]));
    output_img[idx] = rgbv;
}

void parseSceneFile(const std::vector<std::string> &sceneDes,
                    std::vector<Triangle> &geoList,
                    std::vector<Light> &lightList,
                    Camera** cam){

    std::vector<Point3f> vertexList;
    std::vector<Vector3f> textureList;
    std::vector<Vector3f> normalList;

    float x, y, z, x2, y2, z2, x3, y3, z3, x4, y4, z4;
    std::string c;
    char delim;
    unsigned i = 0;
    for(const auto &s: sceneDes){
        std::stringstream sstream(s);

        if(sstream >> c){
            if(c == "light"){
                std::string name;
                float intensity, r, g, b, tx, ty, tz, sx, sy, sz, rx, ry, rz;
                if(sstream >> intensity >> r >> g >> b >> tx >> ty >> tz >> sx >> sy >> sz >> rx >> ry >> rz){
                    Light l = Light(name,
                          intensity,
                          r,g,b,
                          tx,ty,tz,
                          rx,ry,rz,
                          sx,sy,sz);
                    lightList.push_back(l);
                }
            }

            if(c == "cam"){
                float tx, ty, tz, sx, sy, sz, rx, ry, rz, focalLength;
                if(sstream >> tx >> ty >> tz >> sx >> sy >> sz >> rx >> ry >> rz >> focalLength){

//                    *cam = new Camera(Vector3f(tx, ty, tz),
//                                      Vector3f(rx, ry, rz),
//                                      Vector3f(sx, sy, sz), focalLength, 90, float(nx)/float(ny));
                }
            }

            if(c == "v"){
                if(sstream >> x >> y >> z){
                    vertexList.emplace_back(Point3f(x, y, z));
                }
            }
            if(c == "vt"){
                if(sstream >> x >> y){
                    textureList.emplace_back(Vector3f(x, y, 0));
                }
            }
            if(c == "vn"){
                if(sstream >> x >> y >> z){
                    normalList.emplace_back(Vector3f(x, y, z));
                }
            }
            if(c == "f"){
                if(sstream >> x >> delim >> x2 >> delim >> x3
                           >> y >> delim >> y2 >> delim >> y3
                           >> z >> delim >> z2 >> delim >> z3){

                    if(sstream >> x4 >> delim >> y4 >> delim >> z4){

                        Point3f p1 = vertexList[x];
                        Point3f p2 = vertexList[y];
                        Point3f p3 = vertexList[z];
                        Point3f p4 = vertexList[x4];

                        Triangle tri1 = Triangle(p1, p2, p3,
                                                 textureList[x2], textureList[y2], textureList[z2],
                                                 normalList[x3], normalList[y3], normalList[z3]);

                        geoList.push_back(tri1);

                        if(p4.x != p1.x && p4.x != p1.y && p4.x != p1.z &&
                                p4.y != p1.x && p4.y != p1.y && p4.y != p1.z &&
                                p4.z != p1.x && p4.z != p1.y && p4.z != p1.z){
                            Triangle tri2 = Triangle(p4, p2, p3,
                                                     textureList[y2], textureList[z2], textureList[y4],
                                                     normalList[y3], normalList[z3], normalList[z4]);
                            geoList.push_back(tri2);
                        }else if(p4.x != p2.x && p4.x != p2.y && p4.x != p2.z &&
                                 p4.y != p2.x && p4.y != p2.y && p4.y != p2.z &&
                                 p4.z != p2.x && p4.z != p2.y && p4.z != p2.z){

                            Triangle tri2 = Triangle(p4, p1, p3,
                                                     textureList[x2], textureList[z2], textureList[y4],
                                                     normalList[x3], normalList[z3], normalList[z4]);
                            geoList.push_back(tri2);
                        }else{
                            Triangle tri2 = Triangle(p4, p1, p2,
                                                     textureList[x2], textureList[y2], textureList[y4],
                                                     normalList[x3], normalList[y3], normalList[z4]);
                            geoList.push_back(tri2);
                        }



                    }else{

                        Triangle tri = Triangle(vertexList[x], vertexList[y], vertexList[z],
                                 textureList[x2], textureList[y2], textureList[z2],
                                 normalList[x3], normalList[y3], normalList[z3]);
                        geoList.push_back(tri);
                    }
              }
            }
        }


    }
}


#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <functional>
#include <string>

#include "Point3f.h"
#include "Point2f.h"
#include "Square.h"
#include "Triangle.h"
#include "Light.h"
#include "Camera.h"
#include "Ray.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"

void parseSceneFile(const std::vector<std::string> &,
                    std::vector<Triangle>&,
                    std::vector<Light> &,
                    Camera**);
std::string render(Camera*, std::vector<Triangle>*, std::vector<Light>*);
Vector3f color(const Ray& r, Hitable *world, int depth);


std::minstd_rand0 g1(10);
std::uniform_real_distribution<float> zero_to_one_dist(-1,1);
auto one_to_onedice = std::bind(zero_to_one_dist, g1);


int nx = 200;
int ny = 100;

int main(int argc, char **argv) {
    std::string filename(argv[argc - 1]);
    std::ifstream infile;
    infile.open(filename);
    std::vector<std::string> lines;
    std::string line;


    while(std::getline(infile, line)){
        lines.push_back(line);
    }
    infile.close();
    std::vector<Triangle> geoList;
    std::vector<Light> lightList;
    Camera *cam;
    parseSceneFile(lines, geoList, lightList, &cam);
    std::string ppmImg;
    ppmImg = render(cam, &geoList, &lightList);

    std::ofstream outfile("E:/scenes/renderer/img.ppm");
    std::cout << "writing file" << std::endl;
    outfile << ppmImg << std::endl;
    std::cout << "done writing" << std::endl;
    outfile.close();
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
        refracted = niOverNt*(uv - n*dt)- n*sqrtf(discriminant);
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
        p = 2.0f * Vector3f(one_to_onedice(), one_to_onedice(), 0) - Vector3f(1.0, 1.0, 0);
    } while (dot(p,p) >= 1.0f);

    return p;
}

Vector3f randomInUnitSphere(){

    Vector3f p;

    do {
        p = 2.0f * Vector3f(one_to_onedice(), one_to_onedice(), one_to_onedice()) - Vector3f(1.0, 1.0, 1.0);
    } while (p.squared_length() >= 1.0f);

    return p;
}

Vector3f color(const Ray& r, Hitable *world, int depth){

    HitRecord rec;
    if(world->hit(r, 0.001, MAXFLOAT, rec)){
        //Vector3f target = rec.p + rec.normal + randomInUnitSphere();
        //return 0.5*color( Ray(rec.p, target - rec.p), world);
        Ray scattered;
        Vector3f attenuation;
        if (depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered)){
            return attenuation*color(scattered, world, depth+1);
        }else{
            return Vector3f(0,0,0);
        }
    }else{
        Vector3f unitDirection = unit_vector(r.direction());
        float t = 0.5f*(unitDirection.y() + 1.0f);
        return (1.0f - t)*Vector3f(1.0f, 1.0f, 1.0f) + t*Vector3f(0.5f, 0.7f, 1.0f);
    }
}

Hitable* randomScene(){
    int n = 500;
    Hitable **list = new Hitable*[n+1];
    list[0] = new Sphere(Vector3f(0, -1000, 0), 1000, new Lambertian(Vector3f(0.5, 0.5, 0.5)));
    int i = 1;
    std::uniform_real_distribution<float> distribution(0,1);
    auto dice = std::bind(distribution, g1);
    for(int a = -11; a < 11; a++){
        for (int b = -11; b < 11; b++){
            float chooseMat = dice();
            Vector3f center(a+0.9f*dice(), 0.2, b+0.9f*dice());
            if((center - Vector3f(4,0.2, 0)).length() > 0.9){
                if (chooseMat < 0.8){
                    list[i++] = new Sphere(center, 0.2, new Lambertian(Vector3f(dice()*dice(), dice()*dice(), dice()*dice())));
                }else if (chooseMat < 0.95) {
                    list[i++] = new Sphere(center, 0.2,
                                           new Metal(Vector3f(0.5f*(1 + dice()), 0.5f*(1 + dice()), 0.5f*(1 + dice())), 0.5f*dice()));

                }else {
                    list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new Sphere(Vector3f(0, 1, 0), 1.0, new Dielectric(1.5));
    list[i++] = new Sphere(Vector3f(-4, 1, 0), 1.0, new Lambertian(Vector3f(0.4, 0.2, 0.1)));
    list[i++] = new Sphere(Vector3f(4, 1, 0), 1.0, new Metal(Vector3f(0.7, 0.6, 0.6), 0.0));

    return new HitableList(list, i);
}

std::string render(Camera* cam, std::vector<Triangle> *geoList, std::vector<Light> *lightList){

    int ns = 100;

    Hitable *list[5];
    list[0] = new Sphere(Vector3f(0,0,-1), 0.5f, new Lambertian(Vector3f(0.3, 0.3, 0.8)));
    list[1] = new Sphere(Vector3f(0, -100.5f, -1), 100, new Lambertian(Vector3f(0.8, 0.8, 0.0)));
    list[2] = new Sphere(Vector3f(1, 0, -1), 0.5, new Metal(Vector3f(0.8, 0.6, 0.2), 0.1));
    list[3] = new Sphere(Vector3f(-1, 0, -1), 0.5f, new Dielectric(1.5));
    list[4] = new Sphere(Vector3f(-1, 0, -1), -0.45f, new Dielectric(1.5));
//    Hitable *world = new HitableList(list, 5);
//    float R = cosf(float(M_PI)/4);
//    Hitable *list[2];
//    list[0] = new Sphere(Vector3f(-R, 0, -1), R, new Lambertian(Vector3f(0, 0, 1)));
//    list[1] = new Sphere(Vector3f(R, 0, -1), R, new Lambertian(Vector3f(1, 0, 0)));
//    Hitable *world = new HitableList(list, 2);

    Hitable *world = randomScene();
    std::stringstream ss;

    ss << "P3\n" << nx << " " << ny << "\n255\n";
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    //std::minstd_rand0 g1 (10);
    std::uniform_real_distribution<float> distribution(0,1);
    auto dice = std::bind(distribution, g1);
    float aperture = 0.1f;
    Vector3f lookform(13,2,3);
    Vector3f lookat(0,0,0);
    float distToFocus = 10;
    //float distToFocus = (lookform - lookat).length();

    cam = new Camera(lookform, lookat, Vector3f(0,1,0), 20, float(nx)/float(ny), aperture, distToFocus);
    for (int j = ny-1; j >= 0; j--){
        for(int i = 0; i < nx; i++){
            Vector3f col(0, 0, 0);
            for (int s = 0; s < ns; s++){
                float u = float(i + dice()) / float(nx);
                float v = float(j + dice()) / float(ny);
                Ray r = cam->getRay(u,v);
                Vector3f p = r.point_at_t(2.0);
                col += color(r, world, 0);
            }
            col /= float(ns);
            col = Vector3f(sqrtf(col[0]), sqrtf(col[1]), sqrtf(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            //std::cout << ir << " " << ig << " " << ib << std::endl;
            ss << ir << " " << ig << " " << ib << std::endl;
        }
    }
    std::cout << "done" << std::endl;
    return ss.str();
}

void parseSceneFile(const std::vector<std::string> &sceneDes,
                    std::vector<Triangle> &geoList,
                    std::vector<Light> &lightList,
                    Camera** cam){

    std::vector<Point3f> vertexList;
    std::vector<Point2f> textureList;
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

                    *cam = new Camera(Vector3f(tx, ty, tz),
                                      Vector3f(rx, ry, rz),
                                      Vector3f(sx, sy, sz), focalLength, 90, float(nx)/float(ny));
                }
            }

            if(c == "v"){
                if(sstream >> x >> y >> z){
                    vertexList.emplace_back(Point3f(x, y, z));
                }
            }
            if(c == "vt"){
                if(sstream >> x >> y){
                    textureList.emplace_back(Point2f(x, y));
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


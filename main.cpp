#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <functional>

#include "scenes.h"
#include "main.h"

void parseSceneFile(const std::vector<std::string> &,
                    std::vector<Triangle>&,
                    std::vector<Light> &,
                    Camera**);

int nx = 400;
int ny = 400;

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
        p = 2.0* Vector3f(drand48(),drand48(),0) - Vector3f(1,1,0);
    } while (dot(p,p) >= 1.0f);

    return p;
}

Vector3f randomInUnitSphere(){

    Vector3f p;

    do {
        p = 2.0f * Vector3f(drand48(),drand48(),drand48()) - Vector3f(1.0, 1.0, 1.0);
    } while (dot(p,p) >= 1.0f);

    return unit_vector(p);
}

Vector3f color(const Ray& r, Hitable *world, int depth) {
    HitRecord rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        Ray scattered;
        Vector3f attenuation;
        Vector3f emitted = rec.matPtr->emitted(rec.u, rec.v, rec.p);
        if (depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered))
            return emitted + attenuation*color(scattered, world, depth+1);
        else
            return emitted;
    }
    else
        return Vector3f(0,0,0);
}

std::string render(Camera* cam, std::vector<Triangle> *geoList, std::vector<Light> *lightList){

    int ns = 100;
    std::stringstream ss;
    ss << "P3\n" << nx << " " << ny << "\n255\n";
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    float aperture = 0.0f;
    Vector3f lookform(278, 278, -800);
    Vector3f lookat(278, 278, 0);
    float distToFocus = 10.00f;
    float vfov = 40.0;
    Hitable *world = cornell_smoke();


//    Hitable *world = simple_light(); // two_spheres(); //randomScene();
//    float aperture = 0.0f;
//    Vector3f lookform(13,2,3);
//    Vector3f lookat(0,0,0);
//    float distToFocus = (lookform - lookat).length();
//    float vfov = 40;

    cam = new Camera(lookform, lookat, Vector3f(0,1,0), vfov, float(nx)/float(ny), aperture, distToFocus, 0.0f, 1.0f);

    for (int j = ny-1, count = 0; j >= 0; j--, count++){
        std::cout << float(count / float(ny))*100 << "%" << std::endl;
        for(int i = 0; i < nx; i++){
            Vector3f col(0, 0, 0);
            for (int s = 0; s < ns; s++){
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                Ray r = cam->getRay(u, v);
                Vector3f p = r.point_at_t(2.0);
                col += color(r, world, 0);
            }
            col /= float(ns);
            col = Vector3f(sqrtf(col[0]), sqrtf(col[1]), sqrtf(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
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


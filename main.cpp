#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <functional>
#include <ctime>
#include <chrono>

#include "scenes.h"
#include "parallel.h"
#include "parsing/Parser.h"


Vector3f *output_img;
float *depths;
int *workedon;
int nx;
int ny;
int ns;
Camera* render_cam;
Hitable *world;
HitableList *hlist;

int main(int argc, char **argv) {

    const std::string filename(argv[argc - 1]);
    nx = 500;
    ny = 500;
    ns = 100;

    output_img = new Vector3f[nx*ny];
    depths = new float[nx*ny];

    workedon = new int[12];
    for (int i = 0; i < 12; ++i){
        workedon[i] = 0;
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    std::string ppmImg = render_scene();
    auto endTime = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    int mint = int(dur) / 60;
    int sect = int(dur) % 60;
    std::cout << "render time: " << mint << "m " << sect << "s" << std::endl;
    std::ofstream outfile(filename);
    outfile << ppmImg << std::endl;
    outfile.close();
    for (int i = 0; i < 12; ++i){
        std::cout << "thread " << i << " worked on " << workedon[i] << " chunks" << std::endl;
    }
    float sum = 0;
    for (int i = 0; i < nx*ny; ++i){
        sum += depths[i];
    }
    std::cout << "avg bounce depth: " << sum / (nx*ny) << std::endl;
    std::cout << "variance: " << calculate_var(depths, nx*ny) << std::endl;
    return 0;
}

std::string render_scene(){

    std::stringstream ss;
    ss << "P3\n" << nx << " " << ny << "\n255\n";

    cornell_box_v2(&world, &render_cam, float(nx)/float(ny), &hlist);

    int chunkSize =  ((nx * ny) / 11) / 8;
    ParallelInit();
    ParallelFor(threaded_render_pixel, nx*ny, chunkSize);
    ParallelCleanup();
    for(int i = 0; i < nx*ny; ++i){
        const Vector3f &rgbv = output_img[i];
        auto ir = static_cast<int>(255.99 * rgbv[0]);
        auto ig = static_cast<int>(255.99 * rgbv[1]);
        auto ib = static_cast<int>(255.99 * rgbv[2]);
        ss << ir << " " << ig << " " << ib << "\n";
    }
    return ss.str();
}

void threaded_render_pixel(int idx){
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
        Vector3f tcol = deNan(color(r, world, hlist, depth));
        total_depth += depth;
        rgbv += tcol;
    }
    depths[idx] =  total_depth / ns;
    rgbv /= float(ns);
    rgbv = Vector3f(sqrt(rgbv[0]), sqrt(rgbv[1]), sqrt(rgbv[2]));
    output_img[idx] = rgbv;
}



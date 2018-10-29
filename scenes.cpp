//
// Created by Rafiq on 2018-09-16.
//

#include "scenes.h"


void test_scene(Hitable **scene, Camera **cam, float aspect){

    Material *white = new Lambertian( new ConstantTexture(Vector3f(0.73, 0.73, 0.73)) );
    Hitable **list = new Hitable*[8];
    int i = 0;
    Material *glass = new Dielectric(1.5);
    list[i++] = new FlipNormals(new Sphere(Vector3f(0,0,0),1, glass));
    list[i++] = new FlipNormals(new XZRectangle(-20, 20, -20, 20, 20, white));
    Material *l = new DiffuseLight(new ConstantTexture(Vector3f(0.4, 0.45, 0.5)));
    list[i++] = new FlipNormals(new Sphere(Vector3f(0,0,0), 10000, l));
    *scene = new BVHNode(list, i, 0, 1);
    Vector3f lookfrom{3, 4, 1.5};
    Vector3f lookat{0.5, 0.5, 0};
    Vector3f vup{0, 0, 1};
    float vfov = 80.0;
    float aperture = 0.00;
    float distToFocus = 10;
    *cam = new Camera(lookfrom, lookat, vup, vfov, aspect, aperture, distToFocus, 0.0, 1.0);
}

void cornell_box_v2(Hitable **scene, Camera **cam, float aspect){

    int i = 0;
    Hitable **list = new Hitable*[8];

    Material *red = new Lambertian( new ConstantTexture(Vector3f(0.65, 0.05, 0.05)) );
    Material *white = new Lambertian( new ConstantTexture(Vector3f(0.73, 0.73, 0.73)) );
    Material *green = new Lambertian( new ConstantTexture(Vector3f(0.12, 0.45, 0.15)) );
    Material *light = new DiffuseLight( new ConstantTexture(Vector3f(15, 15, 15)) );

    list[i++] = new FlipNormals(new YZRectangle(0, 555, 0, 555, 555, green));
    list[i++] = new YZRectangle(0, 555, 0, 555, 0, red);
    list[i++] = new FlipNormals(new XZRectangle(213, 343, 227, 332, 554, light));
    list[i++] = new FlipNormals(new XZRectangle(0, 555, 0, 555, 555, white));
    list[i++] = new XZRectangle(0, 555, 0, 555, 0, white);
    list[i++] = new FlipNormals(new XYRectangle(0, 555, 0, 555, 555, white));
    Material *glass = new Dielectric(1.5);
    list[i++] = new Sphere(Vector3f(190, 90, 190), 90, glass);
    list[i++] = new Translate(new RotateY(new Box(Vector3f(0, 0, 0), Vector3f(165, 330, 165), white), 15), Vector3f(265, 0, 295));
    *scene = new BVHNode(list, i, 0, 1);
    Vector3f lookfrom(278, 278, -800);
    Vector3f lookat(278, 278, 0);
    float distToFocus = 10;
    float aperture = 0.00;
    float vfov = 40.0;
    *cam = new Camera(lookfrom, lookat, Vector3f(0,1,0), vfov, aspect, aperture, distToFocus, 0.0, 1.0);
}

Hitable* cornell_box(){

    Hitable **list = new Hitable*[8];
    int i = 0;
    Material *red = new Lambertian( new ConstantTexture(Vector3f(0.65, 0.05, 0.05)) );
    Material *white = new Lambertian( new ConstantTexture(Vector3f(0.73, 0.73, 0.73)) );
    Material *green = new Lambertian( new ConstantTexture(Vector3f(0.12, 0.45, 0.15)) );
    Material *light = new DiffuseLight( new ConstantTexture(Vector3f(15, 15, 15)) );
    list[i++] = new FlipNormals(new YZRectangle(0, 555, 0, 555, 555, green));
    list[i++] = new YZRectangle(0, 555, 0, 555, 0, red);
    list[i++] = new XZRectangle(213, 343, 227, 332, 554, light);
    list[i++] = new FlipNormals(new XZRectangle(0, 555, 0, 555, 555, white));
    list[i++] = new XZRectangle(0, 555, 0, 555, 0, white);
    list[i++] = new FlipNormals(new XYRectangle(0, 555, 0, 555, 555, white));

    list[i++] = new Translate(new RotateY(new Box(Vector3f(0, 0, 0), Vector3f(165, 165, 165), white),
                                          -18),
                              Vector3f(130, 0, 65));
    list[i++] = new Translate(new RotateY(new Box(Vector3f(0, 0, 0), Vector3f(165, 330, 165), white),
                                          15),
                              Vector3f(265, 0, 295));
    return new BVHNode(list, i, 0, 1);
}

Hitable* cornell_smoke(){

    Hitable **list = new Hitable*[8];
    int i = 0;
    Material *red = new Lambertian( new ConstantTexture(Vector3f(0.65, 0.05, 0.05)) );
    Material *white = new Lambertian( new ConstantTexture(Vector3f(0.73, 0.73, 0.73)) );
    Material *green = new Lambertian( new ConstantTexture(Vector3f(0.12, 0.45, 0.15)) );
    Material *light = new DiffuseLight( new ConstantTexture(Vector3f(7, 7, 7)) );
    list[i++] = new FlipNormals(new YZRectangle(0, 555, 0, 555, 555, green));
    list[i++] = new YZRectangle(0, 555, 0, 555, 0, red);
    list[i++] = new XZRectangle(113, 443, 127, 432, 554, light);
    list[i++] = new FlipNormals(new XZRectangle(0, 555, 0, 555, 555, white));
    list[i++] = new XZRectangle(0, 555, 0, 555, 0, white);
    list[i++] = new FlipNormals(new XYRectangle(0, 555, 0, 555, 555, white));

    Hitable *b1 = new Translate(new RotateY(new Box(Vector3f(0, 0, 0), Vector3f(165, 165, 165), white),
                                            -18),
                                Vector3f(130, 0, 65));
    Hitable *b2 = new Translate(new RotateY(new Box(Vector3f(0, 0, 0), Vector3f(165, 330, 165), white),
                                            15),
                                Vector3f(265, 0, 295));

    list[i++] = new ConstantMedium(b1, 0.01, new ConstantTexture(Vector3f(1.0, 1.0, 1.0)));
    list[i++] = new ConstantMedium(b2, 0.01, new ConstantTexture(Vector3f(0.0, 0.0, 0.0)));

    return new BVHNode(list, i, 0, 1);
}


Hitable *simple_light(){
    Texture *pertext = new NoiseTexture(4);
    Hitable **list = new Hitable*[4];
    list[0] =  new Sphere(Vector3f(0,-1000, 0), 1000, new Lambertian( pertext ));
    list[1] =  new Sphere(Vector3f(0, 2, 0), 2, new Lambertian( pertext ));
    list[2] =  new Sphere(Vector3f(0, 7, 0), 2, new DiffuseLight( new ConstantTexture(Vector3f(4,4,4))));
    list[3] =  new XYRectangle(3, 5, 1, 3, -2, new DiffuseLight( new ConstantTexture(Vector3f(4,4,4))));
    return new BVHNode(list, 4, 0, 1);
}

Hitable *two_perlin_spheres() {
    Texture *pertext = new NoiseTexture();
    Hitable **list = new Hitable*[2];
    list[0] =  new Sphere(Vector3f(0,-1000, 0), 1000, new Lambertian( pertext ));
    list[1] =  new Sphere(Vector3f(0, 2, 0), 2, new Lambertian( pertext ));
    return new HitableList(list,2);
}


Hitable *two_spheres(){
    Texture *checker =  new CheckerTexture(new ConstantTexture(Vector3f(0.2, 0.2, 0.1)), new ConstantTexture(Vector3f(0.9, 0.9, 0.9)));
    int n = 50;
    Hitable **list = new Hitable*[n+1];
    list[0] = new Sphere(Vector3f(0, -10, 0), 10, new Lambertian(checker));
    list[1] = new Sphere(Vector3f(0, 10, 0), 10, new Lambertian(checker));

    return new BVHNode(list, 2, 0, 1);
}

Hitable* randomScene(){
    int n = 50000;
    Hitable **list = new Hitable*[n+1];
    Texture *checker =  new CheckerTexture(new ConstantTexture(Vector3f(0.2, 0.2, 0.1)), new ConstantTexture(Vector3f(0.9, 0.9, 0.9)));
    list[0] = new Sphere(Vector3f(0, -1000, 0), 1000, new Lambertian(checker));
    int i = 1;

    for(int a = -10; a < 10; a++){
        for (int b = -10; b < 10; b++){
            float chooseMat = gen.UniformFloat(0,1);

            float r1 = gen.UniformFloat(0,1);
            float r2 = gen.UniformFloat(0,1);
            float r3 = gen.UniformFloat(0,1);
            float r4 = gen.UniformFloat(0,1);
            float r5 = gen.UniformFloat(0,1);
            float r6 = gen.UniformFloat(0,1);
            float r7 = gen.UniformFloat(0,1);
            float r8 = gen.UniformFloat(0,1);
            float r9 = gen.UniformFloat(0,1);
            float r10 = gen.UniformFloat(0,1);
            float r11 = gen.UniformFloat(0,1);
            float r12 = gen.UniformFloat(0,1);
            float r13 = gen.UniformFloat(0,1);
            Vector3f center(a+0.9f*r1, 0.2, b+0.9f*r2);
            if((center - Vector3f(4,0.2, 0)).length() > 0.9){
                if (chooseMat < 0.8){
                    list[i++] = new MovingSphere(center,
                                                 center+Vector3f(0, 0.5f*r3, 0),
                                                 0, 1,
                                                 0.2,
                                                 new Lambertian(new ConstantTexture(Vector3f(r4*r5, r6*r7, r8*r9)))
                    );
                }else if (chooseMat < 0.95) {
                    list[i++] = new Sphere(center,
                                           0.2,
                                           new Metal(Vector3f(0.5f*(1 + r10), 0.5f*(1 + r11), 0.5f*(1 + r12)),0.5f*r13));

                }else {
                    list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new Sphere(Vector3f(0, 1, 0), 1.0, new Dielectric(1.5));
    list[i++] = new Sphere(Vector3f(-4, 1, 0), 1.0, new Lambertian(new ConstantTexture(Vector3f(0.4, 0.2, 0.1))));
    list[i++] = new Sphere(Vector3f(4, 1, 0), 1.0, new Metal(Vector3f(0.7, 0.6, 0.6), 0.0));

    return new BVHNode(list, i, 0, 1);
}
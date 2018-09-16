//
// Created by Rafiq on 2018-09-16.
//

#include "scenes.h"

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
            float chooseMat = drand48();
            Vector3f center(a+0.9f*drand48(), 0.2, b+0.9f*drand48());
            if((center - Vector3f(4,0.2, 0)).length() > 0.9){
                if (chooseMat < 0.8){
                    list[i++] = new MovingSphere(center,
                                                 center+Vector3f(0, 0.5f*drand48(), 0),
                                                 0, 1,
                                                 0.2,
                                                 new Lambertian(new ConstantTexture(
                                                         Vector3f(drand48()*drand48(),
                                                                  drand48()*drand48(),
                                                                  drand48()*drand48()))
                                                 )
                    );
                }else if (chooseMat < 0.95) {
                    list[i++] = new Sphere(center,
                                           0.2,
                                           new Metal(Vector3f(0.5f*(1 + drand48()),
                                                              0.5f*(1 + drand48()),
                                                              0.5f*(1 + drand48())),
                                                     0.5f*drand48()));

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
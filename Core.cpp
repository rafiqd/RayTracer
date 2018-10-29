//
// Created by Rafiq on 2018-10-28.
//

#include "Core.h"
#include "PDF.h"
#include "Material.h"

bool refract(const Vector3f& v, const Vector3f& n, float niOverNt, Vector3f& refracted){
    const Vector3f uv = unit_vector(v);
    const float dt = dot(uv, n);
    const float discriminant = 1.0f - niOverNt*niOverNt*(1-dt*dt);
    if (discriminant > 0){
        refracted = niOverNt*(uv - n*dt)- n*sqrtf(discriminant);
        return true;
    }
    return false;
}


Vector3f color(const Ray& r, Hitable *world, Hitable *lightShape, int &depth) {

    const float threshhold = 0.25f;
    int threshold_depth = 2;
    bool once = true;
    Vector3f incoming_color(0,0,0);
    Ray iter_ray = r;
    Vector3f cumulative_val(std::nextafter(1.0f, 0.0f),std::nextafter(1.0f, 0.0f),std::nextafter(1.0f, 0.0f));

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
            break;
        }

        if(srec.isSpecular){
            cumulative_val *= srec.attenuation;
            iter_ray = srec.SpecularRay;
            if (once){
                threshold_depth = depth + 4;
                once = false;
            }

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

    return incoming_color;
}


float calculate_var(const float data[], int num) {
    float sum = 0.0;
    float mean;
    float standardDeviation = 0.0;
    int i;

    for(i = 0; i < num; ++i){
        sum += data[i];
    }

    mean = sum/num;

    for(i = 0; i < num; ++i)
        standardDeviation += pow(data[i] - mean, 2);

    return standardDeviation / num;
}

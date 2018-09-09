cmake_minimum_required(VERSION 3.9)
project(renderer)

set(CMAKE_CXX_STANDARD 17)

add_executable(renderer main.cpp Vector3f.cpp Vector3f.h Point3f.cpp Point3f.h Triangle.cpp Triangle.h Point2f.cpp Point2f.h Square.cpp Square.h Geometry.cpp Geometry.h Light.cpp Light.h Camera.cpp Camera.h Ray.cpp Ray.h Hitable.cpp Hitable.h Sphere.cpp Sphere.h HitableList.cpp HitableList.h Material.cpp Material.h Lambertian.cpp Lambertian.h Metal.cpp Metal.h Dielectric.cpp Dielectric.h Funcs.h)
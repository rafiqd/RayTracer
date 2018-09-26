cmake_minimum_required(VERSION 3.9)
project(renderer)

set(CMAKE_CXX_STANDARD 17)

add_executable(renderer main.cpp Vector3f.cpp Vector3f.h Point3f.cpp Point3f.h Triangle.cpp Triangle.h Geometry.cpp Geometry.h Light.cpp Light.h Camera.cpp Camera.h Ray.cpp Ray.h Hitable.cpp Hitable.h Sphere.cpp Sphere.h HitableList.cpp HitableList.h Material.cpp Material.h AABB.cpp AABB.h BVHNode.cpp BVHNode.h Texture.cpp Texture.h perlin.cpp perlin.h main.h Rectangle.cpp Rectangle.h ConstantMedium.cpp ConstantMedium.h scenes.cpp scenes.h ONB.cpp ONB.h PDF.cpp PDF.h parallel.cpp parallel.h)
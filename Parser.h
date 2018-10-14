//
// Created by Rafiq on 2018-10-06.
//

#ifndef RENDERER_PARSER_H
#define RENDERER_PARSER_H

#include "main.h"
bool parse_pbrt_scene(std::string scene_des, Hitable **lights, Camera**, Hitable **world);
std::vector<std::pair<std::string, std::string>> tokenize(std::string scene_des);
bool parse_num(std::vector<std::pair<std::string, std::string>>::iterator &start,
               std::vector<std::pair<std::string, std::string>>::iterator &end, float &f);
bool parse_cam(std::vector<std::pair<std::string, std::string>>::iterator &start,
               std::vector<std::pair<std::string, std::string>>::iterator &end, Camera **c,
               const Vector3f &lookfrom, const Vector3f &lookat, const Vector3f &vup);

#endif //RENDERER_PARSER_H

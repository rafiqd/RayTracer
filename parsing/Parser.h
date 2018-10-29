//
// Created by Rafiq on 2018-10-06.
//

#ifndef RENDERER_PARSER_H
#define RENDERER_PARSER_H

#include "../main.h"
#include "Tokens.h"

bool
parse_pbrt_scene(const std::string &scene_des, Hitable **lights, Camera **cam, Hitable **world);

#endif //RENDERER_PARSER_H

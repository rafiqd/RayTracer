//
// Created by Rafiq on 2018-10-27.
//

#ifndef RENDERER_RENDERGLOBALS_H
#define RENDERER_RENDERGLOBALS_H

#include <bits/unique_ptr.h>
#include "Camera.h"

class RenderGlobals {

public:
    std::unique_ptr<Camera> render_camera;

    RenderGlobals() = default;

};


#endif //RENDERER_RENDERGLOBALS_H

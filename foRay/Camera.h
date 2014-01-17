//
//  Camera.h
//  Clusterwink
//
//  Created by Kyle Halladay on 12/17/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef __Clusterwink__Camera__
#define __Clusterwink__Camera__

#include "glm.hpp"
using namespace glm;

class Camera
{
public:
    vec3 position;
    vec3 lookPoint;
    vec3 up;
    
    float fov;
    float aspectRatio;
    float zNear;
    float zFar;
    mat4 transform;
};

#endif /* defined(__Clusterwink__Camera__) */

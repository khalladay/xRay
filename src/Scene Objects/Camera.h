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

/* . If the camera is transformed with a 4x4 matrices to render the scene from a different point of view, all we need to do is to apply this transformation matrix to the camera rays' directions and origin. Note that all the camera rays share the same origin. This process is depicted in figure 7.*/

class Camera
{
public:    
    float fov;
    float aspectRatio;
    float zNear;
    float zFar;
    mat4 transform;
};

#endif /* defined(__Clusterwink__Camera__) */

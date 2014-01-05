//
//  RaycastHit.h
//  SimpleRayTracer
//
//  Created by Kyle Halladay on 12/12/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef __SimpleRayTracer__RaycastHit__
#define __SimpleRayTracer__RaycastHit__

#include "glm.hpp"
using namespace glm;

class RaycastHit
{
public:
    float t;
    float u;
    float v;
    
    int objectId;
    
    vec3 contactNormal;
    
    RaycastHit(){}
    ~RaycastHit(){}
};
#endif /* defined(__SimpleRayTracer__RaycastHit__) */

//
//  Ray.h
//  SimpleRayTracer
//
//  Created by Kyle Halladay on 12/12/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef __SimpleRayTracer__Ray__
#define __SimpleRayTracer__Ray__
#include "glm.hpp"

using namespace glm;

class Ray
{
public:
    vec3 origin;
    vec3 direction;
    
    float tmin, tmax;
    
    Ray(vec3 orig, vec3 dir, float min, float max) :
    origin(orig), direction(dir),tmin(min), tmax(max)
    {
        direction = (dir);
    }
    
    ~Ray(){}
};


#endif /* defined(__SimpleRayTracer__Ray__) */

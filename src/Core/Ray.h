//
//  Ray.h
//  xRay
//
//  Created by Kyle Halladay on 12/12/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef __xRay__Ray__
#define __xRay__Ray__
#include "glm.hpp"

using namespace glm;

class Ray
{
public:
    
    enum RayType
    {
        PrimaryRay,
        ShadowRay
    };
    
    vec3 origin;
    vec3 direction;
    
    float tmin, tmax;
    
    RayType type;
    
    Ray(){}
    
    Ray(RayType rayType, vec3 orig, vec3 dir, float min, float max) :
    type(rayType), origin(orig), direction(normalize(dir)),tmin(min), tmax(max)
    {
    }
    
    ~Ray(){}
};


#endif 
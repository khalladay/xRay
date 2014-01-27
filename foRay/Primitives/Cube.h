//
//  Cube.h
//  SimpleRayTracer
//
//  Created by Kyle Halladay on 12/14/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef __SimpleRayTracer__Cube__
#define __SimpleRayTracer__Cube__

#include "Traceable.h"

class Cube : Traceable
{
public:
    
    vec3 center;
    float dimension;
    
    bool intersect(Ray* r, RaycastHit* hit);
};

#endif /* defined(__SimpleRayTracer__Cube__) */

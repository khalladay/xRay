//
//  Cube.h
//  xRay
//
//  Created by Kyle Halladay on 12/14/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef __xRay__Cube__
#define __xRay__Cube__

#include "Traceable.h"

class Cube : Traceable
{
public:
    
    vec3 center;
    float dimension;
    
    bool intersect(Ray* r, RaycastHit* hit);
};

#endif 
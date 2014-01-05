//
//  Triangle.h
//  Clusterwink
//
//  Created by Kyle Halladay on 1/4/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __Clusterwink__Triangle__
#define __Clusterwink__Triangle__

#include "glm.hpp"
#include "Traceable.h"

class Triangle : public Traceable
{
public:
    vec3 A;
    vec3 B;
    vec3 C;
   
    bool intersect(Ray* r, RaycastHit* hit);

    Triangle(vec3 a, vec3 b, vec3 c, Material* m) : A(a), B(b), C(c)
    {
        material = m;
    }
};

#endif /* defined(__Clusterwink__Triangle__) */

//
//  Plane.cpp
//  SimpleRayTracer
//
//  Created by Kyle Halladay on 12/13/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#include "Plane.h"
#include "Ray.h"

bool Plane::intersect(Ray *r, RaycastHit* hit)
{
    float denom = dot(r->direction, n);
    if (denom > 1e-6)
    {
        vec3 toOrigin = center - r->origin;
        float d = dot(toOrigin, n)/denom;
        return d >=0;
    }
    
    return false;
}
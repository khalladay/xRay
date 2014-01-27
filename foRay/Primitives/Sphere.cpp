//
//  Sphere.cpp
//  SimpleRayTracer
//
//  Created by Kyle Halladay on 12/12/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#include "Sphere.h"
#include <stdio.h>
bool Sphere::intersect(Ray *r, RaycastHit* hit)
{
    vec3 L = center - r->origin;
    float t2 = dot(L, r->direction);
    
    if (t2 < 0.0f)
    {
        return false;
    }
    
    float d2 = dot(L,L) - t2*t2;
    if (d2 > radius2) return false;
    
    float t3 = sqrt(radius2 - d2);
    
    float t0 = t2-t3;
    //float t1 = t2+t3;
    
    if (t0 > r->tmax) return  false;
    else hit->t = t0;
    
    vec3 hitPoint = r->origin + (r->direction*hit->t);
    
    hit->contactNormal = normalize(hitPoint - center);
    hit->objectId = objectId;
    return true;
}
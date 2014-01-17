//
//  Triangle.cpp
//  Clusterwink
//
//  Created by Kyle Halladay on 1/4/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#include "Triangle.h"

#define MOLLERTROMBORE

bool Triangle::intersect(Ray *r, RaycastHit* hit)
{
#define EPSILON 1e-6
    
    vec3 AB = B-A;
    vec3 AC = C-A;
    vec3 P = cross(r->direction, AC);
    float Mdet = dot(AB, P);
    
    if (Mdet < 0) return false;
    
    float invDet = 1/Mdet;
    vec3 OA = r->origin - A;
    
    float u = dot(OA, P) * invDet;
    if ( u < 0 || u > 1 )
    {
        return false;
    }
    
    vec3 Q = cross(OA, AB);
    float v = dot(r->direction, Q) * invDet;
    
    if ( v < 0 || v > 1 )
    {
        return false;
    }
    if ( u+v > 1)
    {
        return false;
    }
    
    hit->u = u;
    hit->v = v;
    
    float t = dot(AC, Q) * invDet;
    if ( t < 0.0f )
    {
        return false;
    }
    
    hit->t = t;
    
    hit->contactNormal = N;
    
    return true;

    
}
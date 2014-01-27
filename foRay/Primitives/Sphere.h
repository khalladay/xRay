//
//  Sphere.h
//  SimpleRayTracer
//
//  Created by Kyle Halladay on 12/12/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef __SimpleRayTracer__Sphere__
#define __SimpleRayTracer__Sphere__
#include "Traceable.h"

class Sphere : public Traceable
{
public:
    vec3 center;
    float radius;
    float radius2; //precomputed for trace
    
    bool intersect(Ray* r, RaycastHit* hit);

    Sphere(vec3 pos, float rad, int id, std::shared_ptr<Material> m) : center(pos), radius(rad)
    {
        objectId = id;
        radius2 = radius*radius;
        material = m;
    }
};

#endif /* defined(__SimpleRayTracer__Sphere__) */

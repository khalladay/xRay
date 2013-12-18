//
//  Traceable.h
//  SimpleRayTracer
//
//  Created by Kyle Halladay on 12/12/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef __SimpleRayTracer__Traceable__
#define __SimpleRayTracer__Traceable__

#include "RaycastHit.h"
#include "Ray.h"
#include "glm.hpp"
#include "Material.h"
#include "DiffuseMaterial.h"

using namespace glm;
class Traceable
{
public:
    int objectId;
    mat4 worldToObject;
    mat4 objectToWorld;
    
    Material *material;
    
    Traceable()
    {
        material = new DiffuseMaterial(vec3(1.0f));
    }
    
    Traceable(mat4 transformMatrix, Material* mat, int id) : material(mat)
    {
        objectToWorld = inverse(transformMatrix);
    }
    
    virtual bool intersect(Ray* r, RaycastHit* hit) = 0;
    
    ~Traceable(){}
};
#endif 

/* defined(__SimpleRayTracer__Traceable__) */

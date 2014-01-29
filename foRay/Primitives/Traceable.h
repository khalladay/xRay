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
#include <memory>
#include <string>
using namespace glm;

class Traceable
{
public:
    int objectId;
    std::shared_ptr<Material> material;
    std::string name;
    mat4 transform;
    
    Traceable()
    {
    }
    
    Traceable(mat4 transformMatrix, std::shared_ptr<Material> mat, int id)
    {
        material = mat;
    }
    
    virtual void convertToWorldSpace(){}
    
    virtual bool intersect(Ray* r, RaycastHit* hit) = 0;
    
    ~Traceable(){}
};
#endif 

/* defined(__SimpleRayTracer__Traceable__) */

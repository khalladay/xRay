//
//  Material.h
//  Clusterwink
//
//  Created by Kyle Halladay on 12/15/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef __Clusterwink__Material__
#define __Clusterwink__Material__

#include "glm.hpp"

using namespace glm;
class Material
{
public:
    
    vec3 col;
    
    Material()
    {
        col = vec3(1.0f);
    }
    
    Material(vec3 color) : col(color)
    {
    }
    
    virtual vec3 sample(const vec3& samplePoint, const vec3& normal, const vec3& lightDirection, const vec3& lightPosition, const vec3& viewDirection) = 0;
};

#endif /* defined(__Clusterwink__Material__) */

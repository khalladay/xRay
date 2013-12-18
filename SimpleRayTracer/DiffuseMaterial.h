//
//  DiffuseMaterial.h
//  Clusterwink
//
//  Created by Kyle Halladay on 12/17/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef __Clusterwink__DiffuseMaterial__
#define __Clusterwink__DiffuseMaterial__

#include "Material.h"

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(vec3 color)
    {
        col = color;
    }
    
    vec3 sample(const vec3& samplePoint, const vec3& normal, const vec3& lightDirection, const vec3& lightPosition, const vec3& viewDirection)
    {
        return col * max(0.0f, dot(lightDirection, normal));
    }
    
};
#endif /* defined(__Clusterwink__DiffuseMaterial__) */

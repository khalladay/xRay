//
//  LambertMaterial.h
//  xRay
//
//  Created by Kyle Halladay on 1/15/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __xRay__LambertMaterial__
#define __xRay__LambertMaterial__

#include "Material.h"å

class LambertMaterial : public Material
{
public:
    
    LambertMaterial(MaterialProperties properties) : Material(properties)
    {
        diffuse = properties.diffuse;
        emission = properties.emission;
        ambient = properties.ambient;
    }
    
    virtual vec3 sample(const vec3& samplePoint, const vec3& normal, const vec3& lightDirection, const vec3& lightPosition, const vec3& viewDirection)
    {
        return vec3(diffuse.x, diffuse.y, diffuse.z) *max(0.0f, dot(lightDirection, normal));
    }
    
    ~LambertMaterial(){}
    
private:
    vec4 diffuse;
    vec4 emission;
    vec4 ambient;
};

#endif /* defined(__xRay__LambertMaterial__) */

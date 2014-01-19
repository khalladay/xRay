//
//  LambertMaterial.h
//  foRay
//
//  Created by Kyle Halladay on 1/15/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __foRay__LambertMaterial__
#define __foRay__LambertMaterial__

#include "Material.h"

class LambertMaterial : public Material
{
public:
    
    LambertMaterial(MaterialProperties properties) : Material(properties)
    {
        diffuse = properties.diffuse;
        emission = properties.emission;
        ambient = properties.ambient;
    }
    
    ~LambertMaterial(){}
    
private:
    vec4 diffuse;
    vec4 emission;
    vec4 ambient;
};

#endif /* defined(__foRay__LambertMaterial__) */

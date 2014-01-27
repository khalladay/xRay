//
//  PhongMaterial.h
//  foRay
//
//  Created by Kyle Halladay on 1/15/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __foRay__PhongMaterial__
#define __foRay__PhongMaterial__

#include "Material.h"
#include "glm.hpp"

using namespace glm;

class PhongMaterial : public Material
{
public:
    PhongMaterial(MaterialProperties properties) : Material(properties)
    {
        diffuse = properties.diffuse;
        specular = properties.specular;
        emission = properties.emission;
        shininess = properties.shininess;
        ambient = properties.ambient;
    }
    
    ~PhongMaterial(){}
    
private:
    vec4 diffuse;
    vec4 specular;
    vec4 emission;
    vec4 ambient;
    float shininess;
};

#endif /* defined(__foRay__PhongMaterial__) */

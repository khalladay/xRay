//
//  SpecularMaterial.h
//  Clusterwink
//
//  Created by Kyle Halladay on 12/18/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef Clusterwink_SpecularMaterial_h
#define Clusterwink_SpecularMaterial_h

#include "Material.h"

class SpecularMaterial : public Material
{
public:
    vec3 specCol;
    
    SpecularMaterial(vec3 color, vec3 specColor)
    {
        col = color;
        specCol = specColor;
    }
    
    vec3 sample(const vec3& samplePoint, const vec3& normal, const vec3& lightDirection, const vec3& lightPosition, const vec3& viewDirection)
    {
        vec3 diffuseTerm = col * max(0.0f, dot(lightDirection, normal));
        vec3 r =-normalize(reflect (viewDirection, normal));
        vec3 specTerm = specCol * pow(max(0.0f, dot(r, lightDirection)), 80.0f);
        return diffuseTerm+specTerm+col*vec3(0.05f);
    }
    
};

#endif

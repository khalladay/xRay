//
//  CheckerMaterial.h
//  Clusterwink
//
//  Created by Kyle Halladay on 12/17/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef __Clusterwink__CheckerMaterial__
#define __Clusterwink__CheckerMaterial__

#include "Material.h"

class CheckerMaterial : public Material
{
    vec3 sample(const vec3& samplePoint, const vec3& normal, const vec3& lightDirection, const vec3& lightPosition, const vec3& viewDirection)
    {
        vec3 h= samplePoint*0.2f;
        return vec3( (int)(ceil(h.x) + ceil(h.z))&1?(vec3(0.0f, 0.0f, 0.0f)):(vec3(1.0f)));

    }

};

#endif /* defined(__Clusterwink__CheckerMaterial__) */

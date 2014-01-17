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
    LambertMaterial(){}
    LambertMaterial(vec3 color):col(color){}
    
    void setColor(vec3 color){col = color;}
    
    ~LambertMaterial(){}
    
private:
    vec3 col;
};

#endif /* defined(__foRay__LambertMaterial__) */

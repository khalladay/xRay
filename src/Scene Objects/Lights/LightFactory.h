//
//  LightFactory.h
//  xRay
//
//  Created by Kyle Halladay on 1/16/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __xRay__LightFactory__
#define __xRay__LightFactory__

#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"

class LightFactory
{
public:
    static std::shared_ptr<Light> build(LightProperties plist, std::string type)
    {
        
        if (type.compare("point") == 0)
        {
            return std::shared_ptr<Light>(new PointLight(plist));
        }
        
        if (type.compare("directional") == 0)
        {
            return std::shared_ptr<Light>(new DirectionalLight(plist));
        }
        
        return NULL;
    }
};
#endif /* defined(__xRay__LightFactory__) */

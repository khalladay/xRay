//
//  DirectionalLights.h
//  foRay
//
//  Created by Kyle Halladay on 1/16/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __foRay__DirectionalLights__
#define __foRay__DirectionalLights__

#include "Light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight(LightProperties plist) : Light(plist){}
};
#endif /* defined(__foRay__DirectionalLights__) */

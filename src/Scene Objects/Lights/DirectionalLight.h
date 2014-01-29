//
//  DirectionalLights.h
//  xRay
//
//  Created by Kyle Halladay on 1/16/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __xRay__DirectionalLights__
#define __xRay__DirectionalLights__

#include "Light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight(LightProperties plist) : Light(plist){}
};
#endif /* defined(__xRay__DirectionalLights__) */

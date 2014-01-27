//
//  PointLight.h
//  foRay
//
//  Created by Kyle Halladay on 1/16/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __foRay__PointLight__
#define __foRay__PointLight__

#include "Light.h"

class PointLight : public Light
{
public:
    PointLight(LightProperties plist) : Light(plist){}
};

#endif /* defined(__foRay__PointLight__) */

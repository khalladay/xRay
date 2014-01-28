//
//  Light.cpp
//  Clusterwink
//
//  Created by Kyle Halladay on 12/17/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#include "Light.h"

Light::Light(LightProperties properties)
{
    transform = properties.transform;

    //at least in blender collada, identity matrix camera starts at origin
    //and points down the Y (z in blender)
    vec4 pos = transform * vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 dir =  transform * vec4(0.0f, -1.0f, -1.0f, 0.0f);
    _position = vec3(pos.x, pos.y, pos.z);
    _direction = normalize(vec3(dir.x, dir.y, dir.z));
    
}
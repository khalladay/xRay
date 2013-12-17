//
//  Material.h
//  Clusterwink
//
//  Created by Kyle Halladay on 12/15/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef __Clusterwink__Material__
#define __Clusterwink__Material__

#include "glm.hpp"

using namespace glm;
class Material
{
    vec3 sample(vec3 samplePoint);
};

#endif /* defined(__Clusterwink__Material__) */

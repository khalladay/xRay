//
//  Scene.h
//  Clusterwink
//
//  Created by Kyle Halladay on 12/17/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#ifndef __Clusterwink__Scene__
#define __Clusterwink__Scene__

#include "Traceable.h"
#include "Light.h"
#include "Camera.h"

#include <vector>

class Scene
{
    
public:
    
    //todo - change other code to use smart pointers
    std::vector<std::shared_ptr<Traceable>> traceables;
    std::vector<std::shared_ptr<Light>> lights;
    Camera cam;
    
    Scene(){}
    
    Scene(vec3 camPos, vec3 camLookpoint, vec3 camUp)
    {
        cam.position = camPos;
        cam.lookPoint = camLookpoint;
        cam.up = camUp;
    }

    
};
#endif /* defined(__Clusterwink__Scene__) */

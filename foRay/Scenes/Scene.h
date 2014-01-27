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
    
    std::vector<std::shared_ptr<Traceable>> traceables;
    std::vector<std::shared_ptr<Light>> lights;
    Camera cam;
    
    Scene(){}
    
    void setAspectRatio(float ratio)
    {
        cam.aspectRatio = ratio;
    }

    
};
#endif /* defined(__Clusterwink__Scene__) */

//
//  SceneTestRenderDevice.h
//  foRay
//
//  Created by Kyle Halladay on 1/26/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __foRay__SceneTestRenderDevice__
#define __foRay__SceneTestRenderDevice__

#include "RenderDevice.h"
#include "Ray.h"

class SceneTestRenderDevice : public RenderDevice
{
public:
    SceneTestRenderDevice(std::unique_ptr<Scene> scene);
    
    virtual vec3 traceNDCPoint(float x, float y);
    
private:
    vec3 cameraPosition;
    
    void rayForPixel(Ray* r, int x, int y);
    void traceRay(Ray* r, RaycastHit* hit, vec3* output);
};

#endif /* defined(__foRay__CPURenderDevice__) */

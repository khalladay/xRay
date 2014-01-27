//
//  CPURenderDevice.h
//  foRay
//
//  Created by Kyle Halladay on 1/23/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __foRay__CPURenderDevice__
#define __foRay__CPURenderDevice__

#include "RenderDevice.h"
#include "Ray.h"

class CPURenderDevice : public RenderDevice
{
public:
    CPURenderDevice(std::unique_ptr<Scene> scene);
    
    virtual vec3 traceNDCPoint(float x, float y);
    
private:
    vec3 cameraPosition;
    
    void rayForPixel(Ray* r, float ndcX, float ndcY);
    void traceRay(Ray* r, RaycastHit* hit, vec3* output);
};

#endif /* defined(__foRay__CPURenderDevice__) */

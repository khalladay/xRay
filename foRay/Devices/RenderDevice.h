//
//  RenderDevice.h
//  foRay
//
//  Created by Kyle Halladay on 1/22/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

/* The render device class is responsible
 * for evaluating the end result of a pixel
 * given image information, a scene, and optionally,
 * an acceleration structure. It does not store any
 * pixel data itself.
 *
 * The opposite to the Renderer, the RenderDevice
 * has no information about the image being produced,
 * just about the scene it is tracing.
 */

#ifndef __foRay__RenderDevice__
#define __foRay__RenderDevice__

#include "glm.hpp"
#include "Scene.h"
#include "RenderDevice_intern.h"

using namespace glm;
class RenderDevice
{
public:
    
    //this method will return the appropriate subclass based on system stats
    static std::unique_ptr<RenderDevice> create(std::unique_ptr<Scene> scene)
    {
        //return createSceneTestDevice(std::unique_ptr<Scene>(std::move(scene)));
        return createCPUDevice(std::unique_ptr<Scene>(std::move(scene)));
    }
    
    RenderDevice(std::unique_ptr<Scene> scene)
    {
        this->scene = std::move(scene);
    }
    
    virtual vec3 traceNDCPoint(float x, float y) { return vec3(1.0f); }
    
    virtual ~RenderDevice(){}
    
protected:
    std::unique_ptr<Scene> scene;
};

#endif /* defined(__foRay__RenderDevice__) */

//
//  Renderer.h
//  foRay
//
//  Created by Kyle Halladay on 1/22/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

/* The renderer class is responsible
 * for managing all the threads used
 * to efficiently construct an image from
 * the interface provided by RenderDevice.
 
 * The renderer has no information about the 
 * contents of the scene, just about the image it 
 * is constructing. It is responsible for the creation
 * of render tasks as well.
 */

#ifndef __foRay__Renderer__
#define __foRay__Renderer__

#include "RenderDevice.h"
#include "glm.hpp"
#include <boost/thread.hpp>
#include "util_array.h"

class RenderTask
{
public:
    
    int xMin;
    int xMax;
    int yMin;
    int yMax;

    RenderTask(int xMin, int xMax, int yMin, int yMax) : xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax){}
    ~RenderTask(){}
};

class RenderProductInfo
{
public:
    int imageWidth;
    int imageHeight;
    
    RenderProductInfo(){}
    ~RenderProductInfo(){}
};

class Renderer
{
public:
    
    Renderer(RenderProductInfo* productInfo, std::unique_ptr<RenderDevice> device);
    foray_image render();
    void renderTile(RenderTask task);
    virtual ~Renderer(){}
    
    bool isComplete();
    
private:
    
    bool completed;
    boost::thread_group threads;

    foray_image imageData;
    
    int numCores;
    
    std::unique_ptr<RenderDevice> device;
    RenderProductInfo* info;
};
#endif /* defined(__foRay__Renderer__) */

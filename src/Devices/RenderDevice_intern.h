//
//  RenderDevice_intern.h
//  xRay
//
//  Created by Kyle Halladay on 1/24/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __xRay__RenderDevice_intern__
#define __xRay__RenderDevice_intern__

class RenderDevice;
std::unique_ptr<RenderDevice> createCPUDevice(std::unique_ptr<Scene>);
std::unique_ptr<RenderDevice> createSceneTestDevice(std::unique_ptr<Scene>);

#endif /* defined(__xRay__RenderDevice_intern__) */

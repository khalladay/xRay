//
//  RenderDevice_intern.h
//  foRay
//
//  Created by Kyle Halladay on 1/24/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __foRay__RenderDevice_intern__
#define __foRay__RenderDevice_intern__

class RenderDevice;
std::unique_ptr<RenderDevice> createCPUDevice(std::unique_ptr<Scene>);
std::unique_ptr<RenderDevice> createSceneTestDevice(std::unique_ptr<Scene>);

#endif /* defined(__foRay__RenderDevice_intern__) */

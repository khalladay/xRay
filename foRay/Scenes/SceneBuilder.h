//
//  AbstractSceneBuilder.h
//  foRay
//
//  Created by Kyle Halladay on 1/14/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __foRay__AbstractSceneBuilder__
#define __foRay__AbstractSceneBuilder__

#include "Scene.h"

class SceneBuilder
{
public:
    
    std::unique_ptr<Scene> scene;
    
    virtual void buildCamera() = 0;
    virtual void buildMaterials() = 0;
    virtual void buildLights() = 0;
    virtual void buildMeshes() = 0;
    
    virtual std::unique_ptr<Scene> getProduct(){ return std::unique_ptr<Scene>(std::move(scene)); }
    
    SceneBuilder()
    {
        scene = std::unique_ptr<Scene>(new Scene());
    }
    
    virtual ~SceneBuilder() {}
    
};
#endif /* defined(__foRay__AbstractSceneBuilder__) */

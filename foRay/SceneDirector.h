//
//  SceneDirector.h
//  foRay
//
//  Created by Kyle Halladay on 1/14/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __foRay__SceneDirector__
#define __foRay__SceneDirector__

#include "SceneDirector.h"
#include "SceneBuilder.h"
#include "Scene.h"

class SceneDirector
{
public:
        
    static std::shared_ptr<Scene> buildScene(SceneBuilder* builder)
    {
        builder->buildCamera();
        builder->buildLights();
        builder->buildMaterials();
        builder->buildMeshes();
        return builder->getProduct();
    }
    
};

#endif /* defined(__foRay__SceneDirector__) */

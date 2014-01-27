//
//  MaterialFactory.h
//  foRay
//
//  Created by Kyle Halladay on 1/17/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __foRay__MaterialFactory__
#define __foRay__MaterialFactory__

#include "Material.h"
#include "PhongMaterial.h"
#include "LambertMaterial.h"

class MaterialFactory
{
public:
    static std::shared_ptr<Material> build(MaterialProperties properties, std::string type)
    {
        if (type.compare("phong") == 0)
        {
            return std::shared_ptr<Material>(new PhongMaterial(properties));
        }
        
        if (type.compare("lambert") == 0)
        {
            return std::shared_ptr<Material>(new LambertMaterial(properties));
        }

        return NULL;
    }
    
    static std::shared_ptr<Material> buildDefault()
    {
        MaterialProperties properties;
        properties.ambient = vec4(0.5f);
        properties.emission = vec4(0.0f);
        properties.diffuse = vec4(0.8f);
        return std::shared_ptr<Material>(new LambertMaterial(properties));
    }
};
#endif /* defined(__foRay__MaterialFactory__) */

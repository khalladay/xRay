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
    static std::shared_ptr<Material> build(std::map<std::string, std::string> plist)
    {
        MaterialProperties properties = MaterialFactory::parsePropertyPlist(plist);
        if (plist["type"].compare("phong") == 0)
        {
            return std::shared_ptr<Material>(new PhongMaterial(properties));
        }
        
        if (plist["type"].compare("lambert") == 0)
        {
            return std::shared_ptr<Material>(new LambertMaterial(properties));
        }

        return NULL;
    }
    
    static MaterialProperties parsePropertyPlist(std::map<std::string, std::string> plist)
    {
        MaterialProperties properties;
        
        if (plist.find("index_of_refraction") != plist.end())
        {
            properties.indexOfRefraction = atof(plist["index_of_refraction"].c_str());
        }
        if (plist.find("shininess") != plist.end())
        {
            properties.shininess = atof(plist["shininess"].c_str());
        }
        if (plist.find("emission") != plist.end())
        {
            std::vector<float> elements;
            char* vIter = strtok((char*)(plist["emission"].c_str()), " ");
            while (vIter != NULL)
            {
                elements.push_back(atof(vIter));
                vIter = strtok(NULL, " ");
            }
            
            properties.emission = vec4(elements[0], elements[1], elements[2], elements[3]);
        }
        
        if (plist.find("ambient") != plist.end())
        {
            std::vector<float> elements;
            char* vIter = strtok((char*)plist["ambient"].c_str(), " ");
            while (vIter != NULL)
            {
                elements.push_back(atof(vIter));
                vIter = strtok(NULL, " ");
            }
            
            properties.ambient = vec4(elements[0], elements[1], elements[2], elements[3]);
        }
        
        if (plist.find("diffuse") != plist.end())
        {
            std::vector<float> elements;
            char* vIter = strtok((char*)plist["diffuse"].c_str(), " ");
            while (vIter != NULL)
            {
                elements.push_back(atof(vIter));
                vIter = strtok(NULL, " ");
            }
            
            properties.diffuse = vec4(elements[0], elements[1], elements[2], elements[3]);
        }
        
        if (plist.find("specular") != plist.end())
        {
            std::vector<float> elements;
            char* vIter = strtok((char*)plist["specular"].c_str(), " ");
            while (vIter != NULL)
            {
                elements.push_back(atof(vIter));
                vIter = strtok(NULL, " ");
            }
            
            properties.specular = vec4(elements[0], elements[1], elements[2], elements[3]);
        }
        
        return properties;
    }
};
#endif /* defined(__foRay__MaterialFactory__) */

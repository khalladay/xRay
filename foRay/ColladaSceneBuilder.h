//
//  ColladaSceneBuilder.h
//  foRay
//
//  Created by Kyle Halladay on 1/14/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __foRay__ColladaSceneBuilder__
#define __foRay__ColladaSceneBuilder__

#include <string>
#include <fstream>
#include "rapidxml.hpp"
#include "glm.hpp"

#include "Scene.h"
#include "SceneBuilder.h"
#include "Material.h"

using namespace rapidxml;
using namespace glm;

class ColladaSceneBuilder : public SceneBuilder
{
public:
    
    std::string fileContents;
    char* chContents;
    xml_document<> doc;
    
    std::map<std::string, std::shared_ptr<Material>> materials;
    
    ColladaSceneBuilder(std::string filepath)
    {
        std::ifstream file(filepath);
        fileContents = "";
        std::string temp;
        while(std::getline(file, temp))
        {
            fileContents.append(temp);
        }
        
        char * chContents = new char[fileContents.size() + 1];
        std::copy(fileContents.begin(), fileContents.end(), chContents);
        chContents[fileContents.size()] = '\0';
        
        doc.parse<0>(chContents);
    }
    
    virtual void buildCamera();
    virtual void buildLights();
    virtual void buildMaterials();
    virtual void buildMeshes();

private:
    
    /* getChild will search the immediate child nodes of an xml_node
     * for a child with an attribute named attributeName with value
     * attributeVal or NULL if no result found */
    xml_node<>* getChild(xml_node<>* parent, std::string childName);
    xml_node<>* getChild(xml_node<>* parent, std::string childName, std::string attributeName, std::string attributeVal);

    
};
#endif /* defined(__foRay__ColladaSceneBuilder__) */

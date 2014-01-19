//
//  ColladaSceneBuilder.cpp
//  foRay
//
//  Created by Kyle Halladay on 1/14/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#include "ColladaSceneBuilder.h"
#include "Camera.h"
#include "TriMesh.h"
#include "LightFactory.h"
#include "MaterialFactory.h"

void ColladaSceneBuilder::buildCamera()
{
    xml_node<>* root = doc.first_node("COLLADA");
    xml_node<>* camNodesRoot = getChild(root, "library_cameras");
    
    if( camNodesRoot == NULL)
    {
        fprintf(stderr, "No cameras found in collada file\n");
        return;
    }
 
    //for readability sake, each node we traverse will be represented here
    xml_node<>* firstCameraNode = camNodesRoot->first_node();
    xml_node<>* opticsNode = firstCameraNode->first_node("optics");
    xml_node<>* techniqueCommonNode = opticsNode->first_node("technique_common");
    xml_node<>* perspectiveNode = techniqueCommonNode->first_node("perspective");
    
    xml_node<>* xfovNode = perspectiveNode->first_node("xfov");
    scene->cam.fov = atof(xfovNode->value());
    
    xml_node<>* aspectRatioNode = perspectiveNode->first_node("aspect_ratio");
    scene->cam.aspectRatio = atof(aspectRatioNode->value());
    
    xml_node<>* nearNode = perspectiveNode->first_node("znear");
    scene->cam.zNear = atof(nearNode->value());
    
    xml_node<>* farNode = perspectiveNode->first_node("zfar");
    scene->cam.zFar = atof(farNode->value());
    
    //now jump around because collada is dumb
    xml_node<>* visualScenes = root->first_node("library_visual_scenes");
    xml_node<>* firstSceneNode = visualScenes->first_node();
    xml_node<>* cameraNode = getChild(firstSceneNode, "node", "id", "Camera");
    
    if (cameraNode == NULL)
    {
        fprintf(stderr, "ERROR: No camera found in collada file. %s : %d!",__FILE__, __LINE__);
        return;
    }
    
    xml_node<>* transform = getChild(cameraNode, "matrix", "sid", "transform");
    
    if (transform == NULL)
    {
        fprintf(stderr, "ERROR: No camera transform found in collada file. %s : %d!",__FILE__, __LINE__);
        return;
    }
    
    char* matrixString = transform->value();
    
    
    std::vector<float> elements;
    char* iter = strtok(matrixString, " ");

    while (iter!= NULL)
    {
        elements.push_back(atof(iter));
        iter = strtok(NULL," ");
    }
    
    scene->cam.transform = mat4(elements[0], elements[1], elements[2], elements[3],
                                elements[4], elements[5], elements[6], elements[7],
                                elements[8], elements[9], elements[10], elements[11],
                                elements[12], elements[13], elements[14], elements[15]);
    
    fprintf(stdout, "Success: Built Camera from collada file\n");
}

void ColladaSceneBuilder::buildLights()
{
    xml_node<>* root = doc.first_node("COLLADA");
    xml_node<>* camNodesRoot = getChild(root, "library_lights");
    
    if (camNodesRoot == NULL)
    {
        fprintf(stdout, "No lights found in collada file\n");
        return;
    }
    
    xml_node<>* lightIter = camNodesRoot->first_node("light");
    
    int count = 0;
    while (lightIter != NULL)
    {
        count++;
        std::map<std::string, std::string> lightPlist;
        char* lightName = lightIter->first_attribute("name")->value();
        xml_node<>* techniqueNode = lightIter->first_node("technique_common");
        xml_node<>* technique = techniqueNode->first_node();
        
        lightPlist["type"] = std::string(technique->name());
        
        xml_node<>* propertyIter = technique->first_node();
        
        while (propertyIter != NULL)
        {
            lightPlist[std::string(propertyIter->name())] = std::string(propertyIter->value());
            propertyIter = propertyIter->next_sibling();
        }
        
        xml_node<>* sceneNode = root->first_node("library_visual_scenes");
        xml_node<>* visScene = sceneNode->first_node("visual_scene");
        xml_node<>* sceneLight = getChild(visScene, "node", "name", std::string(lightName));
        
        if (sceneLight == NULL)
        {
            fprintf(stderr, "ERROR: No camera transform found in collada file. %s : %d!",__FILE__, __LINE__);
            return;
        }
        
        char* matrix = sceneLight->first_node("matrix")->value();
        
        lightPlist["transform"] = std::string(matrix);
        
        std::shared_ptr<Light> lightPtr = LightFactory::build(lightPlist);
        
        scene->lights.push_back(lightPtr);
        lightIter = lightIter->next_sibling();
    }
    fprintf(stdout, "Success: Built %i lights from collada file\n", count);

}

void ColladaSceneBuilder::buildMaterials()
{
    xml_node<>* root = doc.first_node("COLLADA");
    xml_node<>* materialsRoot = getChild(root, "library_effects");
    xml_node<>* effectIter = materialsRoot->first_node("effect");
    
    int count = 0;
    while(effectIter != NULL)
    {
        count++;
        char* effectName = effectIter->first_attribute("id")->value();
        std::map<std::string, std::string> materialProperties;
        
        xml_node<>* profile = effectIter->first_node("profile_COMMON");
        xml_node<>* technique = profile->first_node("technique");
        xml_node<>* materialType = technique->first_node();
        
        materialProperties["type"] = materialType->name();
        
        xml_node<>* propertyIter = materialType->first_node();
        
        while(propertyIter != NULL)
        {
            materialProperties[std::string(propertyIter->name())] = std::string(propertyIter->first_node()->value());
            
            propertyIter = propertyIter->next_sibling();
        }
        
        materials["effectName"] = MaterialFactory::build(materialProperties);
        
        effectIter = effectIter->next_sibling();
    }
    fprintf(stdout, "Success: Built %i materials from collada file\n", count);
    
}

void ColladaSceneBuilder::buildMeshes()
{
    xml_node<>* root = doc.first_node("COLLADA");
    xml_node<>* geometryRoot = root->first_node("library_geometries");
    
    xml_node<>* geometryIter = geometryRoot->first_node();
    
    int count = 0;
    
    while (geometryIter != NULL)
    {
        count++;
        std::shared_ptr<TriMesh> mesh (new TriMesh());
        
        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<int> indices;
        
        xml_node<>* meshRoot = geometryIter->first_node("mesh");
        
        char* meshName = geometryIter->first_attribute("id")->value();

        //read in vert positions
        std::string meshAttribName = std::string(meshName)+"-positions";
        xml_node<>* meshPositionSource = getChild(meshRoot, "source", "id", meshAttribName);
        xml_node<>* floatPositionNode = meshPositionSource->first_node("float_array");
        
        char* positionsString = floatPositionNode->value();
        char* posIter = strtok(positionsString, " ");
        while (posIter!= NULL)
        {
            positions.push_back(atof(posIter));
            posIter = strtok(NULL," ");
        }
        
        //read in normal positions
        std::string normalAttribName = std::string(meshName)+"-normals";
        xml_node<>* meshNormalSource = getChild(meshRoot, "source", "id", normalAttribName);
        xml_node<>* normalPositionsNode = meshNormalSource->first_node("float_array");
        
        char* normalString = normalPositionsNode->value();
        char* nIter = strtok(normalString, " ");
        while (nIter != NULL)
        {
            normals.push_back(atof(nIter));
            nIter = strtok(NULL, " ");
        }
        
        //read in indices
        xml_node<>* polylistRoot = meshRoot->first_node("polylist");
        xml_node<>* polylistNode = polylistRoot->first_node("p");
        
        char* polyString = polylistNode->value();
        char* pIter = strtok(polyString, " ");
        while (pIter!= NULL)
        {
            indices.push_back(atoi(pIter));
            pIter = strtok(NULL," ");
        }
        
        //assemble triangles
        int numTris = atoi(polylistRoot->first_attribute("count")->value());
        
        for (int tri = 0; tri < numTris; tri++)
        {
            Triangle t;
            t.A = vec3( positions[indices[tri*6]],      positions[indices[tri*6] + 1],    positions[indices[tri*6] + 2 ]);
            t.na = vec3(normals  [indices[tri*6 + 1]],  normals[indices[tri*6 + 1] + 1],  normals[indices[tri*6 + 1] + 2]);
            
            t.B = vec3( positions[indices[tri*6 +2 ]],  positions[indices[tri*6 + 2] +1], positions[indices[tri*6 + 2]+2]);
            t.nb = vec3(normals[indices[tri*6+3]],      normals[indices[tri*6 + 3]+1],    normals[indices[tri*6 +3]+2]);
            
            t.C = vec3( positions[indices[tri*6 + 4]],  positions[indices[tri*6 + 4]+1],  positions[indices[tri*6 + 4]+2]);
            t.nc = vec3(normals[indices[tri*6 + 5]],    normals[indices[tri*6 + 5]+1],    normals[indices[tri*6 + 5]+2]);
        
            mesh->AddTriangle(t);
        }
        
        xml_node<>* visualSceneRoot = root->first_node("library_visual_scenes");
        xml_node<>* visualScene = visualSceneRoot->first_node("visual_scene");
        xml_node<>* visualMeshNode = getChild(visualScene, "node", "name", strtok(meshName, "-"));
        xml_node<>* meshMatrix = visualMeshNode->first_node("matrix");
        
        std::vector<float> meshElements;
        char* matrixString = meshMatrix->value();
        char* mIter = strtok(matrixString, " ");
        while (mIter != NULL)
        {
            meshElements.push_back(atof(mIter));
            mIter = strtok(NULL, " ");
        }
        
        mesh->setTransform(mat4(meshElements[0], meshElements[1], meshElements[2], meshElements[3],
                                    meshElements[4], meshElements[5], meshElements[6], meshElements[7],
                                    meshElements[8], meshElements[9], meshElements[10], meshElements[11],
                                    meshElements[12], meshElements[13], meshElements[14], meshElements[15]));
        

        
        xml_node<>* materialNode = visualMeshNode->first_node("instance_geometry");
        
        if (materialNode->first_node() != NULL)
        {
            xml_node<>* bindMaterial = materialNode->first_node();
            xml_node<>* techniqueCommon = bindMaterial->first_node("technique_common");
            xml_node<>* mat = techniqueCommon->first_node("instance_material");
            char* materialName = mat->first_attribute("symbol")->value();
            
            mesh->setMaterial(materials[materialName]);

        }
        
        geometryIter= geometryIter->next_sibling();
        
        //add Trimesh to Scene
        scene->traceables.push_back(mesh);
    }
    fprintf(stdout, "Success: Built %i meshes from collada file\n", count);

}

#pragma mark XMLParsingUtils

xml_node<>* ColladaSceneBuilder::getChild(xml_node<>* parent, std::string childName)
{
    xml_node<>* child = parent->first_node();
    while (strcmp( child->name(), childName.c_str()) != 0)
    {
        child = child->next_sibling();
        if (child == NULL)
        {
            return NULL;
        }
    }
    return child;
}

xml_node<>* ColladaSceneBuilder::getChild(xml_node<>* parent, std::string childName, std::string attributeName, std::string attributeVal)
{
    xml_node<>* child = parent->first_node(childName.c_str());
    while(strcmp(child->first_attribute(attributeName.c_str())->value(), attributeVal.c_str()) != 0)
    {
        child = child->next_sibling();
        if (child == NULL)
        {
            return NULL;
        }
    }
    return child;

}


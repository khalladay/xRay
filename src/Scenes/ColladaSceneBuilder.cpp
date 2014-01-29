//
//  ColladaSceneBuilder.cpp
//  xRay
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
    
    
    scene->cam.transform = mat4(elements[0], elements[4], elements[8], elements[12],
                                elements[1], elements[5], elements[9], elements[13],
                                elements[2], elements[6], elements[10], elements[14],
                                elements[3], elements[7], elements[11], elements[15]);
   /* scene->cam.transform = mat4(elements[0], elements[1], elements[2], elements[3],
                                elements[4], elements[5], elements[6], elements[7],
                                elements[8], elements[9], elements[10], elements[11],
                                elements[12], elements[13], elements[14], elements[15]);*/
    
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
            sceneLight = getChild(visScene, "node", "name", std::string(strtok(lightName, ".")));
            //fprintf(stderr, "ERROR: No light transform found in collada file. %s : %d!\n",__FILE__, __LINE__);
        }
        
        char* matrix = sceneLight->first_node("matrix")->value();
        
        lightPlist["transform"] = std::string(matrix);
        LightProperties properties = parseLightProperties(lightPlist);
        
        std::shared_ptr<Light> lightPtr = LightFactory::build(properties, lightPlist["type"]);
        
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
    
    char* effectName;
    
    int count = 0;
    while(effectIter != NULL)
    {
        count++;
        //char* effectName = effectIter->first_attribute("id")->value();
        std::map<std::string, std::string> materialProperties;
        effectName = effectIter->first_attribute("id")->value();
        
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
        
        MaterialProperties properties = parseMaterialProperties(materialProperties);
        std::shared_ptr<Material> m = MaterialFactory::build(properties, materialProperties["type"]);
        materials[effectName] = MaterialFactory::build(properties, materialProperties["type"]);
        
        effectIter = effectIter->next_sibling();
    }
    
    xml_node<>* libraryMaterialsRoot = getChild(root, "library_materials");
    xml_node<>* lmIter = libraryMaterialsRoot->first_node();
    
    while (lmIter != NULL)
    {
        xml_node<>* effectVal = lmIter->first_node();
        char* effectMatName = lmIter->first_attribute("id")->value();
        std::string matName = std::string(effectVal->first_attribute("url")->value());
        matName = matName.substr(1, matName.length());
        std::shared_ptr<Material> m = materials[effectName];
        materials[effectMatName] = materials[matName];
        lmIter = lmIter->next_sibling();
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
        mesh->name = meshName;
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
            t.A = vec3( positions[indices[tri*6]*3],      positions[indices[tri*6]*3 + 1],    positions[indices[tri*6]*3 + 2 ]);
            t.na = vec3(normals  [indices[tri*6 + 1]*3],  normals[indices[tri*6 + 1] *3+ 1],  normals[indices[tri*6 + 1]*3 + 2]);
            
            t.B = vec3( positions[indices[tri*6 +2 ]*3],  positions[indices[tri*6 + 2]*3 +1], positions[indices[tri*6 + 2]*3+2]);
            t.nb = vec3(normals[indices[tri*6+3]*3],      normals[indices[tri*6 + 3]*3+1],    normals[indices[tri*6 +3]*3+2]);
            
            t.C = vec3( positions[indices[tri*6 + 4]*3],  positions[indices[tri*6 + 4]*3+1],  positions[indices[tri*6 + 4]*3+2]);
            t.nc = vec3(normals[indices[tri*6 + 5]*3],    normals[indices[tri*6 + 5]*3+1],    normals[indices[tri*6 + 5]*3+2]);
            
            mesh->AddTriangle(t);
        }
        
        xml_node<>* visualSceneRoot = root->first_node("library_visual_scenes");
        xml_node<>* visualScene = visualSceneRoot->first_node("visual_scene");
        xml_node<>* visualMeshNode = getChild(visualScene, "node", "name", strtok(meshName, "-"));
        
        if (visualMeshNode == NULL)
        {
            fprintf(stdout, "Warning: Mesh in Library Geometries not found in Scene %s\n", meshName);
            geometryIter= geometryIter->next_sibling();
            continue;
        }
        
        xml_node<>* meshMatrix = visualMeshNode->first_node("matrix");
        
        std::vector<float> meshElements;
        char* matrixString = meshMatrix->value();
        char* mIter = strtok(matrixString, " ");
        while (mIter != NULL)
        {
            meshElements.push_back(atof(mIter));
            mIter = strtok(NULL, " ");
        }
        
        mesh->setTransform(mat4(meshElements[0], meshElements[4], meshElements[8], meshElements[12],
                                    meshElements[1], meshElements[5], meshElements[9], meshElements[13],
                                    meshElements[2], meshElements[6], meshElements[10], meshElements[14],
                                    meshElements[3], meshElements[7], meshElements[11], meshElements[15]));
        

        
        xml_node<>* materialNode = visualMeshNode->first_node("instance_geometry");
        
        if (materialNode->first_node() != NULL)
        {
            xml_node<>* bindMaterial = materialNode->first_node();
            xml_node<>* techniqueCommon = bindMaterial->first_node("technique_common");
            xml_node<>* mat = techniqueCommon->first_node("instance_material");
            char* materialName = mat->first_attribute("symbol")->value();
            
            std::shared_ptr<Material> m = materials[materialName];
            if (m != NULL)
            {
                mesh->setMaterial(m);
            }
            else
            {
                mesh->setMaterial(MaterialFactory::buildDefault());
            }
        }
        else mesh->setMaterial(MaterialFactory::buildDefault());
        
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

LightProperties ColladaSceneBuilder::parseLightProperties(std::map<std::string,std::string> plist)
{
    LightProperties properties;
    
    if (plist.find("transform") == plist.end())
    {
        fprintf(stderr, "No transformation matrix was found in LightProperties list %s : %d\n", __FILE__, __LINE__);
    }
    
    std::vector<float> matrixElements;
    char* elements = (char*)plist["transform"].c_str();
    char* eIter = strtok(elements, " ");
    while (eIter != NULL)
    {
        matrixElements.push_back(atof(eIter));
        eIter = strtok(NULL, " ");
    }
    
    
    properties.transform =  mat4(matrixElements[0], matrixElements[4], matrixElements[8], matrixElements[12],
                                 matrixElements[1], matrixElements[5], matrixElements[9], matrixElements[13],
                                 matrixElements[2], matrixElements[6], matrixElements[10], matrixElements[14],
                                 matrixElements[3], matrixElements[7], matrixElements[11], matrixElements[15]);
    
    
    
    
    if (plist.find("constant_attenuation") != plist.end())
    {
        properties.constantAttenuation = atof(plist["constant_attenuation"].c_str());
    }
    if (plist.find("linear_attenuation") != plist.end())
    {
        properties.linearAttenuation = atof(plist["linear_attenuation"].c_str());
    }
    if (plist.find("quadratic_attenuation") != plist.end())
    {
        properties.quadraticAttenuation = atof(plist["quadratic_attenuation"].c_str());
    }
    
    return properties;
}

MaterialProperties ColladaSceneBuilder::parseMaterialProperties(std::map<std::string, std::string> plist)
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




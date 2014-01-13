//
//  colladaparse.h
//  foRay
//
//  This is really ugly right now, but I just want to get better meshes in
//  so I can play with openCL atm.
//
//  Created by Kyle Halladay on 1/5/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef colladaparse_h
#define colladaparse_h

#include <vector>
#include "rapidxml.hpp"
#include "glm.hpp"

using namespace glm;
using namespace rapidxml;

namespace clp
{
    
    
    struct tri3
    {
        vec3 A;
        vec3 B;
        vec3 C;
        vec3 normal;
    };
    
    struct collada_geometry
    {
        
    };
    
    inline xml_node<>* getChild(xml_node<>* parent, std::string childName, std::string attributeName, std::string attributeVal)
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
    
    //returns a shared ptr to an array of tri3 objects
    //each tri3 contains all data for a standalone triange (no indices)
    inline std::shared_ptr<tri3> getRawTriangles(std::string filepath, std::string objectName)
    {
        char *chObjName = new char[objectName.length()];
        std::copy(objectName.begin(), objectName.end(), chObjName);
        
        //load contents of collada file into memory
        std::ifstream file(filepath);
        std::string contents = "";
        std::string temp;
        while(std::getline(file, temp))
        {
            contents.append(temp);
        }
        
        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<tri3> tris;

        //convert read in string to a char*
        char * chContents = new char[contents.size() + 1];
        std::copy(contents.begin(), contents.end(), chContents);
        chContents[contents.size()] = '\0';

        xml_document<> doc;
        doc.parse<0>(chContents);
        
        /************
         Get mesh node for object we want
         ************/
        
        xml_node<>* root = doc.first_node("COLLADA");
        xml_node<>* geomNode = root->first_node("library_geometries");

        geomNode = getChild(geomNode, "geometry", "name", objectName);
        
        xml_node<>* meshNode = geomNode->first_node("mesh");
        
        /************
         Get position values
         ************/
        
        char* positionId = new char[objectName.length()+15];
        strcat(positionId,objectName.c_str());
        strcat(positionId,"-mesh-positions");

        xml_node<>* sourcePositionNode = getChild(meshNode, "source", "id", std::string(positionId));
        
        char* positionNodeId = new char[objectName.length()+21];
        strcat(positionNodeId,objectName.c_str());
        strcat(positionNodeId,"-mesh-positions-array");
        
        xml_node<>* positionsNode = getChild(sourcePositionNode, "float_array", "id", std::string(positionNodeId));
        
        char* positionsString = positionsNode->value();
        char* posIter = strtok(positionsString, " ");
        while (posIter!= NULL)
        {
            positions.push_back(atof(posIter));
            posIter = strtok(NULL," ");
        }
        
    //    free(positionId);
      //  free(positionNodeId);
        
        /************
         Get normal values
         ************/
        char* normalId = chObjName;
       // strcat(normalId,objectName.c_str());
        strcat(normalId,"-mesh-normals");

        xml_node<>* sourceNormalsNode = getChild(meshNode, "source", "id", std::string(normalId));
        
       // char* normalNodeId =  new char[objectName.length()+19];
       // strcat(normalNodeId,objectName.c_str());
        strcat(normalId,"-array");
        
        xml_node<>* normalsNode = getChild(sourceNormalsNode, "float_array", "id", std::string(normalId));

        char* normalsString = normalsNode->value();
        char* nIter = strtok(normalsString, " ");
        while (nIter!= NULL)
        {
            normals.push_back(atof(nIter));
            nIter = strtok(NULL," ");
        }
                
        /************
         Get indices values
         ************/
        xml_node<>* polylistNode = meshNode->first_node("polylist");
        polylistNode = polylistNode->first_node("p");
        
        std::vector<int> indices;
        
        char* polyString = polylistNode->value();
        char* pIter = strtok(polyString, " ");
        while (pIter!= NULL)
        {
            indices.push_back(atoi(pIter));
            pIter = strtok(NULL," ");
        }
        
        
        /************
         Build Tris
         ************/
        
        for (int i = 0; i < indices.size(); i+=6)
        {
            tri3 tri;
            
            float x = positions[indices[i]];
            float y = positions[indices[i]+1];
            float z = positions[indices[i]+2];
        
            
            tri.A = vec3(x,y,z);
            
            float x1 = positions[indices[i+1]];
            float y1 = positions[indices[i+1]+1];
            float z1 = positions[indices[i+1]+2];
            
            tri.B = vec3(x1,y1,z1);
            
            float x2 = positions[indices[i+2]];
            float y2 = positions[indices[i+2]+1];
            float z2 = positions[indices[i+2]+2];
            
            tri.C = vec3(x2,y2,z2);
            
            float nX = normals[indices[i+3]];
            float nY = normals[indices[i+3]+1];
            float nZ = normals[indices[i+3]+2];
            
            tri.normal = vec3(nX, nY, nZ);
            
            tris.push_back(tri);
        }

        printf("Parsed: %i triangles\n", tris.size());
        std::shared_ptr<tri3> triArray(new tri3[tris.size()], std::default_delete<tri3[]>());
        
        return triArray;
    }
}

#endif

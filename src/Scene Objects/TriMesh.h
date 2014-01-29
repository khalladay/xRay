//
//  TriMesh.h
//  Clusterwink
//
//  Created by Kyle Halladay on 1/4/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __Clusterwink__TriMesh__
#define __Clusterwink__TriMesh__

#include "glm.hpp"
#include "Traceable.h"
#include "Triangle.h"
#include "Material.h"
#include <string>
#include <vector>

using namespace glm;

class TriMesh : public Traceable
{
public:
    std::vector<Triangle> triangles;
    TriMesh(){}
    
    void AddTriangle(Triangle t)
    {
        triangles.push_back(t);
    }
    
    bool intersect(Ray* r, RaycastHit* hit)
    {
        bool hitFlag = false;
        
        hit->t = 1000.0f;
        RaycastHit tempHit;
        for (int i = 0; i < triangles.size(); i++)
        {
            if (triangles[i].intersect(r, &tempHit))
            {
                if (tempHit.t < hit->t)
                {
                    hit->t = tempHit.t;
                    hit->u = tempHit.u;
                    hit->v = tempHit.v;
                    
                    hit->contactNormal = tempHit.contactNormal;
                    hitFlag = true;

                }
            }
        }
        
        return hitFlag;
    }
    
    void convertToWorldSpace()
    {
        for (int i = 0; i < triangles.size(); i++)
        {
            triangles[i].transformByMatrix(transform);
        }
    }
    
    void setTransform(mat4 tMatrix)
    {
        transform = tMatrix;
    }
    
    void setMaterial(std::shared_ptr<Material> mat)
    {
        material = mat;
    }

};
#endif /* defined(__Clusterwink__TriMesh__) */

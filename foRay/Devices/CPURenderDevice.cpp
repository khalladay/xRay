//
//  CPURenderDevice.cpp
//  foRay
//
//  Created by Kyle Halladay on 1/23/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#include "CPURenderDevice.h"

#define DEG2RAD 0.0174532925

std::unique_ptr<RenderDevice> createCPUDevice(std::unique_ptr<Scene> s)
{
	return std::unique_ptr<CPURenderDevice>(new CPURenderDevice(std::unique_ptr<Scene>(std::move(s))));
}

CPURenderDevice::CPURenderDevice(std::unique_ptr<Scene> s) : RenderDevice(std::unique_ptr<Scene>(std::move(s)))
{
    for (int i = 0; i < scene->traceables.size(); i++)
    {
        scene->traceables[i]->convertToWorldSpace();
    }
    fprintf(stdout, "Created CPU Device\n");
}

vec3 CPURenderDevice::traceNDCPoint(float ndcX, float ndcY)
{
    Ray r;
    RaycastHit hit;
    rayForPixel(&r, ndcX, ndcY);
    
    vec3 col = vec3(0.0);
    
    traceRay(&r, &hit, &col);
    
    return col;
}

void CPURenderDevice::traceRay(Ray* r, RaycastHit* hit, vec3* col)
{
    bool didHit = false;
    float t = r->tmax;
    
    for (int i = 0; i < scene->traceables.size(); i++)
    {
        if (scene->traceables[i]->intersect(r, hit))
        {
            if (hit->t <= t)
            {
                t = hit->t;
                didHit = true;
                
                vec3 hitPoint = r->origin + r->direction*hit->t;
                
                *col = scene->traceables[i]->material->sample(hitPoint, hit->contactNormal, scene->lights[0]->getDirection(), vec3(0.0f), vec3(0.0f));

            }
        }
    }
    
    if (!didHit)
    {
        *col = vec3(0.0f, 0.0f, 0.0f);
    }
}

void CPURenderDevice::rayForPixel(Ray* r, float ndcX, float ndcY)
{
    //convert NDC to Screen Space
    float remapX = ndcX;
    float remapY = ndcY;
    
    //adjust for aspect ratio and camera fov
    
    float camX = remapX * tan(( (scene->cam.fov / 2.0f)*DEG2RAD));
    float camY = remapY * tan(( (scene->cam.fov/2.0f)*DEG2RAD));

    //camera by default is always looking down negative Z
    //and is 1 unit away from camera plane
    
    //dir = camSpacePixel - origin, origin = 0,0,0
    
    //transform orign and camSpacePixel by camera matrix
    vec4 origin =  scene->cam.transform * vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 camSpacePixel = scene->cam.transform * vec4(camX, camY, -1.0f, 1.0f);
    //get the direction vector
    vec4 dir = normalize(camSpacePixel - origin);
    
    //origin, direction, tmin, tmax
    r->origin =vec3(origin.x, origin.y, origin.z);
    r->direction = vec3(dir.x, dir.y, dir.z);
    r->tmin = scene->cam.zNear;
    r->tmax = scene->cam.zFar;
}
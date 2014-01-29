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
    float t = r->tmax+1;
    
    for (int i = 0; i < scene->traceables.size(); i++)
    {
        if (scene->traceables[i]->intersect(r, hit))
        {
            if (hit->t <= t && hit->t > r->tmin)
            {
                t = hit->t;
                didHit = true;
                
                vec3 hitPoint = r->origin + r->direction*hit->t;
                if (hit->t < 0.1f)
                {
                    printf("%s\n", scene->traceables[i]->name.c_str());
                }
                //color at primary ray
                *col = scene->traceables[i]->material->sample(hitPoint, hit->contactNormal, scene->lights[0]->getDirection(), vec3(0.0f), vec3(0.0f));

                //color from shadow ray
                
               if (r->type == Ray::RayType::PrimaryRay)
                {
                    Ray shadowRay(Ray::RayType::ShadowRay, hitPoint, scene->lights[0]->getDirection(), r->tmin, r->tmax);
                    RaycastHit shadowHit;
                    vec3 shadowCol = vec3(0.0f);
                    
                    traceShadowRay(&shadowRay,scene->traceables[i].get(), &shadowHit, &shadowCol);
                    if (shadowHit.t < shadowRay.tmax)
                    {
                        *col = vec3(0.0f);
                    }
                }
            }
        }
    }
    
    if (!didHit)
    {
        hit->t = r->tmax+1;
        *col = vec3(0.05);
    }
}

void CPURenderDevice::traceShadowRay(Ray* r, Traceable* ignore, RaycastHit* hit, vec3* col)
{
    bool didHit = false;
    float t = r->tmax+1;
    
    for (int i = 0; i < scene->traceables.size(); i++)
    {
        if (scene->traceables[i].get() == ignore)
        {
            continue;
        }
        if (scene->traceables[i]->intersect(r, hit))
        {
            if (hit->t <= t && hit->t > 0.0001 )
            {
                t = hit->t;
                didHit = true;
                vec3 hitPoint = r->origin + r->direction*hit->t;
                *col = scene->traceables[i]->material->sample(hitPoint, hit->contactNormal, -scene->lights[0]->getDirection(), vec3(0.0f), vec3(0.0f));
            }
        }
    }
    
    if (!didHit)
    {
        hit->t = r->tmax+1;
        *col = vec3(0.05);
    }
}

void CPURenderDevice::rayForPixel(Ray* r, float ndcX, float ndcY)
{
    //convert NDC to Screen Space
    float remapX = ndcX;
    float remapY = ndcY;
    
    //adjust for aspect ratio and camera fov
    
    //TODO: explain the 3.0 instead of a 2.0
    //should be by 2, but blender appears to divide by 3 for some reason
    float camX = remapX * tan(( ((scene->cam.fov*DEG2RAD) / 3.0f)));
    float camY = remapY * tan(( ((scene->cam.fov*DEG2RAD) / 3.0f)));

    //camera by default is always looking down negative Z
    //and is 1 unit away from camera plane
    
    //dir = camSpacePixel - origin, origin = 0,0,0
    
    //transform orign and camSpacePixel by camera matrix
    vec4 origin =  scene->cam.transform * vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 camSpacePixel = scene->cam.transform * vec4(camX, camY, -1.0f, 1.0f);
    //get the direction vector
    vec4 dir = normalize(camSpacePixel - origin);
    
    cameraPosition = vec3(origin.x, origin.y, origin.z);
    //origin, direction, tmin, tmax
    r->type = Ray::RayType::PrimaryRay;
    r->origin =vec3(origin.x, origin.y, origin.z);
    r->direction = vec3(dir.x, dir.y, dir.z);
    r->tmin = scene->cam.zNear;
    r->tmax = scene->cam.zFar;
}
//
//  SceneTestRenderDevice.cpp
//  foRay
//
//  Created by Kyle Halladay on 1/26/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#include "SceneTestRenderDevice.h"
#include "Sphere.h"
#include "Material.h"
#include "ColladaSceneBuilder.h"
#include "Camera.h"
#include "TriMesh.h"
#include "LightFactory.h"
#include "MaterialFactory.h"

std::unique_ptr<RenderDevice> createSceneTestDevice(std::unique_ptr<Scene> s)
{
	return std::unique_ptr<SceneTestRenderDevice>(new SceneTestRenderDevice(std::unique_ptr<Scene>(std::move(s))));
}

SceneTestRenderDevice::SceneTestRenderDevice(std::unique_ptr<Scene> s) : RenderDevice(std::unique_ptr<Scene>(std::move(s)))
{
    for (int i = 0; i < scene->traceables.size(); i++)
    {
        scene->traceables[i]->convertToWorldSpace();
    }
        
    fprintf(stdout, "Created CPU Device\n");
}

vec3 SceneTestRenderDevice::traceNDCPoint(float ndcX, float ndcY)
{
    Ray r;
    RaycastHit hit;
    rayForPixel(&r, ndcX, ndcY);
    
    vec3 col = vec3(0.0);
    
    traceRay(&r, &hit, &col);
    
    return col;
}

void SceneTestRenderDevice::traceRay(Ray* r, RaycastHit* hit, vec3* col)
{
    vec3 LIGHT_DIR = normalize(vec3(-0.5, 0.0, -0.5));
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
                
                *col = scene->traceables[i]->material->sample(hitPoint, hit->contactNormal, LIGHT_DIR, vec3(0.0f), vec3(0.0f));
            }
        }
    }
    
    if (!didHit) *col= vec3(0.3f);
    return;
}

void SceneTestRenderDevice::rayForPixel(Ray* r, int x, int y)
{
    
    vec3 camPos = vec3(10.0f, 10.0f, 0.0f);
    vec3 lookPos = vec3(0.0f, 0.0f, 0.0f);
    vec3 camUp = vec3(0.0f, 0.0f, 1.0f);
    
    vec2 uv = -1.0f + 2.0f * vec2(x, y) / vec2(800.0f);
    
    vec3 lookDirection = normalize(lookPos - camPos);
    vec3 viewPlaneU = normalize(cross(camUp, lookDirection));
    vec3 viewPlaneV = cross(lookDirection, viewPlaneU);
    vec3 viewCenter = lookDirection + camPos;
    
    vec3 fragWorldPos = viewCenter + (uv.x * viewPlaneU * 1.0f) + (uv.y * viewPlaneV);
    vec3 fragWorldToCamPos = normalize(fragWorldPos - camPos);
    
    r->origin = camPos;
    r->direction = normalize(fragWorldToCamPos);
    r->tmin = 0.0;
    r->tmax = 1000.0f;
}


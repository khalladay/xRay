//
//  main.cpp
//  SimpleRayTracer
//
//  Created by Kyle Halladay on 12/11/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
//

#include <iostream>
#include "glm.hpp"
#include <fstream>
#include "Ray.h"
#include "RaycastHit.h"
#include "Sphere.h"
#include "Plane.h"
#include "Scene.h"
#include "CheckerMaterial.h"
#include "SpecularMaterial.h"

#define NUM_BOUNCES 4
using namespace glm;

float cube(vec3 point, vec3 box, vec3 boxPos)
{
    return length(max(abs(point - boxPos)-box,0.0));
}

void writeArrayToFile(vec3 array[512][512])
{
    std::ofstream ofs;
    ofs.open("/Users/kylehalladay/Desktop/testtrace5.ppm");
    ofs << "P6\n" << 512 << " " << 512 << "\n255\n";
    
    for (uint32_t j = 0; j < 512; ++j) {
		for (uint32_t i = 0; i < 512; ++i) {

            vec3 col = array[i][j];
            char pixel[3];
            pixel[0] = (unsigned char)min((int)(col.r*255.0f), 255);
            pixel[1] = (unsigned char)min((int)(col.g*255.0f), 255);
            pixel[2] = (unsigned char)min((int)(col.b*255.0f), 255);

            ofs.write(pixel, 3);
		}
	}
    ofs.close();
}

void trace(Ray* r, RaycastHit* hit, vec3* col, float uvy, Scene* scn, int bounces)
{
    vec3 LIGHT_DIR = normalize(vec3(1.0, 0.2, 0.0));
    bool didHit = false;
    
    float t = r->tmax+1.0f;
    
    for (int i = 0; i < scn->traceables.size(); i++)
    {
        if (scn->traceables[i]->intersect(r, hit))
        {
            if (hit->t < t)
            {
                float successfulBounces = 0;

                t = hit->t;
                didHit = true;
                
                vec3 hitPoint = r->origin + r->direction*hit->t;
                vec3 viewDirection = scn->cam.position - hitPoint;
                
                *col = scn->traceables[i]->material->sample(hitPoint, hit->contactNormal, LIGHT_DIR, vec3(0.0f), viewDirection);
                
                
                Ray bounceRay(r->origin + r->direction*hit->t, reflect(r->direction, hit->contactNormal), 0.0f, 100.0f);
                RaycastHit bounceHit;
                bounceHit.t = 101.0f;

                vec3 bounce = vec3(0.0f);
                
                for (int j = 0; j < bounces; j++)
                {
                    if (j > 0)
                    {
                        bounceRay.origin = bounceRay.origin + bounceRay.direction * bounceHit.t;
                        bounceRay.direction = reflect(bounceRay.direction, bounceHit.contactNormal);
                    }
                    
                    vec3 bounceCol = vec3(0.0f);
                    
                    trace(&bounceRay, &bounceHit, &bounceCol, uvy, scn, bounces-1);
                    
                    if (bounceHit.t < bounceRay.tmax)
                    {
                        bounce += bounceCol*(1.0f/((float)(j*0.5f)+1.0f));
                        successfulBounces++;
                    }
                }
                bounce = vec3(min(1.0f, bounce.x), min(1.0f, bounce.y), min(1.0f, bounce.z));
               
                if (NUM_BOUNCES > 0 && successfulBounces > 0)
                    *col = (*col+bounce) / (successfulBounces+1.0f);
            }
        }
    }
    
    if ( !didHit )
    {
        hit->t = 101.0f;
        *col = vec3(0.0f, 0.0f,uvy);
    }
    
    return;
}

int main(int argc, const char * argv[])
{
    vec3 image[512][512];
    
    Scene scene(vec3(13.0f, 3.0f, 10.0f), vec3(0.0f), vec3(0.0f, -1.0f, 0.0f));
    scene.traceables.push_back(new Sphere(vec3(0.0f, 4.0f, -10.0f), 4.0f, 1,new SpecularMaterial(vec3(1.0f, 0.0f, 0.0f), vec3(1.0f))));
    scene.traceables.push_back(new Sphere(vec3(0.0f, 4.0f, 0.0f), 4.0f, 1,new SpecularMaterial(vec3(0.0f, 1.0f, 1.0f), vec3(1.0f))));
    scene.traceables.push_back(new Sphere(vec3(0.0f, 4.0f, 10.0f), 4.0f, 1, new SpecularMaterial(vec3(1.0f, 0.0f, 1.0f), vec3(1.0f))));
    scene.traceables.push_back(new Plane(vec3(0.0, 0.0, 0.0), normalize(vec3(0.0, -1.0,0.0)), new CheckerMaterial()));

    
    vec3 c0,c1,c2,c3;
    
    for (int y = 0; y < 512; y++)
    {
        for (int x = 0; x < 512; x++)
        {
            vec2 uv0 = -1.0f + 2.0f * vec2(x-0.5f,y) / vec2(512.0f);
            vec2 uv1 = -1.0f + 2.0f * vec2(x+0.5f,y) / vec2(512.0f);
            vec2 uv2 = -1.0f + 2.0f * vec2(x,y-0.5f) / vec2(512.0f);
            vec2 uv3 = -1.0f + 2.0f * vec2(x,y+0.5f) / vec2(512.0f);

            vec3 lookDirection = normalize(scene.cam.lookPoint - scene.cam.position);
            vec3 viewPlaneU = normalize(cross(scene.cam.up, lookDirection));
            vec3 viewPlaneV = cross(lookDirection, viewPlaneU);
            vec3 viewCenter = lookDirection + scene.cam.position;
            
            vec3 fragWorldPos0 = viewCenter + (uv0.x * viewPlaneU * 1.0f) + (uv0.y * viewPlaneV);
            vec3 fragWorldPos1 = viewCenter + (uv1.x * viewPlaneU * 1.0f) + (uv1.y * viewPlaneV);
            vec3 fragWorldPos2 = viewCenter + (uv2.x * viewPlaneU * 1.0f) + (uv2.y * viewPlaneV);
            vec3 fragWorldPos3 = viewCenter + (uv3.x * viewPlaneU * 1.0f) + (uv3.y * viewPlaneV);

            vec3 fragWorldToCamPos0 = normalize(fragWorldPos0 - scene.cam.position);
            vec3 fragWorldToCamPos1 = normalize(fragWorldPos1 - scene.cam.position);
            vec3 fragWorldToCamPos2 = normalize(fragWorldPos2 - scene.cam.position);
            vec3 fragWorldToCamPos3 = normalize(fragWorldPos3 - scene.cam.position);

            vec3 col = vec3(0.3);
            
            Ray r0(scene.cam.position, normalize(fragWorldToCamPos0), 0.0f, 100.0f);
            Ray r1(scene.cam.position, normalize(fragWorldToCamPos1), 0.0f, 100.0f);
            Ray r2(scene.cam.position, normalize(fragWorldToCamPos2), 0.0f, 100.0f);
            Ray r3(scene.cam.position, normalize(fragWorldToCamPos3), 0.0f, 100.0f);

            RaycastHit h0;
            RaycastHit h1;
            RaycastHit h2;
            RaycastHit h3;

            
            trace(&r0, &h0, &c0, uv0.y, &scene, NUM_BOUNCES);
            trace(&r1, &h1, &c1, uv1.y, &scene, NUM_BOUNCES);
            trace(&r2, &h2, &c2, uv2.y, &scene, NUM_BOUNCES);
            trace(&r3, &h3, &c3, uv3.y, &scene, NUM_BOUNCES);

            image[x][y] = (c0+c1+c2+c3)/4.0f;
        }
    }
    
    writeArrayToFile(image);
    
    return 0;
}


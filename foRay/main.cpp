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
#include "TriMesh.h"

#define NUM_BOUNCES 4
#define ANTI_ALIAS 3
using namespace glm;

float cube(vec3 point, vec3 box, vec3 boxPos)
{
    return length(max(abs(point - boxPos)-box,0.0));
}

void writeArrayToFile(vec3 array[512][512])
{
    std::ofstream ofs;
    ofs.open("/Users/kylehalladay/Desktop/testtrace_alias16.ppm");
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
    vec3 LIGHT_DIR = normalize(vec3(-0.5, 0.0, -0.5));
    bool didHit = false;
    
    float t = r->tmax;
    
    for (int i = 0; i < scn->traceables.size(); i++)
    {
        if (scn->traceables[i]->intersect(r, hit))
        {
            if (hit->t <= t)
            {
                float successfulBounces = 0;

                t = hit->t;
                didHit = true;
                
                vec3 hitPoint = r->origin + r->direction*hit->t;
                vec3 viewDirection = scn->cam.position - hitPoint;
             //   *col = abs(hit->contactNormal);
                *col = scn->traceables[i]->material->sample(hitPoint, hit->contactNormal, LIGHT_DIR, vec3(0.0f), viewDirection);
                
                
                Ray bounceRay(r->origin + r->direction*hit->t, (reflect(r->direction, hit->contactNormal)), 0.0f, 900.1f);
                RaycastHit bounceHit;

                vec3 bounce = vec3(0.0f);
                
                for (int j = 0; j < bounces; j++)
                {
                    if (j > 0)
                    {
                        bounceRay.origin = bounceRay.origin + bounceRay.direction * bounceHit.t;
                        bounceRay.direction = reflect(bounceRay.direction, bounceHit.contactNormal);
                    }
                    
                    vec3 bounceCol;
                    
                    trace(&bounceRay, &bounceHit, &bounceCol, uvy, scn, bounces-1);
                    
                    if (bounceHit.t <= bounceRay.tmax && bounceHit.t > bounceRay.tmin)
                    {
                        bounce += bounceCol * (1.0f/((float)(j*0.5f)+1.0f));
                        successfulBounces++;
                    }
                    else break;
                }
                bounce /= successfulBounces;
                bounce = vec3(min(1.0f, bounce.x), min(1.0f, bounce.y), min(1.0f, bounce.z));
               
                if (NUM_BOUNCES > 0 && successfulBounces > 0)
                {
                    *col = (*col+ (*col*(bounce*0.8f))) / (2.0f);
                }
            }
        }
    }
    
    if ( !didHit )
    {
        hit->t = r->tmax+1.0f;
        *col = vec3(0.0f, 0.0f,uvy);
    }
    
    return;
}

int main(int argc, const char * argv[])
{
    vec3 image[512][512];
    
    Scene scene(vec3(-18.0f, 7.5f, -11.0f), vec3(0.0f, 4.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));
    TriMesh* mesh2 = new TriMesh(new SpecularMaterial(vec3(1.0,1.0,1.0), vec3(0.0,0.0,1.0f)));
    mesh2->AddTriangle(vec3(-14.0f, 0.0f, 25.0f), vec3(-14.0f, 38.0f, 15.0f), vec3(14.0f, 38.0f, 15.0f));
    mesh2->AddTriangle(vec3(-14.0f, 0.0f, 25.0f), vec3(14.0f, 38.0f, 15.0f), vec3(14.0f, 0.0f, 25.0f));

    TriMesh* mesh =  new TriMesh(new SpecularMaterial(vec3(1.0,0.0,0.0), vec3(1.0f)));
    mesh->AddTriangle(vec3(0.0f, 0.0f, 0.0f), vec3(-2.0f, 3.0f, -3.0f), vec3(2.0f, 3.0f, -3.0f));
    mesh->AddTriangle(vec3(0.0f, 0.0f, 0.0f), vec3(-2.0f, 3.0f, 3.0f),vec3(-2.0f, 3.0f, -3.0f));
    mesh->AddTriangle(vec3(0.0f, 0.0f, 0.0f),vec3(2.0f, 3.0f, -3.0f), vec3(2.0f, 3.0f, 3.0f));
    mesh->AddTriangle(vec3(0.0f, 0.0f, 0.0f),vec3(2.0f, 3.0f, 3.0f),vec3(-2.0f, 3.0f, 3.0f));

    
    mesh->AddTriangle(vec3(2.0f, 3.0f, -3.0f), vec3(-2.0f, 3.0f, -3.0f),vec3(0.0f, 9.0f, 0.0f));
    mesh->AddTriangle(vec3(0.0f, 9.0f, 0.0f), vec3(-2.0f, 3.0f, -3.0f), vec3(-2.0f, 3.0f, 3.0f));
    mesh->AddTriangle(vec3(0.0f, 9.0f, 0.0f), vec3(2.0f, 3.0f, 3.0f), vec3(2.0f, 3.0f, -3.0f));
    mesh->AddTriangle(vec3(0.0f, 9.0f, 0.0f), vec3(-2.0f, 3.0f, 3.0f), vec3(2.0f, 3.0f, 3.0f));

    scene.traceables.push_back(mesh);
   // scene.traceables.push_back(mesh2);

    scene.traceables.push_back(new Sphere(vec3(0.0f, 4.0f, -10.0f), 4.0f, 1,new SpecularMaterial(vec3(1.0f, 1.0f, 0.0f), vec3(1.0f))));
    //scene.traceables.push_back(new Sphere(vec3(0.0f, 4.0f, 0.0f), 2.0f, 1,new SpecularMaterial(vec3(0.0f, 1.0f, 1.0f), vec3(1.0f))));
    scene.traceables.push_back(new Sphere(vec3(0.0f, 4.0f, 10.0f), 4.0f, 1, new SpecularMaterial(vec3(1.0f, 0.0f, 1.0f), vec3(1.0f))));
    scene.traceables.push_back(new Plane(vec3(0.0, 0.0, 0.0), normalize(vec3(0.0, -1.0,0.0)), new CheckerMaterial()));

    
    for (int y = 0; y < 512; y++)
    {
        for (int x = 0; x < 512; x++)
        {
            vec3 col = vec3(0.0f);
            int count = 0;
            for (float antiX = - 0.5f; antiX < 0.5f; antiX+=(1.0f/ANTI_ALIAS))
            {
                //float antiY = 0.0f;
                for (float antiY =  - 0.5f; antiY < 0.5f; antiY += (1.0f/ANTI_ALIAS))
                {
                    vec2 uv = -1.0f + 2.0f * vec2(x + antiX, y + antiY) / vec2(512.0f);
                    vec3 lookDirection = normalize(scene.cam.lookPoint - scene.cam.position);
                    vec3 viewPlaneU = normalize(cross(scene.cam.up, lookDirection));
                    vec3 viewPlaneV = cross(lookDirection, viewPlaneU);
                    vec3 viewCenter = lookDirection + scene.cam.position;
                    
                    vec3 fragWorldPos = viewCenter + (uv.x * viewPlaneU * 1.0f) + (uv.y * viewPlaneV);
                    vec3 fragWorldToCamPos = normalize(fragWorldPos - scene.cam.position);
                    Ray r0(scene.cam.position, normalize(fragWorldToCamPos), 0.0f, 100.0f);
                    RaycastHit h0;
                    vec3 c = vec3(0.0f);
                    trace(&r0, &h0, &c, uv.y, &scene, NUM_BOUNCES);
                    col += c;
                    count++;

                }
            }
            col = col / (float)count;

            image[x][y] =col;
        }
    }
    
    writeArrayToFile(image);
    
    return 0;
}


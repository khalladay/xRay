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
#include "Scene.h"
#include "ColladaSceneBuilder.h"
#include "SceneDirector.h"

#define NUM_BOUNCES 0
#define ANTI_ALIAS 2
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
    /* arguments:
     * input path: path to the input scene file
     * output path: directory */
    if (argc == 0 || argc > 2)
    {
        printf("Incorrect ");
    }
    vec3 image[512][512];

    std::shared_ptr<Scene> scene = SceneDirector::buildScene(new ColladaSceneBuilder("/Users/kylehalladay/Documents/Development/Mac OS X/foRay/res/scene1.dae"));
    
    return 0;
}


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

using namespace glm;

float cube(vec3 point, vec3 box, vec3 boxPos)
{
    return length(max(abs(point - boxPos)-box,0.0));
}

void writeArrayToFile(vec3 array[512][512])
{
    std::ofstream ofs;
    ofs.open("/Users/kylehalladay/Desktop/testtrace.ppm");
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

bool trace(Ray* r, RaycastHit* hit, vec3* col)
{
    Sphere sphere1(vec3(0.0, 2.0, -10.0), 4.0f, 1);
    Sphere sphere2(vec3(0.0, 2.0, 0.0), 4.0f, 1);
    Sphere sphere3(vec3(0.0, 2.0, 10.0), 4.0f, 1);
    Plane plane(vec3(0.0, 0.0, 0.0), normalize(vec3(0.0, -1.0,0.0)));
    vec3 LIGHT_DIR = vec3(0.0, 1.0, 0.0);

    
    if (plane.intersect(r, hit))
    {
        if (hit->t < r->tmax && hit->t > 0.0)
        {
            vec3 h = r->origin + r->direction*hit->t;
            
            h=h*0.2f; //controls size of checkerboard bands
            
            //generate checkerboard texture
            //if h.x + h.z is odd, shade white, else shade black
            vec3 floorcol = vec3( (int)(ceil(h.x) + ceil(h.z))&1?(vec3(0.0f, 0.0f, 0.0f)):(vec3(1.0f, 1.0f, 1.0f)));
            
            Ray r2(r->origin + r->direction*hit->t, reflect(r->direction, hit->contactNormal), 0.0f, 100.0f);
            RaycastHit hit2;
            hit2.t = 101.0f;
            vec3 bounceCol = vec3(0.0);
            
            if (trace(&r2, &hit2, &bounceCol))
            {
                if (hit2.t < r2.tmax && hit2.t > 0.0)
                {
                    *col = (floorcol + bounceCol + bounceCol) / 3.0f;
                }
                else *col = floorcol;
            }
            else *col = floorcol;
        }
    }
    
    
    float t = hit->t;
    
    if (sphere1.intersect(r, hit))
    {
        if (hit->t < t)
        {
            vec3 sphereCol = vec3(1.0,0.0,1.0)*max(0.0f, dot(LIGHT_DIR,hit->contactNormal));
            t = hit->t;
            
            Ray r2(r->origin + r->direction*hit->t, reflect(r->direction, hit->contactNormal), 0.0f, 100.0f);
            RaycastHit hit2;
            hit2.t = 101.0f;
            vec3 bounceCol = vec3(0.0);
            
            if (trace(&r2, &hit2, &bounceCol))
            {
                if (hit2.t < r2.tmax)
                {
                    *col = (sphereCol + (bounceCol*0.4f)) / 2.0f;
                }
                else *col = sphereCol;
            }
            else *col = sphereCol;
        }
    }
    
    if (sphere2.intersect(r, hit))
    {
        if (hit->t < t)
        {
            vec3 sphereCol = vec3(0.0,0.0,1.0)*max(0.0f, dot(LIGHT_DIR,hit->contactNormal));
            t = hit->t;
            
            Ray r2(r->origin + r->direction*hit->t, reflect(r->direction, hit->contactNormal), 0.0f, 100.0f);
            RaycastHit hit2;
            hit2.t = 101.0f;
            vec3 bounceCol = vec3(0.0);
            
            if (trace(&r2, &hit2, &bounceCol))
            {
                if (hit2.t < r2.tmax)
                {
                    *col = (sphereCol + (bounceCol*0.4f)) / 2.0f;
                }
                else *col = sphereCol;
            }
            else *col = sphereCol;
        }
    }

    
    if (sphere3.intersect(r, hit))
    {
        if (hit->t < t)
        {
            vec3 sphereCol = vec3(1.0,0.0,0.0)*max(0.0f, dot(LIGHT_DIR,hit->contactNormal));
            t = hit->t;
            
            Ray r2(r->origin + r->direction*hit->t, reflect(r->direction, hit->contactNormal), 0.0f, 100.0f);
            RaycastHit hit2;
            hit2.t = 101.0f;
            vec3 bounceCol = vec3(0.0);
            
            if (trace(&r2, &hit2, &bounceCol))
            {
                if (hit2.t < r2.tmax)
                {
                    *col = (sphereCol + (bounceCol*0.4f)) / 2.0f;
                }
                else *col = sphereCol;
            }
            else *col = sphereCol;
        }
    }

    
    return hit->t < r->tmax && hit->t > 0.0;
}

int main(int argc, const char * argv[])
{
    vec3 image[512][512];

    
    for (int y = 0; y < 512; y++)
    {
        for (int x = 0; x < 512; x++)
        {
            vec2 uv = -1.0f + 2.0f * vec2(x,y) / vec2(512.0f);
            
            const vec3 CAM_UP = normalize(vec3(0.0, -1.0, 0.0));
            vec3 CAM_POS = vec3(13.0, 14.0,10.0);
            vec3 CAM_LOOKPOINT = vec3(0.0, 0.0, 0.0);
            
            vec3 lookDirection = normalize(CAM_LOOKPOINT - CAM_POS);
            vec3 viewPlaneU = normalize(cross(CAM_UP, lookDirection));
            vec3 viewPlaneV = cross(lookDirection, viewPlaneU);
            vec3 viewCenter = lookDirection + CAM_POS;
            
            vec3 fragWorldPos = viewCenter + (uv.x * viewPlaneU * 1.0f) + (uv.y * viewPlaneV);
            vec3 fragWorldToCamPos = normalize(fragWorldPos - CAM_POS);
            
            vec3 col = vec3(0.3);
            
            Ray r(CAM_POS, normalize(fragWorldToCamPos), 0.0f, 100.0f);
            
            RaycastHit hit;
            
            if (trace(&r, &hit, &col))
            {
             
            }
            else
            {
                col = vec3(0.0, 0.0, uv.y);
            }
            
           
            
            image[x][y] = col;
        }
    }
    
    writeArrayToFile(image);
    
    return 0;
}


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
#include "Scene.h"
#include "ColladaSceneBuilder.h"
#include "SceneDirector.h"
#include "Renderer.h"
#include "RenderDevice.h"

using namespace glm;

void writeArrayToFile(foray_image imageData, int width, int height)
{
    fprintf(stdout, "Writing file");
    std::ofstream ofs;
    ofs.open("/Users/kylehalladay/Desktop/foray_output2.ppm");
    ofs << "P6\n" << width << " " << height << "\n255\n";
    
    for (uint32_t x = 0; x < height; ++x) {
		for (uint32_t y = 0; y < width; ++y) {

            vec3 col = imageData[y][x];
            char pixel[3];
            pixel[0] = (unsigned char)min((int)(col.r*255.0f), 255);
            pixel[1] = (unsigned char)min((int)(col.g*255.0f), 255);
            pixel[2] = (unsigned char)min((int)(col.b*255.0f), 255);

            ofs.write(pixel, 3);
		}
	}
    ofs.close();
}


int main(int argc, const char * argv[])
{
    RenderProductInfo info;
    info.imageHeight = 600;
    info.imageWidth = 800;

    std::unique_ptr<Scene> scene = SceneDirector::buildScene(new ColladaSceneBuilder("/Users/kylehalladay/Documents/Development/Mac OS X/foRay/res/color2.dae"));
    Renderer renderer(&info, RenderDevice::create(std::unique_ptr<Scene>(std::move(scene))));
    foray_image image = renderer.render();
    
    writeArrayToFile(image, info.imageWidth, info.imageHeight);
    
    return 0;
}


//
//  main.cpp
//  SimpleRayTracer
//
//  Created by Kyle Halladay on 12/11/2013.
//  Copyright (c) 2013 Kyle Halladay. All rights reserved.
// "/Users/kylehalladay/Documents/Development/Mac OS X/foRay/res/poly_scene2.dae"
// "/Users/kylehalladay/Desktop/foray_output3.ppm"

#include <iostream>
#include "glm.hpp"
#include <fstream>
#include "Scene.h"
#include "ColladaSceneBuilder.h"
#include "SceneDirector.h"
#include "Renderer.h"
#include "RenderDevice.h"

using namespace glm;

void writeArrayToFile(foray_image imageData, int width, int height, const char* outputFilePath)
{
    fprintf(stdout, "Writing file");
    std::ofstream ofs;
    ofs.open(outputFilePath);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    
    for (uint32_t x = 0; x < height; ++x) {
		for (uint32_t y = 0; y < width; ++y) {

            vec3 col = imageData[y][x];
            char pixel[3];
            pixel[0] = (unsigned char)(std::max(0.f, std::min(255.f, powf(col.r, 1.0f/2.0) * 255.0f + 0.5f)));
            pixel[1] = (unsigned char)(std::max(0.f, std::min(255.f, powf(col.g, 1.0f/2.0) * 255.0f + 0.5f)));
            pixel[2] = (unsigned char)(std::max(0.f, std::min(255.f, powf(col.b, 1.0f/2.0) * 255.0f + 0.5f)));


            ofs.write(pixel, 3);
		}
	}
    ofs.close();
}


int main(int argc, const char * argv[])
{
  

    RenderProductInfo info;
    info.imageHeight = 640;
    info.imageWidth = 960;

    std::unique_ptr<Scene> scene = SceneDirector::buildScene(new ColladaSceneBuilder(std::string(argv[1])));
    Renderer renderer(&info, RenderDevice::create(std::unique_ptr<Scene>(std::move(scene))));
    foray_image image = renderer.render();
    
    writeArrayToFile(image, info.imageWidth, info.imageHeight, argv[2]);
    
    return 0;
}


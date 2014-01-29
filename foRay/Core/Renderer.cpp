//
//  Renderer.cpp
//  foRay
//
//  Created by Kyle Halladay on 1/22/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#include "Renderer.h"

Renderer::Renderer(RenderProductInfo* productInfo, std::unique_ptr<RenderDevice> d) : imageData(boost::extents[productInfo->imageWidth][productInfo->imageHeight])
{
    completed = false;
    info = productInfo;
    numCores = boost::thread::hardware_concurrency();
    device = std::move(d);

}

foray_image Renderer::render()
{
    //construct list of render tasks
    
    int tileHeight = info->imageHeight/2;
    int tileWidth = info->imageWidth / (numCores/2);
    
    for (int i = 0; i < numCores / 2; i++)
    {
        ivec2 originA = ivec2(i*tileWidth,0);
        ivec2 originB = ivec2(i*tileWidth, tileHeight);
        
        RenderTask taskA(originA.x, originA.x+tileWidth, originA.y, originA.y+tileHeight);
        RenderTask taskB(originB.x, originB.x+tileWidth, originB.y, originB.y+tileHeight);
        
        boost::thread* threadA = new boost::thread(boost::bind(&Renderer::renderTile, this, taskA));
        boost::thread* threadB = new boost::thread(boost::bind(&Renderer::renderTile, this, taskB));
        
        threads.add_thread(threadA);
        threads.add_thread(threadB);
    }
    
    //start all tasks (blocks until completed)
    threads.join_all();
        
    return imageData;
}

void Renderer::renderTile(RenderTask task)
{
    
    for (int x = task.xMin; x < task.xMax; x++)
    {
        for (int y = task.yMin; y < task.yMax; y++)
        {
            
            float ndcX = (2 * (x + 0.5) / (float)info->imageWidth - 1) * ((float)info->imageWidth/(float)info->imageHeight);
            float ndcY = (1 - 2 * (y + 0.5) / (float)info->imageHeight);
            
            imageData[x][y] = device->traceNDCPoint(ndcX, ndcY);
        }
    }
}
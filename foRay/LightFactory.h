//
//  LightFactory.h
//  foRay
//
//  Created by Kyle Halladay on 1/16/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __foRay__LightFactory__
#define __foRay__LightFactory__

#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "glm.hpp"

/* Light factory is responsible for converting a map<string,string> or values
 * from the format parser to a LightProperties struct, and returning an appropriate
 * light object constructed with that struct */

class LightFactory
{
public:
    static std::shared_ptr<Light> build(std::map<std::string,std::string> plist)
    {
        
        LightProperties properties = LightFactory::parsePropertyPlist(plist);
        if (plist["type"].compare("point") == 0)
        {
            return std::shared_ptr<Light>(new PointLight(properties));
        }
        
        if (plist["type"].compare("directional") == 0)
        {
            return std::shared_ptr<Light>(new DirectionalLight(properties));
        }
        
        return NULL;
    }
    
    static LightProperties parsePropertyPlist(std::map<std::string,std::string> plist)
    {
        LightProperties properties;
        
        if (plist.find("transform") == plist.end())
        {
            fprintf(stderr, "No transformation matrix was found in LightProperties list %s : %d\n", __FILE__, __LINE__);
        }
        
        std::vector<float> matrixElements;
        char* elements = (char*)plist["transform"].c_str();
        char* eIter = strtok(elements, " ");
        while (eIter != NULL)
        {
            matrixElements.push_back(atof(eIter));
            eIter = strtok(NULL, " ");
        }
        
        
        properties.transform =  mat4(elements[0], elements[1], elements[2], elements[3],
                                elements[4], elements[5], elements[6], elements[7],
                                elements[8], elements[9], elements[10], elements[11],
                                elements[12], elements[13], elements[14], elements[15]);
        
        

        
        if (plist.find("constant_attenuation") != plist.end())
        {
            properties.constantAttenuation = atof(plist["constant_attenuation"].c_str());
        }
        if (plist.find("linear_attenuation") != plist.end())
        {
            properties.linearAttenuation = atof(plist["linear_attenuation"].c_str());
        }
        if (plist.find("quadratic_attenuation") != plist.end())
        {
            properties.quadraticAttenuation = atof(plist["quadratic_attenuation"].c_str());
        }
        
        return properties;

    }
};
#endif /* defined(__foRay__LightFactory__) */

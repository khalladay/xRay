//
//  util_array.h
//  foRay
//
//  Created by Kyle Halladay on 1/26/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef foRay_util_array_h
#define foRay_util_array_h

#include "glm.hpp"
#include <boost/multi_array.hpp>

typedef boost::multi_array<glm::vec3, 2> foray_image;

template< typename T >
struct array_deleter
{
    void operator ()( T const * p)
    {
        delete[] p;
    }
};

#endif

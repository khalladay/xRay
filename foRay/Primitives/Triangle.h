//
//  Triangle.h
//  Clusterwink
//
//  Created by Kyle Halladay on 1/4/2014.
//  Copyright (c) 2014 Kyle Halladay. All rights reserved.
//

#ifndef __Clusterwink__Triangle__
#define __Clusterwink__Triangle__

#include "glm.hpp"
#include "Traceable.h"

class Triangle : public Traceable
{
public:
    vec3 A;
    vec3 B;
    vec3 C;
    
    vec3 na;
    vec3 nb;
    vec3 nc;
    
    vec3 N;
    mat4 transform;
    
    bool intersect(Ray* r, RaycastHit* hit);
    
    Triangle(vec3 a, vec3 b, vec3 c, vec3 n, std::shared_ptr<Material> m ) : A(a), B(b), C(c), N(n)
    {
        material = m;
    }
    
    vec3 normalForBarycentricPoint(float u, float v)
    {
        vec3 N = na + u * (nb - na) + v * (nc - na);
        N /= length(N);
        return normalize(N);
    }
    
    void transformByMatrix(glm::mat4 t)
    {
        transform = t;
        vec4 ta = transform * vec4(A.x, A.y, A.z, 1.0f);
        A = vec3(ta.x, ta.y, ta.z);
        
        vec4 tb = transform * vec4(B.x, B.y, B.z, 1.0f);
        B = vec3(tb.x, tb.y, tb.z);
        
        vec4 tc = transform * vec4(C.x, C.y, C.z, 1.0f);
        C = vec3(tc.x, tc.y, tc.z);
        
        vec4 tna = transform * vec4(na.x, na.y, na.z, 0.0f);
        na = vec3(tna.x, tna.y, tna.z);
        
        vec4 tnb = transform * vec4(nb.x, nb.y, nb.z, 0.0f);
        nb = vec3(tnb.x, tnb.y, tnb.z);
        
        vec4 tnc = transform * vec4(nc.x, nc.y, nc.z, 0.0f);
        nc = vec3(tnc.x, tnc.y, tnc.z);
        
        vec4 tN = transform * vec4(N.x, N.y, N.z, 1.0f);
        N = vec3(tN.x, tN.y, tN.z);
    }
    
    Triangle(){}
    
};

#endif /* defined(__Clusterwink__Triangle__) */

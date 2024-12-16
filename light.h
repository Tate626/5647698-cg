#pragma once
#include"math.h"

class Light
{
public:
    
    vec3 dir;
    vec3 color;


    Light() {
        dir = vec3(0.5f, -1.0f, 0.5f);
        color = vec3(1.0f, 1.0f, 1.0f); 
    }

};
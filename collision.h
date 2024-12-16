#pragma once
#include"math.h"
#include"dxcore.h"
#include"Shaders.h"
#include"GEMLoader.h"
#include"texture.h"
#include"camera.h"
#include"mesh.h"

class AABB
{
public:
    vec3 pmax;
    vec3 pmin;
    AABB() { reset(); }
    void reset()
    {
        pmax = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        pmin = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    }
    void extend(const vec3& p)
    {
        pmax = Max(pmax, p);
        pmin = Min(pmin, p);
    }
};

class spherebox {
public:
    vec3 centre;
    float radius;
    spherebox(){
        
    }
    spherebox(vec3 cen,float r) {
        centre = cen;
        radius = r;
    }

    void set(vec3 cen, float r) {
        centre = cen;
        radius = r;
    }

    bool check(spherebox& box2) {
        float distance = (box2.centre.x-centre.x)* (box2.centre.x - centre.x)+
            (box2.centre.y - centre.y) * (box2.centre.y - centre.y) + 
            (box2.centre.z - centre.z) * (box2.centre.z - centre.z);
        return distance < ((radius + box2.radius) * (radius + box2.radius));
    }
    bool check(AABB& aabb) {
        vec3 p;
        p.x = max(aabb.pmin.x, min(centre.x, aabb.pmax.x));
        p.y = max(aabb.pmin.y, min(centre.y, aabb.pmax.y));
        p.z = max(aabb.pmin.z, min(centre.z, aabb.pmax.z));
        float distance = (centre.x - p.x) * (centre.x - p.x) +
            (centre.y - p.y) * (centre.y - p.y) +
            (centre.z - p.z) * (centre.z - p.z);
        return distance < radius * radius;
    }
};

class ray
{
public:
    vec3 o;
    vec3 dir;
    vec3 invdir;
    ray() {}
    ray(const vec3 _o, const vec3 _dir) {
        init(_o, _dir);
    }
    void init(const vec3 _o, const vec3 _dir) {
        o = _o;
        dir = _dir;
        invdir = vec3(1.0f, 1.0f, 1.0f) / dir;
    }
    vec3 at(const float t) {
        return (o + (dir * t));
    }
};

bool raySphere(const ray& ray, spherebox &box) {
    vec3 oc = ray.o - box.centre;
    float A = ray.dir.Dot(ray.dir);
    float B = 2.0f * ray.dir.Dot(oc); 
    float C = oc.Dot(oc) - box.radius * box.radius;

    float discriminant = B * B - 4 * A * C; 
    if (discriminant <= 0) {
        return false; 
    }
    else {
        return true;
    }
}
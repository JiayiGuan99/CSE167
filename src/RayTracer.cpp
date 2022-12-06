#include "RayTracer.h"
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;
using namespace RayTracer;

void RayTracer::Raytrace(Camera cam, RTScene scene, Image& image)
{
    int w = image.width;
    int h = image.height;
    int recursion_depth = 6;
    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            Ray ray = RayThruPixel(cam, i, j, w, h);
            Intersection hit = Intersect(ray, &scene);
            image.pixels[i + j * w] = FindColor(hit, recursion_depth, &scene);
        }
    }
}

Ray RayTracer::RayThruPixel(Camera cam, int i, int j, int width, int height)
{
    float alpha = ((2 * i + 1) / width) - 1;
    float beta = 1 - (2 * j + 1) / height;
    glm::vec3 eye = cam.eye;
    glm::vec3 w = glm::normalize(cam.eye - cam.target);
    glm::vec3 u = glm::normalize(glm::cross(cam.up, w));
    glm::vec3 v = glm::cross(w, u);
    glm::vec3 d = glm::normalize(-alpha * cam.aspect * glm::tan(cam.fovy / 2) * u + beta * glm::tan(cam.fovy / 2) * v - w);
    Ray ray;
    ray.p0 = eye;
    ray.dir = d;
    return ray;
}

Intersection RayTracer::Intersect(Ray ray, Triangle* triangle)
{
    glm::mat4 p123;
    p123[0] = glm::vec4(triangle->P[0], 1);
    p123[1] = glm::vec4(triangle->P[1], 1);
    p123[2] = glm::vec4(triangle->P[2], 1);
    p123[3] = glm::vec4(-ray.dir, 0);
    glm::vec4 p0 = glm::vec4(ray.p0, 1);
    glm::vec4 lambdat = glm::inverse(p123) * p0;
    Intersection result;
    if (lambdat.x >= 0 && lambdat.y >= 0 && lambdat.z >= 0 && lambdat.w >= 0)
    {
        glm::vec3 q = lambdat.x * triangle->P[0] + lambdat.y * triangle->P[1] + lambdat.z * triangle->P[2];
        glm::vec3 n = glm::normalize(lambdat.x * triangle->N[0] + lambdat.y * triangle->N[1] + lambdat.z * triangle->N[2]);
        result.P = q;
        result.N = n;
        result.V = -ray.dir;
        result.dist = lambdat.w;
    }
    else
    {
        result.dist = INFINITY;
    }
    result.triangle = triangle;
    return result;
}

Intersection RayTracer::Intersect(Ray ray, RTScene* scene)
{
    float minDist = INFINITY;
    Intersection hit;
    hit.dist = minDist;
    std::vector<Triangle> list = scene->triangle_soup;
    Triangle* tri;
    int numTri = list.size();
    for (int i = 0; i < numTri; i++)
    {
        tri = &(list[i]);
        Intersection hit_temp = Intersect(ray, tri);
        if (hit_temp.dist < minDist)
        {
            minDist = hit_temp.dist;
            hit = hit_temp;
        }
    }
    return hit;
}

glm::vec3 RayTracer::FindColor(Intersection hit, int recursion_depth, RTScene* scene)
{
    if (recursion_depth != 0)
    {
        // generate all secondary rays to all lights
    }
    glm::vec4 color;
    if (hit.dist == INFINITY)
    {
        color = glm::vec4(0.0, 0.0, 0.0, 0.0);
        return color;
    }
    map<string, Light*> light = scene->light;
    vector<glm::vec4> lightPosition;
    vector<glm::vec4> lightColors;

    for (pair<string, Light*> element : light)
    {
        lightPosition.push_back(element.second->position);
        lightColors.push_back(element.second->color);
    }

    Material* mt = hit.triangle->material;
    color = mt->emision;
    glm::vec4 sum = glm::vec4(0, 0, 0, 0);
    // unit vector pointing to the light
    glm::vec3 l;
    glm::vec3 n;
    glm::vec3 h;
    glm::vec3 v;
    Ray rayToLight;
    Intersection lightIntersect;
    bool seen = true;
    for (int i = 0; i < light.size(); i++)
    {
        glm::vec4 position = lightPosition[i];
        Ray raytoLight;
        raytoLight.p0 = hit.P;
        l = glm::normalize(glm::vec3(glm::vec4(hit.P, 1) - position));
        raytoLight.dir = l;
        // diffuse * max(dot(n, l), 0)
        sum = mt->ambient + mt->diffuse * glm::max(glm::dot(hit.N, l), (float)0);
        v = glm::normalize(-hit.N);
        h = glm::normalize(v + l);
        if (recursion_depth == 0)
        {
            sum += mt->specular * glm::pow(glm::max(glm::dot(hit.N, h), (float)0), mt->shininess) * lightColors[i];
        }
        // shadow
        rayToLight.p0 = hit.P;
        rayToLight.dir = l;
        float distToLight = glm::sqrt(glm::pow((position.x - hit.P.x), 2) +
            glm::pow((position.y - hit.P.y), 2) +
            glm::pow((position.z - hit.P.z), 2));
        lightIntersect = Intersect(rayToLight, scene);
        if (lightIntersect.dist < distToLight)
        {
            seen = false;
        }
        if (seen == true)
        {
            color = color + sum;
        }
    }
    Ray ray2;
    Intersection hit2;
    if (recursion_depth > 0)
    {
        ray2.dir = (float)2 * glm::dot(hit.N, hit.V) * hit.N - hit.V;
        ray2.p0 = hit.P + 0.1f * ray2.dir;
        hit2 = Intersect(ray2, scene);
        color = color + glm::vec4(FindColor(hit2, recursion_depth - 1, scene),0);
    }
    return color;
}
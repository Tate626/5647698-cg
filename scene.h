#pragma once
#include"windows.h"
#include"dxcore.h"
#include"mesh.h"
#include"Shaders.h"
#include"GamesEngineeringBase .h"
#include"math.h"
#include"animation.h"



class water {
public:
	Shader shad;
    mesh gen;
    Matrix vp;
    Matrix planeWorld;
    float t = 0.f;
    std::string textureFilenames;
    STATIC_VERTEX addVertex(vec3 p, vec3 n, float tu, float tv)
    {
        STATIC_VERTEX v;
        v.pos = p;
        v.normal = vec3(0, 1, 0);
        v.tangent = vec3(1, 0, 0); 
        v.tu = tu;
        v.tv = tv;
        return v;
    }
    void init(DXCore& dxcore, std::string filename, TextureManager& textures) {
        shad.init_static("NewVertixShader_water.txt", "NexPixelShader_onlytext.txt", dxcore);
        int gridSize = 30;
        std::vector<STATIC_VERTEX> vertices;
        std::vector<unsigned int> indices;
        float size = 50.0f;
        float step = size / gridSize;

        for (int z = 0; z <= gridSize; ++z) {
            for (int x = 0; x <= gridSize; ++x) {
                float px = -size / 2 + x * step; 
                float pz = -size / 2 + z * step; 
                float tu = static_cast<float>(x) / gridSize; 
                float tv = static_cast<float>(z) / gridSize;
                vertices.push_back(addVertex(vec3(px, 0, pz), vec3(0, 1, 0), tu, tv));
            }
        }
        for (int z = 0; z < gridSize; ++z) {
            for (int x = 0; x < gridSize; ++x) {
                int topLeft = z * (gridSize + 1) + x;
                int topRight = topLeft + 1;
                int bottomLeft = (z + 1) * (gridSize + 1) + x;
                int bottomRight = bottomLeft + 1;
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }
        gen.init(vertices, indices, dxcore);
        textureFilenames = filename;
        textures.load(&dxcore, filename);
    }
    void draw(camera& cam, DXCore& dxcore,float dt, TextureManager& textures) {
        Matrix clu;
        clu = clu.translation(vec3(30.f,2.f,30.f));
        planeWorld = clu;
        Matrix v = vp.lookat(cam.from, cam.to, cam.up);
        Matrix p = vp.projection(1.f, cam.viewangle, cam.znear, cam.zfar);
        vp = v * p;
        shad.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &planeWorld);
        shad.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
        shad.updateConstantVS("StaticModel", "staticMeshBuffer", "time", &t);
        shad.updateTexturePS(dxcore, "tex", textures.find(textureFilenames));
        shad.apply(&dxcore);
        gen.draw(dxcore);
        t += dt;
    }
};

class sky {
public:
    sphere sky;
    void init(DXCore &c, TextureManager& textures) {
        sky.init(c, "Textures/sky.png", textures);
    }
    void draw(camera& cam,Shader* sh,DXCore& c, TextureManager& textures) {
        sky.draw(cam, sh, c, textures);
    }
};

class scene {
public:
    DXCore* dxcore;
    TextureManager* textures;
    camera* cam;
    water wat;
    plane pla;
    sky s;
    void init(DXCore* _dxcore, TextureManager* _textures, camera* _cam) {
        dxcore = _dxcore;
        textures = _textures;
        cam = _cam;
        wat.init(*dxcore, "Textures/water.jpg", *textures);
        pla.init(*dxcore);
        s.init(*dxcore, *textures);
    }
    void draw(Shader *sh_notext,Shader* sh_onlytex, float dt) {
        pla.draw(*cam, sh_notext, *dxcore);
        wat.draw(*cam, *dxcore, dt, *textures);
        s.draw(*cam,sh_onlytex,*dxcore,*textures);
    }
};


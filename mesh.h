#pragma once
#include"math.h"
#include"dxcore.h"
#include"Shaders.h"
#include"GEMLoader.h"
#include"texture.h"
#include"camera.h"
#include"light.h"

struct Vertex{
	vec3 position;
	Colour colour;
};

class triangle {
public:
	ID3D11Buffer* vertexBuffer;

	void init(DXCore &dxcore ) {
		Vertex vertices[3];
		vertices[0].position = vec3(0, 1.0f, 0);
		vertices[0].colour = Colour(0, 1.0f, 0);
		vertices[1].position = vec3(-1.0f, -1.0f, 0);
		vertices[1].colour = Colour(1.0f, 0, 0);
		vertices[2].position = vec3(1.0f, -1.0f, 0);
		vertices[2].colour = Colour(0, 0, 1.0f);

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA uploadData;
		bd.ByteWidth = sizeof(Vertex) * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		uploadData.pSysMem = vertices;
		uploadData.SysMemPitch = 0;
		uploadData.SysMemSlicePitch = 0;
		dxcore.device->CreateBuffer(&bd, &uploadData, &vertexBuffer);

	}

	void draw(DXCore &dxcore) {
		UINT offsets;
		offsets = 0;
		UINT strides = sizeof(Vertex);
		dxcore.devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dxcore.devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		dxcore.devicecontext->Draw(3, 0);

	}
};


struct STATIC_VERTEX
{
	vec3 pos;   
	vec3 normal;
	vec3 tangent;
	float tu;
	float tv;
};

struct ANIMATED_VERTEX
{
	vec3 pos;
	vec3 normal;
	vec3 tangent;
	float tu;
	float tv;
	unsigned int bonesIDs[4];
	float boneWeights[4];
};

class mesh {
public:
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	int indicesSize;
	UINT strides;
	void init(void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices,DXCore &dxcore) {
		D3D11_BUFFER_DESC bd;
		memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * numIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA data;
		memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = indices;
		dxcore.device->CreateBuffer(&bd, &data, &indexBuffer);
		bd.ByteWidth = vertexSizeInBytes * numVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		data.pSysMem = vertices;
		dxcore.device->CreateBuffer(&bd, &data, &vertexBuffer);
		indicesSize = numIndices;
		strides = vertexSizeInBytes;
	}
	void init(std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& dxcore)
	{
		init(&vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size(), dxcore);
	}

	void init(std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& dxcore)
	{
		init(&vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size(),dxcore);
	}


	void draw(DXCore& dxcore) {
		UINT offsets = 0;
		dxcore.devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dxcore.devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		dxcore.devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		dxcore.devicecontext->DrawIndexed(indicesSize, 0, 0);
	}

};

class plane {
public:
	mesh gen;
	Matrix vp;
	Matrix planeWorld;
	float t = 0.f;
	STATIC_VERTEX addVertex(vec3 p, vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = vec3(0,0,0); // For now
		v.tu = tu;
		v.tv = tv;
		return v;
	}
	void init(DXCore& dxcore) {
		std::vector<STATIC_VERTEX> vertices;
		vertices.push_back(addVertex(vec3(-15, 0, -15), vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(vec3(15, 0, -15), vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(vec3(-15, 0, 15), vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(vec3(15, 0, 15), vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		gen.init(vertices, indices, dxcore);
	}
	void draw(camera &cam, Shader* shaders, DXCore& dxcore) {
		Matrix clu;
		clu.scaling(vec3(10.f, 10.f, 10.f));
		planeWorld = clu;
		Matrix v = vp.lookat(cam.from, cam.to, cam.up);
		Matrix p = vp.projection(1.f, cam.viewangle, cam.znear, cam.zfar);
		vp = v * p;
		shaders->updateConstantVS("StaticModel", "staticMeshBuffer", "W", &planeWorld);
		shaders->updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
		shaders->apply(&dxcore);
		gen.draw(dxcore);

	}
};

class cube {
public:
	mesh gen;
	Matrix vp;
	Matrix planeWorld;
	float t = 0.f;
	STATIC_VERTEX addVertex(vec3 p, vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = vec3(0, 0, 0); // For now
		v.tu = tu;
		v.tv = tv;
		return v;
	}
	void init(DXCore &dxcore) {
		std::vector<STATIC_VERTEX> vertices;
		std::vector<unsigned int> indices;
		vec3 p0 = vec3(-1.0f, -1.0f, -1.0f);
		vec3 p1 = vec3(1.0f, -1.0f, -1.0f);
		vec3 p2 = vec3(1.0f, 1.0f, -1.0f);
		vec3 p3 = vec3(-1.0f, 1.0f, -1.0f);
		vec3 p4 = vec3(-1.0f, -1.0f, 1.0f);
		vec3 p5 = vec3(1.0f, -1.0f, 1.0f);
		vec3 p6 = vec3(1.0f, 1.0f, 1.0f);
		vec3 p7 = vec3(-1.0f, 1.0f, 1.0f);
		vertices.push_back(addVertex(p0, vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p1, vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p2, vec3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p3, vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p5, vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p4, vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p7, vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p6, vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p4, vec3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p0, vec3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p3, vec3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p7, vec3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f));
		
		vertices.push_back(addVertex(p1, vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p5, vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p6, vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p2, vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p3, vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p2, vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p6, vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p7, vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p4, vec3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p5, vec3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p1, vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p0, vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f));


		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(2); indices.push_back(3);
		indices.push_back(4); indices.push_back(5); indices.push_back(6);
		indices.push_back(4); indices.push_back(6); indices.push_back(7);
		indices.push_back(8); indices.push_back(9); indices.push_back(10);
		indices.push_back(8); indices.push_back(10); indices.push_back(11);
		indices.push_back(12); indices.push_back(13); indices.push_back(14);
		indices.push_back(12); indices.push_back(14); indices.push_back(15);
		indices.push_back(16); indices.push_back(17); indices.push_back(18);
		indices.push_back(16); indices.push_back(18); indices.push_back(19);
		indices.push_back(20); indices.push_back(21); indices.push_back(22);
		indices.push_back(20); indices.push_back(22); indices.push_back(23);
		
		gen.init(vertices, indices, dxcore);
	}

	void updatew(Matrix &www) {
		planeWorld = www;
	}

	void draw(Shader* shaders, DXCore& dxcore) {
		vec3 from = vec3(11 * cos(t), 5, 11 * sinf(t));
		Matrix v = vp.lookat(from, vec3(0, 1, 0), vec3(0, 1, 0));
		Matrix p = vp.projection(1.f, 90.f, 0.1f, 100.f);
		vp = v * p;
		shaders->updateConstantVS("StaticModel", "staticMeshBuffer", "W", &planeWorld);
		shaders->updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
		shaders->apply(&dxcore);
		gen.draw(dxcore);

	}

};

class sphere {
public:
	mesh gen;
	Matrix vp;
	Matrix planeWorld;
	int rings;
	int segments;
	float radius;
	float t = 0.f;
	std::string textureFilenames;
	STATIC_VERTEX addVertex(vec3 p, vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = vec3(0, 0, 0); // For now
		v.tu = tu;
		v.tv = tv;
		return v;
	}
	void init(DXCore &dxcore, std::string filename, TextureManager& textures) {
		rings = 30;
		segments = 30;
		radius = 1000.f;
		std::vector<STATIC_VERTEX> vertices;
		std::vector<unsigned int> indices;
		for (int lat = 0; lat <= rings; lat++) {
			float theta = lat * M_PI / rings;
			float sinTheta = sinf(theta);
			float cosTheta = cosf(theta);
			for (int lon = 0; lon <= segments; lon++) {
				float phi = lon * 2.0f * M_PI / segments;
				float sinPhi = sinf(phi);
				float cosPhi = cosf(phi);
				vec3 position(radius * sinTheta * cosPhi, radius * cosTheta, radius * sinTheta * sinPhi);
				vec3 normal = position.normalize();
				float tu = 1.0f - (float)lon / segments;
				float tv = 1.0f - (float)lat / rings;
				vertices.push_back(addVertex(position, normal, tu, tv));
				int current = lat * (segments + 1) + lon;
				int next = current + segments + 1;
				indices.push_back(current);
				indices.push_back(next);
				indices.push_back(current + 1);
				indices.push_back(current + 1);
				indices.push_back(next);
				indices.push_back(next + 1);
			}
		}
		gen.init(vertices, indices, dxcore);
		textureFilenames= filename;
		textures.load(&dxcore, filename);
	}

	void updatew(Matrix& www) {
		planeWorld = www;
	}
	void draw(camera &cam,Shader* shaders, DXCore& dxcore, TextureManager& textures) {
		Matrix v = vp.lookat(cam.from, cam.to, cam.up);
		Matrix p = vp.projection(1.f, cam.viewangle, cam.znear, cam.zfar);
		vp = v * p;
		shaders->updateConstantVS("StaticModel", "staticMeshBuffer", "W", &planeWorld);
		shaders->updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
		shaders->updateTexturePS(dxcore, "tex", textures.find(textureFilenames));
		shaders->apply(&dxcore);
		gen.draw(dxcore);
	}
};

class load {
public:
	std::vector<mesh> meshes;
	Matrix vp;
	Matrix planeWorld;
	float t = 0.f;
	std::vector<std::string> textureFilenames;
	std::vector<std::string> NormalFilenames;
	vec3 centre=vec3(0,0,0); //存储物体质心坐标
	AABB box;

	void init(DXCore& core, std::string filename, TextureManager& textures) {
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		loader.load(filename, gemmeshes);

		int centrenum = 0;


		for (int i = 0; i < gemmeshes.size(); i++) {
			mesh mesh;
			std::vector<STATIC_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
				STATIC_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
				centre += v.pos;
				box.extend(v.pos);
				centrenum++;
				vertices.push_back(v);
			}
			mesh.init(vertices, gemmeshes[i].indices, core);
			textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
			textures.load(&core, gemmeshes[i].material.find("diffuse").getValue());

			NormalFilenames.push_back(gemmeshes[i].material.find("normals").getValue());
			textures.load(&core, gemmeshes[i].material.find("normals").getValue());
			meshes.push_back(mesh);
		}

		centre /= centrenum;

	}
	void updatew(Matrix& www) {
		planeWorld = www;
		box.pmax = planeWorld.mulPoint(box.pmax);
		box.pmin = planeWorld.mulPoint(box.pmin);
	}

	void draw(camera cam, Shader* shaders, DXCore& dxcore, TextureManager& textures, Light &light) {
		Matrix v = vp.lookat(cam.from, cam.to, cam.up);
		Matrix p = vp.projection(1.f, cam.viewangle, cam.znear, cam.zfar);
		vp = v * p;
		shaders->updateConstantVS("StaticModel", "staticMeshBuffer", "W", &planeWorld);
		shaders->updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
		shaders->updateConstantPS("Lighting", "LightingBuffer", "LightDir", &light.dir);
		shaders->updateConstantPS("Lighting", "LightingBuffer", "ViewPos", &cam.from);
		shaders->updateConstantPS("Lighting", "LightingBuffer", "LightColor", &light.color);
		shaders->apply(&dxcore);
		for (int i = 0; i < meshes.size(); i++)
		{
			shaders->updateTexturePS(dxcore, "Nor", textures.find(NormalFilenames[i]));
			shaders->updateTexturePS(dxcore,"tex", textures.find(textureFilenames[i]));
			meshes[i].draw(dxcore);
		}

	}

	void draw(camera cam, Shader* shaders, DXCore& dxcore, TextureManager& textures) {

		Matrix v = vp.lookat(cam.from, cam.to, cam.up);
		Matrix p = vp.projection(1.f, cam.viewangle, cam.znear, cam.zfar);
		vp = v * p;
		shaders->updateConstantVS("StaticModel", "staticMeshBuffer", "W", &planeWorld);
		shaders->updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
		shaders->apply(&dxcore);
		for (int i = 0; i < meshes.size(); i++)
		{
			shaders->updateTexturePS(dxcore, "tex", textures.find(textureFilenames[i]));
			meshes[i].draw(dxcore);
		}
	}

	void drawDepthPass(camera &cam, Shader* depthShader, DXCore& dxcore, Light& light) {

		vec3 lightPosition = vec3(0,100,0) - light.dir * 100.f;
		Matrix v = vp.lookat(lightPosition, vec3(0,0,0), vec3(0, 1, 0));
		float left = -50.0f;
		float right = 50.0f;
		float bottom = -50.0f;
		float top = 50.0f;
		float nearZ = 0.1f;
		float farZ = 200.0f;
		Matrix p = vp.orthographic(left, right, bottom, top, nearZ, farZ);
		vp = v * p;

		depthShader->updateConstantVS("DepthModel", "staticMeshBuffer", "W", &planeWorld);
		depthShader->updateConstantVS("DepthModel", "staticMeshBuffer", "VP", &vp);
		depthShader->apply(&dxcore);


		for (int i = 0; i < meshes.size(); i++) {
			meshes[i].draw(dxcore);
		}
	}
};



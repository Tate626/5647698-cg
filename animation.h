#pragma once
#include"math.h"
#include"dxcore.h"
#include"Shaders.h"
#include"GEMLoader.h"
#include"texture.h"
#include"camera.h"
#include"mesh.h"


struct Bone
{
	std::string name;
	Matrix offset;
	int parentIndex;
};
struct Skeleton
{
	std::vector<Bone> bones;
	Matrix globalInverse;
};
struct AnimationFrame
{
	//需要补上四元数方法
	std::vector<vec3> positions;
	std::vector<Quaternion> rotations;
	std::vector<vec3> scales;
};

class AnimationSequence
{
	// stores animation data for one animation
public:
	std::vector<AnimationFrame> frames; // transformation data for bones
	float ticksPerSecond; // speed of animation

	vec3 interpolate(vec3 p1, vec3 p2, float t) {
		return ((p1 * (1.0f - t)) + (p2 * t)); // linear interpolation
	}
	Quaternion interpolate(Quaternion q1, Quaternion q2, float t) {
		return Quaternion::slerp(q1, q2, t); // ensure smooth rotation
	}
	float duration() {
		return ((float)frames.size() / ticksPerSecond);// total duration of animation sequence
	}

	void calcFrame(float t, int& frame, float& interpolationFact)
	{
		// find frame given time
		interpolationFact = t * ticksPerSecond;
		frame = (int)floorf(interpolationFact);
		interpolationFact = interpolationFact - (float)frame; // fractional part
		frame = min(frame, frames.size() - 1);
	}

	int nextFrame(int frame)
	{
		// find next frame, returns index for next frame
		return min(frame + 1, frames.size() - 1);
	}

	Matrix interpolateBoneToGlobal(Matrix* matrices, int baseFrame, float interpolationFact, Skeleton* skeleton, int boneIndex) {
		//Interpolates transformations for a specific bone and calculates its global transformation.
		int nextFrameIndex = nextFrame(baseFrame);
		Matrix temp;
		// interpolate scale, rotation and translation
		Matrix scale = temp.scaling(interpolate(frames[baseFrame].scales[boneIndex], frames[nextFrameIndex].scales[boneIndex], interpolationFact));
		Matrix rotation = interpolate(frames[baseFrame].rotations[boneIndex], frames[nextFrameIndex].rotations[boneIndex], interpolationFact).toMatrix();
		Matrix translation = Matrix::translation(interpolate(frames[baseFrame].positions[boneIndex], frames[nextFrameIndex].positions[boneIndex], interpolationFact));
		Matrix local = scale * rotation * translation;
		if (skeleton->bones[boneIndex].parentIndex > -1) {
			Matrix global = local * matrices[skeleton->bones[boneIndex].parentIndex];
			return global;
		}
		return local;
	}


};

class Animation
{
	// store skeleton and animations
public:
	std::map<std::string, AnimationSequence> animations; // map of animation sequences
	Skeleton skeleton; // The skeleton structure defining bone hierarchy and transformations.

	int boneSize() {
		int boneSize = skeleton.bones.size();
		return boneSize;
	}
	// Calculation of the current frame and interpolation factor
	void calcFrame(std::string name, float t, int& frame, float& interpolationFact) {
		animations[name].calcFrame(t, frame, interpolationFact);
	}
	// bone transformation interpolation to the corresponding AnimationSequence.
	Matrix interpolateBoneToGlobal(std::string name, Matrix* matrices, int baseFrame, float interpolationFact, int boneIndex) {
		return animations[name].interpolateBoneToGlobal(matrices, baseFrame, interpolationFact, &skeleton, boneIndex);
	}

	void calcFinalTransforms(Matrix* matrices)
	{
		for (int i = 0; i < boneSize(); i++)
		{
			matrices[i] = skeleton.bones[i].offset * matrices[i] * skeleton.globalInverse;
		}
	}
};

class AnimationInstance
{
public:
	Animation* animation;
	std::string currentAnimation;
	Matrix matrices[256];
	float t;

	void resetAnimationTime()
	{
		t = 0;
	}
	bool animationFinished()
	{
		if (t > animation->animations[currentAnimation].duration())
		{
			return true;
		}
		return false;
	}

	void update(std::string name, float dt) {
		if (name == currentAnimation) {
			t += dt;
		}
		else {
			currentAnimation = name;  t = 0;
		}
		if (animationFinished() == true) { resetAnimationTime(); }
		int frame = 0;
		float interpolationFact = 0;
		animation->calcFrame(name, t, frame, interpolationFact);
		for (int i = 0; i < animation->boneSize(); i++)
		{
			matrices[i] = animation->interpolateBoneToGlobal(name, matrices, frame, interpolationFact, i);
		}
		animation->calcFinalTransforms(matrices);//这里删除了一个形参，不确定对不对

	}

};

class loadanimation {
public:
	std::vector<mesh> meshes;
	Matrix vp;
	Matrix planeWorld;
	AnimationInstance instance;
	Animation animation1;
	std::vector<std::string> textureFilenames;
	std::vector<std::string> NormalFilenames;
	float t = 0.f;
	vec3 centre; //存储物体质心坐标


	void init(DXCore& core, std::string filename, TextureManager& textures) {
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		GEMLoader::GEMAnimation gemanimation;
		loader.load(filename, gemmeshes, gemanimation);

		int centrenum = 0;

		for (int i = 0; i < gemmeshes.size(); i++) {
			mesh mesh1;
			std::vector<ANIMATED_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
				ANIMATED_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
				centre += v.pos;
				centrenum++;
				vertices.push_back(v);
			}
			mesh1.init(vertices, gemmeshes[i].indices, core);
			textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
			textures.load(&core, gemmeshes[i].material.find("diffuse").getValue());

			NormalFilenames.push_back(gemmeshes[i].material.find("normals").getValue());
			textures.load(&core, gemmeshes[i].material.find("normals").getValue());
			meshes.push_back(mesh1);
		}

		centre /= centrenum;

		for (int i = 0; i < gemanimation.bones.size(); i++)
		{
			Bone bone;
			bone.name = gemanimation.bones[i].name;
			memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
			bone.parentIndex = gemanimation.bones[i].parentIndex;
			animation1.skeleton.bones.push_back(bone);
		}
		for (int i = 0; i < gemanimation.animations.size(); i++)
		{
			std::string name = gemanimation.animations[i].name;
			AnimationSequence aseq;
			aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
			for (int n = 0; n < gemanimation.animations[i].frames.size(); n++)
			{
				AnimationFrame frame;
				for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++)
				{
					vec3 p;
					Quaternion q;
					vec3 s;
					memcpy(&p, &gemanimation.animations[i].frames[n].positions[index], sizeof(vec3));
					frame.positions.push_back(p);
					memcpy(&q, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternion));
					frame.rotations.push_back(q);
					memcpy(&s, &gemanimation.animations[i].frames[n].scales[index], sizeof(vec3));
					frame.scales.push_back(s);
				}
				aseq.frames.push_back(frame);
			}
			animation1.animations.insert({ name, aseq });
		}
		instance.animation = &animation1;
	}

	void updatew(Matrix& www) {
		planeWorld = www;
		centre = planeWorld.mulPoint(centre);
	}
	void draw(camera& cam, Shader* shaders, DXCore& dxcore, float dt, TextureManager& textures, std::string animationname, Light &light ) {

		Matrix v = vp.lookat(cam.from, cam.to, cam.up);
		Matrix p = vp.projection(1.f, cam.viewangle, cam.znear, cam.zfar);
		vp = v * p;

		instance.update(animationname, dt);
		shaders->updateConstantVS("Animated", "animatedMeshBuffer", "W", &planeWorld);
		shaders->updateConstantVS("Animated", "animatedMeshBuffer", "VP", &vp);
		shaders->updateConstantVS("Animated", "animatedMeshBuffer", "bones", instance.matrices);
		shaders->updateConstantPS("Lighting", "LightingBuffer", "LightDir", &light.dir);
		shaders->updateConstantPS("Lighting", "LightingBuffer", "ViewPos", &cam.from);
		shaders->updateConstantPS("Lighting", "LightingBuffer", "LightColor", &light.color);
		/*shaders->updateTexturePS(dxcore, "shadowMap", dxcore.shaderResourceView);*/
		shaders->apply(&dxcore);
		for (int i = 0; i < meshes.size(); i++)
		{ 
			shaders->updateTexturePS(dxcore, "Nor", textures.find(textureFilenames[i]));
			shaders->updateTexturePS(dxcore, "tex", textures.find(textureFilenames[i]));
			meshes[i].draw(dxcore);
		}
	}

	void draw(camera& cam, Shader* shaders, DXCore& dxcore, float dt, TextureManager& textures, std::string animationname) {

		Matrix v = vp.lookat(cam.from, cam.to, cam.up);
		Matrix p = vp.projection(1.f, cam.viewangle, cam.znear, cam.zfar);
		vp = v * p;

		instance.update(animationname, dt);
		shaders->updateConstantVS("Animated", "animatedMeshBuffer", "W", &planeWorld);
		shaders->updateConstantVS("Animated", "animatedMeshBuffer", "VP", &vp);
		shaders->updateConstantVS("Animated", "animatedMeshBuffer", "bones", instance.matrices);
		shaders->apply(&dxcore);
		for (int i = 0; i < meshes.size(); i++)
		{
			shaders->updateTexturePS(dxcore, "tex", textures.find(textureFilenames[i]));
			meshes[i].draw(dxcore);
		}
	}
};
#pragma once
#include"windows.h"
#include"dxcore.h"
#include"mesh.h"
#include"Shaders.h"
#include"GamesEngineeringBase .h"
#include"math.h"
#include"animation.h"

class player {
public:
	Shader shad;
	loadanimation load;
	spherebox box;
	vec3 pos;
	vec3 lastforward;
	bool ismoving;
	bool isfiring;
	void init(DXCore &dxcore, TextureManager& textures) {
		shad.init_move("NewVertixShader_move.txt", "NexPixelShader_normalMap.txt", dxcore);
		load.init(dxcore, "Automatic_Carbine_5.gem", textures);
		pos = vec3(0, 0, 0);
		ismoving = 0;
	}
	void draw(camera &cam, DXCore& dxcore, float dt, TextureManager& textures,Light& light, Window &win) {
		float initialRotationDegrees = 90.0f * M_PI / 180.f;;
		Matrix initialRotation;
		initialRotation = initialRotation.rotationV(initialRotationDegrees, vec3(1, 0, 0));
		Matrix help;
		help = help.rotationV(M_PI, vec3(0, 1, 0));
		initialRotation = initialRotation * help;

		Matrix initialMove;
		initialMove = initialMove.translation(vec3(0, 6, -6));

		Matrix move;
		vec3 xx=cam.forward.normalize();
		xx *= 3.f;
		xx += cam.from;
		move=move.translation(vec3(xx.x,0,xx.z));
	
		Matrix scal;
		scal.scaling(vec3(0.4f, 0.4f, 0.4f));
	       
		Matrix cameraRotY;
		cameraRotY = cameraRotY.rotationV(cam.yaw * M_PI / 180.f,vec3(0,1,0));

		Matrix cameraRotX;
		cameraRotX = cameraRotX.rotationV(-cam.pitch * M_PI / 180.f, vec3(1, 0, 0));
		Matrix rotation = cameraRotX * cameraRotY;

		Matrix worldplane = scal * initialRotation * initialMove * rotation * move;

		load.updatew(worldplane);

		if (pos != vec3(cam.from.x, 0, cam.from.z))ismoving = 1;

		selectanimation(cam,dxcore,dt,textures,light,win);

		pos = vec3(cam.from.x, 0, cam.from.z);
		ismoving = 0;
		load.t = +dt;
	}



	void selectanimation(camera& cam, DXCore& dxcore, float dt, TextureManager& textures, Light& light, Window &win) {
		int select = 0;
		select = ismoving;
		if (win.mousePressed())select = 2;
		switch (select) {
		case 0:
			load.draw(cam, &shad, dxcore, dt, textures, "Armature|04 Idle", light);
			isfiring = 0;
			break;
		case 1:
			load.draw(cam, &shad, dxcore, dt, textures, "Armature|07 Run", light);
			isfiring = 0;
			break;
		case 2:
			load.draw(cam, &shad, dxcore, dt, textures, "Armature|08 Fire", light);
			isfiring = 1;
			break;
		}

	}
};


class dinosaur {
public:
	Shader shad;
	loadanimation load;
	spherebox box;
	bool isdeath;
	float health;
	float temp;

	void init(DXCore& dxcore, TextureManager& textures){
		shad.init_move("NewVertixShader_move.txt", "NexPixelShader_normalMap.txt", dxcore);
		load.init(dxcore, "TRex.gem", textures);
		
		Matrix W;
		W = W.translation(vec3(0, 0, 11));
		load.updatew(W);

		health = 100;
		temp = 0;
		isdeath = 0;
	}
	void draw(camera& cam, DXCore& dxcore, float dt, TextureManager& textures, Light& light) {
		box.set(load.centre, 3.f);

		if (health <= 0)isdeath = 1;
		else isdeath = 0;
		selectanimation(cam, dxcore, dt, textures, light);
		load.t = +dt;
	}

	void selectanimation(camera& cam, DXCore& dxcore, float dt, TextureManager& textures, Light& light) {
		int select = 0;
		select = isdeath;
		switch (select) {
		case 0:
			load.draw(cam, &shad, dxcore, dt, textures, "Run", light);
			break;
		case 1:
			load.draw(cam, &shad, dxcore, dt, textures, "death", light);
			temp += dt;
			if (temp > 3) {
				health = 100;
				temp = 0;
			}
			break;
		}

	}
};
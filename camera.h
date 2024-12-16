#pragma once
#include "math.h"
#include"collision.h"




//注意实际游戏世界是个左手系，但是我们的叉乘是右手的。
//class camera {
//public:
//	vec3 from;
//	vec3 up;
//	vec3 to;
//	float pitch;
//	float viewangle;
//	float znear;
//	float zfar;
//	vec3 forward;
//
//	spherebox box;
//
//	camera() {
//		from = vec3(0, 5, 0);
//		up = vec3(0, 1, 0);
//		to = vec3(0, 5, -1);
//		pitch = 0.f;
//		viewangle = 60.f;
//		znear = 0.1f;
//		zfar = 1200.f;
//		forward = to - from;
//		box = spherebox(vec3(from.x, 2, from.z), 3.f);
//	}
//
//	void updatefrom(float x, float z) {
//		forward = to - from;
//		vec3 right = forward.Cross(up).normalize();
//		forward = up.Cross(right).normalize();
//		from = from - forward * z - right * x;
//		to = to - forward * z - right * x;
//
//		box = spherebox(vec3(from.x, 2, from.z), 3.f);
//
//	}
//
//	void updateto(float xx, float yy, float dt) {
//		forward = to - from;
//		Matrix clu;
//		vec3 right = forward.Cross(up);
//		vec3 upDirection = right.Cross(forward);
//
//
//		//处理水平旋转
//		if (abs(xx) > 1.f || abs(yy) > 1.f) {
//			xx = xx * dt * 100;
//			forward = clu.rotationV(forward, xx, upDirection);
//
//			right = forward.Cross(up).normalize();
//
//			//处理垂直旋转
//			yy = -yy * dt * 60;
//			float newPitch = pitch + yy;
//
//
//			if (newPitch > 45) {
//				newPitch = 45;
//			}
//			else if (newPitch < -45) {
//				newPitch = -45;
//			}
//
//			if (newPitch <= 45 && newPitch >= -45) {
//				forward = clu.rotationV(forward, yy, right);
//			}
//
//			pitch = newPitch;
//
//			to = from + forward;
//		}
//	}
//};

class camera {
public:
    vec3 from;
    vec3 up;
    vec3 to;
    vec3 forward;
    float yaw;   
    float pitch;  
    float viewangle;
    float znear;
    float zfar;
    spherebox box;

    camera() {
        from = vec3(0, 5, 0);
        up = vec3(0, 1, 0);
        to = vec3(0, 5, -1);
        forward = (to - from).normalize();
        yaw = 0.0f;
        pitch = 0.0f;
        viewangle = 60.f;
        znear = 0.1f;
        zfar = 1200.f;
        updateCameraVectors();
        to = from + forward;

        box.set(vec3(from.x, 2, from.z), 5.f);
    }

    void updateCameraVectors() {
        float yaw01 = yaw * M_PI / 180.f;
        float pitch01 = pitch * M_PI / 180.f;    
        forward.x = cos(pitch01) * sin(yaw01);
        forward.y = sin(pitch01);
        forward.z = cos(pitch01) * cos(yaw01);
        forward = forward.normalize();
        vec3 right = forward.Cross(vec3(0,1,0)).normalize();
        up = right.Cross(forward).normalize();
    }

       
    void updatefrom(float x, float z) {
        updateCameraVectors();
        vec3 right = forward.Cross(up).normalize();   
        vec3 forwardNoY = vec3(forward.x, 0.0f, forward.z);
        forwardNoY = forwardNoY.normalize();
        from = from + forwardNoY * (-z) + right * (-x); 
        to = from + forward;
        box.set(vec3(from.x, 2, from.z), 5.f);
    }
   
    
    void updateto(float xx, float yy, float dt) {
      
        float yawChange = xx * dt * 100.0f;
        float pitchChange = yy * dt * 60.0f;
        yaw += yawChange;
        pitch += pitchChange;     
        if (pitch > 15.0f) pitch = 15.0f;
        if (pitch < -15.0f) pitch = -15.0f;         
        updateCameraVectors();        
        to = from + forward;
    }
};

//鼠标逻辑更新
void updatemouse(HWND hwnd, camera& cam, float dt) {
	POINT center;
	RECT rect;
	GetClientRect(hwnd, &rect);
	center.x = (rect.right - rect.left) / 2;
	center.y = (rect.bottom - rect.top) / 2;

	static POINT lastCursorPos = { center.x, center.y };

	POINT currentPos;
	GetCursorPos(&currentPos);
	ScreenToClient(hwnd, &currentPos);

	float deltaX = -center.x + currentPos.x;
	float deltaY = -currentPos.y + center.y;

	cam.updateto(deltaX, deltaY, dt);

	ClientToScreen(hwnd, &center);
	SetCursorPos(center.x, center.y);

	lastCursorPos = currentPos;
}
#include"windows.h"
#include"dxcore.h"
#include"mesh.h"
#include"Shaders.h"
#include"GamesEngineeringBase .h"
#include"math.h"
#include"animation.h"
#include"NPC.h"
#include"light.h"
#include"collisionmanager.h"
#include"scene.h"

void lightingpass(Shader& sh, DXCore& c,Gbuffer &gbuffer,Light &light,camera &cam) {
	c.devicecontext->OMSetRenderTargets(1, &c.backbufferRenderTargetView, c.depthStencilView);
	sh.apply(&c);
	c.devicecontext->PSSetShaderResources(0, 3, gbuffer.gBufferSRV);
	Sampler samp;
	samp.init(c);
	samp.bind(c);
	sh.updateConstantPS("Lighting", "LightingBuffer", "LightDir", &light.dir);
	sh.updateConstantPS("Lighting", "LightingBuffer", "ViewPos", &cam.from);
	sh.updateConstantPS("Lighting", "LightingBuffer", "LightColor", &light.color);
	drawFullScreenQuad(&c);
	ID3D11ShaderResourceView* nullSRV[3] = { nullptr, nullptr, nullptr };
	c.devicecontext->PSSetShaderResources(0, 3, nullSRV);
}



//originally planned to include all static objects in a Scene class for unified updates, 
// but since the instancing method hasn¡¯t been implemented yet,
// objects (bamboo, flower)that need to be instantiated multiple times are currently handled directly within the main function loop.


//two loops in the main function: 
// the one currently running is for forward rendering, 
// the commented-out one is for deferred rendering, which hasn¡¯t been implemented yet and has issues.
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	Window win;
	DXCore c;
	Gbuffer gbuffer;

	Shader sh_onlytex;
	Shader sh_notex;
	Shader sh_normal;

	Shader shG;
	Shader shL;

	camera cam;
	
	Light light;
	scene _scene;

	TextureManager mmm;
	collisionmanager collision;
	
	dinosaur dino;
	player play;
	
	load flower;

	load bamboo;



	GamesEngineeringBase::Timer tim;
	win.create(1024, 1024, "My Window");
	c.init(1024, 1024, win.hwnd, 0);
	gbuffer.init(c.device, 1024, 1024);

	sh_onlytex.init_static("NewVertixShader_static.txt", "NexPixelShader_onlytext.txt", c);
	sh_notex.init_static("NewVertixShader_static.txt", "NexPixelShader_notexture.txt", c);
	sh_normal.init_static("NewVertixShader_static.txt", "NexPixelShader_normalMap.txt", c);
	
	shG.init_static("NewVertixShader_static01.txt", "NexPixelShader_gbuffer.txt", c);
	shL.init_static("NewVertixShader_static02.txt", "NexPixelShader_gbuffer02.txt", c);

	dino.init(c,mmm);
	play.init(c, mmm);

	flower.init(c,"flower4.gem", mmm);
	bamboo.init(c, "bamboo.gem", mmm);
	
	_scene.init(&c, &mmm, &cam);

	collision.load(cam);
	collision.load(dino);
	collision.load(&bamboo.box);
	collision.load(&flower.box);

	ShowCursor(FALSE);
	//forward
	while (true) {
		win.processMessages();
		c.clear();
		
		float dt = tim.dt();

		updatemouse(win.hwnd,cam,dt);

		float x = 0.f;
		float z = 0.f;
		if (win.keyPressed(VK_ESCAPE)) break;
		if (win.keyPressed('W')) z = -0.02;
		if (win.keyPressed('S')) z = 0.02;
		if (win.keyPressed('A')) x = 0.02;
		if (win.keyPressed('D')) x = -0.02;
		
		collision.checkmove(x,z,play.isfiring);

		_scene.draw(&sh_notex, &sh_onlytex,dt);
		
		dino.draw(cam, c, dt, mmm, light);
		play.draw(cam, c, dt, mmm, light, win);



		Matrix clu;
		clu.scaling(vec3(0.05f,0.05f,0.05f));
		Matrix Wc;
		Wc = Wc.translation(vec3(-11, 0, -11));
		Wc = clu * Wc;
		flower.updatew(Wc);
		flower.draw(cam, &sh_normal, c, mmm, light);

		Matrix clu1;
		clu1.scaling(vec3(0.02f, 0.02f, 0.02f));
		Matrix Wa;
        Wa = Wa.translation(vec3(-20, 0, 11));
		Wa = clu1*Wa;
		bamboo.updatew(Wa);
		bamboo.draw(cam, &sh_normal, c, mmm, light);

		c.present();
	}

	//deferred

	//while (true) {
	//	win.processMessages();
	//	c.clear();

	//	float dt = tim.dt();

	//	updatemouse(win.hwnd, cam, dt);

	//	float x = 0.f;
	//	float z = 0.f;
	//	if (win.keyPressed(VK_ESCAPE)) break;
	//	if (win.keyPressed('W')) z = -0.02;
	//	if (win.keyPressed('S')) z = 0.02;
	//	if (win.keyPressed('A')) x = 0.02;
	//	if (win.keyPressed('D')) x = -0.02;
	//	cam.updatefrom(x,z);

	//	// G-Pass
	//	gbuffer.clear(c.devicecontext);
	//	gbuffer.bind(c.devicecontext);
	//	
	//	Matrix clu;
	//	clu.scaling(vec3(0.05f, 0.05f, 0.05f));
	//	Matrix Wc;
	//	Wc = Wc.translation(vec3(0, 0, 10));
	//	Wc = clu * Wc;
	//	flower.updatew(Wc);
	//	flower.draw(cam, &shG, c, mmm);
	//
	//	Matrix clu1;
	//	clu1.scaling(vec3(0.02f, 0.02f, 0.02f));
	//	Matrix Wa;
	//	Wa = Wa.translation(vec3(-20, 0, 11));
	//	Wa = clu1 * Wa;
	//	bamboo.updatew(Wa);
	//	bamboo.draw(cam, &shG, c, mmm);

	//	gbuffer.unbind(c.devicecontext);

	//	lightingpass(shL, c, gbuffer, light, cam);

	//	c.present();
	//}
}






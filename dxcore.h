#pragma once
#include"adapter.h"

class DXCore {
public:
	ID3D11Device* device;
	ID3D11DeviceContext* devicecontext;
	IDXGISwapChain* swapchain;
	ID3D11RenderTargetView* backbufferRenderTargetView;
	ID3D11Texture2D* backbuffer;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthbuffer;

	ID3D11RasterizerState* rasterizerState;

	ID3D11Texture2D* renderTargetTexture;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11ShaderResourceView* shaderResourceView;




	void init(int width,int height,HWND hwnd,bool window_fullscreen) {
		DXGI_SWAP_CHAIN_DESC sd;
		memset(&sd, 0, sizeof(DXGI_SWAP_CHAIN_DESC));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = window_fullscreen ? false : true;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		D3D_FEATURE_LEVEL fl;
		fl = D3D_FEATURE_LEVEL_11_0;

		Adapter a;
		a.findadapter();
		IDXGIAdapter1* adapter = a.getAdapter();

		D3D11CreateDeviceAndSwapChain(adapter,
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL,
			D3D11_CREATE_DEVICE_DEBUG,
			&fl,
			1,
			D3D11_SDK_VERSION,
			&sd,
			&swapchain,
			&device,
			NULL,
			&devicecontext);
		swapchain->SetFullscreenState(window_fullscreen, NULL);

		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
		device->CreateRenderTargetView(backbuffer, NULL, &backbufferRenderTargetView);
		D3D11_TEXTURE2D_DESC dsvDesc;
		dsvDesc.Width = width;
		dsvDesc.Height = height;
		dsvDesc.MipLevels = 1;
		dsvDesc.ArraySize = 1;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.SampleDesc.Count = 1;
		dsvDesc.SampleDesc.Quality = 0;
		dsvDesc.Usage = D3D11_USAGE_DEFAULT;
		dsvDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsvDesc.CPUAccessFlags = 0;
		dsvDesc.MiscFlags = 0;
		device->CreateTexture2D(&dsvDesc, NULL, &depthbuffer);
		device->CreateDepthStencilView(depthbuffer, NULL, &depthStencilView);
		devicecontext->OMSetRenderTargets(1, &backbufferRenderTargetView, depthStencilView);
		D3D11_VIEWPORT viewport;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		devicecontext->RSSetViewports(1, &viewport);

		D3D11_RASTERIZER_DESC rsdesc;
		ZeroMemory(&rsdesc, sizeof(D3D11_RASTERIZER_DESC));
		rsdesc.FillMode = D3D11_FILL_SOLID;
		rsdesc.CullMode = D3D11_CULL_NONE;
		device->CreateRasterizerState(&rsdesc, &rasterizerState);
		devicecontext->RSSetState(rasterizerState);

		//ÐÂ¼ÓµÄ
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		device->CreateTexture2D(&textureDesc, NULL, &renderTargetTexture);

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		device->CreateRenderTargetView(renderTargetTexture, &renderTargetViewDesc, &renderTargetView);

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		device->CreateShaderResourceView(renderTargetTexture, &shaderResourceViewDesc, &shaderResourceView);

		devicecontext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

		devicecontext->OMSetRenderTargets(1, &backbufferRenderTargetView, depthStencilView);
	}

	void present(){
		swapchain->Present(0, 0);
	}

	void clear() {
		float ClearColour[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
		devicecontext->ClearRenderTargetView(backbufferRenderTargetView, ClearColour);
		devicecontext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f,0);
	}

};

class Gbuffer {
public:
	ID3D11RenderTargetView* gBufferRTV[3];  
	ID3D11ShaderResourceView* gBufferSRV[3];
	ID3D11DepthStencilView* gBufferDSV;  
	ID3D11Texture2D* gBufferTextures[3];
	ID3D11Texture2D* gBufferDepthTexture;

	void init(ID3D11Device* device, int width, int height) {
		DXGI_FORMAT formats[3] = {
			DXGI_FORMAT_R8G8B8A8_UNORM,        
			DXGI_FORMAT_R16G16B16A16_FLOAT,  
			DXGI_FORMAT_R32G32B32A32_FLOAT     
		};

		for (int i = 0; i < 3; i++) {
			D3D11_TEXTURE2D_DESC texDesc = {};
			texDesc.Width = width;
			texDesc.Height = height;
			texDesc.MipLevels = 1;
			texDesc.ArraySize = 1;
			texDesc.Format = formats[i];
			texDesc.SampleDesc.Count = 1;
			texDesc.Usage = D3D11_USAGE_DEFAULT;
			texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

			device->CreateTexture2D(&texDesc, nullptr, &gBufferTextures[i]);

			device->CreateRenderTargetView(gBufferTextures[i], nullptr, &gBufferRTV[i]);

			device->CreateShaderResourceView(gBufferTextures[i], nullptr, &gBufferSRV[i]);
		}

		D3D11_TEXTURE2D_DESC depthDesc = {};
		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		device->CreateTexture2D(&depthDesc, nullptr, &gBufferDepthTexture);

		device->CreateDepthStencilView(gBufferDepthTexture, nullptr, &gBufferDSV);
	}

	void bind(ID3D11DeviceContext* deviceContext) {
		deviceContext->OMSetRenderTargets(3, gBufferRTV, gBufferDSV);
	}

	void clear(ID3D11DeviceContext* deviceContext) {
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; i++) {
			deviceContext->ClearRenderTargetView(gBufferRTV[i], clearColor);
		}

		deviceContext->ClearDepthStencilView(gBufferDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void unbind(ID3D11DeviceContext* deviceContext) {
		ID3D11RenderTargetView* nullRTV[3] = { nullptr, nullptr, nullptr };
		deviceContext->OMSetRenderTargets(3, nullRTV, nullptr);
	}

};

struct ScreenVertex
{
	float x, y, z;
};

void drawFullScreenQuad(DXCore* dxcore)
{
	static ID3D11Buffer* vb = nullptr;
	static ID3D11Buffer* ib = nullptr;

	if (!vb)
	{
		ScreenVertex vertices[4] =
		{
			{ -1.0f, -1.0f, 0.5f },
			{ -1.0f,  1.0f, 0.5f },
			{  1.0f,  1.0f, 0.5f },
			{  1.0f, -1.0f, 0.5f },
		};

		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(vertices);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = vertices;
		dxcore->device->CreateBuffer(&bd, &initData, &vb);

		DWORD indices[6] = { 0, 1, 2, 0, 2, 3 };
		D3D11_BUFFER_DESC ibd = {};
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.ByteWidth = sizeof(indices);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA iinitData = {};
		iinitData.pSysMem = indices;
		dxcore->device->CreateBuffer(&ibd, &iinitData, &ib);
	}

	UINT stride = sizeof(ScreenVertex);
	UINT offset = 0;
	dxcore->devicecontext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	dxcore->devicecontext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	dxcore->devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dxcore->devicecontext->DrawIndexed(6, 0, 0);
}

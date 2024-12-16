#pragma once
#include<D3Dcompiler.h>
#include<d3d11shader.h>
#include"dxcore.h"
#include<fstream>
#include<string>
#include<sstream>
#include <D3D11.h>
#include <map>
#include <vector>
#include"ShaderReflection .h"

#pragma comment(lib,"dxguid.lib")



class Shader {
public:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* constantBuffer;
	ConstantBuffer constBufferCPU;
	std::vector<ConstantBuffer> psConstantBuffers;
	std::vector<ConstantBuffer> vsConstantBuffers;
	std::map<std::string, int> textureBindPointsVS;
	std::map<std::string, int> textureBindPointsPS;
	std::map<std::string, int> textureBindPoints;
	void init_move(std::string filename1, std::string filename2, DXCore& dxcore) {
		std::string VSshadercore = readfile(filename1);
		std::string PSshadercore = readfile(filename2);
		initConstBuffer(sizeof(ConstantBuffer) + (16 - sizeof(ConstantBuffer) % 16), dxcore);
		loadVS_move(VSshadercore, dxcore);
		loadPS(PSshadercore, dxcore);
	}
	void init_static(std::string filename1, std::string filename2, DXCore& dxcore) {
		std::string VSshadercore = readfile(filename1);
		std::string PSshadercore = readfile(filename2);
		initConstBuffer(sizeof(ConstantBuffer) + (16 - sizeof(ConstantBuffer) % 16), dxcore);
		loadVS_static(VSshadercore, dxcore);
		loadPS(PSshadercore, dxcore);
	}
	//void init_shadow(std::string filename1, std::string filename2, DXCore& dxcore) {
	//	std::string VSshadercore = readfile(filename1);
	//	std::string PSshadercore = readfile(filename2);
	//	initConstBuffer(sizeof(ConstantBuffer) + (16 - sizeof(ConstantBuffer) % 16), dxcore);
	//	loadVS_shadow(VSshadercore, dxcore);
	//	loadPS(PSshadercore, dxcore);
	//}
	void initConstBuffer(int sizeInBytes, DXCore& devicecontext) {
		constBufferCPU.time = 0.f;
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		bd.ByteWidth = sizeInBytes;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		devicecontext.device->CreateBuffer(&bd, NULL, &constantBuffer);
	}

	std::string readfile(std::string abc) {
		std::ifstream ifs(abc);
		std::stringstream buffer;
		buffer << ifs.rdbuf(); 
		return buffer.str();  

	}

	

	void loadPS(std::string pixelShaderHLSL, DXCore& device)
	{
		ID3DBlob* compiledPixelShader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(pixelShaderHLSL.c_str(), strlen(pixelShaderHLSL.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &compiledPixelShader, &status);
		if (FAILED(hr))
		{
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Pixel Shader Error", 0);
			exit(0);
		}
		device.device->CreatePixelShader(compiledPixelShader->GetBufferPointer(), compiledPixelShader->GetBufferSize(), NULL, &pixelShader);
		ConstantBufferReflection reflection;
		reflection.build(&device, compiledPixelShader, psConstantBuffers, textureBindPointsPS, ShaderStage::PixelShader);
	}

	void loadVS_move(std::string vertexShaderHLSL, DXCore& device)
	{
		ID3DBlob* compiledVertexShader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(vertexShaderHLSL.c_str(), strlen(vertexShaderHLSL.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status);

		if (FAILED(hr))
		{
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
			exit(0);
		}

		device.device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);
		
		//�������汾
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
	        { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
	        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
	        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
	        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
	        { "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
	        { "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		device.device->CreateInputLayout(layoutDesc, 6, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);


		ConstantBufferReflection reflection;
		reflection.build(&device, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);
	}
	void loadVS_static(std::string vertexShaderHLSL, DXCore& device)
	{
		ID3DBlob* compiledVertexShader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(vertexShaderHLSL.c_str(), strlen(vertexShaderHLSL.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status);

		if (FAILED(hr))
		{
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
			exit(0);
		}

		device.device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);
		//���������汾
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		device.device->CreateInputLayout(layoutDesc, 4, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);

		ConstantBufferReflection reflection;
		reflection.build(&device, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);
	}
	//void loadVS_shadow(std::string vertexShaderHLSL, DXCore& device) {
	//	ID3DBlob* compiledVertexShader;
	//	ID3DBlob* status;
	//	HRESULT hr = D3DCompile(vertexShaderHLSL.c_str(), strlen(vertexShaderHLSL.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status);
	//	if (FAILED(hr)) {
	//		MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
	//		exit(0);
	//	}
	//	device.device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);
	//	
	//	D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
	//		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//		{ "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//		{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	};
	//	device.device->CreateInputLayout(layoutDesc, 3, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);
	//	ConstantBufferReflection reflection;
	//	reflection.build(&device, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);
	//}

	void apply(DXCore* core) {
		core->devicecontext->IASetInputLayout(layout);
		core->devicecontext->VSSetShader(vertexShader, NULL, 0);
		core->devicecontext->PSSetShader(pixelShader, NULL, 0);
		core->devicecontext->PSSetConstantBuffers(0, 1, &constantBuffer);



		for (int i = 0; i < vsConstantBuffers.size(); i++) {
			vsConstantBuffers[i].upload(core);
		}
		for (int i = 0; i < psConstantBuffers.size(); i++) {
			psConstantBuffers[i].upload(core);
		}
	}



	void updateConstantVS(std::string name, std::string constBuffferName, std::string variableName, void* data) {
		for (int i = 0; i < vsConstantBuffers.size(); i++)
		{
			if (vsConstantBuffers[i].name == constBuffferName)
			{
				vsConstantBuffers[i].update(variableName, data);
			}
		}
	}
	void updateConstantPS(std::string name, std::string constBuffferName, std::string variableName, void* data) {
		for (int i = 0; i < psConstantBuffers.size(); i++)
		{
			if (psConstantBuffers[i].name == constBuffferName)
			{
				psConstantBuffers[i].update(variableName, data);
			}
		}
	}

	void updateTexturePS(DXCore &dxcore,std::string name, ID3D11ShaderResourceView* srv) {
		dxcore.devicecontext->PSSetShaderResources(textureBindPoints[name], 1, &srv);
	}

	void updateTextureVS(DXCore& dxcore, std::string name, ID3D11ShaderResourceView* srv) {
		dxcore.devicecontext->VSSetShaderResources(textureBindPoints[name], 1, &srv);
	}

};




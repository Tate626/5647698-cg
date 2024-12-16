#include<D3Dcompiler.h>
#include<d3d11shader.h>
#include"dxcore.h"
#include<fstream>
#include<string>
#include<sstream>
#include"dxcore.h"

#pragma comment(lib,"dxguid.lib")

class Shader {
public:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;

	std::string readfile(std::string abc) {
		std::ifstream ifs(abc);
		if (!ifs.is_open()) {
			return "";  // 如果文件打开失败，返回空字符串
		}

		std::stringstream buffer;
		buffer << ifs.rdbuf();  // 将文件内容读取到字符串流中
		return buffer.str();  // 返回文件内容作为字符串

	}

	void compileVS(std::string vertexShaderHLSL,DXCore &dxcore) {
		ID3DBlob* compiledVertexShader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(vertexShaderHLSL.c_str(), strlen(vertexShaderHLSL.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status);
		dxcore.device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);
		if (FAILED(hr)) {
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vixel_shader Error", 0);
			exit(0);
		}
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		dxcore.device->CreateInputLayout(layoutDesc, 2, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);

	}

	void compilePS(std::string pixelShaderHLSL, DXCore& dxcore) {
		ID3DBlob* compiledPixelShader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(pixelShaderHLSL.c_str(), strlen(pixelShaderHLSL.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &compiledPixelShader, &status);
		dxcore.device->CreatePixelShader(compiledPixelShader->GetBufferPointer(), compiledPixelShader->GetBufferSize(), NULL, &pixelShader);
		if (FAILED(hr)) {
			MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Pixel_shader Error", 0);
			exit(0);
		}
	}

	void apply(DXCore& dxcore) {
		dxcore.devicecontext->VSSetShader(vertexShader, NULL, 0);
		dxcore.devicecontext->PSSetShader(pixelShader, NULL, 0);
	}

	void init( DXCore& dxcore) {
		std::string VSshadercore = readfile("VSshader.txt");
		std::string PSshadercore = readfile("PSshader.txt");
		compileVS(VSshadercore,dxcore);
		compilePS(PSshadercore,dxcore);
		apply(dxcore);

	}
};
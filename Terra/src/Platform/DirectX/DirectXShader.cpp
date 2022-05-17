#include "Terrapch.h"
#include "DirectXShader.h"

//Microsoft::WRL::ComPtr<ID3DBlob> Terra::DirectXShader::pVTERRAyteCodeBlob = NULL;
Terra::DirectXShader::DirectXShader(const std::wstring& path, ShaderType shaderType)
	: m_Path(path), m_Shadertype(shaderType)
{
	TERRA_ASSERT(DirectXRendererAPI::GetDXGraphicsContext(), "Graphics context is null!");
	INFOMAN(*DirectXRendererAPI::GetDXGraphicsContext());
	
	switch (shaderType)
	{	
		case ShaderType::None:
			TERRA_ASSERT("Failed to create shader: unknown shader type");
			break;	
		case ShaderType::Vertex:
		{

			GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pVSByteCodeBlob));
			GFX_THROW_INFO(DirectXRendererAPI::GetDevice()->CreateVertexShader(
				pVSByteCodeBlob->GetBufferPointer(), pVSByteCodeBlob->GetBufferSize(), nullptr, &pVertexShader));
			break;
		}
		case ShaderType::Pixel:
		{
			GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pPSByteCodeBlob));
			GFX_THROW_INFO(DirectXRendererAPI::GetDevice()->CreatePixelShader(
				pPSByteCodeBlob->GetBufferPointer(), pPSByteCodeBlob->GetBufferSize(), nullptr, &pPixelShader));
			break;
		}
	}
}

void Terra::DirectXShader::Bind() const
{
	switch (m_Shadertype)
	{
	case ShaderType::None:
		TERRA_ASSERT("Failed to bind shader: unknown shader type");
		break;
		
	case ShaderType::Vertex:
		DirectXRendererAPI::GetDeviceContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);
		break;

	case ShaderType::Pixel:
		DirectXRendererAPI::GetDeviceContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);
		break;
	}
}

void Terra::DirectXShader::Unbind() const
{
}

void Terra::DirectXShader::SetInt(const std::string& name, int value)
{
}

void Terra::DirectXShader::SetIntArray(const std::string& name, int* values, uint32_t count)
{
}

void Terra::DirectXShader::SetFloat(const std::string& name, float value)
{
}



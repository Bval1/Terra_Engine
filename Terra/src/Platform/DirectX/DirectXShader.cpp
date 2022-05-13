#include "Terrapch.h"
#include "DirectXShader.h"

//Microsoft::WRL::ComPtr<ID3DBlob> Terra::DirectXShader::pVSByteCodeBlob = NULL;
Terra::DirectXShader::DirectXShader(const std::wstring& path, ShaderType shaderType)
	: m_Path(path), m_Shadertype(shaderType)
{
	SB_ASSERT(DirectXRendererAPI::GetDXGraphicsContext(), "Graphics context is null!");
	INFOMAN(*DirectXRendererAPI::GetDXGraphicsContext());
	
	switch (shaderType)
	{	
		case ShaderType::None:
			SB_ASSERT("Failed to create shader: unknown shader type");
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
		SB_ASSERT("Failed to bind shader: unknown shader type");
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

void Terra::DirectXShader::SetFloat3(const std::string& name, const glm::vec3& value)
{
}

void Terra::DirectXShader::SetFloat4(const std::string& name, const glm::vec4& value)
{
}

void Terra::DirectXShader::SetMat4(const std::string& name, const glm::mat4& value)
{
}

#pragma once
#include "DirectXRendererAPI.h"
#include "Terra/Renderer/Shader.h"
namespace Terra
{
	class DirectXShader : public Shader
	{
	public:
		DirectXShader(const std::wstring& path, ShaderType shaderType);
		~DirectXShader() = default;
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		virtual const std::string& GetName() const { return "Shader name"; }
		Microsoft::WRL::ComPtr<ID3DBlob> GetVSByteCode() 
		{ 
			SB_ASSERT(pVSByteCodeBlob, "VS Byte code blob is null"); 
			return pVSByteCodeBlob; 
		}

	private:
		DXGIInfoManager& GetInfoManager(DirectXContext& gfx) { return gfx.infoManager; }
	private:
		std::wstring m_Path;
		ShaderType m_Shadertype;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> pPSByteCodeBlob;
		//static Microsoft::WRL::ComPtr<ID3DBlob> pVSByteCodeBlob; // needed in input layout
		Microsoft::WRL::ComPtr<ID3DBlob> pVSByteCodeBlob;
	};

}
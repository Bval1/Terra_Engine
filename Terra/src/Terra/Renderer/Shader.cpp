#include "Terrapch.h"
#include "Renderer.h"
#include "Platform/DirectX/DirectXShader.h"

namespace Terra {

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	TERRA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::DirectX:	TERRA_CORE_ASSERT(false, "DirectX shader not supported with this method"); return nullptr;
		}

		TERRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	TERRA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::DirectX:	TERRA_CORE_ASSERT(false, "DirectX shader not supported with this method"); return nullptr;
		}

		TERRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::wstring& filepath, ShaderType shaderType)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	TERRA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::DirectX:	return std::make_shared<DirectXShader>(filepath, shaderType);
		}

		TERRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		TERRA_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		TERRA_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}
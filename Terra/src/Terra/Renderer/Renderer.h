#pragma once

#include "RenderCommand.h"
#include "Shader.h"
#include "UniformBuffer.h"
namespace Terra {



	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);


		static void EndScene();

		static void Submit(const Ref<Shader>& vs, const Ref<Shader>& ps,
			const Ref<VertexArray>& vertexArray);

		// 2nd cb is optional
		static void Submit(const Ref<Shader>& vs, const Ref<Shader>& ps, const Ref<VertexArray>& vertexArray, 
			const Ref<UniformBuffer>& constantBuffer, const Ref<UniformBuffer>& cb2 = 0);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			
		};

		static SceneData* m_SceneData;
	};
	
}
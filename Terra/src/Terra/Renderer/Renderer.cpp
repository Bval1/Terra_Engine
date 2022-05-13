#include "Terrapch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

namespace Terra {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		TERRA_PROFILE_FUNCTION();
		RenderCommand::Init();
#ifndef DIRECTX
		Renderer2D::Init();
#endif
		Renderer3D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(0, 0, width, height);
	}

	void Renderer::EndScene()
	{
	}


	void Renderer::Submit(const Ref<Shader>& vs, const Ref<Shader>& ps, const Ref<VertexArray>& vertexArray)
	{
		vs->Bind();
		ps->Bind();
		vertexArray->Bind();	// sets input, binds VB and IB
		RenderCommand::DrawIndexed(vertexArray);
	}


	void Renderer::Submit(const Ref<Shader>& vs, const Ref<Shader>& ps, const Ref<VertexArray>& vertexArray, 
		const Ref<UniformBuffer>& constantBuffer, const Ref<UniformBuffer>& cb2)
	{
		constantBuffer->Bind();
		if (cb2)
			cb2->Bind();
		
		vs->Bind();
		ps->Bind();
		vertexArray->Bind();	// sets input, binds VB and IB
		RenderCommand::DrawIndexed(vertexArray);
	}
}
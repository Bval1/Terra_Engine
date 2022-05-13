#pragma once
#include "RendererAPI.h"

namespace Terra {

	class RenderCommand
	{
	public:

		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewPort(x, y, width, height);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void SetClearColor(const DirectX::XMFLOAT4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			// Render Command/Draw Call
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			// Render Command/Draw Call
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}

		inline static void SetLineWidth(float width)
		{
			s_RendererAPI->SetLineWidth(width);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}
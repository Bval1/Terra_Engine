#pragma once
#include "Terra/Renderer/VertexArray.h"
#include "DirectXRendererAPI.h"
namespace Terra {

	class DirectXVertexArray : public VertexArray
	{
		friend class DirectXVertexBuffer;
	public:
		DirectXVertexArray();
		virtual ~DirectXVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		DXGIInfoManager& GetInfoManager(DirectXContext& gfx) { return gfx.infoManager; }
		static DXGI_FORMAT ShaderDataTypeToDXGIFormat(ShaderDataType type);
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		// List of all vertex buffers 
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;					// only 1 index buffer usually
		uint32_t m_VertexBufferIndex = 0;
	};
}
#pragma once
#include "Terra/Renderer/Buffer.h"
#include "DirectXRendererAPI.h"
#include "DirectXContext.h"
namespace Terra {

	class DirectXVertexBuffer : public VertexBuffer
	{
	public:
		DirectXVertexBuffer(float* vertices, uint32_t size);
		DirectXVertexBuffer(uint32_t size);			// for batch rendering
		virtual ~DirectXVertexBuffer() = default;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;
		virtual void SetNumberofBuffers(uint32_t count) override { m_numOfBuffers = count; }
		virtual void SetLayout(const BufferLayout& layout) override;
		virtual  const BufferLayout& GetLayout() const override { return m_Layout; };
		virtual float* GetVertices() const override { return m_vertices; };
	private:
		static DXGIInfoManager& GetInfoManager(DirectXContext& gfx) { return gfx.infoManager; }
	private:
		float* m_vertices;
		uint32_t m_size;
		uint32_t m_numOfBuffers = 1u;
		BufferLayout m_Layout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		UINT m_Stride;
	};

	// Currently only supports 32-bit index buffers
	class DirectXIndexBuffer : public IndexBuffer
	{
	public:
		DirectXIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~DirectXIndexBuffer();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetCount() const override { return m_Count; }	
	private:
		static DXGIInfoManager& GetInfoManager(DirectXContext& gfx) { return gfx.infoManager; }
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
		uint32_t m_Count;
	};
}
#pragma once
#include <DirectXMath.h>
#include "Model.h"
namespace Terra
{
	class Cube : public Model
	{
	public:
		Cube() = default;
		virtual ~Cube() = default;
	
		static Cube CreateSolid();	// with no texture
		static Cube Create();		// with texture

		virtual float* VertexData() override { return m_vertexData; }
		virtual uint32_t VertexDataSize() const override { return m_vertexSize; }
		virtual void ClearVertexData() override { delete[] m_vertexData; }
		virtual Ref<IndexBuffer>& GetIndexBuffer() override { return m_indexBuffer; }

	private:
		// should only be called from Create() functions
		Cube(std::vector<float>& vertices, std::vector<uint32_t>& indices)
		{
			m_indexBuffer = IndexBuffer::Create(indices.data(), indices.size());
			m_vertexData = new float[vertices.size()];
			m_vertexSize = vertices.size() * sizeof(float);
			memcpy(m_vertexData, vertices.data(), m_vertexSize);
		}

		template<typename T>
		static void SetNormals(std::vector<T>& vertices, std::vector<uint32_t>& indices)
		{
			using namespace DirectX;  // to use - overload
			// assert face-independent vertices w/ normals cleared to zero
			SB_ASSERT(indices.size() % 3 == 0 && indices.size() > 0);
			for (uint32_t i = 0; i < indices.size(); i += 3)
			{
				auto& v0 = vertices[indices[i]];
				auto& v1 = vertices[indices[i + 1]];
				auto& v2 = vertices[indices[i + 2]];

				const auto p0 = XMLoadFloat3(&v0.pos);
				const auto p1 = XMLoadFloat3(&v1.pos);
				const auto p2 = XMLoadFloat3(&v2.pos);

				const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

				XMStoreFloat3(&v0.n, n);
				XMStoreFloat3(&v1.n, n);
				XMStoreFloat3(&v2.n, n);
			}
		}

		template<typename T>
		static void SetTextureCoordinates(std::vector<T>& vertices)
		{
			SB_ASSERT(vertices.size() == 24);
			vertices[0].tex =	{ 0.0f,0.0f };
			vertices[1].tex =	{ 1.0f,0.0f };
			vertices[2].tex =	{ 0.0f,1.0f };
			vertices[3].tex =	{ 1.0f,1.0f };
			vertices[4].tex =	{ 0.0f,0.0f };
			vertices[5].tex =	{ 1.0f,0.0f };
			vertices[6].tex =	{ 0.0f,1.0f };
			vertices[7].tex =	{ 1.0f,1.0f };
			vertices[8].tex =	{ 0.0f,0.0f };
			vertices[9].tex =	{ 1.0f,0.0f };
			vertices[10].tex =	{ 0.0f,1.0f };
			vertices[11].tex =	{ 1.0f,1.0f };
			vertices[12].tex =	{ 0.0f,0.0f };
			vertices[13].tex =	{ 1.0f,0.0f };
			vertices[14].tex =	{ 0.0f,1.0f };
			vertices[15].tex =	{ 1.0f,1.0f };
			vertices[16].tex =	{ 0.0f,0.0f };
			vertices[17].tex =	{ 1.0f,0.0f };
			vertices[18].tex =	{ 0.0f,1.0f };
			vertices[19].tex =	{ 1.0f,1.0f };
			vertices[20].tex =	{ 0.0f,0.0f };
			vertices[21].tex =	{ 1.0f,0.0f };
			vertices[22].tex =	{ 0.0f,1.0f };
			vertices[23].tex =	{ 1.0f,1.0f };
		}

	public:
		DirectX::XMMATRIX transform;
		DirectX::XMFLOAT4 color;
		Ref<Texture2D> texture;
		std::vector<Ref<UniformBuffer>> constantBuffers;

	protected:
		float* m_vertexData;
		uint32_t m_vertexSize;
		Ref<IndexBuffer> m_indexBuffer;
	};
}


#pragma once
#include <DirectXMath.h>
#include "Model.h"
namespace Terra {

	class Plane : public Model
	{
	public:
		~Plane() = default;
		static Plane Create();			// textured, bind white texture for solid color

		virtual float* VertexData() override { return m_vertexData; }
		virtual uint32_t VertexDataSize() const override { return m_vertexSize; }
		virtual void ClearVertexData() override { delete[] m_vertexData; }
		virtual Ref<IndexBuffer>& GetIndexBuffer() override { return m_indexBuffer; }
	
	protected:
		Plane(std::vector<float>& vertices, std::vector<uint32_t>& indices)
		{
			m_indexBuffer = IndexBuffer::Create(indices.data(), indices.size());

			m_vertexData = new float[vertices.size()];
			m_vertexSize = vertices.size() * sizeof(float);
			memcpy(m_vertexData, vertices.data(), m_vertexSize);
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
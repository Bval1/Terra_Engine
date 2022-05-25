#pragma once
#include <DirectXMath.h>
#include "Model.h"
namespace Terra
{
	class Sphere : public Model
	{

	public:
		~Sphere();
		static Sphere Create();
		static Sphere CreateNormalized();
		static Sphere CreateTesselated(uint32_t latDiv, uint32_t longDiv);
		static Sphere CreateTesselatedNormalized(uint32_t latDiv, uint32_t longDiv);
		
		virtual float* VertexData() override { return m_vertexData; }

		virtual uint32_t VertexDataSize() const override { return m_vertexSize; }

		virtual void ClearVertexData() override { delete[] m_vertexData; }

		virtual Ref<IndexBuffer>& GetIndexBuffer() override { return m_indexBuffer; }
	
	protected:
		Sphere(std::vector<float>& vertices, std::vector<uint32_t>& indices)
			
		{
			m_indexBuffer = IndexBuffer::Create(indices.data(), indices.size());
			m_vertexData = new float[vertices.size()];
			m_vertexSize = vertices.size() * sizeof(float);
			memcpy(m_vertexData, vertices.data(), m_vertexSize);
		}

		static void FillIndices(std::vector<uint32_t>& indices_in, uint32_t latDiv, uint32_t longDiv, uint32_t vertexCount);
	
	public:
		DirectX::XMMATRIX transform;
		DirectX::XMFLOAT4 color;
		Ref<Texture2D> texture;
		std::unordered_map<std::string, Ref<UniformBuffer>> constantBuffers;
		struct Material
		{
			DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			float specularIntensity = 0.8f;
			float specularPower = 30.0f;
			float padding[2];
		} material;

	protected:
		float* m_vertexData;
		uint32_t m_vertexSize;
		Ref<IndexBuffer> m_indexBuffer;
	};
}


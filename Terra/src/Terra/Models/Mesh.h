#pragma once
#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <DirectXMath.h>
namespace Terra {


	class Mesh : public Model
	{
	public:
		Mesh() = default;
#if 0
		Mesh(std::vector<float>& vertices, std::vector<uint32_t>& indices, Ref<Texture2D> diffuse)
		{
			m_indexBuffer = IndexBuffer::Create(indices.data(), indices.size());

			m_vertexData = new float[vertices.size()];
			m_vertexSize = vertices.size() * sizeof(float);
			memcpy(m_vertexData, vertices.data(), m_vertexSize);
			texture = diffuse;
		}

		Mesh(std::vector<float>& vertices, std::vector<uint32_t>& indices, std::vector<Ref<Mesh>> childMeshes, Ref<Texture2D> diffuse)
		{
			m_indexBuffer = IndexBuffer::Create(indices.data(), indices.size());

			m_vertexData = new float[vertices.size()];
			m_vertexSize = vertices.size() * sizeof(float);
			memcpy(m_vertexData, vertices.data(), m_vertexSize);

			m_pMeshes.resize(childMeshes.size());
			m_pMeshes = std::move(childMeshes);
			texture = diffuse;
		}
#endif
		Mesh(void* vertices, uint32_t vertexElementCount, uint32_t* indices, uint32_t indexCount, 
			Ref<Texture2D> diffuseTex, DirectX::XMFLOAT4 matColor)
		{
			m_indexBuffer = IndexBuffer::Create(indices, indexCount);

			m_vertexData = new float[vertexElementCount];
			m_vertexSize = vertexElementCount * sizeof(float);
			memcpy(m_vertexData, vertices, m_vertexSize);

			texture = diffuseTex;
			color = matColor;
		}

		Mesh(std::vector<Ref<Mesh>> childMeshes)
		{
			m_pMeshes.resize(childMeshes.size());
			m_pMeshes = std::move(childMeshes);
			//texture = diffuse;
		}

		virtual ~Mesh() = default;
		
		static Mesh Create(const std::string& path);

		virtual Ref<IndexBuffer>& GetIndexBuffer() override { return m_indexBuffer; }
		virtual float* VertexData() override { return m_vertexData; }
		virtual uint32_t VertexDataSize() const override { return m_vertexSize; }
		virtual void ClearVertexData() override;
	
	
		std::vector<Terra::Ref<Mesh>>& GetChildMeshes() { return m_pMeshes; }
	private:
		static Terra::Ref<Mesh> ParseMesh(const std::string& basePath, const aiMesh* mesh, const aiMaterial* const* pMaterials);
	
	public:
		DirectX::XMMATRIX transform;
		DirectX::XMFLOAT4 color;
		Ref<Texture2D> texture;
		std::vector<Ref<UniformBuffer>> constantBuffers;
		float specularPower = 0.0f;
	
	protected:
		float* m_vertexData;
		uint32_t m_vertexSize;
		Ref<IndexBuffer> m_indexBuffer;
		std::vector<Terra::Ref<Mesh>> m_pMeshes;

	};

}
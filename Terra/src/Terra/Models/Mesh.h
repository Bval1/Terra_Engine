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

		Mesh(void* vertices, uint32_t vertexElementCount, uint32_t* indices, uint32_t indexCount, std::vector<Ref<Texture2D>> texs,
			DirectX::XMFLOAT4 matColor = { 1.f , 1.f, 1.f, 1.f }, bool hasSpec = 0)
		{
			m_indexBuffer = IndexBuffer::Create(indices, indexCount);

			m_vertexData = new float[vertexElementCount];
			m_vertexSize = vertexElementCount * sizeof(float);
			memcpy(m_vertexData, vertices, m_vertexSize);

			textures = texs;
			color = matColor;
			hasSpecular = hasSpec;
		}

		Mesh(std::vector<Ref<Mesh>> childMeshes)
		{
			m_pMeshes.resize(childMeshes.size());
			m_pMeshes = std::move(childMeshes);
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
		std::vector<Ref<Texture2D>> textures;
		std::vector<Ref<UniformBuffer>> constantBuffers;
		Ref<UniformBuffer> VertexCB;
		Ref<UniformBuffer> PixelCB;
		float specularPower = 0.0f;
		bool hasSpecular = false;
	
	protected:
		float* m_vertexData;
		uint32_t m_vertexSize;
		Ref<IndexBuffer> m_indexBuffer;
		std::vector<Terra::Ref<Mesh>> m_pMeshes;

	};

}
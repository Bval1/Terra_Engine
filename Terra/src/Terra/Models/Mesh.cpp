#include "Terrapch.h"
#include "Mesh.h"
#include "Platform/DirectX/DirectXTexture2D.h"
Terra::Mesh Terra::Mesh::Create(const std::string& path)
{
	TERRA_PROFILE_FUNCTION();
	Assimp::Importer imp;
	auto pModel = imp.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | 
		aiProcess_GenNormals);


	SB_ASSERT(pModel, imp.GetErrorString());


	const auto root = pModel->mMeshes[0];
	
	//using namespace std::string_literals;
	//const auto base = "assets\\models\\nano_textured\\"s;
	const auto baseDirectory = path.substr(0u, 1u + path.find_last_of("\\"));
	
	std::vector<std::shared_ptr<Mesh>> childMeshes;
	for (size_t i = 0; i < pModel->mNumMeshes; i++)
	{
		childMeshes.emplace_back(ParseMesh(baseDirectory, pModel->mMeshes[i], pModel->mMaterials));
	}

	return { childMeshes };
}

void Terra::Mesh::ClearVertexData()
{
	//delete[] m_vertexData;
	for (auto& childMesh : m_pMeshes)
		delete[] childMesh->m_vertexData;

}

Terra::Ref<Terra::Mesh> Terra::Mesh::ParseMesh(const std::string& basePath, const aiMesh* mesh, const aiMaterial* const* pMaterials)
{
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
		DirectX::XMFLOAT2 uv;
	};

	std::vector<Vertex> vertices(mesh->mNumVertices);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices[i] =
		{
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh->mVertices[i]),
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh->mNormals[i]),
			*reinterpret_cast<DirectX::XMFLOAT2*>(&mesh->mTextureCoords[0][i])
		};
	}

	std::vector<uint32_t> indices;
	indices.reserve(mesh->mNumFaces * 3);
	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		const auto& face = mesh->mFaces[i];
		SB_ASSERT(face.mNumIndices == 3, "Faces should be all triangles");
		indices.emplace_back(face.mIndices[0]);
		indices.emplace_back(face.mIndices[1]);
		indices.emplace_back(face.mIndices[2]);
	}
	Ref<Texture2D> diffuseTex;
	DirectX::XMFLOAT4 meshColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	float shine = 25.0f;
	bool hasSpecularMap = false;
	if (mesh->mMaterialIndex >= 0)
	{
		auto& material = *pMaterials[mesh->mMaterialIndex];
		
		aiColor3D color(0.f, 0.f, 0.f);
		material.Get(AI_MATKEY_COLOR_DIFFUSE, color);
		meshColor = DirectX::XMFLOAT4( color.r, color.g, color.b, 1.f );
		
		aiString texFileName;
		material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName);
		diffuseTex = CreateRef<DirectXTexture2D>(basePath + texFileName.C_Str());
		material.Get(AI_MATKEY_SHININESS, shine);
	}
	else
	{
		TERRA_WARN("No materials found in mesh");
		diffuseTex= CreateRef<DirectXTexture2D>(1.0f, 1.0f);
	}

	const auto& numofElements = sizeof(Vertex) / sizeof(float);

	
	return Terra::CreateRef<Mesh>(vertices.data(), numofElements * vertices.size(), indices.data(),
		indices.size(), diffuseTex, meshColor);
}

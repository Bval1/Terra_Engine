#include "Terrapch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"
#include "VertexArray.h"

namespace Terra {

	struct FrameData
	{
		std::unordered_map<std::string, Ref<Mesh>> MeshMap;
		std::unique_ptr<PointLight> Light = nullptr;
		std::unique_ptr<Cube> Cube = nullptr;	
		std::unique_ptr<Plane> Plane = nullptr;
		std::unique_ptr<Sphere> Sphere = nullptr;

		uint32_t MeshCount = 0u;
		uint32_t CubeCount = 0u;
		uint32_t PlaneCount = 0u;
		uint32_t SphereCount = 0u;
		
		Ref<VertexBuffer> SolidMeshVertexBuffer;	// these meshes have no uv coordinates defined
		Ref<VertexBuffer> TexMeshVertexBuffer;
		Ref<VertexBuffer> LightVertexBuffer;
		Ref<VertexArray> VertexArray;
		
		Ref<Shader> TexturedPhongVS;
		Ref<Shader> TexturedPhongPS;
		Ref<Shader> NormalMapPhongPS;

		Ref<Shader> SpecularPhongPS;

		Ref<Shader> SolidPhongVS;
		Ref<Shader> SolidPhongPS;

		Ref<Shader> BasicVS;
		Ref<Shader> BasicPS;
		
		Ref<Texture2D> WhiteTexture;
	};

	struct Material
	{
		//DirectX::XMFLOAT4 color;
		float specularIntensity = 0.8f;
		float specularPower = 30.0f;
		float padding[2];
	} material;

	struct ModelViewMat
	{
		DirectX::XMMATRIX modelView;
		DirectX::XMMATRIX modelViewProj;
	} modelViewMat;

	struct CameraData
	{
		DirectX::XMMATRIX ViewMatrix;
		DirectX::XMMATRIX ViewProjection;
	} cameraData;

	static FrameData s_FrameData;
	static constexpr size_t MaxModels = 1000;
}

void Terra::Renderer3D::Init()
{
	TERRA_PROFILE_FUNCTION();

	s_FrameData.VertexArray = VertexArray::Create();
	s_FrameData.TexturedPhongVS = Shader::Create(L"assets/shaders/D3D11/output/Renderer3D_VS.cso", Terra::ShaderType::Vertex);
	s_FrameData.TexturedPhongPS = Shader::Create(L"assets/shaders/D3D11/output/Renderer3D_PS.cso", Terra::ShaderType::Pixel);
	s_FrameData.SpecularPhongPS = Shader::Create(L"assets/shaders/D3D11/output/Renderer3D_SpecularPS.cso", Terra::ShaderType::Pixel);
	s_FrameData.NormalMapPhongPS = Shader::Create(L"assets/shaders/D3D11/output/NormalMapPS.cso", Terra::ShaderType::Pixel);

	s_FrameData.SolidPhongVS = Shader::Create(L"assets/shaders/D3D11/output/Renderer3DSolid_VS.cso", Terra::ShaderType::Vertex);
	s_FrameData.SolidPhongPS = Shader::Create(L"assets/shaders/D3D11/output/Renderer3DSolid_PS.cso", Terra::ShaderType::Pixel);
	s_FrameData.BasicVS = Shader::Create(L"assets/shaders/D3D11/output/VS.cso", Terra::ShaderType::Vertex);
	s_FrameData.BasicPS = Shader::Create(L"assets/shaders/D3D11/output/PS.cso", Terra::ShaderType::Pixel);
	
	// Init Light
	s_FrameData.Light = std::make_unique<PointLight>();
	s_FrameData.LightVertexBuffer = VertexBuffer::Create(sizeof(VertexBuffer)); // will resize when setting data
	BufferLayout layout_basic = 
	{
		{"Position", 0, Terra::ShaderDataType::Float3, 0, 0, 0, 0},
	};
	layout_basic.SetVertexShader(s_FrameData.BasicVS);
	s_FrameData.LightVertexBuffer->SetLayout(layout_basic);
	s_FrameData.LightVertexBuffer->SetData(s_FrameData.Light->VertexData(), s_FrameData.Light->VertexDataSize());
	

	// Init Solid Models
	s_FrameData.Sphere = std::make_unique<Sphere>(Sphere::CreateTesselatedNormalized(12, 24));
	s_FrameData.SolidMeshVertexBuffer = VertexBuffer::Create(sizeof(VertexBuffer)); // will resize when setting data
	BufferLayout layout = 
	{
		{"Position", 0, Terra::ShaderDataType::Float3, 0, 0, 0, 0},
		{"Normal", 0, Terra::ShaderDataType::Float3, 0, 12, 0, 0},
	};
	layout.SetVertexShader(s_FrameData.SolidPhongVS);
	s_FrameData.SolidMeshVertexBuffer->SetLayout(layout);


	// Init Textured Models
	s_FrameData.Cube = std::make_unique<Cube>(Cube::Create());
	s_FrameData.Plane = std::make_unique<Plane>(Plane::Create());

	s_FrameData.TexMeshVertexBuffer = VertexBuffer::Create(sizeof(VertexBuffer));
	BufferLayout layout_textured =
	{
		{"Position", 0, Terra::ShaderDataType::Float3, 0, 0, 0, 0},
		{"Normal", 0, Terra::ShaderDataType::Float3, 0, 12, 0, 0},
		{"TexCoord", 0, Terra::ShaderDataType::Float2, 0, 24, 0, 0}
	};
	layout_textured.SetVertexShader(s_FrameData.TexturedPhongVS);
	s_FrameData.TexMeshVertexBuffer->SetLayout(layout_textured);
	
	
	s_FrameData.WhiteTexture = Texture2D::Create(1u, 1u);
}

void Terra::Renderer3D::Shutdown()
{
	s_FrameData.Light->ClearVertexData();
	s_FrameData.Cube->ClearVertexData();
	s_FrameData.Plane->ClearVertexData();
	s_FrameData.Sphere->ClearVertexData();
	for (auto const& [path, model] : s_FrameData.MeshMap)
		model->ClearVertexData(); // delete vertices
}

void Terra::Renderer3D::BeginScene(const PerspectiveCamera& camera)
{
	// init cameraData
	cameraData.ViewMatrix = camera.GetViewMartrix();
	cameraData.ViewProjection = camera.GetCameraProjection();
}

void Terra::Renderer3D::EndScene()
{
	Flush();
	s_FrameData.Light->constantBuffers.clear();
	s_FrameData.Plane->constantBuffers.clear();
	s_FrameData.Plane->textures.clear();
	s_FrameData.Cube->constantBuffers.clear();
	s_FrameData.Cube->textures.clear();
	s_FrameData.Sphere->constantBuffers.clear();


	s_FrameData.MeshCount = 0u;
	s_FrameData.CubeCount = 0u;
	s_FrameData.PlaneCount = 0u;
	s_FrameData.SphereCount = 0u;
}

void Terra::Renderer3D::StartBatch()
{
}

void Terra::Renderer3D::NextBatch()
{
}

void Terra::Renderer3D::Flush()
{
	TERRA_PROFILE_FUNCTION();
	constexpr uint32_t CB_count = 2u;	// cbs per model
	
	// Light
	s_FrameData.Light->Bind(cameraData.ViewMatrix);
	s_FrameData.VertexArray->AddVertexBuffer(s_FrameData.LightVertexBuffer);
	s_FrameData.VertexArray->SetIndexBuffer(s_FrameData.Light->GetIndexBuffer());
	s_FrameData.BasicVS->Bind();
	s_FrameData.BasicPS->Bind();
	s_FrameData.VertexArray->Bind();

	s_FrameData.Light->constantBuffers.at("TCB")->Bind();
	s_FrameData.Light->constantBuffers.at("PCB")->Bind();
	RenderCommand::DrawIndexed(s_FrameData.VertexArray);

	
	// Cube
	if (s_FrameData.CubeCount)
	{
		s_FrameData.TexMeshVertexBuffer->SetData(s_FrameData.Cube->VertexData(), s_FrameData.Cube->VertexDataSize());
		s_FrameData.VertexArray->AddVertexBuffer(s_FrameData.TexMeshVertexBuffer);
		s_FrameData.VertexArray->SetIndexBuffer(s_FrameData.Cube->GetIndexBuffer());

		s_FrameData.TexturedPhongVS->Bind();
		s_FrameData.TexturedPhongPS->Bind();
		s_FrameData.VertexArray->Bind();

		if (s_FrameData.Cube->textures.empty())
			s_FrameData.WhiteTexture->Bind();

		const auto& texDict = s_FrameData.Cube->textures;
		for (size_t i = 0; i < s_FrameData.CubeCount; i++)
		{
			texDict.at("diffuse" + std::to_string(i))->Bind();
			s_FrameData.Cube->constantBuffers.at("TCB" + std::to_string(i))->Bind();
			s_FrameData.Cube->constantBuffers.at("PCB" + std::to_string(i))->Bind();
			RenderCommand::DrawIndexed(s_FrameData.VertexArray);
			i += CB_count;
		}
	}
	// Plane
	s_FrameData.TexMeshVertexBuffer->SetData(s_FrameData.Plane->VertexData(), s_FrameData.Plane->VertexDataSize());
	s_FrameData.VertexArray->AddVertexBuffer(s_FrameData.TexMeshVertexBuffer);
	s_FrameData.VertexArray->SetIndexBuffer(s_FrameData.Plane->GetIndexBuffer());
	s_FrameData.TexturedPhongVS->Bind();
	s_FrameData.NormalMapPhongPS->Bind();
	s_FrameData.VertexArray->Bind();

	const auto& texDict = s_FrameData.Plane->textures;
	for (size_t i = 0; i < s_FrameData.PlaneCount; i++)
	{

		if (texDict.empty())
		{
			s_FrameData.WhiteTexture->Bind();
		}
		else
		{
			texDict.at("diffuse" + std::to_string(i))->Bind();
			if (texDict.find("normal" + std::to_string(i)) != texDict.end())
				texDict.at("normal" + std::to_string(i))->Bind();
		}
	
		s_FrameData.Plane->constantBuffers.at("TCB" + std::to_string(i))->Bind();
		s_FrameData.Plane->constantBuffers.at("PCB" + std::to_string(i))->Bind();
		RenderCommand::DrawIndexed(s_FrameData.VertexArray);
	}

	// Custom Meshes
	for (const auto& [path, meshBase] : s_FrameData.MeshMap)
	{		
		for (auto& childMesh : meshBase->GetChildMeshes())
		{
			FlushMesh(childMesh, childMesh->hasSpecular);
		}
	}
	
	// Sphere
	if (s_FrameData.SphereCount)
	{
		s_FrameData.SolidMeshVertexBuffer->SetData(s_FrameData.Sphere->VertexData(), s_FrameData.Sphere->VertexDataSize());
		s_FrameData.VertexArray->AddVertexBuffer(s_FrameData.SolidMeshVertexBuffer);
		s_FrameData.VertexArray->SetIndexBuffer(s_FrameData.Sphere->GetIndexBuffer());

		s_FrameData.SolidPhongVS->Bind();
		s_FrameData.SolidPhongPS->Bind();
		s_FrameData.VertexArray->Bind();
		for (size_t i = 0; i < s_FrameData.SphereCount; i++)
		{

			s_FrameData.Sphere->constantBuffers.at("TCB" + std::to_string(i))->Bind();
			s_FrameData.Sphere->constantBuffers.at("PCB" + std::to_string(i))->Bind();
			RenderCommand::DrawIndexed(s_FrameData.VertexArray);
		}
	}
}

void Terra::Renderer3D::FlushMesh(const Ref<Mesh>& mesh, bool hasSpecular)
{
	s_FrameData.TexMeshVertexBuffer->SetData(mesh->VertexData(), mesh->VertexDataSize());
	s_FrameData.VertexArray->AddVertexBuffer(s_FrameData.TexMeshVertexBuffer);
	s_FrameData.VertexArray->SetIndexBuffer(mesh->GetIndexBuffer());
	for (const auto& tex : mesh->textures)
	{
		if (tex)
		{
			tex->Bind();
		}
	}

	s_FrameData.TexturedPhongVS->Bind();
	
	if (!hasSpecular)
		s_FrameData.TexturedPhongPS->Bind();
	else
		s_FrameData.SpecularPhongPS->Bind();

	s_FrameData.VertexArray->Bind();

	mesh->VertexCB->Bind();

	
	// Uses a pixel shader that takes in a pCB
	if (!hasSpecular)
	{
		mesh->PixelCB->Bind();
	}

	RenderCommand::DrawIndexed(s_FrameData.VertexArray); 
}


void Terra::Renderer3D::DrawPointLight(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT4& diffusecolor)
{
	auto& pointlight = s_FrameData.Light;
	pointlight->SetPosition(pos);
	const auto& modelView = pointlight->GetTransformMatrix() * cameraData.ViewMatrix;
	modelViewMat =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(modelView * cameraData.ViewProjection)
	};

	pointlight->constantBuffers.insert({ "TCB", UniformBuffer::Create(&modelViewMat, sizeof(ModelViewMat), 0u,
		Terra::UniformBuffer::ConstantBufferType::Vertex) });

	pointlight->SetDiffuseColor(diffusecolor);

	pointlight->constantBuffers.insert({ "PCB", UniformBuffer::Create(&diffusecolor, sizeof(diffusecolor), 1u,
		Terra::UniformBuffer::ConstantBufferType::Pixel) });
}


void Terra::Renderer3D::DrawPointLight(DirectX::XMFLOAT3& pos)
{
	auto& pointlight = s_FrameData.Light;
	pointlight->SetPosition(pos);
	const auto& modelView = pointlight->GetTransformMatrix() * cameraData.ViewMatrix;
	modelViewMat =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(modelView * cameraData.ViewProjection)
	};

	pointlight->constantBuffers.insert({ "TCB", UniformBuffer::Create(&modelViewMat, sizeof(ModelViewMat), 0u,
		Terra::UniformBuffer::ConstantBufferType::Vertex) });
	
	// bind to slot 1 so it doesn't override lightcbuf in other pixel shaders
	const auto colordata = pointlight->GetDiffuseColor();
	pointlight->constantBuffers.insert({ "PCB", UniformBuffer::Create(&colordata, sizeof(colordata), 1u,
		Terra::UniformBuffer::ConstantBufferType::Pixel) });
}

void Terra::Renderer3D::DrawMesh(const std::string& path, DirectX::XMMATRIX& transform, std::optional<DirectX::XMFLOAT4> color)
{
	TERRA_PROFILE_FUNCTION();
	auto modelView = transform * cameraData.ViewMatrix;
	modelViewMat = {
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(modelView * cameraData.ViewProjection)
	};

	Ref<Mesh> meshBase;
	if (s_FrameData.MeshMap.find(path) != s_FrameData.MeshMap.end())
	{
		meshBase = s_FrameData.MeshMap.at(path);
		meshBase->VertexCB->Update(&modelViewMat, sizeof(ModelViewMat));
	}	
	else
	{
		meshBase = CreateRef<Mesh>(Mesh::Create(path)); // sets its index buffer in ctor			
		meshBase->VertexCB = UniformBuffer::Create(&modelViewMat, sizeof(ModelViewMat), 0u, Terra::UniformBuffer::ConstantBufferType::Vertex);
		s_FrameData.MeshMap.insert({ path, meshBase });
	}

	for (auto& childMesh : meshBase->GetChildMeshes())
	{
		childMesh->VertexCB = meshBase->VertexCB;  // TODO: do built up transform here
		if (!childMesh->hasSpecular)
		{
			//material.color = color.has_value() ? color.value() : childMesh->color;
			if (!childMesh->PixelCB)
				childMesh->PixelCB = UniformBuffer::Create(&material, sizeof(Material), 1u, Terra::UniformBuffer::ConstantBufferType::Pixel);
			else
				childMesh->PixelCB->Update(&material, sizeof(Material));
		}
	}
	s_FrameData.MeshCount++;
}

void Terra::Renderer3D::DrawCube(DirectX::XMMATRIX& transform, DirectX::XMFLOAT4& color, const Ref<Texture2D> texture)
{

	auto& cube = s_FrameData.Cube;
	auto& cubeID = s_FrameData.CubeCount;
	//Cube->SetPosition(pos);
	const auto& modelView = transform * cameraData.ViewMatrix;
	modelViewMat =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(modelView * cameraData.ViewProjection)
	};

	cube->constantBuffers.insert({ "TCB" + std::to_string(cubeID), UniformBuffer::Create(&modelViewMat, sizeof(ModelViewMat), 0u,
		Terra::UniformBuffer::ConstantBufferType::Vertex) });

	//material.color = color;
	cube->constantBuffers.insert({ "PCB" + std::to_string(cubeID), UniformBuffer::Create(&cube->material, sizeof(cube->material), 1u,
		Terra::UniformBuffer::ConstantBufferType::Pixel) });

	if (texture)
		cube->textures.insert({ "diffuse" + std::to_string(cubeID), texture });

	s_FrameData.CubeCount++;
}

void Terra::Renderer3D::DrawPlane(DirectX::XMFLOAT3& position, DirectX::XMFLOAT4& color, const Ref<Texture2D> difftexture, const Ref<Texture2D> normaltex)
{
	DirectX::XMMATRIX transform = 
		DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(0.0f, 3.14159f, 0.0f) *
		DirectX::XMMatrixTranslation(position.x, position.y, position.z) *
		DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	
	DrawPlane(transform, color, difftexture, normaltex);
}

void Terra::Renderer3D::DrawPlane(DirectX::XMMATRIX& transform, DirectX::XMFLOAT4& color, const Ref<Texture2D> difftexture, const Ref<Texture2D> normaltex)
{
	auto& planeID = s_FrameData.PlaneCount;
	
	auto& plane = s_FrameData.Plane;
	const auto& modelView = transform * cameraData.ViewMatrix;
	modelViewMat =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(modelView * cameraData.ViewProjection)
	};

	plane->constantBuffers.insert({ "TCB" + std::to_string(planeID), UniformBuffer::Create(&modelViewMat, sizeof(ModelViewMat), 0u,
		Terra::UniformBuffer::ConstantBufferType::Vertex) });
	
	//material.color = color;
	if (!normaltex)
		plane->material.hasNormalMap = FALSE;
	else
		plane->material.hasNormalMap = TRUE;

	plane->constantBuffers.insert({ "PCB" + std::to_string(planeID), UniformBuffer::Create(&plane->material, sizeof(plane->material), 1u,
		Terra::UniformBuffer::ConstantBufferType::Pixel) });

	if (difftexture)
		plane->textures.insert({ "diffuse" + std::to_string(planeID), difftexture });
	if (normaltex)
		plane->textures.insert({ "normal" + std::to_string(planeID), normaltex });


	s_FrameData.PlaneCount++;
}

void Terra::Renderer3D::DrawSphere(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4& color)
{
	DirectX::XMMATRIX transform =
		DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(0.0f, 3.14159f, 0.0f) *
		DirectX::XMMatrixTranslation(std::move(position.x), std::move(position.y), std::move(position.z));

	DrawSphere(transform, color);
}

void Terra::Renderer3D::DrawSphere(DirectX::XMMATRIX& transform, DirectX::XMFLOAT4& color)
{
	auto& sphere = s_FrameData.Sphere;
	auto& sphereID = s_FrameData.SphereCount;

	const auto& modelView = transform * cameraData.ViewMatrix;
	modelViewMat =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(modelView * cameraData.ViewProjection)
	};

	sphere->constantBuffers.insert({ "TCB" + std::to_string(sphereID), UniformBuffer::Create(&modelViewMat, sizeof(ModelViewMat), 0u,
	Terra::UniformBuffer::ConstantBufferType::Vertex) });
	
	sphere->material.color = color;
	sphere->constantBuffers.insert({ "PCB" + std::to_string(sphereID), UniformBuffer::Create(&sphere->material, sizeof(sphere->material), 1u,
	Terra::UniformBuffer::ConstantBufferType::Pixel) });

	s_FrameData.SphereCount++;
}

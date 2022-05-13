#pragma once
#include "Camera.h"
#include "PerspectiveCamera.h"
#include "Texture.h"
#include <DirectXMath.h>

#include "Terra/Models/ModelBase.h"
namespace Terra
{
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const PerspectiveCamera& camera);
		static void EndScene();
		static void StartBatch();
		static void NextBatch();
		static void Flush();
		static void FlushMesh(const Ref<Mesh>& mesh);

		static void DrawPointLight(DirectX::XMFLOAT3& pos);
		static void DrawPointLight(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT4& diffusecolor);
		static void DrawMesh(const std::string& path, DirectX::XMMATRIX& transform, DirectX::XMFLOAT4& color, int32_t entityID, const Ref<Texture2D>& texture = nullptr);
		static void DrawMesh(const std::string& path, DirectX::XMMATRIX& transform, DirectX::XMFLOAT4& color, const Ref<Texture2D>& texture = nullptr);
		
		static void DrawCube(DirectX::XMMATRIX& transform, DirectX::XMFLOAT4& color, const Ref<Texture2D>& texture = nullptr);
		
		static void DrawPlane(DirectX::XMFLOAT3& position, DirectX::XMFLOAT4& color, const Ref<Texture2D>& texture = nullptr);
		static void DrawPlane(DirectX::XMMATRIX& transform, DirectX::XMFLOAT4& color, const Ref<Texture2D>& texture = nullptr);
		
		static void DrawSphere(DirectX::XMFLOAT3& position, DirectX::XMFLOAT4& color);
		static void DrawSphere(DirectX::XMMATRIX& transform, DirectX::XMFLOAT4& color);
	};
}
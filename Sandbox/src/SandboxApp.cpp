#include <Terra.h>	
#include "Terra/Core/EntryPoint.h"	

#include <imgui.h>
#include <DirectXMath.h>


/* Only have one entry point active at a time */

#include "SandboxApp.h"

#include "Terra\Models\ModelBase.h"
#include "Terra\Renderer\Renderer3D.h"

class ExampleLayer : public Terra::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_PerspectiveCamera(1.0f, 9.0f/16.0f, 0.5f, 50.0f)
	{	
		m_TilesTex = Terra::Texture2D::Create("assets/textures/tiles.png");
		m_Texture = Terra::Texture2D::Create("assets/textures/brickwall.jpg");
		m_NormalTex = Terra::Texture2D::Create("assets/textures/brickwall_normal.jpg", 1u);
		mLogoTexture = Terra::Texture2D::Create("assets/textures/sparta.png");
		mWhiteTexture = Terra::Texture2D::Create(1u, 1u);

	}


	void OnUpdate(const Terra::Timestep ts) override
	{

	//#define Terra_POLLING
	#ifdef Terra_POLLING	
		TERRA_TRACE("Delta time: {0}s ({1} ms)", ts.GetSeconds(), ts.GetMilliseconds());
		// Input Polling
		
		auto [x, y] = Terra::Input::GetMousePosition();
		TERRA_CORE_TRACE("{0}, {1}", x, y);

		TERRA_INFO("ExampleLayer::Update");	
		if (Terra::Input::IsKeyPressed(TERRA_KEY_TAB))
			TERRA_TRACE("Tab key is pressed (poll)!");
	#endif
	
		// Update
		m_PerspectiveCamera.OnUpdate(ts);
		Terra::Renderer3D::BeginScene(m_PerspectiveCamera);
	

		// Render
		Terra::RenderCommand::SetClearColor({ 0.07f, 0.0f, 0.12f, 1.0f });
		Terra::RenderCommand::Clear();
		DirectX::XMFLOAT4 whitecolor= { 1.0f, 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT3 lightpos = { 1.5f,14.0f,-4.5f };
		DirectX::XMFLOAT3 pos1 = { -5.0f, 17.0f, -1.0f };
		DirectX::XMFLOAT3 pos2 = { 5.0f, 4.0f, 0.0f };

		auto transform1 =
			DirectX::XMMatrixScaling(2 * m_scale.x, 2 * m_scale.y, m_scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z) *
			DirectX::XMMatrixTranslation(m_pos.x + 5, m_pos.y + 10, m_pos.z);


		auto transform2 =
			DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.z, m_rot.z) *
			DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

		auto transform3 =
			DirectX::XMMatrixScaling(2.0f * m_scale.x, 2.0f* m_scale.y, 2.0f* m_scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, 3.14159f, m_rot.z) *
			DirectX::XMMatrixTranslation(5.0f, 10.0f, 0.0f);
		
		auto meshcolor = materialConstants.color;
		DirectX::XMFLOAT4 c = { 0.0f, 0.0f, 0.0f, 1.0f };

		Terra::Renderer3D::DrawPointLight(pointLight);
		//Terra::Renderer3D::DrawMesh("assets\\models\\shinebox_logo\\shinebox.obj", transform1, materialConstants.color);
		Terra::Renderer3D::DrawMesh("assets\\models\\nano_textured\\nanosuit.obj", transform2, {});
		//Terra::Renderer3D::DrawMesh("assets\\models\\brick_wall\\brick_wall.obj", transform1, {});
		//Terra::Renderer3D::DrawCube(transform1, meshcolor, mLogoTexture);
		//Terra::Renderer3D::DrawPlane(transform3, c, m_Texture, m_NormalTex);
		//Terra::Renderer3D::DrawPlane(pos2, c, m_TilesTex);
		//Terra::Renderer3D::DrawSphere({ 4.0, -2.0f, 0.0f }, meshcolor);
		Terra::Renderer3D::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		bool open = false;
		//ImGui::ShowDemoWindow(&open);
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Color", &materialConstants.color.x);
		ImGui::DragFloat3("Model Position", &m_pos.x, 0.01f, -100.0f, 100.0f);
		ImGui::DragFloat3("Model Scale", &m_scale.x, 0.01f, 0.01f, 100.0f);
		ImGui::SliderAngle("X Rotation", &m_rot.x); 
		ImGui::SliderAngle("Y Rotation", &m_rot.y);
		ImGui::SliderAngle("Z Rotation", &m_rot.z);
		ImGui::End();

		pointLight.RenderControlWindow();
		m_PerspectiveCamera.RenderControlWindow();
	}

	void OnEvent(Terra::Event& e) override
	{
		m_PerspectiveCamera.OnEvent(e);
	}

private:
	Terra::PointLight pointLight;
	DirectX::XMFLOAT3 m_lightpos = { 1.5f,14.0f,-4.5f };

	Terra::Ref<Terra::Texture2D> m_Texture, m_NormalTex, m_AlphaTexture, mLogoTexture, mWhiteTexture, m_TilesTex;

	Terra::PerspectiveCamera m_PerspectiveCamera;

	// model
	DirectX::XMFLOAT3 m_pos = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 m_scale = { 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 m_rot = { 0.0f, 0.0f, 0.0f };
	float m_roll = 0.0f, m_pitch = 0.0f, m_yaw = 0.0f;


	struct PSMaterialConstant
	{
		DirectX::XMFLOAT4 color = { 0.8f, 0.1f, 0.1f, 1.0f };
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[2];
	} materialConstants;


	DirectX::XMMATRIX m_ModelTransform;
};


class Sandbox : public Terra::Application
{
public:
	Sandbox(Terra::ApplicationCommandLineArgs args)
		: Terra::Application(L"Sandbox App", args)
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
		Terra::Renderer3D::Shutdown();
	}
};


Terra::Application* Terra::CreateApplication(ApplicationCommandLineArgs args)
{
	
	return new Sandbox(args);
} 

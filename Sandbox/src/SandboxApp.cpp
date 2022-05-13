#include <Terra.h>	
#include <imgui/imgui.h>
#include <DirectXMath.h>


/* Only have one entry point active at a time */
#include "Terra/Core/EntryPoint.h"	

#include "SandboxApp.h"

#include "Terra\Models\ModelBase.h"
#include "Terra\Renderer\Renderer3D.h"

class ExampleLayer : public Terra::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_PerspectiveCamera(1.0f, 9.0f/16.0f, 0.5f, 50.0f)
	{	
#if 0
		m_Texture = Terra::Texture2D::Create("assets/textures/tiles.png");
		mLogoTexture = Terra::Texture2D::Create("assets/textures/sparta.png");
		mWhiteTexture = Terra::Texture2D::Create(1u, 1u);
#endif
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
		if (Terra::Input::IsKeyPressed(SB_KEY_TAB))
			TERRA_TRACE("Tab key is pressed (poll)!");
	#endif
	
		// Update
		m_PerspectiveCamera.OnUpdate(ts);
		Terra::Renderer3D::BeginScene(m_PerspectiveCamera);
	

		// Render
		Terra::RenderCommand::SetClearColor({ 0.07f, 0.0f, 0.12f, 1.0f });
		Terra::RenderCommand::Clear();
		DirectX::XMFLOAT4 whitecolor= { 1.0f, 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT3 lightpos = { 0.0f, 0.0f, 4.0f };
		DirectX::XMFLOAT3 pos1 = { 2.0f, 2.0f, 0.0f };
		DirectX::XMFLOAT3 pos2 = { 5.0f, 4.0f, 0.0f };

		auto transform1 =
			DirectX::XMMatrixScaling(2 * m_scale.x, 2 * m_scale.y, m_scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z) *
			DirectX::XMMatrixTranslation(m_pos.x + 2, m_pos.y + 2, m_pos.z);


		auto transform2 =
			DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z) *
			DirectX::XMMatrixTranslation(m_pos.x, m_pos.y - 10, m_pos.z + 5);

		
		auto meshcolor = materialConstants.color;


		Terra::Renderer3D::DrawPointLight(m_lightpos);
		int id = 1;
		//Terra::Renderer3D::DrawMesh("assets\\models\\suzanne.obj", transform1, meshcolor, id);
		Terra::Renderer3D::DrawMesh("assets\\models\\nano_textured\\nanosuit.obj", transform2, whitecolor);
		//Terra::Renderer3D::DrawMesh("assets\\models\\Terra_logo\\Terra.obj", transform1, meshcolor);
		//Terra::Renderer3D::DrawCube(transform4, meshcolor, m_Texture);
		//Terra::Renderer3D::DrawPlane(pos1, meshcolor, m_Texture);
		//Terra::Renderer3D::DrawSphere(pos2, meshcolor);
		Terra::Renderer3D::EndScene();
	}

	virtual void OnImGuiRender() override
	{

		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Color", &materialConstants.color.x);
		ImGui::DragFloat3("Model Position", &m_pos.x, 0.01f, -100.0f, 100.0f);
		ImGui::DragFloat3("Model Scale", &m_scale.x, 0.01f, 0.01f, 100.0f);
		ImGui::SliderAngle("X Rotation", &m_rot.x); 
		ImGui::SliderAngle("Y Rotation", &m_rot.y);
		ImGui::SliderAngle("Z Rotation", &m_rot.z);
		ImGui::DragFloat3("Light Position", &m_lightpos.x, 0.01, -100.0f, 100.0f);
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
	DirectX::XMFLOAT3 m_lightpos = { 0.0f, 0.0f, 4.0f };

	Terra::Ref<Terra::Texture2D> m_Texture, m_AlphaTexture, mLogoTexture, mWhiteTexture;

	Terra::PerspectiveCamera m_PerspectiveCamera;

	// model
	DirectX::XMFLOAT3 m_pos = { 0.0f, 0.0f, 4.0f };
	DirectX::XMFLOAT3 m_scale = { 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 m_rot = { 0.0f, 3.14159f, 0.0f };
	float m_roll = 0.0f, m_pitch = 0.0f, m_yaw = 0.0f;


	struct PSMaterialConstant
	{
		DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
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

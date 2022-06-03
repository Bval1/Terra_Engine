#include "Terrapch.h" 
#include "PerspectiveCamera.h"
#include "Terra/Core/Input.h"
#include "Terra/Core/KeyCodes.h"

#include <imgui.h>
#include <numbers>
namespace DX = DirectX;


// directxmath  load and store functions:
//https://docs.microsoft.com/en-us/windows/win32/dxmath/ovw-xnamath-reference-functions-load
//https://docs.microsoft.com/en-us/windows/win32/dxmath/ovw-xnamath-reference-functions-storage

Terra::PerspectiveCamera::PerspectiveCamera(float viewWidth, float viewHeight, float nearClip, float farClip)
	:m_ViewWidth(viewWidth), m_ViewHeight(viewHeight), m_Near(nearClip), m_Far(farClip), Camera(DirectX::XMMatrixPerspectiveLH(viewWidth, viewHeight, nearClip, farClip))
{
	UpdateView();
}

void Terra::PerspectiveCamera::OnUpdate(Timestep ts)
{
	const DX::XMFLOAT2 mousePos{ (float)Input::GetMouseX(), (float)Input::GetMouseY() };
	DX::XMVECTOR delta = DX::XMVectorSubtract(DX::XMLoadFloat2(&mousePos), DX::XMLoadFloat2(&m_InitMousePosition));
	m_InitMousePosition = mousePos;
	DX::XMFLOAT2 deltafloat;
	DX::XMStoreFloat2(&deltafloat, delta);
	
	float mouseSpeedFactor = 0.008f;
	deltafloat.x *= mouseSpeedFactor;
	deltafloat.y *= mouseSpeedFactor;


#ifdef BLENDER
	if (Input::IsKeyPressed(TERRA_KEY_LEFT_SHIFT))	// Blender controls
	{
		if (Input::IsMouseButtonPressed(TERRA_MOUSE_BUTTON_MIDDLE))
			MousePan(deltafloat);

		else if (Input::IsMouseButtonPressed(TERRA_MOUSE_BUTTON_RIGHT))
			MouseZoom(deltafloat.y);
	}
	else if (Input::IsMouseButtonPressed(TERRA_MOUSE_BUTTON_MIDDLE))
		MouseRotate(deltafloat);
#else
	if (Input::IsKeyPressed(TERRA_KEY_LEFT_ALT))	// 3DSMAX
	{
		if (Input::IsMouseButtonPressed(TERRA_MOUSE_BUTTON_MIDDLE))
			MouseRotate(deltafloat);
	}
	else if (Input::IsMouseButtonPressed(TERRA_MOUSE_BUTTON_MIDDLE))
		MousePan(deltafloat);
#endif
	
	UpdateView();
}

void Terra::PerspectiveCamera::RenderControlWindow()
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_position.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_position.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_position.z, -80.0f, 80.0f, "%.1f");
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Pitch", &m_pitch, -90.0f, 90.0f);
		ImGui::SliderAngle("Yaw", &m_yaw, -180.0f, 180.0f);
		if (ImGui::Button("Reset"))
			Reset();
		UpdateView();
	}


	ImGui::End();
}

void Terra::PerspectiveCamera::UpdateView()
{
	using namespace DirectX;
	const DirectX::XMVECTOR defaultLook = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR currentLook = DirectX::XMVector3Transform(defaultLook, 
		DirectX::XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, 0.0f));
	
	const DirectX::XMVECTOR cameraPos = DirectX::XMLoadFloat3(&m_position);
	const DirectX::XMVECTOR cameraTarget = cameraPos + currentLook;
	
	// set top to face toward +y
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(
		cameraPos, cameraTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);

}

void Terra::PerspectiveCamera::Reset()
{
	 m_position = { 0.0f, 0.0f, -5.0f };
	m_roll = 0.0f, m_pitch = 0.0f, m_yaw = 0.0f;
}

void Terra::PerspectiveCamera::UpdateProjection()
{
	m_CameraProjectionMatrix = DirectX::XMMatrixPerspectiveLH(m_ViewWidth, m_ViewHeight, m_Near, m_Far);
}


void Terra::PerspectiveCamera::MouseZoom(float delta)
{
	
	DirectX::XMFLOAT3 translation = { 0.0f, 0.0f, delta };
	DirectX::XMStoreFloat3(&translation, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&translation), DirectX::XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, 0.0f) *
		DirectX::XMMatrixScaling(2.0f , 2.0f, 2.0f))
	);
	m_position.x += translation.x;
	m_position.y += translation.y;
	m_position.z += translation.z;
}

void Terra::PerspectiveCamera::MousePan(DirectX::XMFLOAT2& delta)
{

	auto [xSpeed, ySpeed] = PanSpeed();
	DirectX::XMFLOAT3 translation = { -delta.x, delta.y, 0.0f };

	DirectX::XMStoreFloat3(&translation, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&translation), DirectX::XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, 0.0f) *
		DirectX::XMMatrixScaling(xSpeed, ySpeed, xSpeed))
	);

	m_position.x += translation.x;
	m_position.y += translation.y;
	m_position.z += translation.z;

}

void Terra::PerspectiveCamera::MouseRotate(DirectX::XMFLOAT2& delta)
{

	m_yaw += delta.x;
	m_yaw = m_yaw > 360.0f ? (m_yaw - 360.0f) : m_yaw;
	m_yaw = m_yaw < -360.0f ? (m_yaw + 360.0f) : m_yaw;
	m_pitch = std::clamp(m_pitch + delta.y, std::numbers::pi_v<float> / -2.0f, std::numbers::pi_v<float> / 2.0f);
	
}


void Terra::PerspectiveCamera::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseScrolledEvent>(TERRA_BIND_EVENT_FN(PerspectiveCamera::OnMouseScroll));
}

bool Terra::PerspectiveCamera::OnMouseScroll(MouseScrolledEvent& e)
{
	float delta = e.GetYOffset() * 0.01f;
	MouseZoom(delta);
	UpdateView();
	return false;
}


std::pair<float, float> Terra::PerspectiveCamera::PanSpeed() const
{
	float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
	float xFactor = 0.0366f * (x * x) + 1.1778f * x + 0.3021f;

	float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
	float yFactor = 0.0366f * (y * y) + 1.1778f * y + 0.3021f;

	return { xFactor, yFactor };
}

float Terra::PerspectiveCamera::RotationSpeed() const
{
	return 0.0f;
}

float Terra::PerspectiveCamera::ZoomSpeed() const
{
	float distance = m_position.z * 0.1f;
	distance = std::max(distance, 0.0f);
	float speed = distance * distance;
	speed = std::min(speed, 100.0f); // max speed = 100
	return speed;
}


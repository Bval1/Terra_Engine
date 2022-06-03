#pragma once
#include "Camera.h"
#include "Terra/Core/Timestep.h"
#include "Terra/Events/Event.h"
#include "Terra/Events/MouseEvent.h"

#include <DirectXMath.h>
namespace Terra
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera() = default;
		PerspectiveCamera(float viewWidth, float viewHeight, float nearClip, float farClip);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline void SetDistance(float distance) { m_position.z = distance; }
		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }	
		
		const DirectX::XMMATRIX& GetViewMartrix() const { return m_ViewMatrix; }
		const DirectX::XMMATRIX& GetViewProjection() const { return m_CameraProjectionMatrix * m_ViewMatrix; }
		const DirectX::XMFLOAT3& GetPosition() const { return m_position; }
		inline float GetPitch() const { return m_pitch; }
		inline float GetYaw() const { return m_yaw; }

		void RenderControlWindow();
		void UpdateView();
	private:
		void Reset();
		void UpdateProjection();
		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(DirectX::XMFLOAT2& delta);
		void MouseRotate(DirectX::XMFLOAT2& delta);
		void MouseZoom(float delta);
		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	
	private:
		float m_ViewWidth = 1.0f, m_ViewHeight = 3.0f/4.0f;
		float m_Near = 0.5f, m_Far = 50.0f;
		
		// camera transform
		DirectX::XMFLOAT3 m_position = { 0.0f, 7.5f, -18.0f };
		//DirectX::XMFLOAT3 m_position = { 0.0f, 0.0f, -5.0f };
		float m_roll = 0.0f, m_pitch = 0.0f, m_yaw = 0.0f;

		float m_ViewportWidth = 1280.0f, m_ViewportHeight = 720.0f;
		DirectX::XMMATRIX  m_ViewMatrix;
		DirectX::XMFLOAT2 m_InitMousePosition = { 0.0f, 0.0f };

		DirectX::XMFLOAT3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
	};
}
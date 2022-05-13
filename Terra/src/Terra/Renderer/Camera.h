#pragma once

#include <DirectXMath.h>
namespace Terra {

	class Camera
	{
	public:
		Camera() = default;

		Camera(DirectX::XMMATRIX projection)
			: m_CameraProjectionMatrix(std::move(projection)) {}
		
		virtual ~Camera() = default;
		const DirectX::XMMATRIX& GetCameraProjection() const { return m_CameraProjectionMatrix; }
	protected:
		DirectX::XMMATRIX m_CameraProjectionMatrix = DirectX::XMMatrixIdentity(); 
	};

}
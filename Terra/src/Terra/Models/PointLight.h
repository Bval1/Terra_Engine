#pragma once
#include "Sphere.h"
#include "Platform\DirectX\DirectXConstantBuffer.h"  
namespace Terra {

	class PointLight : public Sphere
	{
	public:
		struct PointLightCBuf;

		PointLight();
		~PointLight();
			
		Ref<UniformBuffer>& GetConstantBuffer() { return m_pointLightCBuf; }
		// updates cb data to take into account view matrix and then binds cb	
		void Bind(const DirectX::XMMATRIX& viewMatrix) const;	
		DirectX::XMFLOAT4 GetDiffuseColor() const;
		DirectX::XMMATRIX GetTransformMatrix() const;

		void RenderControlWindow();
		void SetPosition(DirectX::XMFLOAT3& position) { m_pos = position; }
		void SetDiffuseColor(DirectX::XMFLOAT4& diffusecolor) 
		{
			m_diffuseColor = { diffusecolor.x, diffusecolor.y, diffusecolor.z };
		}

		void Update(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& scale, DirectX::XMFLOAT3& diffuseColor, float diffuseIntensity);

	private:
		void ResetData();

		
	public:
		DirectX::XMFLOAT3 m_ambient = { 0.05f, 0.05f, 0.05f };
		DirectX::XMFLOAT3 m_diffuseColor = { 1.0f, 1.0f, 1.0f };
		float m_diffuseIntensity = 1.0f, m_attConstant = 1.0f, m_attLinear = 0.045f, m_attQuadratic = 0.0075f;
		
		DirectX::XMFLOAT3 m_pos = { 0.0f, 0.0f, 4.0f };
		DirectX::XMFLOAT3 m_scale = { 0.5f, 0.5f, 0.5f };
		DirectX::XMFLOAT3 m_rot = { 0.0f, 3.14159f, 0.0f };
		float m_roll = 0.0f, m_pitch = 0.0f, m_yaw = 0.0f;
	
	private:
		struct PointLightCBuf
		{
			alignas(16) DirectX::XMFLOAT3 pos;
			alignas(16) DirectX::XMFLOAT3 ambient;
			alignas(16) DirectX::XMFLOAT3 diffuseColor;
			float diffuseIntensity;
			float attConstant;
			float attLinear;
			float attQuadratic;
		};
		mutable PointLightCBuf m_cbData;
		
		Ref<UniformBuffer> m_pointLightCBuf;
		Ref<UniformBuffer> m_materialCBuf;

	};
}
#include "Terrapch.h"
#include "PointLight.h"
#include <imgui.h>

Terra::PointLight::PointLight()
	: Sphere(CreateTesselated(12, 24))	
{
	
	ResetData();

	m_pointLightCBuf = Terra::UniformBuffer::Create(&m_cbData, sizeof(PointLightCBuf), 0u,
		Terra::UniformBuffer::ConstantBufferType::Pixel);

}

Terra::PointLight::~PointLight()
{
}

void Terra::PointLight::Bind(const DirectX::XMMATRIX& viewMatrix) const
{
	m_cbData =
	{
		m_pos,
		m_ambient,
		m_diffuseColor,
		m_diffuseIntensity, 
		m_attConstant, 
		m_attLinear,
		m_attQuadratic
	};
	auto temp = m_cbData;
	DirectX::XMStoreFloat3(&temp.pos, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&m_cbData.pos), viewMatrix));
	
	// update with light position view matrix
	m_pointLightCBuf->Update(&temp, sizeof(temp));
	m_pointLightCBuf->Bind();
	
}

void Terra::PointLight::RenderControlWindow()
{
	if (ImGui::Begin("Light Settings"))
	{
		ImGui::DragFloat3("Position", &m_pos.x, 0.01, -100.0f, 100.0f);
		ImGui::DragFloat3("Scale", &m_scale.x, 0.01f, 0.01f, 100.0f);
		ImGui::ColorEdit3("Diffuse Color", &m_diffuseColor.x); // lines up as a float[3], only needs ptr to the head
		ImGui::DragFloat("Diffuse Intensity", &m_diffuseIntensity, 0.001f, 0.01, 2.0f);
		if (ImGui::Button("Reset"))
		{
			ResetData();
		}
	}
	ImGui::End();
}

DirectX::XMFLOAT4 Terra::PointLight::GetDiffuseColor() const
{
	return std::move(
	DirectX::XMFLOAT4(m_diffuseColor.x, m_diffuseColor.y, m_diffuseColor.z, 1.0f));
}

DirectX::XMMATRIX Terra::PointLight::GetTransformMatrix() const
{
	return std::move(
		DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z) *
		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z) 
		);
}

void Terra::PointLight::Update(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& scale, DirectX::XMFLOAT3& diffuseColor, float diffuseIntensity)
{
	m_pos = pos;
	m_scale = scale;
	m_diffuseColor = diffuseColor;
	m_diffuseIntensity = diffuseIntensity;
}

void Terra::PointLight::ResetData()
{
	m_pos = { 0.0f, 15.5f, -3.0f };
	m_scale = { 0.5f, 0.5f, 0.5f };
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_ambient = { 0.1f, 0.1f, 0.1f };
	m_diffuseColor = { 1.0f, 1.0f, 1.0f };
	m_diffuseIntensity = 1.0f, m_attConstant = 1.0f, m_attLinear = 0.045f, m_attQuadratic = 0.0075f;
}


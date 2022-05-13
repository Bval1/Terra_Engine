#pragma once
#include "Terra.h"
#include "SBParticleSystem.h"

class SandboxApp : public Terra::Layer
{
public:
	SandboxApp();
	virtual ~SandboxApp() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(const Terra::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Terra::Event& e) override;
private:
	Terra::Ref<Terra::UniformBuffer> m_TransformCB;
};
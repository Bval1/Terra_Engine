#pragma once
#include "DirectXContext.h"
namespace Terra
{
	class Sampler
	{
	public:
		Sampler();
		void Bind();
	private:
		DXGIInfoManager& GetInfoManager(DirectXContext& gfx) { return gfx.infoManager; }
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}



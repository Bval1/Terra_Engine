#pragma once
#include "DirectXContext.h"
namespace Terra
{
	class Sampler
	{
	public:
		Sampler();
		void Bind(uint32_t slot = 0u);
	private:
#ifndef NDEBUG
		DXGIInfoManager& GetInfoManager(DirectXContext& gfx) { return gfx.infoManager; }
#endif
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}



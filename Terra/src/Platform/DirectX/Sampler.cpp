#include "Terrapch.h"
#include "Sampler.h"
#include "DirectXRendererAPI.h"

Terra::Sampler::Sampler()
{
	INFOMAN(*DirectXRendererAPI::GetDXGraphicsContext());
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // nearest neighbor or bilinear
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_THROW_INFO(DirectXRendererAPI::GetDevice()->CreateSamplerState(&samplerDesc, &pSampler));
}

void Terra::Sampler::Bind(uint32_t slot)
{
	DirectXRendererAPI::GetDeviceContext()->PSSetSamplers(slot, 1, pSampler.GetAddressOf());
}

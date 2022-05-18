#include "Terrapch.h"
#include "DirectXTexture2D.h"
#include "DirectXRendererAPI.h"
#include "stb_image.h"

Terra::DirectXTexture2D::DirectXTexture2D(uint32_t width, uint32_t height)
	:m_width(width), m_height(height)
{	
	INFOMAN(*DirectXRendererAPI::GetDXGraphicsContext());
	m_sampler = CreateRef<Sampler>();
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
	textureDesc.MipLevels = 1; // 1 = no mip mapping
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	// same as backbuffer
	textureDesc.SampleDesc.Count = 1;	 // 1 = no aa
	textureDesc.SampleDesc.Quality = 0;	// 0 = no aa
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	uint32_t tex = 0xFFFFFFFF;		// creates solid white texture
	sd.pSysMem = &tex;
	sd.SysMemPitch = textureDesc.Width * 4;  // distance in byte between 1st pixel in row 0 and 1st pixel  in next row

	GFX_THROW_INFO(DirectXRendererAPI::GetDevice()->CreateTexture2D(
		&textureDesc, &sd, &pTexture
	));

	// create the resource view on the texture (you bind views to the pipeline, not textures)
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0; // 0 = no mip mapping
	srvDesc.Texture2D.MipLevels = 1; // 1 = no mip mapping
	GFX_THROW_INFO(DirectXRendererAPI::GetDevice()->CreateShaderResourceView(
		pTexture.Get(), &srvDesc, &pTextureView
	));
}

Terra::DirectXTexture2D::DirectXTexture2D(const std::string& path, uint32_t slot)
	:m_path(path), m_slot(slot)
{

	INFOMAN(*DirectXRendererAPI::GetDXGraphicsContext());
	
	wchar_t wideName[512];
	mbstowcs_s(nullptr, wideName, path.c_str(), _TRUNCATE);
	GetImageData(path);
	
	m_sampler = CreateRef<Sampler>();


	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
	textureDesc.MipLevels = 1; // 1 = no mip mapping
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	// same as backbuffer
	textureDesc.SampleDesc.Count = 1;	 // 1 = no aa
	textureDesc.SampleDesc.Quality = 0;	// 0 = no aa
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = m_imageData;
	sd.SysMemPitch = textureDesc.Width * 4;  // distance in byte between 1st pixel in row 0 and 1st pixel  in next row
	
	GFX_THROW_INFO(DirectXRendererAPI::GetDevice()->CreateTexture2D(
		&textureDesc, &sd, &pTexture
	));

	// create the resource view on the texture (you bind views to the pipeline, not textures)
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0; // 0 = no mip mapping
	srvDesc.Texture2D.MipLevels = 1; // 1 = no mip mapping
	GFX_THROW_INFO(DirectXRendererAPI::GetDevice()->CreateShaderResourceView(
		pTexture.Get(), &srvDesc, &pTextureView
	));

	stbi_image_free(m_imageData);
}

Terra::DirectXTexture2D::~DirectXTexture2D()
{
}

void Terra::DirectXTexture2D::Bind() const
{
	m_sampler->Bind();
	DirectXRendererAPI::GetDeviceContext()->PSSetShaderResources(m_slot, 1u, pTextureView.GetAddressOf());
}

void Terra::DirectXTexture2D::GetImageData(const std::string& path)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	m_imageData = stbi_load(path.c_str(), &width, &height, &channels, 4);
	TERRA_CORE_ASSERT(m_imageData, "Failed to load image!");
	m_width = width;
	m_height = height;
	m_channels = channels;
}

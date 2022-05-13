#pragma once
#include "Shinebox/Renderer/Texture.h"
#include "DirectXContext.h"
#include "Sampler.h"
namespace Terra
{
	class DirectXTexture2D : public Texture2D
	{
	public:
		DirectXTexture2D(uint32_t width, uint32_t height);
		DirectXTexture2D(const std::string& path);
		virtual ~DirectXTexture2D();

		virtual uint32_t GetWidth() const override { return m_width; }
		virtual uint32_t GetHeight() const override { return m_height; };
		virtual uint32_t GetRendererID() const override { return 0u; }
		
		// Gets ptr to a block of memory and allow you to upload it to the gpu
		virtual void SetData(void* data, uint32_t size) override { m_imageData = static_cast<unsigned char*>(data); }
		virtual bool operator==(const Texture& other) const override { return m_path == ((DirectXTexture2D&)other).m_path; }
		virtual void Bind(uint32_t slot = 0) const override;

	private:
		void GetImageData(const std::string& path);
		static DXGIInfoManager& GetInfoManager(DirectXContext& context)
		{
#ifndef  NDEBUG
			return context.infoManager;
#else
			SB_ASSERT("You tried to access infoManager in Release config");
#endif
		}

	private:
		unsigned char* m_imageData = nullptr;
		std::string m_path;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_channels;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture = NULL;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView = NULL;
		Ref<Sampler> m_sampler;
	};
}



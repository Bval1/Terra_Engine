#pragma once

namespace Terra {

	class UniformBuffer
	{
	public:
		enum class ConstantBufferType
		{
			None = 0, Vertex = 1, Pixel = 2
		};
	public:
		virtual ~UniformBuffer() {}
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
		virtual void Bind(uint32_t numBuffers = 1u) = 0;
		virtual void Update(const void* data, uint32_t size) = 0;
		virtual ConstantBufferType GetType() = 0;
		static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding = 0u);
		static Ref<UniformBuffer> Create(const void* data, uint32_t size, uint32_t slot,
			ConstantBufferType cbType);
	};
}
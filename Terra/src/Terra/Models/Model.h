#pragma once

#include "Terra\Renderer\Buffer.h"
#include "Terra\Renderer\UniformBuffer.h"
#include "Terra\Renderer\Texture.h"
#include <DirectXMath.h>
#include <vector>
namespace Terra {

	class Model
	{
	public:
		virtual ~Model() = default;
		
		// returns vertex data
		virtual float* VertexData() = 0;
		
		// returns vertex data size in bytes
		virtual uint32_t VertexDataSize() const = 0;
		
		// clears vertex data
		virtual void ClearVertexData() = 0;
		
		virtual Ref<IndexBuffer>& GetIndexBuffer() = 0;
	};
}
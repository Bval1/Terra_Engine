#include "Terrapch.h"
#include "DirectXVertexArray.h"
#include "DirectXShader.h"

Terra::DirectXVertexArray::DirectXVertexArray()
{
}

Terra::DirectXVertexArray::~DirectXVertexArray()
{
}

void Terra::DirectXVertexArray::Bind() const
{
	//for (const auto& vb : m_VertexBuffers)
	//	vb->Bind();
	//
	m_VertexBuffer->Bind();
	m_IndexBuffer->Bind();

	// Set all device context IA (input assembler) elements 
	DirectXRendererAPI::GetDeviceContext()->IASetInputLayout(pInputLayout.Get());
}

void Terra::DirectXVertexArray::Unbind() const
{
}

void Terra::DirectXVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
	INFOMAN(*DirectXRendererAPI::GetDXGraphicsContext());
	SB_PROFILE_FUNCTION();
	//vertexBuffer->Bind();  // do this before when binding VA, right before the draw call 
	//m_VertexBuffers.push_back(vertexBuffer);
	m_VertexBuffer = vertexBuffer;
	const auto& layout = vertexBuffer->GetLayout();
	
	std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {};
	ied.reserve(4);
	for (const auto& element : layout)
	{
		// Set layout, needs to have a vertex shader first
		DXGI_FORMAT format = ShaderDataTypeToDXGIFormat(element.Type);
		D3D11_INPUT_CLASSIFICATION inputClass = element.InputSlotClass == 0 ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA;
		ied.emplace_back
		(
			element.Name.c_str(), element.SemanticIndex, format, element.InputSlot, element.Offset, inputClass, element.StepRate
		);	
	}

	DirectXShader* VS = static_cast<DirectXShader*>(layout.GetVertexShader().get());
	auto pVSBytecode = VS->GetVSByteCode().Get();

	GFX_THROW_INFO(DirectXRendererAPI::GetDevice()->CreateInputLayout(
		ied.data(), (UINT)ied.size(), pVSBytecode->GetBufferPointer(), pVSBytecode->GetBufferSize(),
		&pInputLayout));

	 //Bind Layout - this is global, set before per instance draw call 
	//DirectXRendererAPI::GetDeviceContext()->IASetInputLayout(pInputLayout.Get());
	
}

void Terra::DirectXVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
{
	m_IndexBuffer = indexBuffer;
}

DXGI_FORMAT Terra::DirectXVertexArray::ShaderDataTypeToDXGIFormat(ShaderDataType type)
{
	switch (type)
	{
	case Terra::ShaderDataType::None:	return DXGI_FORMAT_UNKNOWN;
	case Terra::ShaderDataType::Float:	return DXGI_FORMAT_R32_FLOAT;
	case Terra::ShaderDataType::Float2:	return DXGI_FORMAT_R32G32_FLOAT;
	case Terra::ShaderDataType::Float3:	return DXGI_FORMAT_R32G32B32_FLOAT;
	case Terra::ShaderDataType::Float4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case Terra::ShaderDataType::Mat3:	return DXGI_FORMAT_R32G32B32_FLOAT;
	case Terra::ShaderDataType::Mat4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case Terra::ShaderDataType::Int:		return DXGI_FORMAT_R32_UINT;
	case Terra::ShaderDataType::Int2:	return DXGI_FORMAT_R32G32_UINT;
	case Terra::ShaderDataType::Int3:	return DXGI_FORMAT_R32G32B32_UINT;
	case Terra::ShaderDataType::Int4:	return DXGI_FORMAT_R32G32B32A32_UINT;
	case Terra::ShaderDataType::Bool:	return DXGI_FORMAT_R8_UINT;
	}

	SB_CORE_ASSERT(false, "Unknown ShaderDataType!");
	return DXGI_FORMAT_UNKNOWN;
}


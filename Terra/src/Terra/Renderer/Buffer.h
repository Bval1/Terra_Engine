#pragma once
#include "Shader.h"
/* Creates v-table of virutal functions that get dispatched to subclasses, doesn't store any data */

namespace Terra {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float :	return 4;
			case ShaderDataType::Float2:	return 8;
			case ShaderDataType::Float3:	return 12;
			case ShaderDataType::Float4:	return 16;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 8;
			case ShaderDataType::Int3:		return 12;
			case ShaderDataType::Int4:		return 16;
			case ShaderDataType::Bool:		return 1;
		}

		SB_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		uint32_t Offset;
		uint32_t Size;
		ShaderDataType Type;
		bool Normalized;
		uint32_t SemanticIndex;
		uint32_t InputSlot;
		uint32_t InputSlotClass; // 0 for Per vertex data, 1 for per instance data
		uint32_t StepRate;

		BufferElement() {}

		// OpenGL
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)			
			:  Type(type), Name(name), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(false)
		{	
		}

		// DirectX
		BufferElement(const std::string& name, uint32_t semanticIndex, ShaderDataType type, uint32_t slot, uint32_t offset, uint32_t inputSlotClass, uint32_t stepRate)
			: Name(name), SemanticIndex(semanticIndex), Type(type), InputSlot(slot),  Size(ShaderDataTypeSize(type)), 
			Offset(offset), InputSlotClass(inputSlotClass), StepRate(stepRate)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Bool:		return 1;
			}

			SB_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};


	class BufferLayout
	{
	public:
		BufferLayout () {}

		// Can't take in a vector<BufferElement> because it
		// requires 2 implicit castings when initilizing a layout with an initilizer list {} 

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)	
		{
			CalculateOffsetsAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }
		void SetVertexShader(Ref<Shader>& vertexShader) { m_vertexShader = vertexShader; }
		const Ref<Shader>& GetVertexShader() const { return m_vertexShader; }
		// So we can iterate through elements in layout without having to access m_Elements directly
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	
	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
		Ref<Shader> m_vertexShader;
	};


	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual  const BufferLayout& GetLayout() const = 0;
		virtual float* GetVertices() const = 0;

		virtual void SetNumberofBuffers(uint32_t count) = 0;

		// for batch rendering
		static Ref<VertexBuffer> Create(uint32_t size);

		// acts like a ctor, but its static so it doesn't actually belong to this class
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

		
	};

	// Currently only supports 32-bit index buffers
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;		// tells us how many indices in index buffer


		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}
//#include "sbpch.h"
//#include "Framebuffer.h"
//#include "Terra/Renderer/Renderer.h"
//#include "Platform/OpenGL/OpenGLFrameBuffer.h"
//namespace Terra
//{
//	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
//	{
//		switch (Renderer::GetAPI())
//		{
//		case RendererAPI::API::None: SB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
//		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
//		}
//
//		SB_CORE_ASSERT(false, "Unknown RendererAPI!");
//		return nullptr;
//	}
//}
#pragma once

#include "Hazel/Render/Framebuffer.h"

namespace Hazel {

	class OpenGLFramebuffer : public FrameBuffer
	{
	public:
		OpenGLFramebuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Resize(uint32_t width, uint32_t height)override;
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FrameBufferSpecification m_Specification;
	};
}
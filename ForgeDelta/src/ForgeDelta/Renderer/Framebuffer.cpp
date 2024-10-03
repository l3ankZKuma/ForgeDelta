#include"fdpch.h"
#include "Framebuffer.h"




namespace ForgeDelta {

  static const uint32_t s_MaxFramebufferSize = 8192;

  namespace Utils {

    static GLenum TextureTarget(bool multisampled)
    {
      return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
    {
      glCreateTextures(TextureTarget(multisampled), count, outID);
    }

    static void BindTexture(bool multisampled, uint32_t id)
    {
      glBindTexture(TextureTarget(multisampled), id);
    }

    static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
    {
      bool multisampled = samples > 1;
      if (multisampled)
      {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
      }
      else
      {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
    }

    static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
    {
      bool multisampled = samples > 1;
      if (multisampled)
      {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
      }
      else
      {
        glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }

      glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
    }

    static bool IsDepthFormat(FramebufferTextureFormat format)
    {
      return format == FramebufferTextureFormat::DEPTH24STENCIL8;
    }

    static GLenum FBTextureFormatToGL(FramebufferTextureFormat format)
    {
      switch (format)
      {
      case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
      case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
      default: return 0;
      }
    }
  }

  Framebuffer::Framebuffer()
  {

  }

  Framebuffer::~Framebuffer()
  {
    glDeleteFramebuffers(1, &m_RendererID);
    glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
    glDeleteTextures(1, &m_DepthAttachment);
  }

   void
   Framebuffer::create(const FramebufferSpecification& spec)
  {
    m_Specification = spec;
    Invalidate();
  }

  void 
  Framebuffer::Invalidate()
  {

    glCreateFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    // Color Attachment
    glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

    // Depth Attachment
    glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
    glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
  }

  void Framebuffer::Bind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, m_Specification.Width, m_Specification.Height);
  }

  void Framebuffer::Unbind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void Framebuffer::Resize(uint32_t width, uint32_t height)
  {
    if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
    {
      return;
    }
    m_Specification.Width = width;
    m_Specification.Height = height;

    Invalidate();
  }

  int Framebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
  {
    assert(attachmentIndex < m_ColorAttachments.size());

    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    int pixelData;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
    return pixelData;
  }

  void Framebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
  {
    assert(attachmentIndex < m_ColorAttachments.size());

    auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
    glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
      Utils::FBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
  }

  uint32_t Framebuffer::GetColorAttachmentRendererID(uint32_t index) const
  {
    return m_ColorAttachment;
  }

  const FramebufferSpecification& Framebuffer::GetSpecification() const
  {
    return m_Specification;
  }

}

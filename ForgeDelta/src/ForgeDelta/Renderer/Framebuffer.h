#pragma once


namespace ForgeDelta {

  enum class FramebufferTextureFormat
  {
    None = 0,

    // Color
    RGBA8,
    RED_INTEGER,

    // Depth/stencil
    DEPTH24STENCIL8,

    // Defaults
    Depth = DEPTH24STENCIL8
  };

  struct FramebufferTextureSpecification
  {
    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FramebufferTextureFormat format)
      : TextureFormat(format) {}

    FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
    // TODO: filtering/wrap
  };

  struct FramebufferAttachmentSpecification
  {
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
      : Attachments(attachments) {}

    std::vector<FramebufferTextureSpecification> Attachments;
  };

  struct FramebufferSpecification
  {
    uint32_t Width = 0, Height = 0;
    FramebufferAttachmentSpecification Attachments;
    uint32_t Samples = 1;

    bool SwapChainTarget = false;
  };

  class Framebuffer
  {
  public:
    Framebuffer();
    virtual ~Framebuffer();

    void create(const FramebufferSpecification& spec);

    void Invalidate();

    void Bind();
    void Unbind();

    void Resize(uint32_t width, uint32_t height);
    int ReadPixel(uint32_t attachmentIndex, int x, int y);

    void ClearAttachment(uint32_t attachmentIndex, int value);

    uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const;

    const FramebufferSpecification& GetSpecification() const;

  private:
    uint32_t m_RendererID{ 0 };
    uint32_t m_ColorAttachment{ 0 }, m_DepthAttachment{ 0 };

    FramebufferSpecification m_Specification;

    std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
    FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

    std::vector<uint32_t> m_ColorAttachments;
  };

}

#pragma once

namespace gd {

	enum class TextureFormat
	{
		None = 0,

		RGBA,
		Depth24Stencil8
	};

	struct FramebufferAttachment
	{
		TextureFormat Format;
	};

	struct FramebufferConfig
	{
		std::vector<FramebufferAttachment> Attachments;

		uint32 Width = 0;
		uint32 Height = 0;

		glm::vec4 ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		bool SwapchainTarget = false;
	};

	class Framebuffer : public ReferenceCounted
	{
	public:
		Framebuffer(const FramebufferConfig& config);
		virtual ~Framebuffer();

		void Resize(uint32 width, uint32 height, bool forceRecreate = false);

		void Bind() const;
		void Unbind() const;

		void BindAttachment(uint32 index = 0) const;
		uint32 GetAttachmentID(uint32 index = 0) const { return m_TextureIDs[index]; }
	private:
		void Invalidate();
	private:
		FramebufferConfig m_Config;
		uint32 m_FramebufferID = 0;

		uint32 m_Width = 0;
		uint32 m_Height = 0;

		std::vector<uint32> m_TextureIDs;
	};

}
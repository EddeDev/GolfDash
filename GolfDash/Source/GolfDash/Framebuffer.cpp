#include "GolfDashPCH.h"
#include "Framebuffer.h"

#include "GolfDash.h"

#include <glad/glad.h>

namespace gd {

	Framebuffer::Framebuffer(const FramebufferConfig& config)
		: m_Config(config), m_Width(config.Width), m_Height(config.Height)
	{
		if (m_Width == 0 || m_Height == 0 || m_Config.SwapchainTarget)
		{
			m_Width = GolfDash::Get().GetWindow()->GetFramebufferWidth();
			m_Height = GolfDash::Get().GetWindow()->GetFramebufferHeight();
		}

		Resize(m_Width, m_Height, true);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteTextures(static_cast<int32>(m_TextureIDs.size()), m_TextureIDs.data());
		glDeleteFramebuffers(1, &m_FramebufferID);
	}

	void Framebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
		if (!forceRecreate && (m_Width == width && m_Height == height))
			return;

		m_Width = width;
		m_Height = height;

		if (!m_Config.SwapchainTarget)
			Invalidate();
	}

	void Framebuffer::Invalidate()
	{
		if (m_FramebufferID)
			glDeleteFramebuffers(1, &m_FramebufferID);

		glCreateFramebuffers(1, &m_FramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

		if (!m_TextureIDs.empty())
			glDeleteTextures(static_cast<int32>(m_TextureIDs.size()), m_TextureIDs.data());

		m_TextureIDs.clear();
		m_TextureIDs.resize(m_Config.Attachments.size());
		glCreateTextures(GL_TEXTURE_2D, static_cast<int32>(m_TextureIDs.size()), m_TextureIDs.data());

		std::vector<uint32> drawBuffers;
		uint32 attachmentIndex = 0;
		for (auto& attachment : m_Config.Attachments)
		{
			uint32 textureID = m_TextureIDs[attachmentIndex];

			// Sampler
			glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (attachment.Format == TextureFormat::RGBA)
			{
				glTextureStorage2D(textureID, 1, GL_RGBA8, m_Width, m_Height);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIndex, GL_TEXTURE_2D, textureID, 0);

				drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + attachmentIndex);
			}
			else if (attachment.Format == TextureFormat::Depth24Stencil8)
			{
				glTextureStorage2D(textureID, 1, GL_DEPTH_COMPONENT24, m_Width, m_Height);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
			}

			attachmentIndex++;
		}

		glDrawBuffers(static_cast<int32>(drawBuffers.size()), drawBuffers.data());

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "Framebuffer is not complete!" << std::endl;
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(m_Config.ClearColor.x, m_Config.ClearColor.y, m_Config.ClearColor.z, m_Config.ClearColor.w);

		glViewport(0, 0, m_Width, m_Height);
	}

	void Framebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::BindAttachment(uint32 index) const
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureIDs[index]);
	}

}
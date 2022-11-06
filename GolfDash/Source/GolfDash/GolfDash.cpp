#include "GolfDashPCH.h"
#include "GolfDash.h"

#include "glad/glad.h"

namespace gd {

	GolfDash::GolfDash()
	{
		WindowConfig windowConfig;
		windowConfig.Width = 1920;
		windowConfig.Height = 1080;
		windowConfig.Title = "Golf Dash";

		m_Window = Ref<Window>::Create(windowConfig);
		m_Window->CreateContext();
		m_Window->AddCloseCallback([this]()
		{
			m_Running = false;
		});
		m_Window->AddFramebufferSizeCallback([this](uint32 width, uint32 height) { SetViewportSize(width, height); });

		SetViewportSize(m_Window->GetFramebufferWidth(), m_Window->GetFramebufferHeight());
	}

	void GolfDash::Run()
	{
		OnInit();

		m_Window->ShowWindow();
		while (m_Running)
		{
			m_Window->PollEvents();
			OnUpdate();
			m_Window->SwapBuffers();
		}
	}

	void GolfDash::OnInit()
	{
		m_Shader = Ref<Shader>::Create("Assets/Shaders/VertexShader.glsl", "Assets/Shaders/FragmentShader.glsl");

		PipelineConfig pipelineConfig;
		pipelineConfig.Shader = m_Shader;
		pipelineConfig.InputLayout = {
			{ "a_Position", ShaderDataType::Vec3 },
			{ "a_TexCoord", ShaderDataType::Vec2 }
		};
		m_Pipeline = Ref<Pipeline>::Create(pipelineConfig);

		struct Vertex
		{
			glm::vec3 Position;
			glm::vec2 TexCoord;
		};

		struct Index
		{
			uint32 V1;
			uint32 V2;
			uint32 V3;
		};

		Vertex vertices[4];
		vertices[0] = { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f } };
		vertices[1] = { {  0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f } };
		vertices[2] = { {  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f } };
		vertices[3] = { { -0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f } };

		Index indices[2];
		indices[0] = { 0, 1, 2 };
		indices[1] = { 2, 3, 0 };

		m_VertexBuffer = Ref<VertexBuffer>::Create(vertices, static_cast<uint32>(4 * sizeof(Vertex)));
		m_IndexBuffer = Ref<IndexBuffer>::Create(indices, static_cast<uint32>(2 * sizeof(Index)));

		m_TestTexture = Ref<Texture>::Create("Assets/Textures/TestTex.png");
		m_GrassTexture = Ref<Texture>::Create("Assets/Textures/Grass.png");
		m_BallTexture = Ref<Texture>::Create("Assets/Textures/Ball.png");
		m_HoleTexture = Ref<Texture>::Create("Assets/Textures/Hole.png");
	}

	void GolfDash::OnUpdate()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		m_VertexBuffer->Bind();
		m_Pipeline->Bind();
		m_IndexBuffer->Bind();

		m_Shader->Bind();
		
		glm::mat4 viewMatrix = glm::inverse(glm::mat4(1.0f));

		// BG
		{
			glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)m_ViewportWidth, 0.0f, (float)m_ViewportHeight);
			m_Shader->SetUniform("u_Uniforms.ViewProjectionMatrix", projectionMatrix * viewMatrix);

			float size = m_ViewportWidth * 0.2f;

			m_GrassTexture->Bind();
			m_Shader->SetUniform("u_Texture", 0);

			for (float x = 0; x < m_ViewportWidth; x += size)
			{
				for (float y = 0; y < m_ViewportHeight; y += size)
				{
					glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size));
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), { x + (size * 0.5f), y + (size * 0.5f), -0.2f }) * scale;

					m_Shader->SetUniform("u_Uniforms.Transform", transform);
					m_Pipeline->DrawIndexed(m_IndexBuffer->GetCount());
				}
			}
		}

		float aspectRatio = (float)m_ViewportWidth / (float)m_ViewportHeight;
		glm::mat4 projectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
		m_Shader->SetUniform("u_Uniforms.ViewProjectionMatrix", projectionMatrix * viewMatrix);
		
		// Ball
		{
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { -1.0f, -0.7f, -0.1f }) * scale;

			m_BallTexture->Bind();
			m_Shader->SetUniform("u_Texture", 0);

			m_Shader->SetUniform("u_Uniforms.Transform", transform);
			m_Pipeline->DrawIndexed(m_IndexBuffer->GetCount());
		}

		// Hole
		{
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { 1.0f, 0.7f, -0.1f }) * scale;

			m_HoleTexture->Bind();
			m_Shader->SetUniform("u_Texture", 0);

			m_Shader->SetUniform("u_Uniforms.Transform", transform);
			m_Pipeline->DrawIndexed(m_IndexBuffer->GetCount());
		}
	}

	void GolfDash::SetViewportSize(uint32 width, uint32 height)
	{
		if (width != m_ViewportWidth || height != m_ViewportHeight)
		{
			glViewport(0, 0, width, height);

			m_ViewportWidth = width;
			m_ViewportHeight = height;
		}
	}

}
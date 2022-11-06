#include "GolfDashPCH.h"
#include "GolfDash.h"

#include "glad/glad.h"

namespace gd {

#define PRINT(message) std::cout << message << std::endl

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

		m_Mouse = Ref<Mouse>::Create(m_Window);

		SetViewportSize(m_Window->GetFramebufferWidth(), m_Window->GetFramebufferHeight());
	}

	void GolfDash::Run()
	{
		OnInit();

		float deltaTime = 0.0f;
		float lastTime = 0.0f;

		m_Window->ShowWindow();
		while (m_Running)
		{
			float time = m_Window->GetTime();
			deltaTime = time - lastTime;
			lastTime = time;

			m_Mouse->OnUpdate();
			m_Window->PollEvents();
			OnUpdate(time, deltaTime);
			m_Window->SwapBuffers();
		}
	}

	void GolfDash::OnInit()
	{
		// Default
		{
			m_DefaultShader = Ref<Shader>::Create("Assets/Shaders/Default_Vertex.glsl", "Assets/Shaders/Default_Fragment.glsl");

			PipelineConfig pipelineConfig;
			pipelineConfig.Shader = m_DefaultShader;
			pipelineConfig.InputLayout = {
				{ "a_Position", ShaderDataType::Vec3 },
				{ "a_TexCoord", ShaderDataType::Vec2 }
			};
			m_DefaultPipeline = Ref<Pipeline>::Create(pipelineConfig);
		}

		// Composite
		{
			m_CompositeShader = Ref<Shader>::Create("Assets/Shaders/Composite_Vertex.glsl", "Assets/Shaders/Composite_Fragment.glsl");

			PipelineConfig pipelineConfig;
			pipelineConfig.Shader = m_CompositeShader;
			pipelineConfig.InputLayout = {
				{ "a_Position", ShaderDataType::Vec3 },
				{ "a_TexCoord", ShaderDataType::Vec2 }
			};
			m_CompositePipeline = Ref<Pipeline>::Create(pipelineConfig);
		}

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

		m_GrassTexture = Ref<Texture>::Create("Assets/Textures/Grass.png");
		m_BallTexture = Ref<Texture>::Create("Assets/Textures/Ball.png");
		m_HoleTexture = Ref<Texture>::Create("Assets/Textures/Hole.png");

		m_HolePosition = { 0.5f, 0.5f };
	}

	static void VelocityEpsilonClamp(const glm::vec2& initialVelocity, glm::vec2& inoutVelocity)
	{
		const float epsilon = 0.005f;

		if (initialVelocity.x > 0.0f)
		{
			if (inoutVelocity.x <= epsilon)
				inoutVelocity.x = 0.0f;
		}
		else
		{
			if (inoutVelocity.x >= -epsilon)
				inoutVelocity.x = 0.0f;
		}

		if (initialVelocity.y > 0.0f)
		{
			if (inoutVelocity.y <= epsilon)
				inoutVelocity.y = 0.0f;
		}
		else
		{
			if (inoutVelocity.y >= -epsilon)
				inoutVelocity.y = 0.0f;
		}
	}

	void GolfDash::OnUpdate(float time, float deltaTime)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		m_VertexBuffer->Bind();
		m_DefaultPipeline->Bind();
		m_IndexBuffer->Bind();

		m_DefaultShader->Bind();

		float aspectRatio = (float)m_ViewportWidth / (float)m_ViewportHeight;
		glm::mat4 projectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
		glm::mat4 viewMatrix = glm::inverse(glm::mat4(1.0f));
		m_DefaultShader->SetUniform("u_Uniforms.ViewProjectionMatrix", projectionMatrix * viewMatrix);

		glm::vec2 orthoMousePos = m_Mouse->GetMouseOrthoPosition(viewMatrix, projectionMatrix);

		// BG
		{
			m_GrassTexture->Bind();
			m_DefaultShader->SetUniform("u_Texture", 0);

			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

			float size = 3.0f;

			for (float x = -size; x <= size; x += 1.0f)
			{
				for (float y = -size; y <= size; y += 1.0f)
				{
					glm::vec3 position = { x, y, -0.2f };
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scaleMatrix;
					m_DefaultShader->SetUniform("u_Uniforms.Transform", transform);
					m_DefaultPipeline->DrawIndexed(m_IndexBuffer->GetCount());
				}
			}
		}
		
		// Ball
		{
			glm::vec2 ballScale = glm::vec2(0.1f);

			if (m_Mouse->GetMouseButton(GD_MOUSE_BUTTON_LEFT))
			{
				if (glm::epsilonEqual(orthoMousePos, m_BallPosition, ballScale * 0.5f) == glm::bvec2(true))
				{
					if (m_BallVelocity.x == 0.0f && m_BallVelocity.y == 0.0f)
						m_IsDragging = true;
				}
			}

			if (m_Mouse->GetMouseButtonUp(GD_MOUSE_BUTTON_LEFT))
			{
				if (m_IsDragging)
				{
					const float maxDistance = 2.0f;

					float distance = glm::distance(orthoMousePos, m_BallPosition);
					if (distance <= ballScale.x)
					{
						PRINT("Ball was dragged within it's boundaries (no force applied!)");
					}
#if 0
					else if (distance >= maxDistance)
					{
						PRINT("Reached the max distance!");
					}
#endif
					else
					{
						PRINT("Stopped dragging, adding force! distance: " << distance);

						m_InitialBallVelocity = glm::normalize(m_BallPosition - orthoMousePos);
						m_InitialBallVelocity *= glm::sqrt(distance);
						PRINT("Initial velocity: " << m_InitialBallVelocity.x << ", " << m_InitialBallVelocity.y);

						m_BallVelocity = m_InitialBallVelocity;
					}
					m_IsDragging = false;
				}
			}

			// TODO: tweak this
			m_BallVelocity *= 1.0f - 0.005f;
			VelocityEpsilonClamp(m_InitialBallVelocity, m_BallVelocity);
			m_BallPosition += m_BallVelocity * deltaTime;

			if (m_BallPosition.x >= aspectRatio - (ballScale.x * 0.5f))
			{
				m_InitialBallVelocity = glm::vec2(-m_BallVelocity.x, m_BallVelocity.y);
				m_BallVelocity = m_InitialBallVelocity;
			}

			if (m_BallPosition.x <= -aspectRatio + (ballScale.x * 0.5f))
			{
				m_InitialBallVelocity = glm::vec2(-m_BallVelocity.x, m_BallVelocity.y);
				m_BallVelocity = m_InitialBallVelocity;
			}

			if (m_BallPosition.y >= 1.0f - (ballScale.x * 0.5f))
			{
				m_InitialBallVelocity = glm::vec2(m_BallVelocity.x, -m_BallVelocity.y);
				m_BallVelocity = m_InitialBallVelocity;
			}

			if (m_BallPosition.y <= -1.0f + (ballScale.x * 0.5f))
			{
				m_InitialBallVelocity = glm::vec2(m_BallVelocity.x, -m_BallVelocity.y);
				m_BallVelocity = m_InitialBallVelocity;
			}

			float magnitude = glm::length(m_BallVelocity);
			if (magnitude < 1.0f)
			{
				float size = 0.35f;
				if (glm::epsilonEqual(m_BallPosition, m_HolePosition, ballScale * size) == glm::bvec2(true))
				{
					if (!m_IsBallInHole)
					{
						// Reset ball velocity
						m_InitialBallVelocity = { 0.0f, 0.0f };
						m_BallVelocity = { 0.0f, 0.0f };

						m_IsBallInHole = true;
					}

					m_TimeInHole += deltaTime;
					if (m_TimeInHole >= 2.0f)
					{
						m_IsBallInHole = false;
						m_TimeInHole = 0.0f;

						// temp
						m_BallPosition = { -0.5f, -0.5f };
					}
				}
			}

			if (!m_IsBallInHole)
			{
				glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(ballScale, 1.0f));
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), { m_BallPosition, -0.1f }) * scaleMatrix;

				m_BallTexture->Bind();
				m_DefaultShader->SetUniform("u_Texture", 0);

				m_DefaultShader->SetUniform("u_Uniforms.Transform", transform);
				m_DefaultPipeline->DrawIndexed(m_IndexBuffer->GetCount());
			}
		}

		// Hole
		{
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { m_HolePosition, -0.1f }) * scaleMatrix;

			m_HoleTexture->Bind();
			m_DefaultShader->SetUniform("u_Texture", 0);

			m_DefaultShader->SetUniform("u_Uniforms.Transform", transform);
			m_DefaultPipeline->DrawIndexed(m_IndexBuffer->GetCount());
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
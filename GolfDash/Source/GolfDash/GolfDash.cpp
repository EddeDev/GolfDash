#include "GolfDashPCH.h"
#include "GolfDash.h"

#include "ResourceManager.h"

namespace gd {

	GolfDash::GolfDash()
	{
		s_Instance = this;

		WindowConfig windowConfig;
		windowConfig.Width = 1920;
		windowConfig.Height = 1080;
		windowConfig.Title = "Golf Dash";
		windowConfig.Fullscreen = false;
		windowConfig.VSync = true;

		m_Window = Ref<Window>::Create(windowConfig);
		m_Window->CreateContext();

		ResourceManager::Init();

		m_Window->SetCursor(ResourceManager::GetTexture("Cursor"));
		m_Window->AddCloseCallback([this]()
		{
			Close();
		});

		m_Mouse = Ref<Mouse>::Create(m_Window);

		// Create level manager
		m_LevelManager = Ref<LevelManager>::Create();
		m_LevelManager->BindLevel(LevelType::Level1, CreateLevel1(), true);
		m_LevelManager->BindLevel(LevelType::Level2, CreateLevel2());
		m_LevelManager->BindLevel(LevelType::Level3, CreateLevel3());
		m_LevelManager->BindLevel(LevelType::Level4, CreateLevel4());
		m_LevelManager->BindLevel(LevelType::Level5, CreateLevel5());

		m_Window->AddFramebufferSizeCallback([this](uint32 width, uint32 height) { m_LevelManager->SetViewportSize(width, height); });
		m_LevelManager->SetViewportSize(m_Window->GetFramebufferWidth(), m_Window->GetFramebufferHeight());
	}

	GolfDash::~GolfDash()
	{
		ResourceManager::Shutdown();
	}

	void GolfDash::Run()
	{
		float deltaTime = 0.0f;
		float lastFrameTime = 0.0f;
		float lastTime = 0.0f;

		uint32 frames = 0;

		m_Window->ShowWindow();
		while (m_Running)
		{
			float time = m_Window->GetTime();
			deltaTime = time - lastFrameTime;
			lastFrameTime = time;

			frames++;
			while (time >= lastTime + 1.0f)
			{
				m_Window->SetTitle("Golf Dash - " + std::to_string(frames) + " fps");
				lastTime += 1.0f;
				frames = 0;
			}

			m_Mouse->OnUpdate();
			m_Window->PollEvents();
			m_LevelManager->OnUpdate(time, deltaTime);
			m_Window->SwapBuffers();
		}
	}

	void GolfDash::Close()
	{
		m_Running = false;
	}

	Ref<Level> GolfDash::CreateLevel1() const
	{
		LevelSpecification levelSpec;
		levelSpec.Index = 1;
		levelSpec.BallPosition = { -1.0f, 0.0f };
		levelSpec.HolePosition = { 1.0f, 0.0f };
		
		levelSpec.BackgroundTexture = ResourceManager::GetTexture("BG_Tiles_Default");

		Ref<Texture> obstacle1Texture = ResourceManager::GetTexture("Obstacle");
		Ref<Texture> obstacle2Texture = ResourceManager::GetTexture("Obstacle_Gray");

		Obstacle& obstacle1 = levelSpec.Obstacles.emplace_back();
		obstacle1.Position = { 0.0f, -0.3f };
		obstacle1.Texture = obstacle2Texture;
		obstacle1.Scale.x = 0.5f;
		obstacle1.Scale.y = 0.5f;

		Obstacle& obstacle2 = levelSpec.Obstacles.emplace_back();
		obstacle2.Position = { 0.0f, 0.3f };
		obstacle2.Texture = obstacle2Texture;
		obstacle2.Scale.x = 0.5f;
		obstacle2.Scale.y = 0.5f;

		Obstacle& obstacle3 = levelSpec.Obstacles.emplace_back();
		obstacle3.Position = { -0.5f, 0.0f };
		obstacle3.Texture = obstacle1Texture;
		obstacle3.Scale.x = 0.3f;
		obstacle3.Scale.y = 0.3f;

		Obstacle& obstacle4 = levelSpec.Obstacles.emplace_back();
		obstacle4.Position = { 0.5f, 0.0f };
		obstacle4.Texture = obstacle1Texture;
		obstacle4.Scale.x = 0.3f;
		obstacle4.Scale.y = 0.3f;

		Obstacle& obstacle5 = levelSpec.Obstacles.emplace_back();
		obstacle5.Position = { -1.5f, 0.75f };
		obstacle5.Texture = obstacle1Texture;
		obstacle5.Scale.x = 0.3f;
		obstacle5.Scale.y = 0.3f;

		Obstacle& obstacle6 = levelSpec.Obstacles.emplace_back();
		obstacle6.Position = { 1.5f, 0.75f };
		obstacle6.Texture = obstacle1Texture;
		obstacle6.Scale.x = 0.3f;
		obstacle6.Scale.y = 0.3f;

		Obstacle& obstacle7 = levelSpec.Obstacles.emplace_back();
		obstacle7.Position = { -1.5f, -0.75f };
		obstacle7.Texture = obstacle1Texture;
		obstacle7.Scale.x = 0.3f;
		obstacle7.Scale.y = 0.3f;

		Obstacle& obstacle8 = levelSpec.Obstacles.emplace_back();
		obstacle8.Position = { 1.5f, -0.75f };
		obstacle8.Texture = obstacle1Texture;
		obstacle8.Scale.x = 0.3f;
		obstacle8.Scale.y = 0.3f;

		return Ref<Level>::Create(levelSpec);
	}

	Ref<Level> GolfDash::CreateLevel2() const
	{
		LevelSpecification levelSpec;
		levelSpec.Index = 2;
		levelSpec.BallPosition = { -1.0f, -0.5f };
		levelSpec.HolePosition = { 1.0f, -0.5f };
		levelSpec.BackgroundTexture = ResourceManager::GetTexture("BG_Tiles_Default");

		Ref<Texture> obstacleTexture = ResourceManager::GetTexture("Obstacle_Gray");

		Obstacle& obstacle1 = levelSpec.Obstacles.emplace_back();
		obstacle1.Position = { -0.5f, -0.5f };
		obstacle1.Texture = obstacleTexture;
		obstacle1.Scale.x = 0.25f;
		obstacle1.Scale.y = 0.25f;

		Obstacle& obstacle2 = levelSpec.Obstacles.emplace_back();
		obstacle2.Position = { -0.5f, 0.0f };
		obstacle2.Texture = obstacleTexture;
		obstacle2.Scale.x = 0.25f;
		obstacle2.Scale.y = 0.25f;

		Obstacle& obstacle3 = levelSpec.Obstacles.emplace_back();
		obstacle3.Position = { 0.0f, -0.5f };
		obstacle3.Texture = obstacleTexture;
		obstacle3.Scale.x = 0.25f;
		obstacle3.Scale.y = 0.25f;

		Obstacle& obstacle4 = levelSpec.Obstacles.emplace_back();
		obstacle4.Position = { 0.0f, 0.0f };
		obstacle4.Texture = obstacleTexture;
		obstacle4.Scale.x = 0.25f;
		obstacle4.Scale.y = 0.25f;

		Obstacle& obstacle5 = levelSpec.Obstacles.emplace_back();
		obstacle5.Position = { 0.5f, -0.5f };
		obstacle5.Texture = obstacleTexture;
		obstacle5.Scale.x = 0.25f;
		obstacle5.Scale.y = 0.25f;

		Obstacle& obstacle6 = levelSpec.Obstacles.emplace_back();
		obstacle6.Position = { 0.5f, 0.0f };
		obstacle6.Texture = obstacleTexture;
		obstacle6.Scale.x = 0.25f;
		obstacle6.Scale.y = 0.25f;

		Obstacle& obstacle7 = levelSpec.Obstacles.emplace_back();
		obstacle7.Position = { 1.0f, 0.0f };
		obstacle7.Texture = obstacleTexture;
		obstacle7.Scale.x = 0.25f;
		obstacle7.Scale.y = 0.25f;

		return Ref<Level>::Create(levelSpec);
	}

	Ref<Level> GolfDash::CreateLevel3() const
	{
		LevelSpecification levelSpec;
		levelSpec.Index = 3;
		levelSpec.BallPosition = { -1.0f, -0.5f };
		levelSpec.HolePosition = { 1.0f, -0.75f };
		levelSpec.BackgroundTexture = ResourceManager::GetTexture("Assets/Textures/BG_Tiles_Default");

		BoostPad& boostPad1 = levelSpec.BoostPads.emplace_back();
		boostPad1.Position = { -1.5f, 0.5f };
		boostPad1.Scale = { 0.5f, 0.5f };
		boostPad1.Rotation = 90.0f;

		BoostPad& boostPad2 = levelSpec.BoostPads.emplace_back();
		boostPad2.Position = { 1.0f, 0.5f };
		boostPad2.Scale = { 0.5f, 0.5f };
		boostPad2.Rotation = 180.0f;
		
		Ref<Texture> obstacleTexture = ResourceManager::GetTexture("Assets/Textures/Obstacle_Gray");

		Obstacle& obstacle1 = levelSpec.Obstacles.emplace_back();
		obstacle1.Position = { -1.0f, 0.0f };
		obstacle1.Texture = obstacleTexture;
		obstacle1.Scale.x = 0.5f;
		obstacle1.Scale.y = 0.5f;

		Obstacle& obstacle2 = levelSpec.Obstacles.emplace_back();
		obstacle2.Position = { -0.5f, 0.0f };
		obstacle2.Texture = obstacleTexture;
		obstacle2.Scale.x = 0.5f;
		obstacle2.Scale.y = 0.5f;

		Obstacle& obstacle3 = levelSpec.Obstacles.emplace_back();
		obstacle3.Position = { -0.5f, -0.5f };
		obstacle3.Texture = obstacleTexture;
		obstacle3.Scale.x = 0.5f;
		obstacle3.Scale.y = 0.5f;

		Obstacle& obstacle4 = levelSpec.Obstacles.emplace_back();
		obstacle4.Position = { -0.5f, -1.0f };
		obstacle4.Texture = obstacleTexture;
		obstacle4.Scale.x = 0.5f;
		obstacle4.Scale.y = 0.5f;

		Obstacle& obstacle5 = levelSpec.Obstacles.emplace_back();
		obstacle5.Position = { 0.0f, 0.0f };
		obstacle5.Texture = obstacleTexture;
		obstacle5.Scale.x = 0.5f;
		obstacle5.Scale.y = 0.5f;

		Obstacle& obstacle6 = levelSpec.Obstacles.emplace_back();
		obstacle6.Position = { 0.0f, -0.5f };
		obstacle6.Texture = obstacleTexture;
		obstacle6.Scale.x = 0.5f;
		obstacle6.Scale.y = 0.5f;

		Obstacle& obstacle7 = levelSpec.Obstacles.emplace_back();
		obstacle7.Position = { 0.0f, -1.0f };
		obstacle7.Texture = obstacleTexture;
		obstacle7.Scale.x = 0.5f;
		obstacle7.Scale.y = 0.5f;

		return Ref<Level>::Create(levelSpec);
	}

	Ref<Level> GolfDash::CreateLevel4() const
	{
		LevelSpecification levelSpec;
		levelSpec.Index = 4;
		levelSpec.BallPosition = { -1.25f, 0.0f };
		levelSpec.HolePosition = { 1.25f, 0.0f };
		levelSpec.BackgroundTexture = ResourceManager::GetTexture("BG_Tiles_Ice");
		levelSpec.GroundFriction = 0.5f * 0.5f; // slipperiness

		Obstacle& obstacle1 = levelSpec.Obstacles.emplace_back();
		obstacle1.Position = { -0.5f, 0.0f };
		obstacle1.Texture = ResourceManager::GetTexture("Obstacle_LightBlue");
		obstacle1.Scale.x = 0.5f;
		obstacle1.Scale.y = 0.5f;
		obstacle1.UpdateFunction = [](ObstacleUpdateFunctionData& data)
		{
			Obstacle& obstacle = *data.Instance;
			obstacle.Position.y = glm::sin(data.Time * 2.0f) * 0.5f;
		};

		Obstacle& obstacle2 = levelSpec.Obstacles.emplace_back();
		obstacle2.Position = { 0.0f, 0.0f };
		obstacle2.Texture = ResourceManager::GetTexture("Obstacle_LightBlue");
		obstacle2.Scale.x = 0.5f;
		obstacle2.Scale.y = 0.5f;
		obstacle2.UpdateFunction = [](ObstacleUpdateFunctionData& data)
		{
			Obstacle& obstacle = *data.Instance;
			obstacle.Position.y = -glm::cos(data.Time * 2.0f) * 0.5f;
		};

		Obstacle& obstacle3 = levelSpec.Obstacles.emplace_back();
		obstacle3.Position = { 0.5f, 0.0f };
		obstacle3.Texture = ResourceManager::GetTexture("Obstacle_LightBlue");
		obstacle3.Scale.x = 0.5f;
		obstacle3.Scale.y = 0.5f;
		obstacle3.UpdateFunction = [](ObstacleUpdateFunctionData& data)
		{
			Obstacle& obstacle = *data.Instance;
			obstacle.Position.y = glm::sin(data.Time * 2.0f) * 0.5f;
		};

		return Ref<Level>::Create(levelSpec);
	}

	Ref<Level> GolfDash::CreateLevel5() const
	{
		LevelSpecification levelSpec;
		levelSpec.Index = 5;
		levelSpec.BallPosition = { -1.0f, 0.0f };
		levelSpec.HolePosition = { 1.0f, 0.0f };
		levelSpec.BackgroundTexture = ResourceManager::GetTexture("BG_Tiles_Ice");
		levelSpec.GroundFriction = 0.5f * 0.5f; // slipperiness

		Obstacle& obstacle1 = levelSpec.Obstacles.emplace_back();
		obstacle1.Position = { 0.0f, 0.0f };
		obstacle1.Texture = ResourceManager::GetTexture("Obstacle_Gray");
		obstacle1.Scale.x = 0.5f;
		obstacle1.Scale.y = 0.5f;
		obstacle1.UpdateFunction = [](ObstacleUpdateFunctionData& data)
		{
			Obstacle& obstacle = *data.Instance;
			obstacle.Position.x = glm::sin(data.Time * 2.0f) * 0.5f;
		};

		Obstacle& obstacle2 = levelSpec.Obstacles.emplace_back();
		obstacle2.Position = { 0.0f, 0.5f };
		obstacle2.Texture = ResourceManager::GetTexture("Obstacle_Gray");
		obstacle2.Scale.x = 0.5f;
		obstacle2.Scale.y = 0.5f;
		obstacle2.UpdateFunction = [](ObstacleUpdateFunctionData& data)
		{
			Obstacle& obstacle = *data.Instance;
			obstacle.Position.x = glm::cos(data.Time * 0.5f) * 1.25f;
		};

		Obstacle& obstacle3 = levelSpec.Obstacles.emplace_back();
		obstacle3.Position = { 0.0f, -0.5f };
		obstacle3.Texture = ResourceManager::GetTexture("Obstacle_Gray");
		obstacle3.Scale.x = 0.5f;
		obstacle3.Scale.y = 0.5f;
		obstacle3.UpdateFunction = [](ObstacleUpdateFunctionData& data)
		{
			Obstacle& obstacle = *data.Instance;
			obstacle.Position.x = glm::cos(data.Time * 0.5f) * 1.25f;
		};

		return Ref<Level>::Create(levelSpec);
	}

	gd::GolfDash* GolfDash::s_Instance;

}
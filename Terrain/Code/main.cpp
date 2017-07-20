// Include standard headers
#include "gl.h"

#include "Maths.h"
#include "Camera.h"
#include "DebugConsole.h"
#include "EntityManager.h"
#include "FontType.h"
#include "GameInfo.h"
#include "GUIRenderer.h"
#include "GUIText.h"
#include "GUITexture.h"
#include "Input.h"
#include "Loader.h"
#include "MasterRenderer.h"
#include "ModelTexture.h"
#include "Player.h"
#include "StaticShader.h"
#include "TerrainManager.h"
#include "TerrainTexture.h"
#include "TextMaster.h"
#include "TexturedModel.h"
#include "WaterFrameBuffer.h"
#include "WaterRenderer.h"
#include "WaterShader.h"
#include "WaterTile.h"
#include "Sun.h"
#include "SunRenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <iomanip>
#include <iostream>


using namespace std;

void DebugControls(TerrainManager& aTerrainManager)
{
	GameInfo::ourRenderTimes.clear();
	
	if (DebugConsole::IsActive())
	{
		return;
	}

	if (Input::IsPressed(GLFW_KEY_P))
	{
		GameInfo::ourWireframeMode = !GameInfo::ourWireframeMode;
		glPolygonMode(GL_FRONT_AND_BACK, GameInfo::ourWireframeMode ? GL_LINE : GL_FILL);
	}
	if (Input::IsPressed(GLFW_KEY_O))
	{
		GameInfo::ourDrawEntities = !GameInfo::ourDrawEntities;
	}
	if (Input::IsPressed(GLFW_KEY_L))
	{
		GameInfo::ourDrawTerrain = !GameInfo::ourDrawTerrain;
	}
	if (Input::IsPressed(GLFW_KEY_K))
	{
		GameInfo::ourDrawWater = !GameInfo::ourDrawWater;
	}
	if (Input::IsPressed(GLFW_KEY_N))
	{
		GameInfo::ourFlyCamera = !GameInfo::ourFlyCamera;
	}
	if (Input::IsPressed(GLFW_KEY_U))
	{
		GameInfo::ourGenerateTerrainThreaded = !GameInfo::ourGenerateTerrainThreaded;
	}
	if (Input::IsPressed(GLFW_KEY_Y))
	{
		GameInfo::ourGenerateTerrainCaching = !GameInfo::ourGenerateTerrainCaching;
	}
	if (Input::IsPressed(GLFW_KEY_M))
	{
		std::cout.imbue(std::locale(""));
		cout << std::fixed;

		cout << "Frame Time: " << GameInfo::ourDeltaTime << endl;
		double total = 0;
		unsigned int totalVerts = 0;
		cout << std::setw(30) << left << "Location"
			<< std::setw(30) << left << "ms"
			<< std::setw(10) << right << "Triangles" << endl;
		for (auto& pass : GameInfo::ourRenderTimes)
		{
			total += pass.second.first;
			totalVerts += pass.second.second;
			cout << std::setw(30) << left << pass.first.c_str() 
				<< std::setw(30) << std::setprecision(4) << pass.second.first 
				<< std::setw(10) << right << pass.second.second << endl;
		}
		std::cout << std::string(70, '-') << std::endl;

		cout << std::setw(30) << left << "Total" 
			<< std::setw(30) << total 
			<< std::setw(10) << right << totalVerts << endl;
		std::cout << std::string(70, '=') << std::endl;
	}
	if (Input::IsPressed(GLFW_KEY_F12))
	{
		aTerrainManager.Regenerate();
	}
}

bool Setup()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

																   // Open a window and create its OpenGL context
	GLFWwindow* window; // (In the accompanying source code, this variable is global)
	window = glfwCreateWindow(GameInfo::ourScreenWidth, GameInfo::ourScreenHeight, "Terrain Test", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	GameInfo::SetWindow(window);

	Input::Setup();
	return true;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	double startLoadTime = glfwGetTime();
	if (!Setup())
	{
		system("color 47");
		return -1;
	}
	
	Loader loader;
	TextMaster::Init(&loader);

	// Temp Loading code for now
	{
		FontType font(loader.LoadTexture("Fonts/times.png"), "data/Fonts/times.fnt");
		GUIText *loadingText = new GUIText("Loading...", 1.0f, font, vec2(0.0, 0.5), 1.0f, true);
		loadingText->SetColour(1.0f, 0.0f, 0.0f);
		TextMaster::Render();
		glfwSwapBuffers(GameInfo::ourWindow);
		glfwPollEvents();
		delete loadingText;
		loadingText = nullptr;
	}

	DebugConsole::Setup(loader);

	// TODO:CW Set up proper day/night time so console can type "settime noon" or "settime 18" etc
		
	TerrainTexture blendMap = loader.LoadTexture("blendMap.png");
	TerrainTexturePack texturePack("grass.png", "grassFlowers.png", "mud.png", "grassy2.png", loader);
	TerrainManager terrainManager(loader, texturePack, blendMap);

	EntityManager entityManager(terrainManager);
	vector<GUITexture> guis;
	
	RawModel bunnyModel = loader.LoadToVAO("stanfordBunny.obj");
	ModelTexture bunnyTexture(loader.LoadTexture("white.png"));
	TexturedModel texturedBunnyModel(bunnyModel, bunnyTexture);

	vec3 playerStartPos = GameInfo::ourPlayerSpawnPos;
	playerStartPos.y = terrainManager.GetHeight(playerStartPos.x, playerStartPos.z);
	Player player(texturedBunnyModel, playerStartPos, vec3(0, 0, 0), 1);

	Camera camera(player);

	ModelTexture sunTexture(loader.LoadTexture("sun.png"));

	Sun sun(sunTexture, 120);
	SunRenderer sunRenderer(loader);
	MasterRenderer renderer(loader, sunRenderer);

	WaterFrameBuffer fbos;
	WaterShader waterShader;
	WaterRenderer waterRenderer(loader, waterShader, fbos);
	waterRenderer.Setup();
	vector<WaterTile> waters;

	GUIRenderer guiRenderer(loader);
	guiRenderer.Setup();


	//system("color 20"); // TODO:CW add console commands to show all FBOs
	//guis.push_back(GUITexture(fbos.myReflectionTexture, vec2(-0.5, 0.5f), vec2(0.3f, 0.3f)));
	//guis.push_back(GUITexture(fbos.myRefractionDepthTexture, vec2(0.5, 0.5f), vec2(0.3f, 0.3f)));

	cout << "Loading took: " << glfwGetTime() - startLoadTime << endl;
	terrainManager.Regenerate();
	waters.push_back(WaterTile(vec3(75, GameInfo::ourWaterHeight, -500)));
	cout << "Loading took: " << glfwGetTime() - startLoadTime << endl;

	double currentFrame = glfwGetTime();
	double lastFrame = currentFrame;
	do {
		Input::UpdateInput();
		currentFrame = glfwGetTime();
		GameInfo::ourDeltaTime = (float)(currentFrame - lastFrame);
		lastFrame = currentFrame;

		camera.Update(terrainManager);
		DebugControls(terrainManager);
		sun.Update();
		
		player.Update(terrainManager);

		glEnable(GL_CLIP_DISTANCE0);

		fbos.BindReflectionFrameBuffer();
		{
			float distance = 2 * (camera.myPosition.y - GameInfo::ourWaterHeight);
			camera.myPosition.y -= distance;
			camera.InvertCamera();
			renderer.ProcessEntity(player);
			renderer.RenderScene(entityManager.GetEntities(), terrainManager.GetTerrains(), sun, camera, vec4(0, 1, 0, -GameInfo::ourWaterHeight + 5.0f));
			camera.myPosition.y += distance;
			camera.InvertCamera();
		}
		
		fbos.BindRefractionFrameBuffer();
		//renderer.ProcessEntity(player);
		// TODO:CW don't draw any of these entites under the water?
		renderer.RenderScene(entityManager.GetEntities(), terrainManager.GetTerrains(), sun, camera, vec4(0, -1, 0, GameInfo::ourWaterHeight + 1.0f));
		fbos.UnbindCurrentFrameBuffer();
		
		glDisable(GL_CLIP_DISTANCE0);
		renderer.ProcessEntity(player);
		renderer.RenderScene(entityManager.GetEntities(), terrainManager.GetTerrains(), sun, camera, vec4(0, 1, 0, 6));
		waterRenderer.Render(waters, camera, sun, GameInfo::ourFogColour);
		
		{
			//GLint polygonMode;
			//glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (DebugConsole *console = DebugConsole::GetInstance())
			{
				console->Update();
				if (console->IsActive())
					guis.push_back(*console->myGUITexture);
			}
			guiRenderer.Render(guis);
			TextMaster::Render();
			guis.clear();
			glPolygonMode(GL_FRONT_AND_BACK, GameInfo::ourWireframeMode ? GL_LINE : GL_FILL);
		}
		// Swap buffers
		glfwSwapBuffers(GameInfo::ourWindow);
		glfwPollEvents();
		
		ShaderBase::CheckReloadShaders();

	} // Check if the ESC key was pressed or the window was closed
	while (!Input::IsPressed(GLFW_KEY_ESCAPE) && glfwWindowShouldClose(GameInfo::ourWindow) == 0);

	DebugConsole::CleanUp();
	TextMaster::CleanUp(); // TODO:CW FIX
	//loader.CleanUp();
	
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}


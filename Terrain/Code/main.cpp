// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <iomanip>

#include "gl.h"

#include "Maths.h"
#include "Camera.h"
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

void DebugControls()
{
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
	GameInfo::ourRenderTimes.clear();
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

	GameInfo::ourWindow = window;

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

	FontType font(loader.LoadTexture("Fonts/times.png"), "data/Fonts/times.fnt");
	GUIText text("The Quick Brown Fox Jumped Over The Lazy Dog", 1, font, vec2(0.0, 0.0), 1.0f, false);
	/*
	RawModel model = loader.LoadToVAO("Tree005/tree_oak.obj");
	ModelTexture texture(loader.LoadTexture("Tree006/Branches0018_1_S.png"));
	TexturedModel texturedModel(model, texture);
	texture.myHasTransparency = true;
	/*texture.myReflectivity = 0.2f;
	texture.myShineDamper = 1;
	texture.myHasTransparency = true;
	texture.myUseFakeLighting = true;
*/
	//RawModel treeModel = loader.LoadToVAO("Tree001/Tree.obj");
	//ModelTexture treeTexture(loader.LoadTexture("white.png"));
	//TexturedModel texturedTreeModel(treeModel, texture);
	//treeTexture.myReflectivity = 0.2f;
	//treeTexture.myShineDamper = 1;


	//RawModel grassModel = loader.LoadToVAO("Tree002/tree.obj");
	//ModelTexture grassTexture(loader.LoadTexture("Tree002/DB2X2_L01.png"));
	//TexturedModel texturedGrassModel(grassModel, texture);
	/*grassTexture.myReflectivity = 0.2f;
	grassTexture.myShineDamper = 1;
	grassTexture.myHasTransparency = true;
	texture.myUseFakeLighting = true;*/
	
	/*
	for (int i = 0; i < 400; i++)
	{
		float x = rand() % 100 - 50;
		float y = 0;
		float z = rand() % 100 - 50;

		float rY = rand() % 180;
		float rS = float((rand() % 3) + 3.0f) / 20.0f;
		entityManager.AddEntity(texturedGrassModel, glm::vec3(x, y, z), glm::vec3(180, rY, 0), rS);
	}*/


	Light light(vec3(2000, 100, -200), vec3(0.988, 0.831, 0.25));
	ModelTexture grass = loader.LoadTexture("grass.bmp");
	grass.myReflectivity = 1;
	grass.myShineDamper = 10;

	TerrainTexture blendMap = loader.LoadTexture("blendMap.png");
	TerrainTexturePack texturePack("grass.png", "grassFlowers.png", "mud.png", "grassy2.png", loader);
	TerrainManager terrainManager(loader, texturePack, blendMap);

	EntityManager entityManager(terrainManager);;


	vector<GUITexture> guis;
	/*GUITexture gui = GUITexture(loader.LoadTexture("grassTexture.png"), vec2(0.5f, 0.5f), vec2(0.25f, 0.25f));
	GUITexture gui2 = GUITexture(loader.LoadTexture("fern.png"), vec2(0.3f, 0.75f), vec2(0.4f, 0.4f));

	guis.push_back(gui);
	guis.push_back(gui2);*/


	RawModel bunnyModel = loader.LoadToVAO("stanfordBunny.obj");
	ModelTexture bunnyTexture(loader.LoadTexture("white.png"));
	TexturedModel texturedBunnyModel(bunnyModel, bunnyTexture);

	vec3 playerStartPos = GameInfo::ourPlayerSpawnPos;
	playerStartPos.y = terrainManager.GetHeight(playerStartPos.x, playerStartPos.z);
	Player player(texturedBunnyModel, playerStartPos, vec3(0, 0, 0), 1);

	Camera camera(player);

	MasterRenderer renderer(loader);

	WaterFrameBuffer fbos;
	WaterShader waterShader;
	WaterRenderer waterRenderer(loader, waterShader, fbos);
	waterRenderer.Setup(renderer.GetProjectionMatrix());
	vector<WaterTile> waters;
	waters.push_back(WaterTile(vec3(75, GameInfo::ourWaterHeight, -500)));
	
	/*const float range = 500.0;
	const int num = 250;

	for (int i = 0; i < num; i++)
	{
		entityManager.AddEntityRandom(texturedModel, vec3(-range, 0, -range), vec3(range, 0, range), vec2(1, 5));
	}

	for (int i = 0; i < num; i++)
	{
		entityManager.AddEntityRandom(texturedGrassModel, vec3(-range, 0, -range), vec3(range, 0, range), vec2(5, 25));
	}

	for (int i = 0; i < num; i++)
	{
		entityManager.AddEntityRandom(texturedTreeModel, vec3(-range, 0, -range), vec3(range, 0, range), vec2(0.1, 0.5));
	}*/

	GUIRenderer guiRenderer(loader);
	guiRenderer.Setup(mat4(1));


	//system("color 20");
	//guis.push_back(GUITexture(fbos.myReflectionTexture, vec2(-0.5, 0.5f), vec2(0.3f, 0.3f)));
	//guis.push_back(GUITexture(fbos.myRefractionDepthTexture, vec2(0.5, 0.5f), vec2(0.3f, 0.3f)));
	
	cout << "Loading took: " << glfwGetTime() - startLoadTime << endl;

	double currentFrame = glfwGetTime();
	double lastFrame = currentFrame;
	do {
		Input::UpdateInput();
		currentFrame = glfwGetTime();
		GameInfo::ourDeltaTime = (float)(currentFrame - lastFrame);

		lastFrame = currentFrame;

		camera.Update(terrainManager);
		DebugControls();
		{
			static float time = -3.6f;
			float speed = (Input::IsDown(GLFW_KEY_LEFT_SHIFT) ? 10.0f : 1.0f) * GameInfo::ourDayNightSpeed;
			time -= GameInfo::ourDeltaTime / 20.0f * speed;

			if (Input::IsPressed(GLFW_KEY_KP_ADD))
			{
				time += 3.14f / 8.0f;
			}
			light.myPosition.y = sin(time) * 2000.0f;
			light.myPosition.x = cos(time) * 2000.0f;	

			GameInfo::ourDayNightTime = 1-(pow(1-dot(normalize(light.myPosition), vec3(0, 1, 0)), 3));
			float mix1 = clamp(GameInfo::ourDayNightTime, 0.0f, 1.0f);
			float mix2 = clamp(-GameInfo::ourDayNightTime, 0.0f, 1.0f);
			light.myColour = mix(vec3(0.992, 0.369, 0.325), vec3(0.988, 0.831, 0.25), mix1);
			light.myColour = mix(light.myColour, vec3(0.0, 0.02, 0.1), mix2);
		}

		if (Input::IsPressed(GLFW_KEY_KP_SUBTRACT))
		{
			terrainManager.Regenerate();
		}
		player.Update(terrainManager);

		glEnable(GL_CLIP_DISTANCE0);

		fbos.BindReflectionFrameBuffer();
		{
			float distance = 2 * (camera.myPosition.y - GameInfo::ourWaterHeight);
			camera.myPosition.y -= distance;
			camera.InvertCamera();
			renderer.ProcessEntity(player);
			renderer.RenderScene(entityManager.GetEntities(), terrainManager.GetTerrains(), light, camera, vec4(0, 1, 0, -GameInfo::ourWaterHeight + 5.0f));
			camera.myPosition.y += distance;
			camera.InvertCamera();
		}
		
		fbos.BindRefractionFrameBuffer();
		//renderer.ProcessEntity(player);
		// TODO:CW don't draw any of these entites under the water?
		renderer.RenderScene(entityManager.GetEntities(), terrainManager.GetTerrains(), light, camera, vec4(0, -1, 0, GameInfo::ourWaterHeight + 1.0f));
		fbos.UnbindCurrentFrameBuffer();
		
		glDisable(GL_CLIP_DISTANCE0);

		renderer.ProcessEntity(player);
		renderer.RenderScene(entityManager.GetEntities(), terrainManager.GetTerrains(), light, camera, vec4(0, 1, 0, 6));
		waterRenderer.Render(waters, camera, light);

		guiRenderer.Render(guis);
		TextMaster::Render();
		
		// Swap buffers
		glfwSwapBuffers(GameInfo::ourWindow);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (!Input::IsPressed(GLFW_KEY_ESCAPE) && glfwWindowShouldClose(GameInfo::ourWindow) == 0);

	//TextMaster::CleanUp(); // TODO:CW FIX
	//loader.CleanUp();
	
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}


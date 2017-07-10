// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <iomanip>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

// Include GLFW
#include "GL\glfw3.h"

// Include GLM
#include "glm/glm.hpp"
using namespace glm;

#include "Camera.h"
#include "EntityManager.h"
#include "GameInfo.h"
#include "GUIRenderer.h"
#include "GUITexture.h"
#include "Input.h"
#include "Loader.h"
#include "MasterRenderer.h"
#include "ModelTexture.h"
#include "Player.h"
#include "StaticShader.h"
#include "Terrain.h"
#include "TerrainTexture.h"
#include "TexturedModel.h"
#include "WaterFrameBuffer.h"
#include "WaterRenderer.h"
#include "WaterShader.h"
#include "WaterTile.h"

#define WATER_HEIGHT -20

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
	
	RawModel model = loader.LoadToVAO("fern.obj");
	ModelTexture texture(loader.LoadTexture("fern.png"));
	TexturedModel texturedModel(model, texture);
	texture.myReflectivity = 0.2f;
	texture.myShineDamper = 1;
	texture.myHasTransparency = true;
	texture.myUseFakeLighting = true;

	RawModel treeModel = loader.LoadToVAO("tree.obj");
	ModelTexture treeTexture(loader.LoadTexture("tree.png"));
	TexturedModel texturedTreeModel(treeModel, treeTexture);
	treeTexture.myReflectivity = 0.2f;
	treeTexture.myShineDamper = 1;


	RawModel grassModel = loader.LoadToVAO("grassModel.obj");
	ModelTexture grassTexture(loader.LoadTexture("grassTexture.png"));
	TexturedModel texturedGrassModel(grassModel, grassTexture);
	grassTexture.myReflectivity = 0.2f;
	grassTexture.myShineDamper = 1;
	grassTexture.myHasTransparency = true;
	texture.myUseFakeLighting = true;

	EntityManager entityManager;

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

	const float range = 200.0;
	const int num = 250;

	for (int i = 0; i < num; i++)
	{
		//entityManager.AddEntityRandom(texturedModel, vec3(-range, 0, -range), vec3(range, 0, range), vec2(0.15, 0.25));
	}

	for (int i = 0; i < num/2; i++)
	{
		//entityManager.AddEntityRandom(texturedTreeModel, vec3(-range, 0, -range), vec3(range, 0, range), vec2(1.5, 2.5));
	}

	
	RawModel bunnyModel = loader.LoadToVAO("stanfordBunny.obj");
	ModelTexture bunnyTexture(loader.LoadTexture("white.png"));
	TexturedModel texturedBunnyModel(bunnyModel, bunnyTexture);

	Player player(texturedBunnyModel, vec3(0, 0, 0), vec3(0, 0, 0), 1);

	Camera camera(player);
	Light light(vec3(0, 25, -200), vec3(1, 1, 1));
	ModelTexture grass = loader.LoadTexture("grass.bmp");

	TerrainTexture blendMap = loader.LoadTexture("blendMap.png");
	TerrainTexturePack texturePack("grass.png", "grassFlowers.png", "mud.png", "grassy2.png", loader);

	grass.myReflectivity = 1;
	grass.myShineDamper = 10;
	vector<Terrain> terrains;
	terrains.push_back(Terrain(-1, -1, loader, texturePack, blendMap));
	terrains.push_back(Terrain(0, -1, loader, texturePack, blendMap));
	terrains.push_back(Terrain(0, 0, loader, texturePack, blendMap));
	terrains.push_back(Terrain(-1, 0, loader, texturePack, blendMap));
	
	vector<GUITexture> guis;
	/*GUITexture gui = GUITexture(loader.LoadTexture("grassTexture.png"), vec2(0.5f, 0.5f), vec2(0.25f, 0.25f));
	GUITexture gui2 = GUITexture(loader.LoadTexture("fern.png"), vec2(0.3f, 0.75f), vec2(0.4f, 0.4f));

	guis.push_back(gui);
	guis.push_back(gui2);*/


	MasterRenderer renderer(loader);

	WaterFrameBuffer fbos;
	WaterShader waterShader;
	WaterRenderer waterRenderer(loader, waterShader, fbos);
	waterRenderer.Setup(renderer.GetProjectionMatrix());
	vector<WaterTile> waters;
	waters.push_back(WaterTile(vec3(75, WATER_HEIGHT, -500)));

	GUIRenderer guiRenderer(loader);
	guiRenderer.Setup(mat4(1));


	system("color 20");
	//guis.push_back(GUITexture(fbos.myReflectionTexture, vec2(-0.5, 0.5f), vec2(0.3f, 0.3f)));
	//guis.push_back(GUITexture(fbos.myRefractionDepthTexture, vec2(0.5, 0.5f), vec2(0.3f, 0.3f)));
	
	cout << "Loading took: " << glfwGetTime() - startLoadTime << endl;

	double currentFrame = glfwGetTime();
	double lastFrame = currentFrame;
	do {
		currentFrame = glfwGetTime();
		GameInfo::ourDeltaTime = (float)(currentFrame - lastFrame);
		lastFrame = currentFrame;

		Input::UpdateInput();
		camera.Update();
		DebugControls();

		player.Update();

		glEnable(GL_CLIP_DISTANCE0);

		fbos.BindReflectionFrameBuffer();
		{
			float distance = 2 * (camera.myPosition.y - WATER_HEIGHT);
			camera.myPosition.y -= distance;
			camera.InvertCamera();
			renderer.ProcessEntity(player);
			renderer.RenderScene(entityManager.myEntities, terrains, light, camera, vec4(0, 1, 0, -WATER_HEIGHT + 5.0f));
			camera.myPosition.y += distance;
			camera.InvertCamera();
		}
		
		fbos.BindRefractionFrameBuffer();
		//renderer.ProcessEntity(player);
		renderer.RenderScene(entityManager.myEntities, terrains, light, camera, vec4(0, -1, 0, WATER_HEIGHT + 1.0f));
		fbos.UnbindCurrentFrameBuffer();
		
		glDisable(GL_CLIP_DISTANCE0);

		renderer.ProcessEntity(player);
		renderer.RenderScene(entityManager.myEntities, terrains, light, camera, vec4(0, 1, 0, 6));
		waterRenderer.Render(waters, camera, light);

		guiRenderer.Render(guis);
		
		// Swap buffers
		glfwSwapBuffers(GameInfo::ourWindow);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (!Input::IsPressed(GLFW_KEY_ESCAPE) && glfwWindowShouldClose(GameInfo::ourWindow) == 0);

	//loader.CleanUp();
	
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}


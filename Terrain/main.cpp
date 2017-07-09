// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

// Include GLFW
#include "GL\glfw3.h"

// Include GLM
#include "glm/glm.hpp"
using namespace glm;

#include "Camera.h"
#include "GameInfo.h"
#include "GUIRenderer.h"
#include "GUITexture.h"
#include "Loader.h"
#include "MasterRenderer.h"
#include "ModelTexture.h"
#include "StaticShader.h"
#include "Terrain.h"
#include "TerrainTexture.h"
#include "TexturedModel.h"
#include "WaterFrameBuffer.h"
#include "WaterRenderer.h"
#include "WaterShader.h"
#include "WaterTile.h"

#define WATER_HEIGHT -20

int LastFrameKeys[GLFW_KEY_LAST + 1] = { 0 };

bool WasJustPressed(int aKey)
{
	return glfwGetKey(GameInfo::ourWindow, aKey) == GLFW_PRESS && LastFrameKeys[aKey] == GLFW_RELEASE;
}

void DebugControls(GLFWwindow* aWindow, Camera& aCamera)
{
	if (WasJustPressed(GLFW_KEY_P))
	{
		GameInfo::ourWireframeMode = !GameInfo::ourWireframeMode;
		glPolygonMode(GL_FRONT_AND_BACK, GameInfo::ourWireframeMode ? GL_LINE : GL_FILL);
	}
	if (WasJustPressed(GLFW_KEY_O))
	{
		GameInfo::ourDrawEntities = !GameInfo::ourDrawEntities;
	}
	if (WasJustPressed(GLFW_KEY_L))
	{
		GameInfo::ourDrawTerrain = !GameInfo::ourDrawTerrain;
	}
	if (WasJustPressed(GLFW_KEY_K))
	{
		GameInfo::ourDrawWater = !GameInfo::ourDrawWater;
	}

	for (int i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST; i++)
	{
		LastFrameKeys[i] = glfwGetKey(aWindow, i);
	}
}

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
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
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	GameInfo::ourWindow = window;

	Loader loader;

#pragma region old
	//float vertices[] = { -0.5f, 0.5f, 0.f, -0.5f, -0.5f, 0.f, 0.5f, -0.5f, 0.f, 0.5f, -0.5f, 0.f, 0.5f, 0.5f, 0.f, -0.5f, 0.5f, 0.f };
	//vector<float> vertices = { -0.5f, 0.5f, 0.f, -0.5f, -0.5f, 0.f, 0.5f, -0.5f, 0.f, 0.5f, -0.5f, 0.f, 0.5f, 0.5f, 0.f, -0.5f, 0.5f, 0.f };
	vector<float> vertices = { -0.5f, 0.5f, 0.f, -0.5f, -0.5f, 0.f, 0.5f, -0.5f, 0.f, 0.5f, 0.5f, 0.f};
	vector<int> indices = { 0, 1, 3, 3, 1, 2 };
	vector<float> textureCoords = { 0,0, 0,1, 1,1, 1,0 };
	//RawModel model = loader.LoadToVAO(vertices, textureCoords, indices);
	//ModelTexture texture(loader.LoadTexture("data/image.bmp"));
#pragma endregion

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

	vector<Entity> allEntities;

	/*
	for (int i = 0; i < 400; i++)
	{
		float x = rand() % 100 - 50;
		float y = 0;
		float z = rand() % 100 - 50;

		float rY = rand() % 180;
		float rS = float((rand() % 3) + 3.0f) / 20.0f;
		allEntities.push_back(Entity(texturedGrassModel, glm::vec3(x, y, z), glm::vec3(180, rY, 0), rS));
	}*/

	for (int i = 0; i < 200; i++)
	{
		float x = (float)(rand() % 200 - 100);
		float y = 0;
		float z = (float)(-rand() % 200);

		float rY = (float)(rand() % 180);
		float rS = float((rand() % 3) + 3.0f) / 20.0f;
		allEntities.push_back(Entity(texturedModel, glm::vec3(x, y, z), glm::vec3(0, rY, 0), rS));
	}

	for (int i = 0; i < 100; i++)
	{
		float x = (float)(rand() % 200 - 100);
		float y = 0;
		float z = (float)(-rand() % 200);

		float rY = (float)(rand() % 180);
		float rS = float((rand() % 3) + 3.0f) / 2.0f;
		allEntities.push_back(Entity(texturedTreeModel, glm::vec3(x, y, z), glm::vec3(0, -rY, 0), rS));
	}/**/

	double currentFrame = glfwGetTime();
	double lastFrame = currentFrame;
	
	Camera camera(*window);
	Light light(vec3(0, 25, -200), vec3(1, 1, 1));
	ModelTexture grass = loader.LoadTexture("grass.bmp");

	TerrainTexture blendMap = loader.LoadTexture("blendMap.png");
	TerrainTexturePack texturePack("grass.png", "grassFlowers.png", "mud.png", "grassy2.png", loader);

	grass.myReflectivity = 1;
	grass.myShineDamper = 10;
	vector<Terrain> terrains;
	terrains.push_back(Terrain(-1, -1, loader, texturePack, blendMap));
	terrains.push_back(Terrain(0, -1, loader, texturePack, blendMap));
	
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


	//guis.push_back(GUITexture(fbos.myReflectionTexture, vec2(-0.5, 0.5f), vec2(0.3f, 0.3f)));
	//guis.push_back(GUITexture(fbos.myRefractionDepthTexture, vec2(0.5, 0.5f), vec2(0.3f, 0.3f)));

	do {
		currentFrame = glfwGetTime();
		GameInfo::ourDeltaTime = (float)(currentFrame - lastFrame);
		lastFrame = currentFrame;

		camera.Move();
		DebugControls(window, camera);

		glEnable(GL_CLIP_DISTANCE0);

		fbos.BindReflectionFrameBuffer();
		{
			float distance = 2 * (camera.myPosition.y - WATER_HEIGHT);
			camera.myPosition.y -= distance;
			camera.InvertCamera();
			renderer.RenderScene(allEntities, terrains, light, camera, vec4(0, 1, 0, -WATER_HEIGHT + 5.0f));
			camera.myPosition.y += distance;
			camera.InvertCamera();
		}
		
		fbos.BindRefractionFrameBuffer();
		renderer.RenderScene(allEntities, terrains, light, camera, vec4(0, -1, 0, WATER_HEIGHT + 1.0f));
		fbos.UnbindCurrentFrameBuffer();
		
		glDisable(GL_CLIP_DISTANCE0);

		renderer.RenderScene(allEntities, terrains, light, camera, vec4(0, 1, 0, 6));
		waterRenderer.Render(waters, camera, light);

		guiRenderer.Render(guis);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	loader.CleanUp();
}


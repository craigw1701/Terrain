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
#include "Loader.h"
#include "MasterRenderer.h"
#include "ModelTexture.h"
#include "StaticShader.h"
#include "Terrain.h"
#include "TerrainTexture.h"
#include "TexturedModel.h"

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

	RawModel model = loader.LoadToVAO("data/fern.obj");
	ModelTexture texture(DecodeOneStep("data/fern.png"));
	TexturedModel texturedModel(model, texture);
	texture.myReflectivity = 0.2;
	texture.myShineDamper = 1;
	texture.myHasTransparency = true;
	texture.myUseFakeLighting = true;

	RawModel treeModel = loader.LoadToVAO("data/tree.obj");
	ModelTexture treeTexture(DecodeOneStep("data/tree.png"));
	TexturedModel texturedTreeModel(treeModel, treeTexture);
	treeTexture.myReflectivity = 0.2;
	treeTexture.myShineDamper = 1;


	RawModel grassModel = loader.LoadToVAO("data/grassModel.obj");
	ModelTexture grassTexture(DecodeOneStep("data/grassTexture.png"));
	TexturedModel texturedGrassModel(grassModel, grassTexture);
	grassTexture.myReflectivity = 0.2;
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
		float x = rand() % 200 - 100;
		float y = 0;
		float z = -rand() % 200;

		float rY = rand() % 180;
		float rS = float((rand() % 3) + 3.0f) / 20.0f;
		allEntities.push_back(Entity(texturedModel, glm::vec3(x, y, z), glm::vec3(0, rY, 0), rS));
	}

	for (int i = 0; i < 100; i++)
	{
		float x = rand() % 200 - 100;
		float y = 0;
		float z = -rand() % 200;

		float rY = rand() % 180;
		float rS = float((rand() % 3) + 3.0f) / 2.0f;
		allEntities.push_back(Entity(texturedTreeModel, glm::vec3(x, y, z), glm::vec3(0, -rY, 0), rS));
	}

	double currentFrame = glfwGetTime();
	double lastFrame = currentFrame;
	
	Camera camera(*window);
	Light light(vec3(0, 25, 0), vec3(1, 1, 1));
	ModelTexture grass = loader.LoadTexture("data/grass.bmp");

	TerrainTexture blendMap = DecodeOneStep("data/blendMap.png");
	TerrainTexturePack texturePack("data/grassy2.png", "data/grassFlowers.png", "data/mud.png", "data/path.png");

	grass.myReflectivity = 1;
	grass.myShineDamper = 10;
	Terrain terrain(-1, -1, loader, texturePack, blendMap);
	Terrain terrain2(0, -1, loader, texturePack, blendMap);
	
	MasterRenderer renderer;

	do {
		currentFrame = glfwGetTime();
		GameInfo::ourDeltaTime = (float)(currentFrame - lastFrame);
		lastFrame = currentFrame;

		camera.Move();

		for(Entity& entity : allEntities)
			renderer.ProcessEntity(entity);

		renderer.ProcessTerrain(terrain);
		renderer.ProcessTerrain(terrain2);

		renderer.Render(light, camera);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	loader.CleanUp();
}


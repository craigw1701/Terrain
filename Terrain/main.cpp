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

#include "Loader.h"
#include "MasterRenderer.h"
#include "StaticShader.h"
#include "ModelTexture.h"
#include "TexturedModel.h"
#include "GameInfo.h"
#include "Camera.h"
#include "ObjLoader.h"

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

	std::vector<glm::vec3> out_vertices;
	std::vector<glm::vec2> out_uvs;
	std::vector<glm::vec3> out_normals;
	std::vector<int> out_indices;
	LoadOBJ("data/stall.obj", out_vertices, out_uvs, out_normals, out_indices);
	RawModel model = loader.LoadToVAO(out_vertices, out_uvs, out_normals, out_indices);

	ModelTexture texture(loader.LoadTexture("data/stallTexture.bmp"));
	TexturedModel texturedModel(model, texture);

	texturedModel.GetTexture().myReflectivity = 1;
	texturedModel.GetTexture().myShineDamper = 10;

	vector<Entity> allEntities;

	for (int i = 0; i < 200; i++)
	{
		float x = rand() % 100 - 50;
		float y = rand() % 100 - 50;
		float z = rand() % 100 - 50;

		float rX = rand() % 180;
		float rY = rand() % 180;
		allEntities.push_back(Entity(texturedModel, glm::vec3(x, y, z), glm::vec3(rX, rY, 0), 1));
	}

	double currentFrame = glfwGetTime();
	double lastFrame = currentFrame;
	
	Camera camera(*window);
	Light light(vec3(0, 0, -20), vec3(1, 1, 1));
	
	MasterRenderer renderer;

	do {
		currentFrame = glfwGetTime();
		GameInfo::ourDeltaTime = (float)(currentFrame - lastFrame);
		lastFrame = currentFrame;

		camera.Move();

		for(Entity& entity : allEntities)
			renderer.ProcessEntity(entity);

		renderer.Render(light, camera);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	loader.CleanUp();
}


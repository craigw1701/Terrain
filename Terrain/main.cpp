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
#include "Renderer.h"
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
	window = glfwCreateWindow(1024, 768, "Terrain Test", NULL, NULL);
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
	StaticShader staticShader;
	staticShader.Setup();
	Renderer renderer(staticShader);

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
	LoadOBJ("data/dragon.obj", out_vertices, out_uvs, out_normals, out_indices);
	RawModel model = loader.LoadToVAO(out_vertices, out_uvs, out_normals, out_indices);

	ModelTexture texture(loader.LoadTexture("data/white.bmp"));
	TexturedModel texturedModel(model, texture);

	texturedModel.GetTexture().myReflectivity = 1;
	texturedModel.GetTexture().myShineDamper = 10;

	Entity entity(texturedModel, glm::vec3(0, 0, -25), glm::vec3(0, -5, 0), 1);

	double currentFrame = glfwGetTime();
	double lastFrame = currentFrame;
	
	Camera camera(*window);
	Light light(vec3(0, 0, -20), vec3(1, 1, 1));
	

	do {
		currentFrame = glfwGetTime();
		GameInfo::ourDeltaTime = (float)(currentFrame - lastFrame);
		lastFrame = currentFrame;

		camera.Move();

		renderer.Prepare();
		staticShader.Start();
		staticShader.LoadLight(light);
		staticShader.LoadViewMatrix(camera);
		entity.myPosition += vec3(0, 0, -0.002f);
		entity.myRotation += vec3(0, 1.0f*GameInfo::ourDeltaTime, 0);
		renderer.Render(entity, staticShader);
		staticShader.Stop();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	loader.CleanUp();
}


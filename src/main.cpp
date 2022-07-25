#include "includes.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

const float SCR_WIDTH = 1280.0f;
const float SCR_HEIGHT = 720.0f;

rnd::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const int ssaaLevel = 8;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PBR Render", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		throw std::runtime_error("failed to create window");
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("failed to inititalize GLAD");

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, cursorCallback);
	glfwSetScrollCallback(window, scrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	uint32_t textures;
	glGenTextures(1, &textures);
	rnd::Texture baseColor(&textures, GL_TEXTURE0, GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_NEAREST);
	baseColor.setupImage("E:/PBR_render/textures/gun/Gun_BaseColor.tga", GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	rnd::TexturesPBR modelTextures(&baseColor);

	std::vector<rnd::ModelWithTextures> models;
	models.push_back(rnd::ModelWithTextures("E:/PBR_render/models/SM_Gun_1.fbx", &modelTextures));
	
	rnd::Shader modelShader("E:/PBR_render/shaders/modelShader/model.vert", "E:/PBR_render/shaders/modelShader/model.frag");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), SCR_WIDTH / SCR_HEIGHT, 0.1f, 1000.0f);

	modelShader.use();
	modelShader.setInt("material.baseColor", 0);
	modelShader.setMat4("model", model);
	modelShader.setMat4("projection", projection);

	float ssaaResolution[2] = { SCR_WIDTH * ssaaLevel, SCR_HEIGHT * ssaaLevel };

	rnd::RenderSSAA ssaaRender(ssaaResolution);
	rnd::OutputQuad outputQuad;

	rnd::Shader shaderSSAA("E:/PBR_render/shaders/SSAA/SSAA.vert", "E:/PBR_render/shaders/SSAA/SSAA.frag");
	shaderSSAA.use();
	shaderSSAA.setVec2f("resolution", ssaaResolution[0], ssaaResolution[1]);
	shaderSSAA.setInt("ssaaLevel", ssaaLevel);
	shaderSSAA.setInt("renderTexture", 0);

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		processInput(window);

		glEnable(GL_CULL_FACE);

		modelShader.use();
		modelShader.setMat4("view", camera.GetViewMatrix());
		ssaaRender.render(models);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		shaderSSAA.use();
		outputQuad.render(ssaaRender.fboColor());
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(rnd::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(rnd::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(rnd::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(rnd::RIGHT, deltaTime);
}

void cursorCallback(GLFWwindow* window, double xposIn, double yposIn)
{	
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

#include "renderer.hpp"

namespace rnd
{
	const float SCR_WIDTH = 1280.0f;
	const float SCR_HEIGHT = 720.0f;

	rnd::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	bool firstMouse = true;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	float dirLightPitch = 30.0f;
	float dirLightYaw = 0.0f;

	float lastXDirLight, lastYDirLight;
	bool firstLight = true;

	glm::vec3 globalDirLightView = glm::vec3(0.0f, 0.0f, 2.0f);
}

Renderer::Renderer(int ssaaLevel)
{
	initWindow();
	
	this->ssaaLevel = ssaaLevel;
	
	ssaaResolution.push_back(rnd::SCR_WIDTH * this->ssaaLevel);
	ssaaResolution.push_back(rnd::SCR_HEIGHT * this->ssaaLevel);

	renderSSAA.init(ssaaResolution);
	outputQuad.init();
	dirLightShadowMap.init({ 2048.0f, 2048.0f });
}

void Renderer::initRenderer()
{
	initShaders();
	loadModels();
	initScene();
}

void Renderer::draw()
{
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), rnd::SCR_WIDTH / rnd::SCR_HEIGHT, 0.1f, 1000.0f);

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		rnd::deltaTime = currentTime - rnd::lastFrame;
		rnd::lastFrame = currentTime;

		processInput(window);

		glEnable(GL_CULL_FACE);

		float nearPlane = 1.0f, farPlane = 100.5f;
		glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, nearPlane, farPlane);
		glm::mat4 lightView = glm::lookAt(rnd::globalDirLightView,
										  glm::vec3(0.0f, 0.0f, 0.0f), 
										  glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		dirLightShadowMap.render(renderables, shaders["shadowPassShader"], lightSpaceMatrix);
		
		renderSSAA.render(
			renderables, 
			rnd::camera, 
			projection,
			lightSpaceMatrix,
			dirLightShadowMap.fboDepth());

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);

		glViewport(0, 0, rnd::SCR_WIDTH, rnd::SCR_HEIGHT);

		shaders["shaderSSAA"].use();
		outputQuad.render(renderSSAA.fboColor());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Renderer::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(rnd::SCR_WIDTH, rnd::SCR_HEIGHT, "PBR Render", nullptr, nullptr);
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
	glfwSetMouseButtonCallback(window, lightCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
}

void Renderer::initShaders()
{
	rnd::Shader modelShader("E:/PBR_render/shaders/modelShader/model.vert", "E:/PBR_render/shaders/modelShader/model.frag");
	modelShader.use();
	modelShader.setInt("material.baseColor", 0);
	modelShader.setInt("material.normal", 1);
	modelShader.setInt("material.arm", 2);
	modelShader.setInt("shadowMap", 3);
	modelShader.setVec3f("lightPos", glm::vec3(30.0f, 60.0f, 60.0f));
	shaders["modelShader"] = modelShader;

	rnd::Shader shaderSSAA("E:/PBR_render/shaders/SSAA/SSAA.vert", "E:/PBR_render/shaders/SSAA/SSAA.frag");
	shaderSSAA.use();
	shaderSSAA.setVec2f("resolution", ssaaResolution[0], ssaaResolution[1]);
	shaderSSAA.setInt("ssaaLevel", ssaaLevel);
	shaderSSAA.setInt("renderTexture", 0);
	shaders["shaderSSAA"] = shaderSSAA;

	rnd::Shader shadowPassShader("E:/PBR_render/shaders/shadowMap/shadowMap.vert", "E:/PBR_render/shaders/shadowMap/shadowMap.frag");
	shaders["shadowPassShader"] = shadowPassShader;

	rnd::Shader debug("E:/PBR_render/shaders/shadowMap/debug.vert", "E:/PBR_render/shaders/shadowMap/debug.frag");
	shaders["debug"] = debug;
}

void Renderer::loadModels()
{
	std::vector<std::string> gunTexturesPath = {
		"./textures/gun/Gun_BaseColor.tga",
		"./textures/gun/Gun_Normal.tga",
		"./textures/gun/Gun_ARM.tga"
	};

	rnd::TexturesPBR gunTextures = loadTextures(gunTexturesPath);

	models.insert({"gun", {"E:/PBR_render/models/SM_Gun_1.fbx", gunTextures}});

	std::vector<std::string> wallTexturesPath = {
		"./textures/wall/Tile_basecolor_light.tga",
		"./textures/wall/Tile_normal.tga",
		"./textures/wall/Tile_ARM.tga"
	};

	rnd::TexturesPBR wallTextures = loadTextures(wallTexturesPath);

	models.insert({"wall", {"E:/PBR_render/models/wall.fbx", wallTextures}});
}

rnd::TexturesPBR Renderer::loadTextures(std::vector<std::string> paths)
{
	uint32_t textures[3];
	glGenTextures(3, textures);

	rnd::TexturesPBR texturesPBR;

	for (int i = 0; i != paths.size(); ++i)
	{
		rnd::Texture texture(textures[i], GL_TEXTURE0 + i, GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_NEAREST);
		texture.setupImage(paths[i], GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
		texturesPBR.addTexture(texture);
	}

	return texturesPBR;
}

void Renderer::initScene()
{
	rnd::RenderableObject gun;
	gun.model = &models["gun"];
	gun.shader = &shaders["modelShader"];
	gun.modelMatrix = glm::mat4(1.0f);

	renderables.push_back(gun);

	rnd::RenderableObject wall;
	wall.model = &models["wall"];
	wall.shader = &shaders["modelShader"];
	wall.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 6.0f));

	renderables.push_back(wall);
}

// callbacks
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processGlobalLightPosition(float xpos, float ypos)
{	
	std::cout << "Light callback start" << std::endl;
	if (rnd::firstLight)
	{
		rnd::lastXDirLight = xpos;
		rnd::lastYDirLight = ypos;
		rnd::firstLight = false;
	}

	std::cout << "xpos " << xpos << std::endl;
	std::cout << "ypos" << ypos << std::endl;

	float xoffset = xpos - rnd::lastYDirLight;
	float yoffset = rnd::lastXDirLight - ypos;
	rnd::lastXDirLight = xpos;
	rnd::lastYDirLight = ypos;

	std::cout << "xoffset " << xoffset << std::endl;
	std::cout << "yoffset " << yoffset << std::endl;

	rnd::dirLightYaw += xoffset;
	rnd::dirLightPitch += yoffset;

	std::cout << "Yaw " << rnd::dirLightYaw << std::endl; 
	std::cout << "Pitch " << rnd::dirLightPitch << std::endl; 

	rnd::globalDirLightView.x = cos(glm::radians(rnd::dirLightYaw)) * cos(glm::radians(rnd::dirLightPitch));
	rnd::globalDirLightView.y = sin(glm::radians(rnd::dirLightPitch));
	rnd::globalDirLightView.z = sin(glm::radians(rnd::dirLightYaw)) * cos(glm::radians(rnd::dirLightPitch));

	std::cout << "x " << rnd::globalDirLightView.x << " y " << rnd::globalDirLightView.y << " z " << rnd::globalDirLightView.z << std::endl;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5f * rnd::deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		rnd::camera.processKeyboard(rnd::FORWARD, rnd::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		rnd::camera.processKeyboard(rnd::BACKWARD, rnd::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		rnd::camera.processKeyboard(rnd::LEFT, rnd::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		rnd::camera.processKeyboard(rnd::RIGHT, rnd::deltaTime);
}

void cursorCallback(GLFWwindow* window, double xposIn, double yposIn)
{	
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (glfwGetKey(window, GLFW_MOD_SHIFT) != GLFW_PRESS)
	{
		if (rnd::firstMouse)
		{
			rnd::lastX = xpos;
			rnd::lastY = ypos;
			rnd::firstMouse = false;
		}

		float xoffset = xpos - rnd::lastX;
		float yoffset = rnd::lastY - ypos;
		rnd::lastX = xpos;
		rnd::lastY = ypos;

		rnd::camera.ProcessMouseMovement(xoffset, yoffset);
	}
	else
	{
		processGlobalLightPosition(xpos, ypos);
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	rnd::camera.ProcessMouseScroll(yoffset);
}


void lightCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && mods == GLFW_MOD_SHIFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		processGlobalLightPosition(xpos, ypos);
	}
}

#pragma once

#include "includes.hpp"

class Renderer
{
	public:
		Renderer() = default;
		Renderer(int ssaaLevel);
		void initRenderer();
		void draw();
		void cleanup();
	private:
		GLFWwindow* window = nullptr;
		
		int ssaaLevel = 1;
		std::vector<float> ssaaResolution;

		rnd::RenderSSAA renderSSAA{};
		rnd::OutputQuad outputQuad{};
		rnd::ShadowMaps dirLightShadowMap{};

		std::unordered_map<std::string, rnd::Shader> shaders;
		std::unordered_map<std::string, rnd::ModelWithTextures> models;
		std::vector<rnd::RenderableObject> renderables;

		void initWindow();
		void initShaders();
		void loadModels();
		void initScene();
		rnd::TexturesPBR loadTextures(std::vector<std::string> paths);
};

// callbacks
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void lightCallback(GLFWwindow* window, int button, int action, int mods);

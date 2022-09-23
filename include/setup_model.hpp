#pragma once

#include <string>

#include "texture.hpp"
#include "import/model.hpp"
#include "shader.hpp"

namespace rnd
{
	struct TexturesPBR
	{
		public:
			TexturesPBR() = default;
			void addTexture(Texture texture);
			void bindTextures();
		private:
			std::vector<rnd::Texture> textures;
	};

	struct ModelWithTextures
	{
		public:
			ModelWithTextures() = default;
			ModelWithTextures(const std::string& modelPath, TexturesPBR textures);
			void draw();
			void drawInShadowPass();
		private:
			rnd::Model model;
			TexturesPBR textures;
	};

	struct RenderableObject
	{
		rnd::ModelWithTextures* model;
		glm::mat4 modelMatrix;
		rnd::Shader* shader;
	};
};
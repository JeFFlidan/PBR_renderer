#pragma once

#include <string>

#include "texture.hpp"
#include "import/model.hpp"

namespace rnd
{
	class TexturesPBR
	{
		public:
			TexturesPBR(rnd::Texture* baseColor);
			void bindTextures();
		private:
			std::vector<rnd::Texture*> textures;
	};

	class ModelWithTextures
	{
		public:
			ModelWithTextures(const std::string& modelPath, TexturesPBR* textures);
			void draw();
		private:
			rnd::Model model;
			TexturesPBR* textures;
	};
};
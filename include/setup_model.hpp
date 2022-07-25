#pragma once

#include <string>

#include "texture.hpp"
#include "import/model.hpp"

struct TexturesPBR
{
	public:
		TexturesPBR(rnd::Texture* baseColor);
		void bindTextures();
	private:
		std::vector<rnd::Texture*> textures;
};

struct Model
{
	public:
		Model(const std::string& modelPath, TexturesPBR* textures);
		void draw();
	private:
		rnd::Model model;
		TexturesPBR* textures;
};
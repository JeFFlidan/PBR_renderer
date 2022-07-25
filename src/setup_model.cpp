#include <GLAD/glad.h>

#include <iostream>
#include <string>

#include "setup_model.hpp"

namespace rnd
{
	TexturesPBR::TexturesPBR(rnd::Texture* baseColor)
	{
		textures.push_back(baseColor);
	}

	void TexturesPBR::bindTextures()
	{
		for (auto texture : textures)
		{
			glActiveTexture(texture->textureSet());
			glBindTexture(texture->textureType(), texture->textureObj());
		}
	}

	ModelWithTextures::ModelWithTextures(const std::string& modelPath, TexturesPBR* textures)
	{
		rnd::Model temp(modelPath);
		model = temp;
		this->textures = textures;
	}

	void ModelWithTextures::draw()
	{
		textures->bindTextures();
		model.Draw();
	}
};
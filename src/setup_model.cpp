#include <GLAD/glad.h>

#include <iostream>
#include <string>

#include "setup_model.hpp"

namespace rnd
{
	void TexturesPBR::addTexture(Texture texture)
	{
		textures.push_back(texture);
	}

	void TexturesPBR::bindTextures()
	{
		for (int i = 0; i != textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(textures[i].textureType(), textures[i].textureObj());
		}
	}

	ModelWithTextures::ModelWithTextures(const std::string& modelPath, TexturesPBR textures)
	{
		rnd::Model temp(modelPath);
		model = temp;
		this->textures = textures;
	}

	void ModelWithTextures::draw()
	{
		textures.bindTextures();
		model.Draw();
	}

	void ModelWithTextures::drawInShadowPass()
	{
		model.Draw();
	}
};

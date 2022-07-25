#include <GLAD/glad.h>

#include "setup_model.hpp"

TexturesPBR::TexturesPBR(rnd::Texture* baseColor)
{
	textures.push_back(baseColor);
}

void TexturesPBR::bindTextures()
{
	for (auto& texture : textures)
	{
		glActiveTexture(texture->textureSet());
		glBindTexture(texture->textureType(), texture->textureObj());
	}
}

Model::Model(const std::string& modelPath, TexturesPBR* textures)
{
	rnd::Model temp(modelPath);
	model = temp;
	this->textures = textures;
}

void Model::draw()
{
	textures->bindTextures();
	model.Draw();
}
#include <string>
#include <stdexcept>

#include <glad/glad.h>

#include <stb_image/stb_image.h>

#include <texture.hpp>

namespace rnd
{
	Texture::Texture(uint32_t* text, GLenum textSet, GLenum txType, GLint sWrap, GLint tWrap, GLint minFilt, GLint magFilt)
	{
		textureObject = text;
		textureSet = textSet;
		textureType = txType;

		glActiveTexture(textureSet);
		glBindTexture(textureType, *textureObject);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_S, sWrap);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_T, tWrap);
		glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, minFilt);
		glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, magFilt);
	}

	void Texture::setupImage(const std::string& imageName, GLint txtColorFormat, GLint newColorFormat, GLint dataType)
	{
		glBindTexture(textureType, *textureObject);

		int width, height, nrChannel;
		unsigned char* data = stbi_load(imageName.c_str(), &width, &height, &nrChannel, 0);
		if (data)
		{
			glTexImage2D(textureType, 0, txtColorFormat, width, height, 0, newColorFormat, dataType, data);
			glGenerateMipmap(textureType);
		}
		else
		{
			throw std::runtime_error("failed to load texture " + imageName);
		}

		stbi_image_free(data);
	}
}

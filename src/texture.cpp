#include <stdexcept>

#include <stb_image/stb_image.h>

#include <texture.hpp>

namespace rnd
{
	Texture::Texture(uint32_t* text, GLenum textSet, GLenum txType, GLint sWrap, GLint tWrap, GLint minFilt, GLint magFilt)
	{
		textObject = text;
		textSet = textSet;
		textType = txType;

		glActiveTexture(textSet);
		glBindTexture(textType, *textObject);
		glTexParameteri(textType, GL_TEXTURE_WRAP_S, sWrap);
		glTexParameteri(textType, GL_TEXTURE_WRAP_T, tWrap);
		glTexParameteri(textType, GL_TEXTURE_MIN_FILTER, minFilt);
		glTexParameteri(textType, GL_TEXTURE_MAG_FILTER, magFilt);
		glBindTexture(textType, 0);
	}

	void Texture::setupImage(const std::string& imageName, GLint txtColorFormat, GLint newColorFormat, GLint dataType)
	{
		glBindTexture(textType, *textObject);

		int width, height, nrChannel;
		unsigned char* data = stbi_load(imageName.c_str(), &width, &height, &nrChannel, 0);
		if (data)
		{
			glTexImage2D(textType, 0, txtColorFormat, width, height, 0, newColorFormat, dataType, data);
			glGenerateMipmap(textType);
		}
		else
		{
			throw std::runtime_error("failed to load texture " + imageName);
		}

		stbi_image_free(data);

		glBindTexture(textType, 0);
	}
}

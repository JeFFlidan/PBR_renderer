#pragma once

#include <glad/glad.h>

#include <string>

namespace rnd
{
	class Texture
	{
		public:
			Texture(uint32_t* text, GLenum textSet, GLenum txType, GLint sWrap, GLint tWrap, GLint minFilt, GLint magFilt);
			Texture(const Texture&) = delete;
			Texture& operator=(const Texture&) = delete;
			~Texture() = default;

			void setupImage(const std::string& imageName, GLint txtColorFormat, GLint newColorFormat, GLint dataType);

			uint32_t obj() const { return *textureObject; };
			GLenum textSet() const { return textureSet; };
			GLenum textType() const { return textureType; };
		private:
			uint32_t* textureObject;
			GLenum textureSet, textureType;
	};
};

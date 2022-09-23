#pragma once

#include <glad/glad.h>

#include <string>

namespace rnd
{
	class Texture
	{
		public:
			Texture() = default;
			Texture(uint32_t text, GLenum textSet, GLenum txType, GLint sWrap, GLint tWrap, GLint minFilt, GLint magFilt);
			~Texture() = default;

			void setupImage(const std::string& imageName, GLint txtColorFormat, GLint newColorFormat, GLint dataType);

			uint32_t textureObj() const { return textObject; };
			GLenum textureSet() const { return textSet; };
			GLenum textureType() const { return textType; };
		private:
			uint32_t textObject;
			GLenum textSet, textType;
	};
};

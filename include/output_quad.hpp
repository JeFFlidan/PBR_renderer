#pragma once

#include <glad/glad.h>
#include "texture.hpp"

namespace rnd
{
	class OutputQuad
	{
		public:
			OutputQuad();
			void render(uint32_t texture);
		private:
			uint32_t VAO, VBO;
			float verticesInfo[30] = {
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				 -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			};
	};
};
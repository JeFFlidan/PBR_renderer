#pragma once

#include <vector>
#include <GLAD/glad.h>

#include "setup_model.hpp"
#include "shader.hpp"

namespace rnd
{
	class RenderSSAA
	{
		public:
			RenderSSAA(float resolution[2]);
			void render(std::vector<ModelWithTextures>& models);
			uint32_t fboColor() { return fboColorTexture; } 
		private:
			float width, height;

			uint32_t ssaaFbo;
			uint32_t fboColorTexture;
			uint32_t fboRboDepthStencil;
	};
};


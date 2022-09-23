#pragma once

#include <iostream>
#include <vector>
#include <GLAD/glad.h>

#include "setup_model.hpp"
#include "shader.hpp"
#include "camera.hpp"

namespace rnd
{
	class RenderSSAA
	{
		public:
			RenderSSAA() = default;
			void init(std::vector<float> resolution);
			void render(
				std::vector<RenderableObject>& renderables, 
				rnd::Camera& camera,
				glm::mat4 projection,
				glm::mat4 lightSpaceMatrix, 
				Texture shadowMap);
			uint32_t fboColor() { return fboColorTexture; }
		private:
			float width, height;

			uint32_t ssaaFbo;
			uint32_t fboColorTexture;
			uint32_t fboRboDepthStencil;
	};
};

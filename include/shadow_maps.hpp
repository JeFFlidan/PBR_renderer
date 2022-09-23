#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.hpp"
#include "setup_model.hpp"

namespace rnd
{
	class ShadowMaps
	{
		public:
			ShadowMaps() = default;
			void init(std::vector<float> shadowResolution);
			void render(std::vector<RenderableObject> renderables, Shader shadowPassShader, glm::mat4 lightSpaceMatrix);
			Texture fboDepth() { return fboDepthTexture; }
		private:
			float width, height;

			uint32_t shadowPassFbo;
			Texture fboDepthTexture;
	};
}
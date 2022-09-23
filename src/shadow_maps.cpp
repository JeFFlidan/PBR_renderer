#include <stdexcept>
#include <iostream>

#include "shadow_maps.hpp"

namespace rnd
{
	void ShadowMaps::init(std::vector<float> shadowResolution)
	{
		width = shadowResolution[0];
		height = shadowResolution[1];

		uint32_t texture;
		glGenTextures(1, &texture);
		Texture temp(texture, GL_TEXTURE1, GL_TEXTURE_2D, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NEAREST, GL_NEAREST);
		fboDepthTexture = temp;
		glBindTexture(GL_TEXTURE_2D, fboDepthTexture.textureObj());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glGenFramebuffers(1, &shadowPassFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowPassFbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fboDepthTexture.textureObj(), 0);
		glDrawBuffer(0);
		glReadBuffer(0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw std::runtime_error("failed to create SSAA framebuffer");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ShadowMaps::render(std::vector<RenderableObject> renderables, Shader shadowPassShader, glm::mat4 lightSpaceMatrix)
	{
		shadowPassShader.use();
		shadowPassShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, width, height);
		glCullFace(GL_FRONT);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowPassFbo);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		for (auto& object : renderables)
		{
			shadowPassShader.setMat4("model", object.modelMatrix);
			object.model->drawInShadowPass();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);
	}
}
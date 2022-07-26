#include <stdexcept>
#include <iostream>

#include "renderSSAA.hpp"
#include "texture.hpp"

namespace rnd
{
	void RenderSSAA::init(std::vector<float> resolution)
	{
		this->width = resolution[0];
		this->height = resolution[1];

		glGenFramebuffers(1, &ssaaFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, ssaaFbo);

		glGenTextures(1, &fboColorTexture);
		Texture colorTexture(fboColorTexture, GL_TEXTURE0, GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, colorTexture.textureObj());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture.textureObj(), 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenRenderbuffers(1, &fboRboDepthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, fboRboDepthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fboRboDepthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw std::runtime_error("failed to create SSAA framebuffer");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderSSAA::render(
		std::vector<RenderableObject>& renderables, 
		rnd::Camera& camera, 
		glm::mat4 projection, 
		glm::mat4 lightSpaceMatrix,
		Texture shadowMap)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, ssaaFbo);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, width, height);
		
		rnd::Shader* tempShader = nullptr;

		for (auto& object : renderables)
		{
			if (tempShader != object.shader)
			{
				tempShader = object.shader;
				tempShader->use();
			}

			tempShader->setMat4("model", object.modelMatrix);
			tempShader->setMat4("projection", projection);
			tempShader->setMat4("view", camera.GetViewMatrix());
			tempShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
			tempShader->setVec3f("viewPos", camera.GetPosition());

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, shadowMap.textureObj());

			object.model->draw();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

namespace rnd
{
	class Shader
	{
		public:
			// the program ID
			uint32_t ID;

			// constructor reads and builds the shader
			Shader(const char* vertexPath, const char* fragmentPath);

			Shader(const Shader&) = delete;
			Shader& operator=(const Shader&) = delete;

			//use/activate the shader
			void use();

			//utility uniform functions
			void setBool(const std::string& name, bool value);

			void setInt(const std::string& name, int value);

			void setFloat(const std::string& name, float value);

			void setVec2f(const std::string& name, float val1, float val2);

			void setVec2f(const std::string& name, const glm::vec2& data);

			void setVec3f(const std::string& name, float val1, float val2, float val3);

			void setVec3f(const std::string& name, const glm::vec3& data);

			void setMat4(const std::string& name, const glm::mat4& data);

		private:
			std::string getShaderCode(const char*);
			void setupShader(GLenum, uint32_t&, const char*);
	};
};
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

namespace rnd
{
	// Public methods
	Shader::Shader(const char* vertexPath, const char* fragmentPath)
	{
		std::string vShaderCode = getShaderCode(vertexPath);
		std::string fShaderCode = getShaderCode(fragmentPath);

		uint32_t vertex = 0, fragment = 0;
		setupShader(GL_VERTEX_SHADER, vertex, vShaderCode.c_str());
		setupShader(GL_FRAGMENT_SHADER, fragment, fShaderCode.c_str());

		ID = glCreateProgram();
		glAttachShader(ID, vertex);

		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		int success;
		char infoLog[512];

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if(!success)
		{
			glGetProgramInfoLog(ID, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::use()
	{
		glUseProgram(ID);
	}

	//utility uniform functions
	void Shader::setBool(const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void Shader::setInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void Shader::setFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void Shader::setVec2f(const std::string& name, float val1, float val2)
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), val1, val2);
	}

	void Shader::setVec2f(const std::string& name, const glm::vec2& data)
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &data[0]);
	}

	void Shader::setVec3f(const std::string& name, float val1, float val2, float val3)
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), val1, val2, val3);
	}

	void Shader::setVec3f(const std::string& name, const glm::vec3& data)
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &data[0]);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& data)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(data));
	}
	
	// Private methods
	std::string Shader::getShaderCode(const char* filePath)
	{
		std::string code;

		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			shaderFile.open(filePath);
			std::stringstream shaderStream;

			shaderStream << shaderFile.rdbuf();

			shaderFile.close();

			code = shaderStream.str();
		}
		catch(const std::exception& e)
		{
			throw std::runtime_error("failed to read shader code " + std::string(filePath));
		}
		
		return code;
	}

	void Shader::setupShader(GLenum shaderType, uint32_t& shaderID, const char* shaderCode)
	{
		int success;
		char infoLog[512];

		shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, &shaderCode, nullptr);
		glCompileShader(shaderID);

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
			throw std::runtime_error("failed to setup shader:\n" + std::string(infoLog) + "\n");
		}
	}
};

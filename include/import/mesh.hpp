#pragma once

#include <vector>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>

namespace rnd
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
	};

	class Mesh
	{
		public:
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			
			Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

			void Draw();
			unsigned int VAO, VBO, EBO;
		private:
			void setupMesh();
	};
};

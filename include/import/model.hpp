#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <assimp/scene.h>

#include "shader.hpp"
#include "import/mesh.hpp"

namespace rnd
{
	unsigned int TextureFromFile(const char* path, const std::string& directory);

	class Model
	{
		public:
			Model(const std::string& path)
			{
				loadModel(path);
			}
			void Draw(Shader& shader);

			std::vector<Mesh> meshes;
		private:
			std::string directory;

			void loadModel(std::string path);
			void processNode(aiNode* node, const aiScene* scene);
			Mesh processMesh(aiMesh* mesh, const aiScene* scene);
			void readVertices(std::vector<Vertex>& vertices, aiMesh* mesh);
			void readIndices(std::vector<unsigned int>& indices, aiMesh* mesh);
	};
};

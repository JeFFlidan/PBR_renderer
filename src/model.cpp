#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <mutex>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.hpp"
#include "import/mesh.hpp"
#include "import/model.hpp"

namespace rnd
{
	void Model::Draw()
	{
		for (unsigned int i = 0; i != meshes.size(); ++i)
			meshes[i].Draw();
	}

	void Model::loadModel(std::string path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);		// Read file and add it in the Assimp scene node. The second parametr is some post-processing fichers

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}

		directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];		// Get a mesh from the scene
			meshes.push_back(processMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene);		// Pass in processNode new node which may have some meshes. This is recursive function
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		std::thread readVerticesThread(&Model::readVertices, this, std::ref(vertices), mesh);
		std::thread readIndicesThread(&Model::readIndices, this, std::ref(indices), mesh);

		readVerticesThread.join();
		readIndicesThread.join();

		return Mesh(vertices, indices);
	}

	void Model::readVertices(std::vector<Vertex>& vertices, aiMesh* mesh)
	{
		for (unsigned int i = 0; i != mesh->mNumVertices; ++i)
		{
			Vertex vertex;
			glm::vec3 vector;
			// Get vertex position
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			// Get normal of vertex
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
			}

			// Get texture coords. Now I need to get only the first set of texture coords, but assimp has possibility for defining 8 texture coords sets for one vertex
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}
	}

	void Model::readIndices(std::vector<unsigned int>& indices, aiMesh* mesh)
	{
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}
	}
}
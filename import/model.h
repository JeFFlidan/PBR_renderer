#ifndef MODEL_H
#define MODEL_H

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
#include <stb_image/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shaderclass/shaderclass.h"
#include "import/mesh.h"
#include "textimport/textimport.h"

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
		std::vector<TextureData> textures_loaded;

		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		void readVertices(std::vector<Vertex>& vertices, aiMesh* mesh);
		void readIndices(std::vector<unsigned int>& indices, aiMesh* mesh);
		std::vector<TextureData> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i != meshes.size(); ++i)
		meshes[i].Draw(shader);
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
	std::vector<TextureData> textures;

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];		// Get a pointer on a material in the scene. Mesh has this material. This material has names of the textures in the folder(?)
	std::thread readVerticesThread(&Model::readVertices, this, std::ref(vertices), mesh);
	std::thread readIndicesThread(&Model::readIndices, this, std::ref(indices), mesh);

	std::vector<TextureData> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<TextureData> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	readVerticesThread.join();
	readIndicesThread.join();

	return Mesh(vertices, indices, textures);
}

std::vector<TextureData> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<TextureData> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); ++j)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			TextureData texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);		// Add to loaded textures
		}
	}
	return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = path;
	filename = directory + '/' + filename;
	std::cout << filename << std::endl;

	unsigned int textureID;
	glGenTextures(1, &textureID);
	
	Texture texture(&textureID, GL_TEXTURE0, GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	texture.setupImage(filename, false, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);

	return textureID;
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

#endif
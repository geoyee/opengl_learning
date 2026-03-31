#include "model.h"

#include <iostream>

Model::Model(const std::string& path)
{
    std::cout << "Loading model from: " << path << std::endl;
    loadModel(path);
    std::cout << "Model loaded, meshes count: " << m_meshes.size() << std::endl;
}

void Model::draw(Renderer& renderer, Shader& shader)
{
    size_t mesheSize = m_meshes.size();
    for (unsigned int i = 0; i < mesheSize; ++i)
    {
        m_meshes[i]->draw(renderer, shader);
    }
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    size_t lastSlash = path.find_last_of("/\\");
    m_directory = path.substr(0, lastSlash);
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh *Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::pair<std::string, std::string>> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;
        glm::vec3 vector;
        // Position
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // Normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        // Texture Coordinates
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        // Diffuse maps
        auto diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "u_TextureDiffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // Specular maps
        auto specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "u_TextureSpecular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    // Return a mesh object created from the extracted mesh data
    return new Mesh(vertices, indices, textures);
}

std::vector<std::pair<std::string, std::string>> Model::loadMaterialTextures(aiMaterial *mat,
                                                                             aiTextureType type,
                                                                             std::string typeName)
{
    std::vector<std::pair<std::string, std::string>> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string texturePath = m_directory + "/" + std::string(str.C_Str());
        std::cout << "Loading texture: " << texturePath << std::endl;
        textures.push_back(std::make_pair(typeName + "_" + std::to_string(i), texturePath));
    }
    return textures;
}
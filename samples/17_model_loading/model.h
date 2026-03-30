#pragma once

#ifndef __MODEL_H__
#define __MODEL_H__

#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model(const std::string& path);
    void draw(Renderer& renderer, Shader& shader);

private:
    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<std::pair<std::string, std::string>> loadMaterialTextures(aiMaterial *mat,
                                                                          aiTextureType type,
                                                                          std::string typeName);

    std::vector<Mesh> m_meshes;
    std::string m_directory;
};

#endif // !__MODEL_H__
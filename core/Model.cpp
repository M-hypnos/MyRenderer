#include "Model.h"
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <iostream>
#include "TextureCache.h"
#include "Pipeline.h"
using namespace std;

Model::Model(string path) {
    loadModel(path);
}

Model::~Model() {

}

void Model::loadModel(string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenNormals);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        //·µ»Ø´íÎó±¨¸æ
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    directory_ = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes_.push_back(this->processMesh(mesh, scene));
    }
    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    vector<Vertex> vertices;
    vector<size_t> indices;
    vector<int> textureIdx;
    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        Vec3f vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.pos = vector;
        
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
 
        if (mesh->mTextureCoords[0]) {
            Vec2f vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texcoords = vec;
        }
        else {
            vertex.texcoords = Vec2f(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(*(face.mIndices + j));
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        vector<int> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textureIdx.insert(textureIdx.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<int> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textureIdx.insert(textureIdx.end(), specularMaps.begin(), specularMaps.end());
        //3. normal maps
        /*vector<int> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textureIdx.insert(textureIdx.end(), normalMaps.begin(), normalMaps.end());*/
        //// 4. height maps
        //vector<int> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        //textureIdx.insert(textureIdx.end(), heightMaps.begin(), heightMaps.end());
    }

    return Mesh(vertices, indices, textureIdx);
}

vector<int> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
    vector<int> textureIndex;
    for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        string path = directory_ + '/' + str.C_Str();

        int texIdx = TextureCache::getInstance().isTextureLoaded(path);
        if (texIdx < 0) {
            Texture texture;
            if (texture.loadFromImage(path, typeName)) {
                texIdx = TextureCache::getInstance().pushToCache(texture);
            }
        }
        textureIndex.push_back(texIdx);
    }
    return textureIndex;
}

void Model::onRender() {
    for (auto mesh : meshes_) {
        PipeLine::getInstance().drawMesh(mesh);
    }
}
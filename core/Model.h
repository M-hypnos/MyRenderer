#ifndef __MODEL_H__
#define __MODEL_H__

#include "assimp/scene.h"
#include "Mesh.h"
#include "Texture.h"
#include <vector>

class Model {
public:
	Model(std::string path);
	~Model();
	void onRender();
	
private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<int> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	std::string directory_;
	std::vector<Mesh> meshes_;
};

#endif
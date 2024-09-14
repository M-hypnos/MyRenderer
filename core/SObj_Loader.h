#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "STexturePool.h"
#include "../utils/Math.h"

#define OBJL_CONSOLE_OUTPUT

struct Vertex {
	Vec3f pos;
	Vec3f normal;
	Vec2f texcoords;
};

struct Material
{
	Material()
	{
		name;
		Ns = 0.0f;
		Ni = 0.0f;
		d = 0.0f;
		illum = 0;
	}

	// Material Name
	std::string name;
	// Ambient Color
	Vec3f Ka;
	// Diffuse Color
	Vec3f Kd;
	// Specular Color
	Vec3f Ks;
	// Specular Exponent
	float Ns;
	// Optical Density
	float Ni;
	// Dissolve
	float d;
	// Illumination
	int illum;
	// Ambient Texture Map
	std::string map_Ka;
	// Diffuse Texture Map
	std::string map_Kd;
	// Specular Texture Map
	std::string map_Ks;
	// Specular Hightlight Map
	std::string map_Ns;
	// Alpha Texture Map
	std::string map_d;
	// Bump Map
	std::string map_bump;
	int Ka_texIdx;
	int Kd_texIdx;
	int Ks_texIdx;
	int Ns_texIdx;
	int d_texIdx;
	int bump_texIdx;
};

struct Mesh
{
	// Default Constructor
	Mesh()
	{

	}
	// Variable Set Constructor
	Mesh(std::vector<Vertex>& _Vertices, std::vector<unsigned int>& _Indices)
	{
		Vertices = _Vertices;
		Indices = _Indices;
	}
	// Mesh Name
	std::string MeshName;
	// Vertex List
	std::vector<Vertex> Vertices;
	// Index List
	std::vector<unsigned int> Indices;

	// Material
	Material MeshMaterial;
};

template <class T>
inline const T& getElement(const std::vector<T>& elements, std::string& index)
{
	int idx = std::stoi(index);
	if (idx < 0)
		idx = int(elements.size()) + idx;
	else
		idx--;
	return elements[idx];
}

inline void split(const std::string& in,
	std::vector<std::string>& out,
	std::string token)
{
	out.clear();

	std::string temp;

	for (int i = 0; i < int(in.size()); i++)
	{
		std::string test = in.substr(i, token.size());

		if (test == token)
		{
			if (!temp.empty())
			{
				out.push_back(temp);
				temp.clear();
				i += (int)token.size() - 1;
			}
			else
			{
				out.push_back("");
			}
		}
		else if (i + token.size() >= in.size())
		{
			temp += in.substr(i, token.size());
			out.push_back(temp);
			break;
		}
		else
		{
			temp += in[i];
		}
	}
}

//o Legs
//v 1.899165 2.317634 - 0.120600
//找到非空缩进字符之间的字符之后，第一个非空缩进字符，从末尾往前找到第一个非空缩进字符。 即legs

inline std::string tail(const std::string& in)
{
	size_t token_start = in.find_first_not_of(" \t");
	size_t space_start = in.find_first_of(" \t", token_start);
	size_t tail_start = in.find_first_not_of(" \t", space_start);
	size_t tail_end = in.find_last_not_of(" \t");
	if (tail_start != std::string::npos && tail_end != std::string::npos)
	{
		return in.substr(tail_start, tail_end - tail_start + 1);
	}
	else if (tail_start != std::string::npos)
	{
		return in.substr(tail_start);
	}
	return "";
}

//o Legs
//v 1.899165 2.317634 - 0.120600
//找到第一个非空缩进字符，截取到之后的第一个空缩进，即o
inline std::string firstToken(const std::string& in)
{
	if (!in.empty())
	{
		size_t token_start = in.find_first_not_of(" \t");
		size_t token_end = in.find_first_of(" \t", token_start);
		if (token_start != std::string::npos && token_end != std::string::npos)
		{
			return in.substr(token_start, token_end - token_start);
		}
		else if (token_start != std::string::npos)
		{
			return in.substr(token_start);
		}
	}
	return "";
}

class SObj_Loader {
public:
	SObj_Loader() = default;
	~SObj_Loader() {
		LoadedMeshes.clear();
	}

	bool loadFile(std::string path) {
		if (path.substr(path.size() - 4, 4) != ".obj")
			return false;
		std::ifstream file(path);

		if (!file.is_open())
			return false;

		LoadedMeshes.clear();
		LoadedVertices.clear();
		LoadedIndices.clear();

		std::vector<Vec3f> Positions;
		std::vector<Vec2f> TCoords;
		std::vector<Vec3f> Normals;

		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;

		std::vector<std::string> MeshMatNames;

		bool listening = false;
		std::string meshname;

		Mesh tempMesh;

#ifdef OBJL_CONSOLE_OUTPUT
		const unsigned int outputEveryNth = 1000;
		unsigned int outputIndicator = outputEveryNth;
#endif

		std::string curline;
		while (std::getline(file, curline))
		{
#ifdef OBJL_CONSOLE_OUTPUT
			if ((outputIndicator = ((outputIndicator + 1) % outputEveryNth)) == 1)
			{
				if (!meshname.empty())
				{
					std::cout
						<< "\r- " << meshname
						<< "\t| vertices > " << Positions.size()
						<< "\t| texcoords > " << TCoords.size()
						<< "\t| normals > " << Normals.size()
						<< "\t| triangles > " << (Vertices.size() / 3)
						<< (!MeshMatNames.empty() ? "\t| material: " + MeshMatNames.back() : "");
				}
			}
#endif

			if (firstToken(curline) == "o" || firstToken(curline) == "g" || curline[0] == 'g') {
				if (!listening) {
					listening = true;

					if (firstToken(curline) == "o" || firstToken(curline) == "g")
					{
						meshname = tail(curline);
					}
					else
					{
						meshname = "unnamed";
					}
				}
				else {
					if (!Indices.empty() && !Vertices.empty())
					{
						// Create Mesh
						tempMesh = Mesh(Vertices, Indices);
						tempMesh.MeshName = meshname;

						// Insert Mesh
						LoadedMeshes.push_back(tempMesh);

						// Cleanup
						Vertices.clear();
						Indices.clear();
						meshname.clear();

						meshname = tail(curline);
					}
					else
					{
						if (firstToken(curline) == "o" || firstToken(curline) == "g")
						{
							meshname = tail(curline);
						}
						else
						{
							meshname = "unnamed";
						}
					}
				}
#ifdef OBJL_CONSOLE_OUTPUT
				std::cout << std::endl;
				outputIndicator = 0;
#endif
			}

			if (firstToken(curline) == "v") {
				std::vector<std::string> spos;
				Vec3f vpos;
				split(tail(curline), spos, " ");
				vpos.x = std::stof(spos[0]);
				vpos.y = std::stof(spos[1]);
				vpos.z = std::stof(spos[2]);

				Positions.push_back(vpos);
			}
			if (firstToken(curline) == "vt")
			{
				std::vector<std::string> stex;
				Vec2f vtex;
				split(tail(curline), stex, " ");

				vtex.x = std::stof(stex[0]);
				vtex.y = std::stof(stex[1]);

				TCoords.push_back(vtex);
			}
			if (firstToken(curline) == "vn")
			{
				std::vector<std::string> snor;
				Vec3f vnor;
				split(tail(curline), snor, " ");

				vnor.x = std::stof(snor[0]);
				vnor.y = std::stof(snor[1]);
				vnor.z = std::stof(snor[2]);

				Normals.push_back(vnor);
			}
			if (firstToken(curline) == "f") {
				std::vector<Vertex> vVerts;
				GenVerticesFromRawOBJ(vVerts, Positions, TCoords, Normals, curline);

				for (int i = 0; i < int(vVerts.size()); i++)
				{
					Vertices.push_back(vVerts[i]);

					LoadedVertices.push_back(vVerts[i]);
				}

				std::vector<unsigned int> iIndices;

				VertexTriangluation(iIndices, vVerts);

				// Add Indices
				for (int i = 0; i < int(iIndices.size()); i++)
				{
					unsigned int indnum = (unsigned int)((Vertices.size()) - vVerts.size()) + iIndices[i];
					Indices.push_back(indnum);

					indnum = (unsigned int)((LoadedVertices.size()) - vVerts.size()) + iIndices[i];
					LoadedIndices.push_back(indnum);

				}
			}

			if (firstToken(curline) == "usemtl") {
				MeshMatNames.push_back(tail(curline));
				if (!Indices.empty() && !Vertices.empty()) {
					// Create Mesh
					tempMesh = Mesh(Vertices, Indices);
					tempMesh.MeshName = meshname;
					int i = 2;
					while (1) {
						tempMesh.MeshName = meshname + "_" + std::to_string(i);

						for (auto& m : LoadedMeshes)
							if (m.MeshName == tempMesh.MeshName)
								continue;
						break;
					}
					// Insert Mesh
					LoadedMeshes.push_back(tempMesh);

					// Cleanup
					Vertices.clear();
					Indices.clear();
				}

#ifdef OBJL_CONSOLE_OUTPUT
				outputIndicator = 0;
#endif
			}

			// Load Materials
			if (firstToken(curline) == "mtllib")
			{
				// Generate LoadedMaterial

				// Generate a path to the material file
				std::vector<std::string> temp;
				split(path, temp, "/");

				std::string pathtomat = "";

				if (temp.size() != 1)
				{
					for (int i = 0; i < temp.size() - 1; i++)
					{
						pathtomat += temp[i] + "/";
					}
				}


				pathtomat += tail(curline);

#ifdef OBJL_CONSOLE_OUTPUT
				std::cout << std::endl << "- find materials in: " << pathtomat << std::endl;
#endif

				// Load Materials
				LoadMaterials(pathtomat);
			}
		}
#ifdef OBJL_CONSOLE_OUTPUT
		std::cout << std::endl;
#endif

		// Deal with last mesh

		if (!Indices.empty() && !Vertices.empty())
		{
			// Create Mesh
			tempMesh = Mesh(Vertices, Indices);
			tempMesh.MeshName = meshname;

			// Insert Mesh
			LoadedMeshes.push_back(tempMesh);
		}

		file.close();

		// Set Materials for each Mesh
		for (int i = 0; i < MeshMatNames.size(); i++)
		{
			std::string matname = MeshMatNames[i];

			// Find corresponding material name in loaded materials
			// when found copy material variables into mesh material
			for (int j = 0; j < LoadedMaterials.size(); j++)
			{
				if (LoadedMaterials[j].name == matname)
				{
					LoadedMeshes[i].MeshMaterial = LoadedMaterials[j];
					break;
				}
			}
		}

		if (LoadedMeshes.empty() && LoadedVertices.empty() && LoadedIndices.empty())
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	std::vector<Mesh> LoadedMeshes;
	std::vector<Vertex> LoadedVertices;
	std::vector<unsigned int> LoadedIndices;
	std::vector<Material> LoadedMaterials;
private:
	void GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts,
		const std::vector<Vec3f>& iPositions,
		const std::vector<Vec2f>& iTCoords,
		const std::vector<Vec3f>& iNormals,
		std::string icurline) 
	{
		std::vector<std::string> sface, svert;
		Vertex vVert;
		split(tail(icurline), sface, " ");

		bool noNormal = false;
		for (int i = 0; i < int(sface.size()); i++) {
			split(sface[i], svert, "/");
			if (svert.size() == 1)
			{
				// Only position
				vVert.pos = getElement(iPositions, svert[0]);
				vVert.texcoords = Vec2f(0, 0);
				noNormal = true;
				oVerts.push_back(vVert);
			}
			else if (svert.size() == 2) {
				// Position & Texture
				vVert.pos = getElement(iPositions, svert[0]);
				vVert.texcoords = getElement(iTCoords, svert[1]);
				if (vVert.texcoords.x > 1)
					vVert.texcoords.x = vVert.texcoords.x - floor(vVert.texcoords.x);
				if (vVert.texcoords.y > 1)
					vVert.texcoords.y = vVert.texcoords.y - floor(vVert.texcoords.y);
				noNormal = true;
				oVerts.push_back(vVert);
			}
			else if (svert.size() == 3) {
				// Check for Position, Texture and Normal - v1/vt1/vn1
				// or if Position and Normal - v1//vn1
				if (svert[1] != "")
				{
					// Position, Texture, and Normal
					vVert.pos = getElement(iPositions, svert[0]);
					vVert.texcoords = getElement(iTCoords, svert[1]);
					if (vVert.texcoords.x > 1)
						vVert.texcoords.x = vVert.texcoords.x - floor(vVert.texcoords.x);
					if (vVert.texcoords.y > 1)
						vVert.texcoords.y = vVert.texcoords.y - floor(vVert.texcoords.y);
					vVert.normal = getElement(iNormals, svert[2]);
					oVerts.push_back(vVert);
				}
				else
				{
					// Position & Normal
					vVert.pos = getElement(iPositions, svert[0]);
					vVert.texcoords = Vec2f(0, 0);
					vVert.normal = getElement(iNormals, svert[2]);
					oVerts.push_back(vVert);
				}
			}
		}

		if (noNormal)
		{
			Vec3f A = oVerts[0].pos - oVerts[1].pos;
			Vec3f B = oVerts[2].pos - oVerts[1].pos;

			Vec3f normal = cross(A, B);

			for (int i = 0; i < int(oVerts.size()); i++)
			{
				oVerts[i].normal = normal;
			}
		}
	}

	void VertexTriangluation(std::vector<unsigned int>& oIndices,
		const std::vector<Vertex>& iVerts)
	{
		if (iVerts.size() < 3) {
			return;
		}
		if (iVerts.size() == 3)
		{
			oIndices.push_back(0);
			oIndices.push_back(1);
			oIndices.push_back(2);
			return;
		}
		std::vector<Vertex> tVerts = iVerts;
		while (true)
		{
			for (int i = 0; i < int(tVerts.size()); i++) {
				Vertex pPrev;
				if (i == 0) {
					pPrev = tVerts[tVerts.size() - 1];
				} else {
					pPrev = tVerts[i - 1];
				}

				Vertex pCur = tVerts[i];

				Vertex pNext;
				if (i == tVerts.size() - 1) {
					pNext = tVerts[0];
				} else {
					pNext = tVerts[i + 1];
				}

				// Check to see if there are only 3 verts left
				// if so this is the last triangle
				if (tVerts.size() == 3) {
					for (int j = 0; j < int(tVerts.size()); j++) {
						if (iVerts[j].pos == pCur.pos)
							oIndices.push_back(j);
						if (iVerts[j].pos == pPrev.pos)
							oIndices.push_back(j);
						if (iVerts[j].pos == pNext.pos)
							oIndices.push_back(j);
					}
					tVerts.clear();
					break;
				}
				if (tVerts.size() == 4) {
					// Create a triangle from pCur, pPrev, pNext
					for (int j = 0; j < int(iVerts.size()); j++)
					{
						if (iVerts[j].pos == pCur.pos)
							oIndices.push_back(j);
						if (iVerts[j].pos == pPrev.pos)
							oIndices.push_back(j);
						if (iVerts[j].pos == pNext.pos)
							oIndices.push_back(j);
					}

					Vec3f tempVec;
					for (int j = 0; j < int(tVerts.size()); j++)
					{
						if (tVerts[j].pos != pCur.pos
							&& tVerts[j].pos != pPrev.pos
							&& tVerts[j].pos != pNext.pos)
						{
							tempVec = tVerts[j].pos;
							break;
						}
					}

					// Create a triangle from pCur, pPrev, pNext
					for (int j = 0; j < int(iVerts.size()); j++)
					{
						if (iVerts[j].pos == pPrev.pos)
							oIndices.push_back(j);
						if (iVerts[j].pos == pNext.pos)
							oIndices.push_back(j);
						if (iVerts[j].pos == tempVec)
							oIndices.push_back(j);
					}

					tVerts.clear();
					break;
				}

				// If Vertex is not an interior vertex
				float angle = AngleBetweenV3(pPrev.pos - pCur.pos, pNext.pos - pCur.pos) * (180 / 3.14159265359);
				if (angle <= 0 && angle >= 180)
					continue;

				// If any vertices are within this triangle
				bool inTri = false;
				for (int j = 0; j < int(iVerts.size()); j++) {
					if (inTriangle(iVerts[j].pos, pPrev.pos, pCur.pos, pNext.pos)
						&& iVerts[j].pos != pPrev.pos
						&& iVerts[j].pos != pCur.pos
						&& iVerts[j].pos != pNext.pos)
					{
						inTri = true;
						break;
					}
				}
				if (inTri)
					continue;
				// Create a triangle from pCur, pPrev, pNext
				for (int j = 0; j < int(iVerts.size()); j++)
				{
					if (iVerts[j].pos == pCur.pos)
						oIndices.push_back(j);
					if (iVerts[j].pos == pPrev.pos)
						oIndices.push_back(j);
					if (iVerts[j].pos == pNext.pos)
						oIndices.push_back(j);
				}

				// Delete pCur from the list
				for (int j = 0; j < int(tVerts.size()); j++)
				{
					if (tVerts[j].pos == pCur.pos)
					{
						tVerts.erase(tVerts.begin() + j);
						break;
					}
				}

				// reset i to the start
					// -1 since loop will add 1 to it
				i = -1;
			}

			// if no triangles were created
			if (oIndices.size() == 0)
				break;

			// if no more vertices
			if (tVerts.size() == 0)
				break;
		}
	}

	// Load Materials from .mtl file
	bool LoadMaterials(std::string path) {
		// If the file is not a material file return false
		if (path.substr(path.size() - 4, path.size()) != ".mtl")
			return false;
		std::ifstream file(path);
		// If the file is not found return false
		if (!file.is_open())
			return false;

		Material tempMaterial;
		
		std::string filepath = path.substr(0, path.find_last_of('/')+1);

		bool listening = false;

		// Go through each line looking for material variables
		std::string curline;

		while (std::getline(file, curline)) {
			if (firstToken(curline) == "newmtl") {
				if (!listening)
				{
					listening = true;

					if (curline.size() > 7)
					{
						tempMaterial.name = tail(curline);
					}
					else
					{
						tempMaterial.name = "none";
					}
				}
				else {
					// Generate the material

						// Push Back loaded Material
					LoadedMaterials.push_back(tempMaterial);

					// Clear Loaded Material
					tempMaterial = Material();

					if (curline.size() > 7)
					{
						tempMaterial.name = tail(curline);
					}
					else
					{
						tempMaterial.name = "none";
					}
				}
			}
			// Ambient Color
			if (firstToken(curline) == "Ka")
			{
				std::vector<std::string> temp;
				split(tail(curline), temp, " ");

				if (temp.size() != 3)
					continue;

				tempMaterial.Ka.x = std::stof(temp[0]);
				tempMaterial.Ka.y = std::stof(temp[1]);
				tempMaterial.Ka.z = std::stof(temp[2]);
			}
			// Diffuse Color
			if (firstToken(curline) == "Kd")
			{
				std::vector<std::string> temp;
				split(tail(curline), temp, " ");

				if (temp.size() != 3)
					continue;

				tempMaterial.Kd.x = std::stof(temp[0]);
				tempMaterial.Kd.y = std::stof(temp[1]);
				tempMaterial.Kd.z = std::stof(temp[2]);
			}
			// Specular Color
			if (firstToken(curline) == "Ks")
			{
				std::vector<std::string> temp;
				split(tail(curline), temp, " ");

				if (temp.size() != 3)
					continue;

				tempMaterial.Ks.x = std::stof(temp[0]);
				tempMaterial.Ks.y = std::stof(temp[1]);
				tempMaterial.Ks.z = std::stof(temp[2]);
			}
			// Specular Exponent
			if (firstToken(curline) == "Ns")
			{
				tempMaterial.Ns = std::stof(tail(curline));
			}
			// Optical Density
			if (firstToken(curline) == "Ni")
			{
				tempMaterial.Ni = std::stof(tail(curline));
			}
			// Dissolve
			if (firstToken(curline) == "d")
			{
				tempMaterial.d = std::stof(tail(curline));
			}
			// Illumination
			if (firstToken(curline) == "illum")
			{
				tempMaterial.illum = std::stoi(tail(curline));
			}
			// Ambient Texture Map
			if (firstToken(curline) == "map_Ka")
			{
				tempMaterial.map_Ka = filepath + tail(curline);
				tempMaterial.Ka_texIdx = loadTexture(tempMaterial.map_Ka);
			}
			// Diffuse Texture Map
			if (firstToken(curline) == "map_Kd")
			{
				tempMaterial.map_Kd = filepath + tail(curline);
				tempMaterial.Kd_texIdx = loadTexture(tempMaterial.map_Kd);
			}
			// Specular Texture Map
			if (firstToken(curline) == "map_Ks")
			{
				tempMaterial.map_Ks = filepath + tail(curline);
				tempMaterial.Ks_texIdx = loadTexture(tempMaterial.map_Ks);
			}
			// Specular Hightlight Map
			if (firstToken(curline) == "map_Ns")
			{
				tempMaterial.map_Ns = filepath + tail(curline);
				tempMaterial.Ns_texIdx = loadTexture(tempMaterial.map_Ns);
			}
			// Alpha Texture Map
			if (firstToken(curline) == "map_d")
			{
				tempMaterial.map_d = filepath + tail(curline);
				tempMaterial.d_texIdx = loadTexture(tempMaterial.map_d);
			}
			// Bump Map
			if (firstToken(curline) == "map_Bump" || firstToken(curline) == "map_bump" || firstToken(curline) == "bump")
			{
				tempMaterial.map_bump = filepath + tail(curline);
				tempMaterial.bump_texIdx = loadTexture(tempMaterial.map_bump);
			}
		}
		// Deal with last material

			// Push Back loaded Material
		LoadedMaterials.push_back(tempMaterial);

		// Test to see if anything was loaded
		// If not return false
		if (LoadedMaterials.empty())
			return false;
		// If so return true
		else
			return true;
	}

	int loadTexture(std::string path) {
		int texIdx = STexturePool::getInstance().isTextureLoaded(path.c_str());
		if (texIdx < 0) {
		    STexture texture;
		    if (texture.loadFromImage(path.c_str(), "")) {
		        texIdx = STexturePool::getInstance().pushToCache(texture);
		    }
		}
		return texIdx;
	}
};
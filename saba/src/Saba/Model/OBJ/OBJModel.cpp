//
// Copyright(c) 2016-2017 benikabocha.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#include "OBJModel.h"
#include "../../Base/Path.h"
#include "../../Base/Log.h"
#include "../../Base/File.h"
#include "tinyply.h"

#include <iostream>
#include <sstream>
#include <limits>
#include <glm/glm.hpp>
#include <tiny_obj_loader.h>

using namespace tinyply;
using namespace std;

typedef std::chrono::time_point<std::chrono::high_resolution_clock> timepoint;
std::chrono::high_resolution_clock c;

inline std::chrono::time_point<std::chrono::high_resolution_clock> now()
{
	return c.now();
}

inline double difference_micros(timepoint start, timepoint end)
{
	return (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

namespace saba
{
	namespace
	{
		class SabaMaterialReader : public tinyobj::MaterialReader
		{
		public:
			SabaMaterialReader(const std::string objPath)
				: m_objPath(objPath)
			{
			}

			bool operator()(
				const std::string& matId,
				std::vector<tinyobj::material_t>* materials,
				std::map<std::string, int>* matMap,
				std::string* err
				) override
			{
				std::string fileDir = PathUtil::GetDirectoryName(m_objPath);
				std::string mtlPath = PathUtil::Combine(fileDir, matId);
				TextFileReader fr;
				if (!fr.Open(mtlPath))
				{
					SABA_WARN("Failed to open MTL file.");
					SABA_INFO("Try obj name + .mtl.");
					std::string objFileName = PathUtil::GetFilenameWithoutExt(m_objPath);
					mtlPath = PathUtil::Combine(fileDir, objFileName + ".mtl");
					if (!fr.Open(mtlPath))
					{
						SABA_WARN("Failed to open MTL file.");
						if (err)
						{
							*err = "Failed to open MTL file.";
						}
						return false;
					}
				}
				std::string allText = fr.ReadAll();
				std::stringstream mtlSS(allText);

				tinyobj::MaterialStreamReader msr(mtlSS);

				if (!msr(matId, materials, matMap, err))
				{
					return false;
				}
				return true;
			}

		private:
			std::string	m_objPath;
		};
	}

	bool OBJModel::Load(const char * filepath)
	{
/************************************Load PLY File use tinyply********************************************/
		std::string fpath(filepath);
		std::ifstream ss(fpath, std::ios::binary);

		// Parse the ASCII header fields
		PlyFile file(ss);

		for (auto e : file.get_elements())
		{
			std::cout << "element - " << e.name << " (" << e.size << ")" << std::endl;
			for (auto p : e.properties)
			{
				std::cout << "\tproperty - " << p.name << " (" << PropertyTable[p.propertyType].str << ")" << std::endl;
			}
		}
		std::cout << std::endl;

		for (auto c : file.comments)
		{
			std::cout << "Comment: " << c << std::endl;
		}

		// Define containers to hold the extracted data. The type must match
		// the property type given in the header. Tinyply will interally allocate the
		// the appropriate amount of memory.
		std::vector<float> verts;
		std::vector<float> norms;
		std::vector<uint8_t> colors;

		std::vector<uint32_t> faces;
		std::vector<float> uvCoords;

		uint32_t vertexCount, normalCount, colorCount, faceCount, faceTexcoordCount, faceColorCount;
		vertexCount = normalCount = colorCount = faceCount = faceTexcoordCount = faceColorCount = 0;

		// The count returns the number of instances of the property group. The vectors
		// above will be resized into a multiple of the property group size as
		// they are "flattened"... i.e. verts = {x, y, z, x, y, z, ...}
		vertexCount = file.request_properties_from_element("vertex", { "x", "y", "z" }, verts);
		normalCount = file.request_properties_from_element("vertex", { "nx", "ny", "nz" }, norms);
		colorCount = file.request_properties_from_element("vertex", { "red", "green", "blue", "alpha" }, colors);

		// For properties that are list types, it is possibly to specify the expected count (ideal if a
		// consumer of this library knows the layout of their format a-priori). Otherwise, tinyply
		// defers allocation of memory until the first instance of the property has been found
		// as implemented in file.read(ss)
		faceCount = file.request_properties_from_element("face", { "vertex_indices" }, faces, 3);
		faceTexcoordCount = file.request_properties_from_element("face", { "texcoord" }, uvCoords, 6);

		// Now populate the vectors...
		timepoint before = now();
		
		file.read(ss);
		timepoint after = now();

		// Good place to put a breakpoint!
		std::cout << "Parsing took " << difference_micros(before, after) << "μs: " << std::endl;
		std::cout << "\tRead " << verts.size() << " total vertices (" << vertexCount << " properties)." << std::endl;
		std::cout << "\tRead " << norms.size() << " total normals (" << normalCount << " properties)." << std::endl;
		std::cout << "\tRead " << colors.size() << " total vertex colors (" << colorCount << " properties)." << std::endl;
		std::cout << "\tRead " << faces.size() << " total faces (triangles) (" << faceCount << " properties)." << std::endl;
		std::cout << "\tRead " << uvCoords.size() << " total texcoords (" << faceTexcoordCount << " properties)." << std::endl;

		ss.close();

		//vertexCount = vertexCount / 3;

/*********************************************************************************************************/
		SABA_INFO("Open OBJ file. {}", filepath);

		// Materialをコピー
		m_materials.clear();
		m_materials.reserve(0);

		// Meshを作成
		m_positions.resize(vertexCount);
		m_normals.resize(vertexCount);
		m_uvs.resize(0);

//////////////////////////////CALCULATE NORMAL///////////////////////////////////////////////


		vector<float> vertexNorm(vertexCount*3, 0);
		//cout<<"start "<<vertexCount<<"   "<<vertexNorm[3]<< endl;

		//cout << "Face Normals: "<< endl;
		// Loop through each face and calculate face normal, i = row # of faces starting from 0
		// could optimize it, Xian is good at it.
		uint32_t Vertex_1, Vertex_2, Vertex_3;
		glm::vec3 Vertext_1_Coord, Vertext_2_Coord, Vertext_3_Coord;
		for (int i = 0; i< faceCount; i++){
			uint32_t Vertex_1 = faces[3*i];
			Vertext_1_Coord = glm::vec3(verts[3*Vertex_1], verts[3*Vertex_1+1], verts[3*Vertex_1+2]);
	//		cout << pModel->verts[pModel->faces[3*i]]<<pModel->verts[pModel->faces[3*i+1]]<<pModel->verts[pModel->faces[3*i+2]] <<endl;
			//If needs to treat other shapes that are not triangle, need to read first column and change code
			Vertex_2 = faces[3*i+1];
			Vertext_2_Coord = glm::vec3(verts[3*Vertex_2], verts[3*Vertex_2+1], verts[3*Vertex_2+2]);
			Vertex_3 = faces[3*i+2];
			Vertext_3_Coord = glm::vec3(verts[3*Vertex_3], verts[3*Vertex_3+1], verts[3*Vertex_3+2]);

			vector<float> normVec = PlyFile::computeNormals(Vertext_1_Coord, Vertext_2_Coord, Vertext_3_Coord);
			//norms.insert(norms.end(),normVec.begin(),normVec.end());
			//cout<< "Normal="<< vertexNorm[3*faces[3*i]]<<"Normal="<< vertexNorm[3*faces[3*i]+1] << "Normal="<< vertexNorm[3*faces[3*i]+2]<<endl;

			vertexNorm[3*Vertex_1] += normVec[0];
			vertexNorm[3*Vertex_1 +1] += normVec[1];
			vertexNorm[3*Vertex_1 +2] += normVec[2];

			vertexNorm[3*Vertex_2] += normVec[0];
			vertexNorm[3*Vertex_2 +1] += normVec[1];
			vertexNorm[3*Vertex_2 +2] += normVec[2];

			vertexNorm[3*Vertex_3] += normVec[0];
			vertexNorm[3*Vertex_3 +1] += normVec[1];
			vertexNorm[3*Vertex_3 +2] += normVec[2];
			//cout<<"number "<<vertexNorm[3]<< "normal vec:" << normVec[0]<< normVec[1]<< normVec[2]<<endl;

		}
		//cout<<"Vertex Normals: ";
		//for (int i = 0; i < 15; i++){
		//	cout<<" "<<vertexNorm[i];
		//}
		//cout<<" "<<endl;

////////////////////////////////////////////////////////////////////////////////////////////////////////		


		for (size_t posIdx = 0; posIdx < vertexCount; posIdx++)
		{
			m_positions[posIdx].x = verts[posIdx * 3 + 0];
			m_positions[posIdx].y = verts[posIdx * 3 + 1];
			m_positions[posIdx].z = verts[posIdx * 3 + 2];
		}
		for (size_t norIdx = 0; norIdx < vertexCount; norIdx++)
		{
			m_normals[norIdx].x = vertexNorm[norIdx * 3 + 0];
			m_normals[norIdx].y = vertexNorm[norIdx * 3 + 1];
			m_normals[norIdx].z = vertexNorm[norIdx * 3 + 2];
		}

		

		if (!m_positions.empty())
		{
			m_bboxMin = glm::vec3(std::numeric_limits<float>::max());
			m_bboxMax = glm::vec3(-std::numeric_limits<float>::max());
			for (const auto& vec : m_positions)
			{
				m_bboxMin = glm::min(m_bboxMin, vec);
				m_bboxMax = glm::max(m_bboxMax, vec);
			}
		}
		else
		{
			m_bboxMin = glm::vec3(0);
			m_bboxMax = glm::vec3(0);
		}

		int emptyMatIdx = -1;
		std::cout<<"face count is "<< faceCount <<std::endl;
			for (size_t faceIdx = 0; faceIdx < faceCount; faceIdx++)
			{
				auto material = -1;

				if (material == -1)
				{
					if (emptyMatIdx == -1)
					{
						SABA_INFO("Material Not Assigned.");
						Material emptyMat;
						emptyMatIdx = (int)m_materials.size();
						emptyMat.m_ambient = glm::vec3(0.2f);
						emptyMat.m_diffuse = glm::vec3(0.5f);
						emptyMat.m_specularPower = 1.0f;
						m_materials.push_back(emptyMat);
					}
					material = emptyMatIdx;
				}

				Face face;
				face.m_position[0] = faces[faceIdx * 3 + 0];
				face.m_position[1] = faces[faceIdx * 3 + 1];
				face.m_position[2] = faces[faceIdx * 3 + 2];
				face.m_normal[0] = faces[faceIdx * 3 + 0];
				face.m_normal[1] = faces[faceIdx * 3 + 1];
				face.m_normal[2] = faces[faceIdx * 3 + 1];
				face.m_uv[0] = -1;
				face.m_uv[1] = -1;
				face.m_uv[2] = -1;
				face.m_material = material;

				m_faces.emplace_back(std::move(face));
			}
		

		SABA_INFO("OBJ File Success. {}", filepath);
		return true;
	}

	void OBJModel::Destroy()
	{
		m_positions.clear();
		m_normals.clear();
		m_uvs.clear();
		m_materials.clear();
		m_faces.clear();
	}

}

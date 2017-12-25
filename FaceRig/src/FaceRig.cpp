//============================================================================
// Name        : FaceRig.cpp
// Author      : Vultus
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "FaceRig.h"

using namespace std;
using namespace tinyply;

vector<float> computeNormals(glm::vec3 x, glm::vec3 y, glm::vec3 z){
	vector<float> normalVec;

	float s[3], t[3], v[3], length;
	// Finding Vectors
			s[0] = y.x - x.x;
			s[1] = y.y - x.y;
			s[2] = y.z - x.z;

			t[0] = z.x - x.x;
			t[1] = z.y - x.y;
			t[2] = z.z - x.z;

	// Cross Product
	v[0] = s[1] * t[2] - t[1] * s[2];
	v[1] = s[2] * t[0] - t[2] * s[0];
	v[2] = s[0] * t[1] - t[0] * s[1];

	// Normalization Factor
//	length = sqrt( ( v[0] * v[0] ) + (v[1] * v[1]) + (v[2] * v[2]) );

//	normalVec.push_back(v[0] / length);
//	normalVec.push_back(v[1] / length);
//	normalVec.push_back(v[2] / length);
	normalVec.push_back(v[0]);
	normalVec.push_back(v[1]);
	normalVec.push_back(v[2]);
	cout<<v[0]<<" "<<v[1]<<" "<<v[2]<<endl;
	return normalVec;
}





int main() {
	cout << "start reading ply file" << endl; // prints !!!Hello World!!!
	Model* pModel = new Model();
	read_ply_file("pyramid.ply", pModel);
	cout << "read complete" << endl;
	vector<float> vertexNorm(pModel->verts.size(), 0);

	for (int i = 0; i< (pModel->faces.size()/3); i++){
	//	for (int i = 0; i< 30; i++){
	//		cout << pModel->faces[i] << endl;
	//		uint32_t pModel->faces[i];
			uint32_t currentVertex = pModel->faces[3*i];
			glm::vec3 x = glm::vec3(pModel->verts[3*currentVertex],pModel->verts[3*currentVertex+1],pModel->verts[3*currentVertex+2]);
	//		cout << pModel->verts[pModel->faces[3*i]]<<pModel->verts[pModel->faces[3*i+1]]<<pModel->verts[pModel->faces[3*i+2]] <<endl;
			//If needs to treat other shapes that are not triangle, need to read first column and change code
			currentVertex = pModel->faces[3*i+1];
			glm::vec3 y = glm::vec3(pModel->verts[3*currentVertex],pModel->verts[3*currentVertex+1],pModel->verts[3*currentVertex+2]);
			currentVertex = pModel->faces[3*i+2];
			glm::vec3 z = glm::vec3(pModel->verts[3*currentVertex],pModel->verts[3*currentVertex+1],pModel->verts[3*currentVertex+2]);
	//		cout << x.x << " next is " << x.y << x.z <<endl;
	//		cout << y.x << " next is " << y.y << y.z <<endl;
	//		cout << z.x << " next is " << z.y << z.z <<endl;
			vector<float> normVec = computeNormals(x,y,z);
			pModel->norms.insert(pModel->norms.end(),normVec.begin(),normVec.end());
	//		cout<< "Normal="<< pModel->norms[i]<<endl;
			vertexNorm[3*pModel->faces[3*i]] +=  normVec[0];
			vertexNorm[3*pModel->faces[3*i]+1] +=  normVec[1];
			vertexNorm[3*pModel->faces[3*i]+2] +=  normVec[2];
			//cout<< "Normal="<< vertexNorm[3*pModel->faces[3*i]]<<"Normal="<< vertexNorm[3*pModel->faces[3*i]+1] << "Normal="<< vertexNorm[3*pModel->faces[3*i]+2]<<endl;
			float length = sqrt(
					(vertexNorm[3*pModel->faces[3*i]] * vertexNorm[3*pModel->faces[3*i]] )
					+ (vertexNorm[3*pModel->faces[3*i]+1] * vertexNorm[3*pModel->faces[3*i]+1])
					+ (vertexNorm[3*pModel->faces[3*i]+2] * vertexNorm[3*pModel->faces[3*i]+2])
					);
//			vertexNorm[3*pModel->faces[3*i]] = vertexNorm[3*pModel->faces[3*i]]/length;
//			vertexNorm[3*pModel->faces[3*i]+1] = vertexNorm[3*pModel->faces[3*i]+1]/length;
//			vertexNorm[3*pModel->faces[3*i]+2] = vertexNorm[3*pModel->faces[3*i]+2]/length;
//
			vertexNorm[3*pModel->faces[3*i+1]] += normVec[0];
			vertexNorm[3*pModel->faces[3*i+1]+1] += normVec[1];
			vertexNorm[3*pModel->faces[3*i+1]+2] += normVec[2];

//			uint32_t currentFace = pModel->faces[3*i+1];
//			length = sqrt(
//					(vertexNorm[3*currentFace] * vertexNorm[3*currentFace] )
//					+ (vertexNorm[3*currentFace+1] * vertexNorm[3*currentFace+1])
//					+ (vertexNorm[3*currentFace+2] * vertexNorm[3*currentFace+2])
//					);
//			vertexNorm[3*currentFace] = vertexNorm[3*currentFace]/length;
//			vertexNorm[3*currentFace+1] = vertexNorm[3*currentFace+1]/length;
//			vertexNorm[3*currentFace+2] = vertexNorm[3*currentFace+2]/length;

			vertexNorm[3*pModel->faces[3*i+2]] += normVec[0];
			vertexNorm[3*pModel->faces[3*i+2]+1] += normVec[1];
			vertexNorm[3*pModel->faces[3*i+2]+2] += normVec[2];

//			currentFace = pModel->faces[3*i+2];
//			length = sqrt(
//					(vertexNorm[3*currentFace] * vertexNorm[3*currentFace] )
//					+ (vertexNorm[3*currentFace+1] * vertexNorm[3*currentFace+1])
//					+ (vertexNorm[3*currentFace+2] * vertexNorm[3*currentFace+2])
//					);
//			vertexNorm[3*currentFace] = vertexNorm[3*currentFace]/length;
//			vertexNorm[3*currentFace+1] = vertexNorm[3*currentFace+1]/length;
//			vertexNorm[3*currentFace+2] = vertexNorm[3*currentFace+2]/length;
		}
	for(int i = 0; i<15;i++)
		cout << vertexNorm[i] << endl;






	display(pModel);
	cout << "display complete" << endl;
	delete pModel;
	return 0;
}


void read_ply_file(const std::string & filename, Model* pModel)
{
	// Tinyply can and will throw exceptions at you!
	try
	{
		// Read the file and create a std::istringstream suitable
		// for the lib -- tinyply does not perform any file i/o.
		std::ifstream ss(filename, std::ios::binary);

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
//		std::vector<float> verts;
//		std::vector<float> norms;
//		std::vector<uint8_t> colors;
//
//		std::vector<uint32_t> faces;
//		std::vector<float> uvCoords;

		uint32_t vertexCount, normalCount, colorCount, faceCount, faceTexcoordCount, faceColorCount;
		vertexCount = normalCount = colorCount = faceCount = faceTexcoordCount = faceColorCount = 0;

		// The count returns the number of instances of the property group. The vectors
		// above will be resized into a multiple of the property group size as
		// they are "flattened"... i.e. verts = {x, y, z, x, y, z, ...}
		vertexCount = file.request_properties_from_element("vertex", { "x", "y", "z" }, pModel->verts);
		normalCount = file.request_properties_from_element("vertex", { "nx", "ny", "nz" }, pModel->norms);
		colorCount = file.request_properties_from_element("vertex", { "red", "green", "blue", "alpha" }, pModel->colors);

		// For properties that are list types, it is possibly to specify the expected count (ideal if a
		// consumer of this library knows the layout of their format a-priori). Otherwise, tinyply
		// defers allocation of memory until the first instance of the property has been found
		// as implemented in file.read(ss)
		faceCount = file.request_properties_from_element("face", { "vertex_indices" }, pModel->faces, 3);
		faceTexcoordCount = file.request_properties_from_element("face", { "texcoord" }, pModel->uvCoords, 6);

		// Now populate the vectors...
		timepoint before = now();
		file.read(ss);
		timepoint after = now();

		// Good place to put a breakpoint!
		std::cout << "Parsing took " << difference_micros(before, after) << "μs: " << std::endl;
		std::cout << "\tRead " << pModel->verts.size() << " total vertices (" << vertexCount << " properties)." << std::endl;
		std::cout << "\tRead " << pModel->norms.size() << " total normals (" << normalCount << " properties)." << std::endl;
		std::cout << "\tRead " << pModel->colors.size() << " total vertex colors (" << colorCount << " properties)." << std::endl;
		std::cout << "\tRead " << pModel->faces.size() << " total faces (triangles) (" << faceCount << " properties)." << std::endl;
		std::cout << "\tRead " << pModel->uvCoords.size() << " total texcoords (" << faceTexcoordCount << " properties)." << std::endl;

		/*
		// Fixme - tinyply isn't particularly sensitive to mismatched properties and prefers to crash instead of throw. Use
		// actual data from parsed headers instead of manually defining properties added to a new file like below:

		std::filebuf fb;
		fb.open("converted.ply", std::ios::out | std::ios::binary);
		std::ostream outputStream(&fb);

		PlyFile myFile;

		myFile.add_properties_to_element("vertex", { "x", "y", "z" }, verts);
		myFile.add_properties_to_element("vertex", { "red", "green", "blue" }, colors);
		myFile.add_properties_to_element("face", { "vertex_indices" }, faces, 3, PlyProperty::Type::UINT8);

		myFile.comments.push_back("generated by tinyply");
		myFile.write(outputStream, true);

		fb.close();
		*/
		ss.close();
	}

	catch (const std::exception & e)
	{
		std::cerr << "Caught exception: " << e.what() << std::endl;
	}
}

void display(Model* pModel){

	/*glewInit();
	char *argv [1];
	int argc=1;
	argv [0]=strdup ("FaceRig");
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(100, 100);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glGenBuffers(1, &m_posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_posVBO);  // coordinates
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pModel->verts.size(), nullptr, GL_STATIC_DRAW);
	glVertexPointer(pModel->verts.size(), GL_FLOAT, 0, 0);

	glGenBuffers(1, &m_colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);  // coordinates
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint8_t) * pModel->colors.size(), nullptr, GL_STATIC_DRAW);
	glColorPointer(pModel->colors.size(), GL_BYTE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, pModel->faces.size()*3);


	glutMainLoop();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	// Disable the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}


void Initialize_VAO(Model* pModel)
{
	/*glGenVertexArrays(1, &(pModel->m_VAO));
	glBindVertexArray(pModel->m_VAO);*/

}

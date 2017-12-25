/*
 * FaceRig.h
 *
 *  Created on: Dec 22, 2017
 *      Author: ece496
 */

#ifndef FACERIG_H_
#define FACERIG_H_


#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>


#include "tinyply.h"

using namespace tinyply;

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

class Model{
public:
	std::vector<float> verts;
	std::vector<float> norms;
	std::vector<uint8_t> colors;

	std::vector<uint32_t> faces;
	std::vector<float> uvCoords;

	GLuint	m_posVBO = 0;
	GLuint	m_norVBO = 0;
	GLuint	m_colorVBO = 0;
	GLuint	m_ibo = 0;
	GLenum	m_indexType;

	GLuint	m_VAO = 0;
};

void read_ply_file(const std::string & filename, Model* pModel);
void display(Model* pModel);
void Initialize_VAO(Model* pModel);

GLuint	m_posVBO;
GLuint	m_colorVBO;

#endif /* FACERIG_H_ */

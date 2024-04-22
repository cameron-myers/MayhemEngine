/*****************************************************************//**
 * \file         Mesh.h
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

#pragma once
#include "glfw/glfw3.h"
#include <string>
typedef struct Vertex Vertex;
typedef GLuint GLuint;
typedef GLuint MEMesh;
typedef GLuint MEMeshVBuffer;

class Mesh
{
public:
	Mesh();
	Mesh(std::string filename);
	//Mesh(Mesh& rhs); --DO NOT MAKE COPY CONSTRUCTOR, see cpp for more info
	~Mesh();
	void Read(const char* name);
	void Write();
	bool IsNamed(const std::string _name);
	MEMesh* GetMesh();
	std::string GetName();
	unsigned GetVertexCount();
	MEMeshVBuffer* GetVertexBuffer();
	Vertex* GetVertices();
	void ScaleX(float scale);
	//float& ScaleY();
	void SetName(std::string _name);
	void SetVertexCount(int _vertexCount);
	void SetVertexBuffer(GLuint* _vertexBuffer);
	void SetxHalfSize(float _xHalfSize);
	void SetyHalfSize(float _yHalfSize);
	void SetVertices(Vertex* _vertices);

private:

	std::string name;

	MEMesh mesh;
	MEMeshVBuffer vertexBuffer;
	int vertexCount;
	float xHalfSize;
	float yHalfSize;
	Vertex* vertices;

};

/*****************************************************************//**
 * \file         Mesh.cpp
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "stdafx.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "MeshManager.h"
#include "MESerializer.h"
#include "Log.h"
#include "MEResources.h"
#include "mesh.h"
//#pragma warning(disable:4996)


using namespace rapidjson;
/**
 * \brief Mesh constructor
 */
Mesh::Mesh()
{
	name = "nullptr";
}

/**
 * \brief Mesh constructor
 * \param MeshName Name of the mesh to construct with
 */
Mesh::Mesh(std::string MeshName)
{
	
	const Mesh* temp = MeshManager::Build(MeshName);
	if (temp)
	{
		*this = *temp;
	}
}

/**
 * !!!! DO NOT MAKE COPY CONSTRUCTOR!!! CAUSES ISSUES IN MESerializer.cpp!!! -ethan
Mesh::Mesh(Mesh& rhs)
{
	//copy some stuff
	const boost::uuids::uuid uuid = boost::uuids::random_generator()();
	vertexCount = rhs.vertexCount;
	name = "Bar_Mesh" + to_string(uuid);
	vertexBuffer = rhs.vertexBuffer;
	xHalfSize = rhs.xHalfSize;
	yHalfSize = rhs.yHalfSize;
	
	MeshManager::Add(this);


	CreateMesh(GetMesh(), GetVertexBuffer(), GetVertices(), GetVertexCount());
	if (!GetMesh())
	{
		ME_CORE_ERROR("MEMesh: BAR Mesh failed to load");
		assert(false);
	}

}*/

/**
 * \brief Mesh deconstructor
 */
Mesh::~Mesh()
{
	vertexCount = 0;
	name.clear();
	FreeMesh(&mesh, &vertexBuffer);

	mesh = NULL;
	vertexBuffer = NULL;
	xHalfSize = NULL;
	yHalfSize = NULL;
	//delete vertices array
}

/**
 * \brief Read in the Mesh
 * \param filename name of the file to open
 */
void Mesh::Read(const char* filename)
{

	Mesh* newMesh = MESerializer::LoadMesh(filename);
	
	name = newMesh->name;
	xHalfSize = newMesh->xHalfSize;
	yHalfSize = newMesh->yHalfSize;
	vertexCount = newMesh->vertexCount;
	vertices = newMesh->vertices;


}

void Mesh::Write()
{/*
	FILE* fp = fopen_s("../Assets/dataDumpMesh.json", "wb");
	StringBuffer w;
	PrettyWriter<StringBuffer>writer(w);
	writer.StartObject();
	writer.SetMaxDecimalPlaces(2);
	writer.Key("Mesh");
	writer.StartObject();
	writer.Key("Name");
	writer.String(name.c_str());
	writer.Key("xHalfSize");
	writer.Double(xHalfSize);
	writer.Key("yHalfSize");
	writer.Double(yHalfSize);
	writer.EndObject();
	writer.EndObject();
	writer.IsComplete();
	fprintf(fp, w.GetString());
	fclose(fp);
	writer.Reset(w);*/
}

/**
 * \brief Checks if the mesh is named a name
 * \param _name name to check against
 * \return 
 */
bool Mesh::IsNamed(const std::string _name)
{
	if (_name == name) return true;

	return false;
}


/**
 * \brief Gets the mesh
 * \return 
 */
MEMesh* Mesh::GetMesh()
{
	return &mesh;
}
/// <summary>
/// get the vertex buffer
/// </summary>
/// <returns></returns>
MEMeshVBuffer* Mesh::GetVertexBuffer()
{
	return &vertexBuffer;
}
/// <summary>
/// get vertices
/// </summary>
/// <returns></returns>
Vertex* Mesh::GetVertices()
{
	return vertices;
}
/// <summary>
/// get the name of the mesh
/// </summary>
/// <returns></returns>
std::string Mesh::GetName()
{
	return name;
}
/**
 * \brief Gets the vertex count of the mesh
 * \return 
 */
unsigned Mesh::GetVertexCount()
{
	return vertexCount;
}


/**
 * \brief sets the name of the Mesh
 * \param _name string to set name to
 */
void Mesh::SetName(std::string _name)
{
	name = _name;
}

/**
 * \brief sets the mesh's vertex buffer
 * \param _vertexBuffer 
 */
void Mesh::SetVertexBuffer(GLuint* _vertexBuffer)
{
	vertexBuffer = *_vertexBuffer;
}
void Mesh::SetVertices(Vertex* _vertices)
{
	vertices = _vertices;
}
/**
 * \brief Sets the vertex count for the mesh
 * \param _vertexCount 
 */
void Mesh::SetVertexCount(int _vertexCount)
{
	vertexCount = _vertexCount;
}

/**
 * \brief Sets the x half size
 * \param _xHalfSize 
 */
void Mesh::SetxHalfSize(float _xHalfSize)
{
	xHalfSize = _xHalfSize;
}

/**
 * \brief sets the y half size
 * \param _yHalfSize 
 */
void Mesh::SetyHalfSize(float _yHalfSize)
{
	yHalfSize = _yHalfSize;
}

void Mesh::ScaleX(float scale)
{

	//move the right side of the mesh
	vertices[1].position.x = scale;
	vertices[3].position.x = scale;
	vertices[0].position.x = scale;

	MeshManager::Remove(this);
	FreeMesh(GetMesh(), GetVertexBuffer());
	CreateMesh(GetMesh(), GetVertexBuffer(), GetVertices(), GetVertexCount());
	MeshManager::Add(this);

}

/*
float& Mesh::ScaleY()
{
	return 
}
*/

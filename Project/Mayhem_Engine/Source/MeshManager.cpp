/*****************************************************************//**
 * \file         MeshManager.cpp
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

#include "MeshManager.h"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Log.h"
#include "MEResources.h"
#include "Mesh.h"

std::vector<Mesh*> MeshManager::MEMeshList;

/**
 * \brief Builds a new mesh
 * \param meshName name of the mesh to make
 * \return Pointer to the new mesh
 */
Mesh* MeshManager::Build(std::string& meshName)
{
	Mesh* meshSrc = nullptr;
	if(meshName != "Bar_Mesh") meshSrc = Find(meshName);

	if (!meshSrc)
	{
		Mesh* meshSrc = new Mesh();

#ifdef _DEBUG
		meshName.insert(0, "../Assets/Mesh/");
#endif // _DEBUG

#ifdef _DISTRIBUTE
		meshName.insert(0, "./Assets/Mesh/");
#endif // _DISTRIBUTE


		meshName.append(".json");
		const char* filename = meshName.c_str();
		meshSrc->Read(filename);
		Add(meshSrc);

		CreateMesh(meshSrc->GetMesh(), meshSrc->GetVertexBuffer(), meshSrc->GetVertices(), meshSrc->GetVertexCount());
		if (!meshSrc->GetMesh())
		{
			ME_CORE_ERROR("MEMesh: %s failed to load", filename);
			assert(false);
		}

		return meshSrc;
	}
	return meshSrc;
}

/**
 * \brief Frees all the meshs in the list
 */
void MeshManager::FreeAll()
{
	for (Mesh* mesh : MEMeshList)
	{
		
		delete mesh;
	}
	MEMeshList.clear();
}

/**
 * \brief Adds a mesh to the list
 * \param mesh mesh to add to the list
 */
void MeshManager::Add(Mesh* mesh)
{
	MEMeshList.insert(MEMeshList.begin(), mesh);
}

/**
 * \brief pop a mesh from the manager
 * \param mesh 
 */
void MeshManager::Remove(Mesh* mesh)
{
	std::vector<Mesh*>::iterator it = MEMeshList.begin();

	//loop though the list till the mesh is found
	for( it; it != MEMeshList.end(); ++it)
	{
		if(*it == mesh)
		{
			MEMeshList.erase(it);
			break;
		}
	}

}

/**
 * \brief Finds a mesh in the list given a name
 * \param name name of the mesh to look for
 * \return pointer to the mesh
 */
Mesh* MeshManager::Find(const std::string name)
{
	for (Mesh* mesh : MEMeshList)
	{
		if (mesh->IsNamed(name)) return mesh;
	}
	return NULL;
}

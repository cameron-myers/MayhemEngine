/*****************************************************************//**
 * \file         MeshManager.h
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once
#include <string>
#include <vector>

class Mesh;

class MeshManager
{
public:
	static Mesh* Build(std::string& meshName);
	static void FreeAll();
	static void Add(Mesh* mesh);
	static void Remove(Mesh* mesh);
	static Mesh* Find(const std::string name);

private:


	static std::vector<Mesh*> MEMeshList;
};
/*****************************************************************//**
 * \file         SpriteSourceManager.cpp
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

#include "stdafx.h"
#include "SpriteSourceManager.h"

#include <vector>

#include "SpriteSource.h"

std::vector<SpriteSource*> SpriteSourceManager::spriteSourceList;

/**
 * \brief Builds a new sprite source, also checks if it has already been loaded
 * \param spriteSourceName name of the sprite source
 * \return a pointer to the sprite source
 */
SpriteSource* SpriteSourceManager::Build(std::string& spriteSourceName)
{
	//We look to see if we have already loaded this asset
	SpriteSource* source = Find(spriteSourceName);

	//if we cant find it then we need to load it
	if(source == NULL)
	{
		SpriteSource* newSource = new SpriteSource();

#ifdef _DEBUG
		spriteSourceName.insert(0, "../Assets/Art/");
#endif // _DEBUG

#ifdef _DISTRIBUTE
		spriteSourceName.insert(0, "./Assets/Art/");
#endif // _DISTRIBUTE
		spriteSourceName.append(".json");
		const char* filename = spriteSourceName.c_str();

		//Read the JSON file in
		newSource->Read(filename);
		//Add the newly read Source to the manager
		Add(newSource);
		return newSource;
	}
	//we found the source in the Find() call, return
	return source;
}

/**
 * \brief reloads a given sprite source
 * \param spriteSourceName 
 * \return 
 */
SpriteSource* SpriteSourceManager::ReBuild(std::string spriteSourceName)
{
	SpriteSource* source = Find(spriteSourceName);

	if(source)
	{
#ifdef _DEBUG
		spriteSourceName.insert(0, "../Assets/Art/");
#endif // _DEBUG

#ifdef _DISTRIBUTE
		spriteSourceName.insert(0, "./Assets/Art/");
#endif // _DISTRIBUTE


		spriteSourceName.append(".json");
		const char* filename = spriteSourceName.c_str();
		source->Read(filename);
	}
	//if it doesnt already exist, add it to the manager too
	else
	{
		source = SpriteSourceManager::Build(spriteSourceName);
	}

	return source;
}

/**
 * \brief frees all the sprite sources
 */
void SpriteSourceManager::FreeAll()
{
	for(SpriteSource* source : spriteSourceList)
	{
		delete source;
	}
}

/**
 * \brief Add a sprite source to the list
 * \param spriteSrc pointer to the spritesource to add
 */
void SpriteSourceManager::Add(SpriteSource* spriteSrc)
{
	spriteSourceList.insert(spriteSourceList.begin(), spriteSrc);
}

/**
 * \brief Finds a sprite source in the list
 * \param name Name of the sprite source to look for
 * \return a pointer to the sprite source
 */
SpriteSource* SpriteSourceManager::Find(const std::string name)
{
	for(SpriteSource* source : spriteSourceList)
	{
		if (source->IsNamed(name)) return source;
	}
	return NULL;
}
/// <summary>
/// shutdown the sprite source manager
/// </summary>
void SpriteSourceManager::Shutdown()
{
	for(auto src : spriteSourceList)
	{
		src = NULL;
	}
	spriteSourceList.clear();
}
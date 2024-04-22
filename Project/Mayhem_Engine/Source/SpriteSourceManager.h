/*****************************************************************//**
 * \file         SpriteSourceManager.h
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

class SpriteSource;

class SpriteSourceManager
{
public:
	static SpriteSource* Build(std::string& spriteSourceName);
	static SpriteSource* ReBuild(std::string spriteSourceName);
	static void FreeAll();
	static void Shutdown();
	static SpriteSource* Find(const std::string name);
	static void Add(SpriteSource* spriteSrc);


private:

	static std::vector<SpriteSource*> spriteSourceList;
};
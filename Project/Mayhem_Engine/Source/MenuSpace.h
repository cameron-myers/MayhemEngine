/*****************************************************************//**
 * \file         SpriteSource.h
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

#include "MESpace.h"
class MESpace;

class MenuSpace :public MESpace
{
public:

	void Load();

	void Init();

	void Update(float dt);

	void Draw();

	void Shutdown();

	void Unload();

private:
	void Build(std::string const& filename);
};

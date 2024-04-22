
/*****************************************************************//**
 * \file         SpaceSandbox.h
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include "MESpace.h"

class SpaceSandbox :public MESpace
{
public:

	void Load();

	void Init();

	void Update(float dt);

	void Draw();

	void Shutdown();

	void Unload();

private:

};


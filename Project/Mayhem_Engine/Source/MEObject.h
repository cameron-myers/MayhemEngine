/*****************************************************************//**
 * \file         MEObject.h
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/


#pragma once
#define UNREFERENCED_PARAMETER(P) (P)

class MayhemObject
{
public:

	MayhemObject()
	{

	}
	virtual ~MayhemObject() {}

	virtual void Init() {}
	virtual void Load() {}
	virtual void Update(float dt) { UNREFERENCED_PARAMETER(dt); }
	virtual void Unload() {}
	virtual void Shutdown() {}

};

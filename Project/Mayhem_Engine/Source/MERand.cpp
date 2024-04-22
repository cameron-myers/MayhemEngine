/*****************************************************************//**
 * \file         MERand.cpp
 * \author       Nate White
 * \par          Email: Nate.white\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include "MERand.h"
#include "Component.h"
#include "GameObjectManager.h"
#include <glm/gtc/random.hpp>
#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "Transform.h"
#include "GameObjectFactory.h"
#include "Log.h"
#include "ZeppelinBehavior.h"
#include "MESpaceManager.h"
#include "MESpace.h"
#include "GameObjectManager.h"

/// <summary>
/// spawn zeppelin at random x and y position
/// </summary>
/// <returns>vec2 of spawn position</returns>
glm::vec2 SpawnZepplinXY()
{
	/*coordinates are drawn from top left*/
	float x = -1000; /*the x value of zepplin spawn*/
	float y;/* the y position of zepplin spawn*/
	int lane = glm::linearRand(1, 3);
	if (lane == 1)
	{
		y = -280.0f;
	}
	else if (lane == 2)
	{
		y = 50.0f;
	}
	else if (lane == 3)
	{
		y = 350.0f;
	}

	glm::vec3 vector = glm::vec3(x, y, 1.0f);

	MESpace* space = MESpaceManager::GetSpace("Sandbox");
	GameObjectManager* active = space->GetActive();
	GameObjectPtr gam = active->GetObjectByName("Zepplin1");

	Transform* trans = gam->Has(Transform);

	trans->SetTranslation(vector);

		return vector;

}
/// <summary>
/// create random number from given range
/// </summary>
/// <param name="min"></param>
/// <param name="max"></param>
/// <returns>int of random number</returns>
int RandRange(int min, int max)
{
	return glm::linearRand(min, max);
}
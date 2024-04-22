/*********************************************************************************************************************/
/*!
  \file              MESerializer.h
  \author            Cameron Myers,Nate White, Ethan Mclaughlin
  \par               Email: cameron.myers\@digipen.edu, nate.white\@digipen.edu,ethan.mclaughlin@digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.

  \brief
	this file has functions that do serialization/de-serialization things
 */
 /*********************************************************************************************************************/


/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/

#pragma once
#include "MEtimer.h"
#include <string>
#include "glm/glm.hpp"

typedef FILE* Stream;
class GameObject;
class Mesh;
class SpriteSource;
class Timer;

class MESerializer
{

public:
	void Init();

	static void LoadShader();

	static GameObject LoadGameObject(const char* filepath);

	static Mesh* LoadMesh(const char* filepath);

	static SpriteSource LoadSpriteSource(const char* filepath);

	/*void LoadUnity();

	void LoadImGuiLayout();*/

	static void SaveGameObject(GameObject* Obj, int itterater = 0);
	static void SaveGameObjectTransform(GameObject* Obj);
	static void SaveGameObjectSprite(GameObject* Obj);


	void Destroy();

	static glm::vec3 readVec3(const char* buffer);

	static std::string OpenFileRead(const char* filename);

private:
	static Timer* SaveCooldown;

};


/*****************************************************************//**
Code uncounted 
 * 
 *********************************************************************/

/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#pragma once
#include "AK/SoundEngine/Common/AkTypes.h"
#include <string>
#include "Transform.h"
#include "GameObject.h"

typedef FILE* Stream;
typedef struct Behavior* BehaviorPtr;
typedef Transform* TransformPtr;

class Lane
{
public:
	Lane();
	~Lane();
	Lane* Clone();
	void Add(GameObjectPtr gameObject);
	GameObjectPtr Find(GameObjectPtr gameObject);
	void Update(float dt);
	void Draw();
	void Free();
	bool Destroyed();
	void Destroy();
	void Read(const char* path);
	std::string GetName();
	bool IsNamed(std::string input);
	static void setName(std::string input);
private:
	static std::string name;
	bool isDestroyed;
	int numZeppelens;
	int numTowers;
	GameObjectPtr Objects[cCount];
};


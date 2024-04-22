/*****************************************************************//**
 * \file         BehaviorButton.h
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/

 /*********************************************************************************************************************/
 /* Includes */
 /*********************************************************************************************************************/
#pragma once
#include "Behavior.h"
#include "Transform.h"

#include <glm/vec3.hpp>
class Timer;
class Transform;
class Behavior;
class Audio;
typedef Behavior* BehaviorPtr;


class Button : public Behavior
{
public:
	Button();
	Button(const Button& rhs);
	virtual ~Button();
	BehaviorPtr Clone() const override;
	virtual void Init();
	virtual void Update(float dt);
	virtual void Exit()override;
	void Read(const char* path);
	virtual void CollisionHandler(GameObjectPtr other) {};
protected:
	//different updates
	/*void PlayUpdate(float dt);
	void ExitUpdate(float dt);
	void TutorialUpdate(float dt);
	void UpgradeUpdate(float dt);
	void CoinUpdate(float dt);
	void ZeppelinSelectUpdate(float dt);*/

	void SetPosition(glm::vec3 pos);
	void RangeCollision(GameObjectPtr collider, GameObjectPtr other);
	void SetScale(glm::vec3 _scale);
	void SetScale(float _scale);
	virtual bool Collision();
	Transform oldTransform;
	Audio* audio;
	bool justhover;
	Timer* click_timer;
	float pressed_time = 0.25f;
	typedef enum cButton { cButtonInvalid = -1, cButtonIdle, cButtonHovering, cButtonPressed, cButtonDestroyed } ButtonStates;

};


/*****************************************************************//**
 * \file         BackgroundBehavior.h
 * \authors       Cameron Myers, Ethan Mclaughlin
 * \par          Email: cameron.myers\@digipen.edu, Ethan.mclaughlin\@digipen.edu
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
#include "InterpolManager.h"

class Transform;
class Behavior;
typedef Behavior* BehaviorPtr;

enum cBackgroundType { cBackgroundTypeInvalid = -1, cMainMenuBackground, cGameBackground, cPauseMenuBackground, cWinMenuBackground, cLoseMenuBackground, cCreditMenuBackground, cDigiBackground };

class Background : public Behavior
{
public:
	Background();
	Background(const Background& rhs);
	Background(cBackgroundType _type, std::string name);
	~Background();
	BehaviorPtr Clone() const override;
	void Init();
	void Update(float dt);
	void Exit();
	void CollisionHandler(GameObjectPtr other) {};
	//different updates
	void MainMenuUpdate(float dt);
	void PauseMenuUpdate(float dt);
	void GameBackUpdate(float dt);
	void LoseMenuUpdate(float dt);
	void WinMenuUpdate(float dt);
	void CreditMenuUpdate(float dt);
	void DigiUpdate(float dt);
	
	//geters/seters, self explanitory
	int GetStarData();
	void SetStarData(int var);
	float GetTimeData();
	void SetTimeData(float var);


private:
	Transform oldTransform;
	Audio* audio;
	cBackgroundType type;

	int StarData;
	float TimeRemaining;


};


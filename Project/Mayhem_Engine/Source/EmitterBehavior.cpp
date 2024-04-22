/*****************************************************************//**
 * \file         EmitterBehavior.cpp
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
#include "Log.h"
#include "EmitterBehavior.h"

#include "GameObject.h"
#include "rapidjson/document.h"


/*void EmitterBehavior::Read(const char* bufchar)
{
	
	//json document
	rapidjson::Document d;
	//parse the json
	d.Parse(bufchar);
	const rapidjson::Value& e = d["GameObject"];
	//go into the EmitterBehavior section
	const rapidjson::Value& value = e["Behavior"];
	//set a to thee EmitterBehavior
	const rapidjson::Value& a = value["stateCurr"];

	this->stateCurr = a.GetInt();

	//set d to nextstate
	const rapidjson::Value& h = value["stateNext"];
	this->stateNext = h.GetInt();

}*/

/// @brief gets the current state of the EmitterBehavior
/// @return 
int EmitterBehavior::GetStateCurr()
{
	return stateCurr;
}

/// @brief gets the next state of the EmitterBehavior
/// @return 
int EmitterBehavior::GetStateNext()
{
	return stateNext;
}

/// @brief set the current state of the EmitterBehavior
/// @param _stateCurr 
void EmitterBehavior::SetStateCurr(int _stateCurr)
{
	stateCurr = _stateCurr;
}

/// @brief set the next state of the EmitterBehavior
/// @param _stateNext 
void EmitterBehavior::SetStateNext(int _stateNext)
{
	stateNext = _stateNext;
}



/*********************************************************************
 * \file         MEFunctionalTesting.h
 * \author       Cameron Myers
 * \brief		 
 *
 *********************************************************************/

#pragma once
#include <map>
#include <string>


#include "MEFunctionalTest.h"

#include "FTLoadScene.h"

inline extern std::map<std::string, MEFunctionalTest*> TEST_SUITE{
	std::make_pair("Load_Menu", new FTLoadScene("Load_Menu")),
	std::make_pair("Load_Level", new FTLoadScene("Load_Level")),

};
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


std::map<std::string, MEFunctionalTest> TEST_SUITE{
	std::make_pair("Load_Menu", FTLoadScene("Load_Menu")),
	std::make_pair("Load_Level", FTLoadScene("Load_Level")),

};
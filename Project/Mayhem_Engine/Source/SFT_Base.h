/*********************************************************************
 * \file         SFT_Base.h
 * \author       Cameron Myers
 * \brief		 Scriptable functional test base class
 *
 *********************************************************************/

#pragma once
#include <map>
#include <stack>
#include <string>
#include "MEFunctionalTest.h"

class SFT_Base : public MEFunctionalTest
{

public:
	static SFT_Base* CreateTest(const char* suite);

	SFT_Base() = default;
	SFT_Base(const char* suite):MEFunctionalTest(suite){};
	SFT_Base(const SFT_Base& rhs):MEFunctionalTest(rhs)
	{
		m_Scene = rhs.m_Scene;
	};

	virtual void Init() = 0;
	virtual std::string Read() = 0;
	virtual void Update(float dt) = 0;
	virtual void Shutdown() = 0;

	std::string m_Scene;
};

/*********************************************************************
 * \file         MEFunctionalTesting.h
 * \author       Cameron Myers
 * \brief		 
 *
 *********************************************************************/

#pragma once
#include <string>

#include "SFT_Base.h"
#include <boost/any/basic_any.hpp>

class SFTMainMenuUI : public SFT_Base
{

public:
	static SFTMainMenuUI* CreateTest(const char* suite)
	{
		return new SFTMainMenuUI("temp");
	};
	SFTMainMenuUI() = default;
	SFTMainMenuUI(const char* suite):SFT_Base(suite){};
	SFTMainMenuUI(const SFTMainMenuUI& rhs):SFT_Base(rhs)
	{
		m_Scene = rhs.m_Scene;
		m_Object = rhs.m_Object;
	};

	virtual void Init() override;
	virtual std::string Read()override;
	virtual void Update(float dt)override;
	virtual void Shutdown()override;

	std::string m_Object;
};

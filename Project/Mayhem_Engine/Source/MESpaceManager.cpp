/*****************************************************************//**
 * \file         MESpaceManager.cpp
 * \author       Nick Leben, Ethan mclaughlin
 * \par          Email: Nick.leben\@digipen.edu,ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 *
 *********************************************************************/
#include "MESpaceManager.h"
#include "MESpace.h"
#include "Log.h"
#include <vector>
#include "MEInput.h"
#include "LaneManager.h"
#include "MEEditor.h"
#include "GameObjectManager.h"
#include "MEtimer.h"
#include "Audio.h"
#include "Engine.h"
#include "MEAudio.h"


static std::deque<MESpace*>spaces;
static MESpace* prev;

static float Localtime = 0.0f;
bool hasHappened = false;
bool hasHappened2 = false;

/*
Init()
{
	Init(StartScene);
	spaces.push(StartScene);
}

Init(PlaySpace);
spaces.push(PlaySpace);
Shutdown()
*/

/**
 * Initialize the first space (currently splash screen).
 *
 */
void MESpaceManager::Init()
{
	if(!Engine::isHeadless)
	{
		MESpace* splash = new MESpace("SplashScreen");
		prev = splash;
		splash->SetActive(true);
		spaces.push_back(splash);
		splash->Init();
		splash->Load();
	}
}
	
/**
 * Update loop for space manager.
 *
 * \param dt
 */
void MESpaceManager::Update(float dt)
{
	std::string MenuUI = "MenuUI";
	std::string Sandbox = "Sandbox";
	std::string Pause = "PauseMenu";
	std::string Credits = "CreditMenu";

	Localtime += dt;
	if(!Engine::isHeadless)
	{
		
		if (Input::IsKeyReleased(GLFW_KEY_B))
		{
			Localtime = 0.0f;
			hasHappened = false;
			hasHappened2 = false;
		}

		if (Localtime == 0.0f)
		{
			if (GetSpace("SplashScreen"))
			{
				if (!GetSpace("SplashScreen")->IsActive())
				{
					GetSpace("SplashScreen")->GetActive()->GetObjectByName("DigiSplash")->Has(Audio)->PlayAudio(SOUND(PLAY_SPLASH_SCREEN_DIGI));
					SetSpaceExclusive("SplashScreen");
				}
			}
		}

		if (Localtime > 5.0f || (Input::IsKeyReleased(GLFW_MOUSE_BUTTON_1) || (Input::IsKeyReleased(GLFW_MOUSE_BUTTON_2) ||
			(Input::IsKeyReleased(GLFW_KEY_ENTER) || (Input::IsKeyReleased(GLFW_KEY_SPACE) || (Input::IsKeyReleased(GLFW_KEY_ESCAPE) && !MEEditor::IsOpen()))))))
		{
			if (!hasHappened2)
			{
				prev->SetActive(false);
				SetSpaceExclusive("WwiseSplash");
				hasHappened2 = true;
			}
		}

		if (Localtime > 10.0f || (Input::IsKeyReleased(GLFW_MOUSE_BUTTON_1) || (Input::IsKeyReleased(GLFW_MOUSE_BUTTON_2)
			|| (Input::IsKeyReleased(GLFW_KEY_ENTER) || (Input::IsKeyReleased(GLFW_KEY_SPACE) || (Input::IsKeyReleased(GLFW_KEY_ESCAPE) && !MEEditor::IsOpen()))))))
		{
			if (!hasHappened)
			{
				prev->SetActive(false);
				//SetMenuUI();
				SetSpaceExclusive("MenuUI");
				hasHappened = true;
			}
		}

		if (Input::IsKeyReleased(GLFW_KEY_R) && !MEEditor::IsOpen())
		{
			//RefreshAll();
		}

		if (GetSpace("AreUSure") && GetSpace("PauseMenu"))
		{
			if (GetSpace("AreUSure")->IsActive() && GetSpace("PauseMenu")->IsActive())
			{
				GetSpace("PauseMenu")->SetActive(false);
			}
		}

		if (GetSpace("CreditMenu") && GetSpace("PauseMenu"))
		{
			if (GetSpace("CreditMenu")->IsActive() && GetSpace("PauseMenu")->IsActive())
			{
				GetSpace("PauseMenu")->SetActive(false);
			}
		}

		if (GetSpace("AreUSureMain") && GetSpace("PauseMenu"))
		{
			if (GetSpace("AreUSureMain")->IsActive() && GetSpace("PauseMenu")->IsActive())
			{
				GetSpace("PauseMenu")->SetActive(false);
			}
		}

		if (GetSpace("AreUSure") && GetSpace("MenuUI"))
		{
			if (GetSpace("AreUSure")->IsActive() && GetSpace("MenuUI")->IsActive())
			{
				GetSpace("MenuUI")->SetActive(false);
			}
		}

		if (GetSpace("Tutorial") && GetSpace("PauseMenu"))
		{
			if (GetSpace("Tutorial")->IsActive() && GetSpace("PauseMenu")->IsActive())
			{
				GetSpace("PauseMenu")->SetActive(false);
				GetSpace("Sandbox")->SetActive(false);
				prev = GetSpace("PauseMenu");
			}
		}

		if (GetSpace("AreUSureMain") && GetSpace("Sandbox"))
		{
			if (GetSpace("AreUSureMain")->IsActive() && GetSpace("Sandbox")->IsActive())
			{
				GetSpace("Sandbox")->SetActive(false);
			}
		}
	}

	for (MESpace* space : spaces)
	{
		if (space->IsActive())
		{
			if (*space->GetName() != MenuUI)
			{
				if (Input::IsKeyReleased(GLFW_KEY_ESCAPE) && !MEEditor::IsOpen())
				{
					if (*space->GetName() == Pause && space->IsActive())
					{
						GetSpace(Sandbox)->SetActive(true);
						space->SetActive(false);
						METimerManager::GlobalUnpause();
						break;
					}
					else
					{
						//SetPause();
						GetSpace(Sandbox)->SetActive(false);
						METimerManager::GlobalPause();
						SetSpaceInclusive("PauseMenu");
						MoveSpaceToBack("PauseMenu");
						prev = space;
						break;
					}
				}
			}
			/**/


			if (*space->GetName() == Sandbox)
			{
				LaneManager::Update(dt);
			}
			if (GetSpace("PauseMenu"))
			{
				if (*space->GetName() != Sandbox || !(GetSpace("PauseMenu")->IsActive()))
				{
					space->Update(dt);
				}
			}
			else
			{
				space->Update(dt);
			}
		}
	}

	if (HasSpace("Sandbox") && (HasSpace("MenuUI") || HasSpace("GameLose") || HasSpace("GameWin")))
	{
		MESpace* sand = GetSpace("Sandbox");

		if (HasSpace("MenuUI"))
		{
			MESpace* menu = GetSpace("MenuUI");
			if (!sand->IsActive() && menu->IsActive())
			{
				sand->Shutdown();
				sand->Unload();
				std::deque<MESpace*>::iterator pos = std::find(spaces.begin(), spaces.end(), sand);
				spaces.erase(pos);
			}
		}
		if (HasSpace("GameLose"))
		{
			MESpace* lose = GetSpace("GameLose");
			if (!sand->IsActive() && lose->IsActive())
			{
				sand->Shutdown();
				sand->Unload();
				std::deque<MESpace*>::iterator pos = std::find(spaces.begin(), spaces.end(), sand);
				spaces.erase(pos);
			}
		}
		if (HasSpace("GameWin"))
		{
			MESpace* win = GetSpace("GameWin");
			if (!sand->IsActive() && win->IsActive())
			{
				sand->Shutdown();
				sand->Unload();
				std::deque<MESpace*>::iterator pos = std::find(spaces.begin(), spaces.end(), sand);
				spaces.erase(pos);
			}
		}
	}
	if (HasSpace("MenuUI"))
	{
		MESpace* menu = GetSpace("MenuUI");

		if (!menu->IsActive())
		{
			menu->Shutdown();
			menu->Unload();
			std::deque<MESpace*>::iterator pos = std::find(spaces.begin(), spaces.end(), menu);
			spaces.erase(pos);
		}
	}
	if (HasSpace("GameWin"))
	{
		MESpace* win = GetSpace("GameWin");
		if (!win->IsActive())
		{
			win->Shutdown();
			win->Unload();
			std::deque<MESpace*>::iterator pos = std::find(spaces.begin(), spaces.end(), win);
			spaces.erase(pos);
		}
	}
	if (HasSpace("GameLose"))
	{
		MESpace* lose = GetSpace("GameLose");
		if (!lose->IsActive())
		{
			lose->Shutdown();
			lose->Unload();
			std::deque<MESpace*>::iterator pos = std::find(spaces.begin(), spaces.end(), lose);
			spaces.erase(pos);
		}
	}
}
/**
 * shutdown sequence for space manager.
 * 
 */
void MESpaceManager::Shutdown()
{
	for (MESpace* space : spaces)
	{
		//if (space->IsActive())
		//{
			space->Shutdown();
			
			delete space;
			
		//}
	}
}
/**
 * render function for space manager.
 * 
 */
void MESpaceManager::Render()
{
	for (MESpace* space : spaces)
	{
		if (space->IsActive())
		{
			space->Draw();
		}
	}
}
/**
 * gets a space by name.
 * 
 * \param name
 *		the name of the space to find
 * \return 
 *		a pointer to the space if it has been found
 */
MESpace* MESpaceManager::GetSpace(std::string name)
{
	for (MESpace* space : spaces)
	{
		if (name == *space->GetName())
		{
			return space;
		}
	}
	return nullptr;
}
/**
 *	check if space manager has a space.
 * 
 * \param name
 *		name of space to check for
 * \return 
 */
bool MESpaceManager::HasSpace(std::string name)
{
	for (MESpace* space : spaces)
	{
		if (name == *space->GetName())
		{
			return true;
		}
	}
	return false;
}
/**
 * check if a game object is active in the space.
 * 
 * \param gamObj
 *		object to check for
 * \return 
 *		T/F if object is active
 */
bool MESpaceManager::IsActive(GameObjectPtr gamObj)
{
	for (MESpace* space : spaces)
	{
		if (space->IsActive())
		{
			GameObjectManager* activeMan = space->GetActive();
			
			if (activeMan->GetObjectByName(gamObj->GetName()))
			{
				return true;
			}
		}
	}
	return false;
}
/**
 * gets the names of all spaces.
 * 
 * \return 
 *		a vector of strings that stores the names of spaces
 */
std::vector<std::string> MESpaceManager::GetSpaceNames()
{
	std::vector<std::string> temp;
	for (auto space : spaces)
	{
		temp.push_back(*space->GetName());
	}
	return temp;
}
/**
 * refresh all spaces (re-read from json).
 * 
 */
void MESpaceManager::RefreshAll()
{
	std::vector<std::string> names;
	std::vector<bool> activity;
	int i = 0;
	MEEditor::ResetSelected();
	for (MESpace* spac : spaces)
	{
		names.push_back(*spac->GetName());
		activity.push_back(spac->IsActive());
		std::deque<MESpace*>::iterator pos = std::find(spaces.begin(), spaces.end(), spac);
		spaces.erase(pos);
		spac->Shutdown();
		spac->Unload();
		//delete spac;
	}
	for (std::string name : names)
	{
		MESpace* spa = new MESpace(name);
		spa->SetActive(activity[i]);
		spa->Init();
		spa->Load();
		spaces.push_back(spa);
		i++;
	}
}
/// <summary>
/// set the space to active without setting others to inactive
/// </summary>
/// <param name="spaceName"></param>
void MESpaceManager::SetSpaceInclusive(std::string spaceName)
{
	MESpace* spac;

	if (HasSpace(spaceName))
	{
		spac = GetSpace(spaceName);
	}
	else
	{
		spac = new MESpace(spaceName);
		spaces.push_back(spac);
	}

	MEEditor::ResetSelected();

	spac->Init();
	spac->Load();
	spac->SetActive(true);

}
/// <summary>
/// set the space to active and set all others to inactive
/// </summary>
/// <param name="spaceName"></param>
void MESpaceManager::SetSpaceExclusive(std::string spaceName)
{
	if (spaceName == "CreditMenu")
	{
		RefreshAll();
	}

	MESpace* spac;

	if (HasSpace(spaceName))
	{
		spac = GetSpace(spaceName);
	}
	else
	{
		spac = new MESpace(spaceName);
		spaces.push_back(spac);
	}

	for (MESpace* spa : spaces)
	{
		prev = spa;
		spa->SetActive(false);
	}

	MEEditor::ResetSelected();
	spac->Init();
	spac->Load();
	spac->SetActive(true);
}

void MESpaceManager::SetSpacePrevInclusive()
{
	MEEditor::ResetSelected();
	prev->Init();
	prev->Load();
	prev->SetActive(true);
}

void MESpaceManager::SetSpacePrevExclusive()
{
	MESpace* temp;

	for (MESpace* spa : spaces)
	{
		temp = spa;
		spa->SetActive(false);
	}

	MEEditor::ResetSelected();
	prev->Init();
	prev->Load();
	prev->SetActive(true);
}

void MESpaceManager::MoveSpaceToBack(std::string spaceName)
{
	int i = 0;

	for (MESpace* spa : spaces)
	{
		if (*spa->GetName() == spaceName)
		{
			std::deque<MESpace*>::iterator iter = spaces.begin();
			std::advance(iter, i);
			spaces.erase(iter);
			spaces.push_back(spa);
			break;
		}
		++i;
	}
}

void MESpaceManager::Remove(std::string spacename)
{
	MoveSpaceToBack(spacename);
	spaces.pop_back();
}

/**
 * sets pause menu to only active space.
 * 
 */
/*
void MESpaceManager::SetPause()
{
	std::string Pause = "PauseMenu";
	
	if (HasSpace("PauseMenu"))
	{
		MESpace* PauseMenu = GetSpace("PauseMenu");
		for (MESpace* spac : spaces)
		{
			if (spac->IsActive() && *spac->GetName() != Pause)
			{
				prev = spac;
				spac->SetActive(false);
			}
		}
		PauseMenu->Init();
		PauseMenu->Load();
		PauseMenu->SetActive(true);
	}
	else
	{
		MESpace* pauseMenu = new MESpace("PauseMenu");
		for (MESpace* spac : spaces)
		{
			if (spac->IsActive())
			{
				prev = spac;
				spac->SetActive(false);
			}
		}
		spaces.push_back(pauseMenu);
		pauseMenu->SetActive(true);
		pauseMenu->Init();
		pauseMenu->Load();
	}
}
*/
/**
 * sets sanbox to only active space.
 * 
 */
/*
void MESpaceManager::SetSandbox()
{
	for (MESpace* space : spaces)
	{
		if (HasSpace("PauseMenu"))
		{
			MESpace* pause = GetSpace("PauseMenu");
			pause->SetActive(false);
		}

		prev = space;
		space->SetActive(false);

		if (HasSpace("Sandbox"))
		{
			MESpace* sandbox = GetSpace("Sandbox");
			sandbox->Init();
			sandbox->Load();
			sandbox->SetActive(true);
		}
		else
		{
			MESpace* sandbox = new MESpace("Sandbox");
			spaces.push_back(sandbox);
			sandbox->Init();
			sandbox->Load();
			sandbox->SetActive(true);
		}
	}
}
*/
/**
 * sets main menu to only active space.
 * 
 */
/*
void MESpaceManager::SetMenuUI()
{
	for (MESpace* space : spaces)
	{
		if (HasSpace("PauseMenu"))
		{
			MESpace* pause = GetSpace("PauseMenu");
			pause->SetActive(false);
		}

		prev = space;
		space->SetActive(false);

		if (HasSpace("MenuUI"))
		{
			MESpace* Menu = GetSpace("MenuUI");
			Menu->Init();
			Menu->Load();
			Menu->SetActive(true);
		}
		else
		{
			MESpace* Menu = new MESpace("MenuUI");
			spaces.push_back(Menu);
			Menu->Init();
			Menu->Load();
			Menu->SetActive(true);
		}
	}
}
*/
/**
 * sets game over to only active space.
 *
 */
/*
void MESpaceManager::SetGameLose()
{
	for (MESpace* space : spaces)
	{
		//if (HasSpace("GameOver"))
		//{
		//	MESpace* pause = GetSpace("GameOver");
		//	pause->SetActive(false);
		//}

		prev = space;
		space->SetActive(false);

		if (HasSpace("GameLose"))
		{
			MESpace* go = GetSpace("GameLose");
			go->Init();
			go->Load();
			go->SetActive(true);
		}
		else
		{
			MESpace* go = new MESpace("GameLose");
			spaces.push_back(go);
			go->Init();
			go->Load();
			go->SetActive(true);
		}
	}
}
*/
/**
 * sets game over to only active space.
 *
 */
/*
void MESpaceManager::SetGameWin()
{
	for (MESpace* space : spaces)
	{

		prev = space;
		space->SetActive(false);

		if (HasSpace("GameWin"))
		{
			MESpace* go = GetSpace("GameWin");
			go->Init();
			go->Load();
			go->SetActive(true);
		}
		else
		{
			MESpace* go = new MESpace("GameWin");
			spaces.push_back(go);
			go->Init();
			go->Load();
			go->SetActive(true);
		}
	}
}
*/



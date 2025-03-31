/*********************************************************************************************************************/
/*!
  \file              GameObject.cpp
 * \authors       Nate White, Ethan Mclaughlin,Nick Leben
 * \par          Email: Nate.white\@digipen.edu, EthanMclaughlin\@digipen.edu, nick.leben\@digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.
  \par				 credit to: https://www.youtube.com/watch?v=dZr-53LAlOw (thecherno on youtube)

  \brief
	this implements the game object class
 */
 /*********************************************************************************************************************/


/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include "Log.h"
#include "GameObject.h"

#include <filesystem>

#include "MESerializer.h"
#include "Sprite.h"
#include "rapidjson/document.h"
#include "Component.h"
#include "Animation.h"
#include "Audio.h"
#include "BackgroundBehavior.h"
#include "Transform.h"
#include "Collider.h"
#include "MEAudio.h"
#include "Physics.h"
#include "Stats.h"
#include "TowerBehavior.h"
#include "BehaviorButton.h"
#include "BehaviorCoinButton.h"
#include "BehaviorPlayButton.h"
#include "BehaviorSpawnButton.h"
#include "ZeppelinBehavior.h"
#include "CityBehavior.h"
#include "CannonBehavior.h"
#include "BehaviorRoundTimer.h"
#include "BehaviorGoldCount.h"
#include "BehaviorHealthBar.h"
#include "BehaviorTutorialButton.h"
#include "BehaviorZeppelinSelectButton.h"
#include "BehaviorMenuButton.h"
#include "MEEditor.h"
#include "BehaviorUpgradeDButton.h"
#include "BehaviorResumeButton.h"
#include "BehaviorBackButton.h"
#include "BehaviorStar_Programmer.h"
#include "ParticleSystem.h"
#include "BehaviorEmitterTest.h"
#include "BehaviorVolumeSlider.h"
#include "EmitterBehavior.h"
#include "BehaviorEmitterExplosion.h"
#include "BehaviorEmitterDeath.h"
#include "BehaviorEmitterKey.h"
#include "BehaviorStar_Programer_Results.h"
#include "BehaviorTimer_Results.h"
#include "BehaviorFPSDisplay.h"
#include "BehaviorCreditsButton.h"
#include "BehaviorQuitButton.h"
#include "BehaviorYesButton.h"
#include "BehaviorNoButton.h"
#include "BehaviorBlackSquare.h"
#include "Mesh.h"
#include "BehaviorSettingsButton.h"
#include "BehaviorRetryButton.h"
#include "BehaviorScreenToggle.h"
#include "BehaviorVolumeToggle.h"
#include "BehaviorCreditReturnButton.h"
#include "BehaviorMainConfirmButton.h"
#include "Engine.h"
#pragma warning(disable:4996)


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
using namespace rapidjson;
/*!********************************************************************************************************************
	\brief
		default constructor for game object
**********************************************************************************************************************/
GameObject::GameObject()

	:name("\0"), Parent(nullptr), id(0), children(0)
{
	isDestroyed = false;
	hidden = false;
	//perhaps set AKID here?
	//for all components in component array, set to NULL
	for (unsigned i = 0; i < cCount; ++i)
	{
		components[i] = nullptr;
	}
}

/**
 * \brief 
 * destructor
 */
GameObject::~GameObject()
{
	if(components)
	{
		for (Component* cmp : components)
		{
			if (cmp)
			{
				delete cmp;
				cmp = nullptr;
			}
		}
	}

	for (GameObject* child : children)
	{
		if (child)
		{
			delete child;
			child = nullptr;
		}
	}
	name.clear();
	isDestroyed = NULL;

}
/// <summary>
/// copy constructor
/// </summary>
/// <param name="cpy"></param>
GameObject::GameObject(const GameObjectPtr& cpy)
{
	Parent = cpy->GetParent();
	SetName(cpy->name);

	for (unsigned i = 0; i < cCount; ++i)
	{
		components[i] = nullptr;
	}

	for(int i = 0; i < cCount; ++i)
	{
		if (cpy->components[i])
		{
			if (this->HasParent() && cpy->components[i]->Type() == cTransform && !cpy->HasComponent(cEmitter))
			{
				
				//transform of the og to get the relative translation and get the scale
				Transform ogTrans = *static_cast<Transform*>(cpy->components[i]);

				//new transform to put in the child components array
				Transform* newTransform = new Transform;
				newTransform->Parent(this);
				//new transform need the scale of the ogTransform
				newTransform->SetScale(ogTrans.GetScale());
				//new transform needs the old relative translation from og transform
				newTransform->SetTranslation(ogTrans.GetRelativeTran());
				//add new transfrom to the child component array
				components[i] = newTransform;

			}
			else
			{
				components[i] = cpy->components[i]->Clone();
			}
			components[i]->Parent(this);
		}
	}

	//this is mainly for the particle system
	for(ComponentPtr cmp : components)
	{
		if(cmp) cmp->Init();
	}

	for (GameObject* child : cpy->children)
	{
		GameObject* newChild = new GameObject(child);
		newChild->Parent = this;
		children.push_back(newChild);
	}
	isDestroyed = cpy->isDestroyed;
	hidden = false;
}
/*!********************************************************************************************************************
	\brief
		clone a game object
	\return
		GameObjectPtr to the clone
**********************************************************************************************************************/
GameObjectPtr GameObject::Clone()
{
	GameObjectPtr clone = new GameObject;

	clone->isDestroyed = this->isDestroyed;
	clone->SetName(this->name);
	clone->SetParent(this->Parent);

	for (ComponentPtr component : components)
	{
		if (component)
		{
			if (this->HasParent() && component->Type() == cTransform)
				{
					/*Transform* oldTran = dynamic_cast<Transform*>(cpy->components[i]);
					Transform newTran = *dynamic_cast<Transform*>(cpy->components[i]->Clone());
					newTran.SetTranslation(oldTran->GetRelativeTran());
					components[i] = &newTran;*/

					//transform of the og to get the relative translation and get the scale
					Transform ogTrans = *static_cast<Transform*>(component);

					//new transform to put in the child components array
					Transform* newTransform = new Transform;
					newTransform->Parent(this);
					//new transform need the scale of the ogTransform
					newTransform->SetScale(ogTrans.GetScale());
					//new transform needs the old relative translation from og transform
					newTransform->SetTranslation(ogTrans.GetRelativeTran());
					//add new transfrom to the child component array
					component = newTransform;

				}
			else
			{
				clone->Add(component->Clone());
			}
		}
	}

	for (GameObject* child : children)
	{
		clone->children.push_back(child->Clone());
	}

	return clone;
}
/*!********************************************************************************************************************
	\brief
		adds a component to the game object
	\param component
		pointer to the component to be added to the gameObject
**********************************************************************************************************************/
void GameObject::Add(ComponentPtr component)
{
	if (component)
	{
		component->Parent(this);

 		components[component->Type()] = component;
	}
}/*!********************************************************************************************************************
	\brief
		finds a component in a game object
	\param component
		pointer to the component to find
	\return
		Pointer to the found component (NULL if component is not found)
**********************************************************************************************************************/
ComponentPtr GameObject::Find(ComponentPtr component)
{
	for (ComponentPtr comp : components)
	{
		if (comp == component)
		{
			return comp;
		}
	}
	return NULL;
}

/*!********************************************************************************************************************
	\brief
		updates the component
	\param dt
		delta time
**********************************************************************************************************************/
void GameObject::Update(float dt)
{
	for (ComponentPtr component : components)
	{
		if (component)
		{
			if (MEEditor::IsOpen())
			{
				if (component->Type() != cBehavior && component->Type() != cPhysics/* && component->Type() != cAudio && component->Type() != cCollider*/)
				{
					component->Update(dt);
				}
			}
			else
			{
				component->Update(dt);
				if (component->Type() == cSprite)
				{
					if (HasParent() && name != "ZeppelinExplosion")
					{
						Sprite* sprit = static_cast<Sprite*>(component);
						sprit->SetAlpha(GetParent()->Has(Sprite)->GetAlpha());
					}
				}
			}
		}
	}
	if (IsParent())
	{
		for (GameObject* gam : children)
		{
			gam->Update(dt);
		}
	}
}
/*!********************************************************************************************************************
	\brief
		draws the components
**********************************************************************************************************************/
void GameObject::Draw()
{
	for (ComponentPtr component : components)
	{
		if (component)
		{
			component->Draw();
		}
	}
	if (IsParent())
	{
		for (GameObject* gam : children)
		{
			gam->Draw();
			
		}
	}
}
/// <summary>
/// checks if game object has type component
/// </summary>
/// <param name="type"></param>
/// <returns></returns>
bool GameObject::HasComponent(TypeEnum type)
{
	if (components[type])
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/*!********************************************************************************************************************
	\brief
		gets a component from the game object
	\param type
		the enum for the component to get
	\return
		pointer to the component
**********************************************************************************************************************/
ComponentPtr GameObject::Get(TypeEnum type)
{
	if (components[type])
	{
		return components[type];
	}
	return nullptr;
}

/*!********************************************************************************************************************
	\brief
		checks if the game object is destroyed
	\return
		boolean corresponding to the status of destruction
**********************************************************************************************************************/
bool GameObject::Destroyed()
{
	return isDestroyed;
}
/*!********************************************************************************************************************
	\brief
		sets game objects destroyed to true
**********************************************************************************************************************/
void GameObject::Destroy()
{
	isDestroyed = true;
}
/*!********************************************************************************************************************
	\brief
		reads a game object from a json
	\param path
		the path for the json to be read from
**********************************************************************************************************************/
void GameObject::Read(const char* path)
{
	std::string json;
	std::string clearData(json);
	json = MESerializer::OpenFileRead(path);
	const char* bufchar = json.c_str();
	Document d;
	d.Parse(bufchar);
	//load each name, audioID, etc. into each game object
	const rapidjson::Value& value = d["GameObject"];
	const char* name_ = value["name"].GetString();
	this->name = name_;

	Sprite* sprite = new Sprite;
	if (d["GameObject"].HasMember("Sprite"))
	{
		Add(sprite);
		sprite->Read(bufchar);
	}

	if (d["GameObject"].HasMember("Transform"))
	{
		TransformPtr transf = new Transform;
		Add(transf);
		transf->Read(bufchar);
		
		if (HasParent())
		{
			//glm::vec3 newTran = transf->Get
			// ()  - Parent->Has(Transform)->GetTranslation();
			//transf->SetTranslation(newTran);
		}
	}

	if (d["GameObject"].HasMember("Animation"))
	{
		AnimationPtr animation = new Animation;
		Add(animation);
		animation->Read(bufchar);
	}
	if (d["GameObject"].HasMember("Physics"))
	{
		Physics* physics = new Physics;
		physics->Read(bufchar);
		Add(physics);
	}

	if (d["GameObject"].HasMember("Collider"))
	{
		ColliderPtr collider = new Collider;
		Add(collider);
		collider->Read(bufchar);
	}
	if (d["GameObject"].HasMember("Stats"))
	{
		Stats* stats = new Stats;
		Add(stats);
		stats->Read(bufchar);
	}
	if (d["GameObject"].HasMember("Audio"))
	{
		Audio* audio = new Audio;
		Add(audio);
	}
	if (d["GameObject"].HasMember("Emitter"))
	{
		Emitter* emitter = new Emitter;
		Add(emitter);
		emitter->Read(bufchar);
		emitter->Init();
	}
	if (d["GameObject"].HasMember("EmitterBehavior"))
	{
		const Value& ghead = d["GameObject"];
		const Value& head = ghead["EmitterBehavior"];
		const char* string = head.GetString();
		if (strstr(string, "EmitterTest"))
		{
			EmitterBehaviorPtr behavior = new EmitterTest;
			behavior->Name = "EmitterTest";
			Add(behavior);
		}
		else if (strstr(string, "CannonHit"))
		{
			EmitterBehaviorPtr behavior = new EmitterExplosion;
			behavior->Name = "EmitterExplosion";
			Add(behavior);
		}
		else if (strstr(string, "OnDeath"))
		{
			EmitterBehaviorPtr behavior = new EmitterDeath;
			behavior->Name = "EmitterDeath";
			Add(behavior);
		}
		else if (strstr(string, "EmitterKey"))
		{
			EmitterBehaviorPtr behavior = new EmitterKey;
			behavior->Name = "EmitterKey";
			Add(behavior);
		}
	}
	if (d["GameObject"].HasMember("Behavior"))
	{
		const Value& e = d["GameObject"];
		//go into the transform section
		const Value& value = e["Behavior"];
		//set a to the type
		const Value& a = value["Name"];
		/*TODO make this able to do an array of names
		 * would require a behavior manager/object would store a vector of behaviors
		 */

		const char* string = a.GetString();
		
		//after its created do behavior read || i need to create read virtual function once i understand why i need to make one
		if (strstr(string, "Tower"))
		{
			//Spawn Pos
			if (strstr(string, "Wood"))
			{
				BehaviorPtr behavior = new Tower(cTowerTypeWood);
				behavior->Name = "WoodTower";
				Add(behavior);
			}
			else if (strstr(string, "Stone"))
			{
				BehaviorPtr behavior = new Tower(cTowerTypeStone);
				behavior->Name = "StoneTower";
				Add(behavior);
			}

		}
		else if (strstr(string, "Zeppelin"))
		{
			if (strstr(string, "TypeS"))
			{
				BehaviorPtr behavior = new Zeppelin(cZeppelinTypeS);
				behavior->Name = "ZeppelinS";
				Add(behavior);
			}
			if (strstr(string, "TypeM"))
			{
				BehaviorPtr behavior = new Zeppelin(cZeppelinTypeM);
				behavior->Name = "ZeppelinM";
				Add(behavior);
			}
			if (strstr(string, "TypeL"))
			{
				BehaviorPtr behavior = new Zeppelin(cZeppelinTypeL);
				behavior->Name = "ZeppelinL";
				Add(behavior);
			}


	
		}
		else if (strstr(string, "Cannon"))
		{
			BehaviorPtr behavior = new Cannon;
			behavior->Name = "Cannon";
			Add(behavior);
		}
		else if (strstr(string, "NewUITimer"))
		{
			BehaviorPtr behavior = new RoundTimer;
			behavior->Name = "NewUITimer";
			Add(behavior);
		}
		else if (strstr(string, "GoldCount"))
		{
			BehaviorPtr behavior = new GoldCounter();
			behavior->Name = "GoldCount";
			Add(behavior);
			//ME_CORE_INFO("GOLD");
		}
		else if (strstr(string, "TimerResult"))
		{
			BehaviorPtr behavior = new TimerResult();
			behavior->Name = "TimerResult";
			Add(behavior);
			//ME_CORE_INFO("GOLD");
		}
		else if (strstr(string, "VolumeSlider"))
		{
			BehaviorPtr behavior = new VolumeSlider();
			behavior->Name = "VolumeSlider";
			Add(behavior);
		}
		else if (strstr(string, "VolumeToggle"))
		{
			BehaviorPtr behavior = new VolumeToggle();
			behavior->Name = "VolumeToggle";
			Add(behavior);
		}
		else if (strstr(string, "ScreenToggle"))
		{
			BehaviorPtr behavior = new ScreenToggle();
			behavior->Name = "ScreenToggle";
			Add(behavior);
		}
		else if (strstr(string, "FPSDisplay"))
		{
			BehaviorPtr behavior = new FPSDisplay();
			behavior->Name = "FPSDisplay";
			Add(behavior);
		}
		//UPGRADE
		else if (strstr(string, "Upgrade"))
		{
			//Spawn Pos
			if (strstr(string, "D"))
			{
				BehaviorPtr beh = new UpgradeButton(cUpgrDamage);
				Add(beh);
			}
			else if (strstr(string, "H"))
			{
				BehaviorPtr beh = new UpgradeButton(cUpgrHealth);
				Add(beh);
			}
			else if (strstr(string, "S"))
			{
				BehaviorPtr beh = new UpgradeButton(cUpgrSpeed);
				Add(beh);
			}

		}
		//Check for button
		else if (strstr(string, "Button"))
		{
			//Check for type of button
			//MENUS
			if (strstr(string, "Start"))
			{
				BehaviorPtr beh = new PlayButton;
				Add(beh);
			}
			else if (strstr(string, "CreditReturn"))
			{
				Behavior* beh = new CreditReturn;
				Add(beh);
			}
			else if (strstr(string, "SettingsButton"))
			{
				Behavior* behavior = new SettingsButton;
				Add(behavior);
			}
			else if (strstr(string, "Tut"))
			{
				BehaviorPtr beh = new TutorialButton;
				Add(beh);
			}
			else if (strstr(string, "Yes"))
			{
				BehaviorPtr beh = new YesButton;
				Add(beh);
			}
			else if (strstr(string, "No"))
			{
				BehaviorPtr beh = new NoButton;
				Add(beh);
			}
			else if (strstr(string, "Quit"))
			{
				BehaviorPtr beh = new QuitButton;
				Add(beh);
			}
			/*else if (strstr(string, "Exit"))
			{
				BehaviorPtr beh = new ExitButton();
				Add(beh);
			}*/
			else if (strstr(string, "Coin"))
			{
				BehaviorPtr beh = new CoinButton;
				Add(beh);
			}
			else if (strstr(string, "MainConfirm"))
			{
				Behavior* beh = new MainConfirm;
				Add(beh);
			}
			else if (strstr(string, "ToMainMenu"))
			{
				BehaviorPtr beh = new MenuButton;
				Add(beh);
			}
			else if (strstr(string, "ToCredits"))
			{
				BehaviorPtr beh = new CreditButton;
				Add(beh);
			}
			else if (strstr(string, "Resume"))
			{
				BehaviorPtr beh = new ResumeButton;
				Add(beh);
			}
			else if (strstr(string, "BackButton"))
			{
				BehaviorPtr beh = new BackButton;
				Add(beh);
			}
			else if (strstr(string, "Retry"))
			{
				Behavior* beh = new RetryButton;
				Add(beh);
			}
			//SPAWNS
			else if (strstr(string, "Spawn"))
			{
				//Spawn Pos
				if (strstr(string, "Top"))
				{
					BehaviorPtr beh = new SpawnButton(cSpawnTop);
					Add(beh);
				}
				else if (strstr(string, "Mid"))
				{
					BehaviorPtr beh = new SpawnButton(cSpawnMid);
					Add(beh);
				}
				else if (strstr(string, "Bot"))
				{
					BehaviorPtr beh = new SpawnButton(cSpawnBot);
					Add(beh);
				}

			}
			
			else if (strstr(string, "Select"))
			{
				if (strstr(string, "Sm"))
				{
					BehaviorPtr beh = new ZeppelinSelectButton(cZeppelinTypeS);
					Add(beh);
				}
				else if (strstr(string, "Med"))
				{
					BehaviorPtr beh = new ZeppelinSelectButton(cZeppelinTypeM);
					Add(beh);
				}
				else if (strstr(string, "Large"))
				{
					BehaviorPtr beh = new ZeppelinSelectButton(cZeppelinTypeL);
					Add(beh);
				}
			}
		}
		//CHECK for Backgrounds
		else if (strstr(string, "Background"))
		{
			//Which background
			if (strstr(string, "Main"))
			{
				BehaviorPtr beh = new Background(cMainMenuBackground, string);
				Add(beh);
			}
			else if (strstr(string, "Digi"))
			{
				Behavior* beh = new Background(cDigiBackground, string);
				Add(beh);
			}
			else if (strstr(string, "Game"))
			{
				BehaviorPtr beh = new Background(cGameBackground, string);
				Add(beh);
			}
			else if (strstr(string, "Pause"))
			{
				BehaviorPtr beh = new Background(cPauseMenuBackground, string);
				Add(beh);
			}
			else if (strstr(string, "Win"))
			{
				BehaviorPtr beh = new Background(cWinMenuBackground, string);
				Add(beh);
			}
			else if (strstr(string, "Lose"))
			{
				BehaviorPtr beh = new Background(cLoseMenuBackground, string);
				Add(beh);
			}
			else if (strstr(string, "Credit"))
			{
				Behavior* beh = new Background(cCreditMenuBackground, string);
				Add(beh);
			}
		}
		//check for city
		else if (strstr(string, "City"))
		{
			BehaviorPtr behavior = new City;
			behavior->Name = "Tower";
			Add(behavior);
		}

		//Star check
		else if (strstr(string, "Star"))
		{
			if (strstr(string, "_Programer_Results"))
			{
				BehaviorPtr behavior = new StarResult;
				Add(behavior);
			}
			else
			{
				if (d["GameObject"].HasMember("Time"))
				{
					const Value& Tghead = d["GameObject"];
					const Value& Thead = Tghead["Time"];

					BehaviorPtr behavior = new Star(static_cast<float>(Thead.GetInt()));
					Add(behavior);
					//const char* Tstring = Thead.GetString();
				}
				else
				{
					BehaviorPtr behavior = new Star(30.0);
					Add(behavior);
				}
				//put a timer lenght in the json, then use that to set how long the timer is.
				//static_cast<Star*>(behavior)->;
			}
		}

		//mock health bar for designers
		else if(strstr(string, "healthbar"))
		{
			//creating a behavior component
			BehaviorPtr behavior = new HealthBar;
			//adding the new component to the game object
			Add(behavior);
		}

		else if (strstr(string, "BlackSquare"))
		{
			BehaviorPtr behavior = new BehaviorBlackSquare;
			Add(behavior);
		}
		

	}
	if (d["GameObject"].HasMember("Children"))
	{
		const rapidjson::Value& head = value["Children"];
		std::vector<std::string> result(head.Size());
		for (rapidjson::SizeType i = 0; i < head.Size(); ++i)
		{
			result[i] = head[i].GetString();
		}
		for (const std::string obj : result)
		{
			GameObject* newObj = ChildBuild(obj.c_str());
			children.push_back(newObj);
		}
	}
	/* read the name from the json
	* 
	set the name of the object

	loop through and have a case for each component

	in each case make a new variable of the component

	read the component using the components corresponding read function

	add the component to the game object using the components add function

	(in some cases set the parent)*/
}

void GameObject::Save()
{
	for (auto cmp : components)
	{
		cmp->Save();
	}
}


/// <summary>
/// does nothing
/// </summary>
void GameObject::Write()
{

}

/*!********************************************************************************************************************
	\brief
		gets the name of the game object
	\return
		string of the game object
**********************************************************************************************************************/
std::string GameObject::GetName()
{
	return name;
}

/**
 * \brief Gets the ID of the game object
 * \return id
 */
int GameObject::GetID()
{
	return id;
}
/// <summary>
/// set id of object
/// </summary>
/// <param name="_id"></param>
void GameObject::SetID(int _id)
{
	if(_id >= 0)
	{
		id = _id;
	}
}


/*!********************************************************************************************************************
	\brief
		compares the game object name to the input
	\param input
		string of the name to be compared to
	\return
		boolean corresponding to the status of the comparison
**********************************************************************************************************************/
bool GameObject::IsNamed(std::string input)
{
	if (input == name)
	{
		return true;
	}
	else
	{
		return false;
	}

}
/*!********************************************************************************************************************
	\brief
		this does nothing
**********************************************************************************************************************/
/*void GameObject::load()
{
	std::string json;
	json = MESerializer::OpenFileRead("../Assets/test.json");
	const char* bufchar = json.c_str();
	Document d;
	d.Parse(bufchar);
	//load each name, audioID, etc. into each game object
 	const char* name = d["name"].GetString();
	std::string s(name);
	GameObject::setName(name);
	
}*/
/*!********************************************************************************************************************
	\brief
		sets the name of the game object
	\param input
		the string to set the name of the game object to
**********************************************************************************************************************/
void GameObject::SetName(std::string input)
{
	name = input;
	ME_CORE_INFO(name);
}
/**
 * Builds child object.
 * 
 * \param objectName
 *		name of child to build
 * \return 
 *	pointer to the child object
 */
GameObject* GameObject::ChildBuild(const char* objectName)
{
	if (objectName)
	{
		GameObject* gamObj = GetChildByName(objectName);

		if (!gamObj)
		{
			char pathName[FILENAME_MAX] = "";

#ifdef _DEBUG
			sprintf_s(pathName, _countof(pathName), Engine::MAYHEM_DIR("\\Assets\\GameObjects\\Children\\%s.json").c_str(), objectName);
#endif // _DEBUG

#ifdef _DISTRIBUTE
			sprintf_s(pathName, _countof(pathName), Engine::MAYHEM_DIR_BASE("\\Assets\\GameObjects\\Children\\%s.json").c_str(), objectName);
#endif // _DISTRIBUTE

			//sprintf_s(pathName, _countof(pathName), "../Assets/GameObjects/%s.json", objectName);
			/*
			std::string json;
			std::string clearData(json);
			json = MESerializer::OpenFileRead(pathName);
			const char* bufchar = json.c_str(); 
			rapidjson::Document d;
			d.Parse(bufchar);
			*/

			gamObj = new GameObject;
			gamObj->SetParent(this);
			gamObj->Read(pathName);
			
			return gamObj;
		}
		else if (gamObj)
		{
			GameObject* gamClone = new GameObject(gamObj);

			return gamClone;
		}
	}
	return NULL;
}
/**
 * gets the child with the specified name.
 * 
 * \param name
 *		name of the child to get
 * \return 
 *		pointer to the child with the corresponding name
 */
GameObject* GameObject::GetChildByName(std::string name)
{
	for (GameObject* child : children)
	{
		if (name == child->GetName())
		{
			return child;
		}
	}
	return NULL;
}
/// <summary>
/// search for a child with given name
/// </summary>
/// <param name="query"></param>
/// <returns>pointer to child</returns>

GameObject* GameObject::SearchChildByName(std::string query)
{
	for (GameObject* child : children)
	{
		if (strstr(child->GetName().c_str(), query.c_str() ))
		{
			return child;
		}
	}
	return NULL;
}
/// <summary>
/// get the parent of this child
/// </summary>
/// <returns>pointer to parent</returns>
GameObject* GameObject::GetParent()
{
	return Parent;
}
/// <summary>
/// get the children of this parent
/// </summary>
/// <returns>deque of all children</returns>
std::deque<GameObject*>& GameObject::GetChildren()
{
	/*std::deque<GameObject*> objects;
	for(auto obj : children)
	{
		objects.push_back(obj);
	}
	return objects;*/

	return children;
}
/// <summary>
/// set the parent of this now child
/// </summary>
/// <param name="parent"></param>
void GameObject::SetParent(GameObject* parent)
{
	Parent = parent;
}
/// <summary>
/// check if this object has a parent (or rather if this object is a child)
/// </summary>
/// <returns></returns>
bool GameObject::HasParent()
{
	if (Parent)
	{
		return true;
	}
	else
	{
		return false;
	}
}
/// <summary>
/// check if this object has children (or is a parent)
/// </summary>
/// <returns></returns>
bool GameObject::HasChildren()
{
	if(!children.empty())
	{
		return true;
	}

	return false;
}
/// <summary>
/// check if object is a parent
/// </summary>
/// <returns></returns>
bool GameObject::IsParent()
{
	for (GameObject* child : children)
	{
		if (child)
		{
			return true;
		}
	}
	return false;
}
/// <summary>
/// checks if an object is its own parent or child
/// </summary>
/// <returns></returns>
bool GameObject::IsTimeTraveler()
{
	if (this == this->Parent)
	{
		return true;
	}
	else
	{
		if (IsParent())
		{
			for (GameObject* obj : children)
			{
				if (obj->GetParent() == this)
				{
					return true;
				}
			}
		}
	}
	return false;
}
/// <summary>
/// add a child to this object
/// </summary>
/// <param name="child"></param>
void GameObject::AddChild(GameObject* child)
{
	if (child)
	{
		child->SetParent(this);
		children.push_back(child);
	}
}
/// <summary>
/// gets the amount of children this parent has
/// </summary>
/// <returns>int of children amount</returns>
int GameObject::GetChildCount()
{
	return children.size();
}
/// <summary>
/// get all children names
/// </summary>
/// <returns>vector of strings of childrens names</returns>
std::vector<std::string> GameObject::GetChildNames()
{

	std::vector<std::string> childNames;
	for (auto child : children)
	{
		childNames.push_back(child->name);
	}
	return childNames;
}

void GameObject::SetHidden(bool hide)
{
	hidden = hide;
}

std::vector<std::string> GameObject::GetChildrenFromFile(std::string filename)
{
	std::string pathName;

#ifdef _DEBUG
	if (Engine::s_UnitTesting)
	{
		pathName.insert(0, Engine::MAYHEM_DIR_PARENT("\\Assets\\GameObjects\\"));

	}
	else
	{
		pathName.insert(0, Engine::MAYHEM_DIR("\\Assets\\GameObjects\\"));
	}
#endif // _DEBUG


#ifdef _DISTRIBUTE
	pathName = "./Assets/GameObjects/" + filename ".json";
#endif

	pathName += filename + ".json";

	std::string json;
	std::string clearData(json);
	json = MESerializer::OpenFileRead(pathName.c_str());
	const char* bufchar = json.c_str();
	Document d;
	d.Parse(bufchar);
	//load each name, audioID, etc. into each game object
	const rapidjson::Value& value = d["GameObject"];

	if (d["GameObject"].HasMember("Children"))
	{
		const rapidjson::Value& head = value["Children"];
		std::vector<std::string> result;
		for (rapidjson::SizeType i = 0; i < head.Size(); ++i)
		{
			result.push_back(head[i].GetString());
		}
		return result;
	}
	return std::vector<std::string>() = {};
}

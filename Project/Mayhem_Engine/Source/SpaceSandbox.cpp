
#include "SpaceSandbox.h"
#include "Log.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameObjectManager.h"
#include "ZeppelinBehavior.h"
#include "BehaviorTower.h"
#include "BehaviorButton.h"

void SpaceSandbox::Load()
{
	
}

void SpaceSandbox::Init()
{
	GameObjectManager* activeList = this->GetActive();
	GameObjectManager* InactiveList = this->GetInactive();
	activeList->Init();
	InactiveList->Init();

	GameObjectPtr background = activeList->FactoryBuild("Background");

	GameObjectPtr gamObj = activeList->FactoryBuild("Zepplin1");

	BehaviorPtr test = ZeppelinCreate();
	gamObj->Add(test);

	GameObjectPtr TowerShell = activeList->FactoryBuild("Tower");
	BehaviorPtr towerBehavior = TowerCreate();
	TowerShell->Add(towerBehavior);

	GameObjectPtr ButtonShell = activeList->FactoryBuild("Button");
	BehaviorPtr buttonBehavior = ButtonCreate();
	ButtonShell->Add(buttonBehavior);

	//AnimationPtr animation = new Animation;
	//gamObj->Add(animation);

	GameObjectPtr gamObj1 = activeList->FactoryBuild("Zepplin2");
	BehaviorPtr butt = ButtonCreate();
	gamObj1->Add(butt);
}

void SpaceSandbox::Update(float dt)
{
	this->GetActive()->Update(dt);
}

void SpaceSandbox::Draw()
{
	this->GetActive()->Draw();
}

void SpaceSandbox::Shutdown()
{
	this->GetActive()->Shutdown();
	this->GetInactive()->Shutdown();
}

void SpaceSandbox::Unload()
{

}

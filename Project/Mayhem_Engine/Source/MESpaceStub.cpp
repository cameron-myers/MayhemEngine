

#include "MESpaceStub.h"
#include "Log.h"

void MESpaceStub::Load()
{

}

void MESpaceStub::Init()
{
	GameObjectManager* active = this->GetActive();
	GameObjectManager* Inactive = this->GetInactive();

	active->Init();
	Inactive->Init();
}

void MESpaceStub::Update(float dt)
{
	this->GetActive()->Update(dt);
}

void MESpaceStub::Draw()
{
	this->GetActive()->Draw();
}

void MESpaceStub::Shutdown()
{
	this->GetActive()->Shutdown();
	this->GetInactive()->Shutdown();
}

void MESpaceStub::Unload()
{

}

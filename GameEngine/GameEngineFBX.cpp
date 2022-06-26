#include "PreCompile.h"
#include "GameEngineFBX.h"

GameEngineFBX::GameEngineFBX()
{

}

GameEngineFBX::~GameEngineFBX()
{
	if (nullptr != Manager)
	{
		Manager->Destroy();
	}
}

void GameEngineFBX::Load()
{
	Manager = fbxsdk::FbxManager::Create();
}


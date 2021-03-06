#include "PreCompile.h"
#include "GameEngineRasterizerManager.h"
#include "GameEngineRasterizer.h"

GameEngineRasterizerManager* GameEngineRasterizerManager::Inst = new GameEngineRasterizerManager();

GameEngineRasterizerManager::GameEngineRasterizerManager() // default constructer 디폴트 생성자
{

}

GameEngineRasterizerManager::~GameEngineRasterizerManager() // default destructer 디폴트 소멸자
{
	for (const std::pair<std::string, GameEngineRasterizer*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineRasterizerManager::GameEngineRasterizerManager(GameEngineRasterizerManager&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}



GameEngineRasterizer* GameEngineRasterizerManager::Create(const std::string& _Name, const D3D11_RASTERIZER_DESC& _RasterizerDesc)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineRasterizer* FindRes = Find(UpperName);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(UpperName + " Is Overlap Create");
	}


	GameEngineRasterizer* NewRes = new GameEngineRasterizer();
	NewRes->SetName(UpperName);
	NewRes->Create(_RasterizerDesc);

	// 그리고 뭘할거냐?

	ResourcesMap.insert(std::map<std::string, GameEngineRasterizer*>::value_type(UpperName, NewRes));
	return NewRes;
}

GameEngineRasterizer* GameEngineRasterizerManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

GameEngineRasterizer* GameEngineRasterizerManager::Load(const std::string& _Name, const std::string& _Path)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineRasterizer* FindRes = Find(UpperName);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineRasterizer* NewRes = new GameEngineRasterizer();
	NewRes->SetName(UpperName);


	ResourcesMap.insert(std::map<std::string, GameEngineRasterizer*>::value_type(UpperName, NewRes));
	return NewRes;
}

GameEngineRasterizer* GameEngineRasterizerManager::Find(const std::string& _Name)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	std::map<std::string, GameEngineRasterizer*>::iterator FindIter = ResourcesMap.find(UpperName);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}
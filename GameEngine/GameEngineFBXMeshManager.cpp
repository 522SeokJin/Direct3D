#include "PreCompile.h"
#include "GameEngineFBXMeshManager.h"
#include "GameEngineFBXMesh.h"

GameEngineFBXMeshManager* GameEngineFBXMeshManager::Inst = new GameEngineFBXMeshManager();

GameEngineFBXMeshManager::GameEngineFBXMeshManager() // default constructer ����Ʈ ������
{

}

GameEngineFBXMeshManager::~GameEngineFBXMeshManager() // default destructer ����Ʈ �Ҹ���
{
	for (const std::pair<std::string, GameEngineFBXMesh*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineFBXMeshManager::GameEngineFBXMeshManager(GameEngineFBXMeshManager&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

GameEngineFBXMesh* GameEngineFBXMeshManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

GameEngineFBXMesh* GameEngineFBXMeshManager::Load(const std::string& _Name, const std::string& _Path)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineFBXMesh* FindRes = Find(UpperName);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineFBXMesh* NewRes = new GameEngineFBXMesh();
	NewRes->SetName(UpperName);
	NewRes->Load(_Path);

	ResourcesMap.insert(std::map<std::string, GameEngineFBXMesh*>::value_type(UpperName, NewRes));
	return NewRes;
}

GameEngineFBXMesh* GameEngineFBXMeshManager::Find(const std::string& _Name)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	std::map<std::string, GameEngineFBXMesh*>::iterator FindIter = ResourcesMap.find(UpperName);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}
#include "PreCompile.h"
#include "GameEngineFBX.h"

GameEngineFBX::GameEngineFBX()
	: Manager(nullptr)
	, IOSetting(nullptr)
	, Importer(nullptr)
	, Scene(nullptr)
{

}

GameEngineFBX::~GameEngineFBX()
{
	if (nullptr != Scene)
	{
		Scene->Destroy();
		Scene = nullptr;
	}
	if (nullptr != Manager)
	{
		Importer->Destroy();
		Importer = nullptr;
	}
	if (nullptr != Manager)
	{
		IOSetting->Destroy();
		IOSetting = nullptr;
	}
	if (nullptr != Manager)
	{
		Manager->Destroy();
		Manager = nullptr;
	}
}

bool GameEngineFBX::CreateFBXSystemInitialize(const std::string& _Path)
{
	Manager = fbxsdk::FbxManager::Create();

	if (nullptr == Manager)
	{
		GameEngineDebug::MsgBoxError("FBX 매니저 생성에 실패했습니다.");
		return false;
	}

	// fbx파일을 읽는 방법을 정의한다.
	// these defines are used for hierarchical properties names 구조도를 타고 읽겠다.
	// IOSROOT -> 루트부터 읽겠다.
	IOSetting = fbxsdk::FbxIOSettings::Create(Manager, IOSROOT);
	
	// 마지막이 경로일텐데 만들고나서 할 수 있다.
	Importer = fbxsdk::FbxImporter::Create(Manager, "");

	if (false == Importer->Initialize(GameEngineString::AnsiToUTF8Return(_Path).c_str(), -1, IOSetting))
	{
		Importer->Destroy();
		IOSetting->Destroy();
		GameEngineDebug::MsgBoxError("FBX 로드 이니셜라이즈 실패.");
		return false;
	}

	Scene = fbxsdk::FbxScene::Create(Manager, "");

	if (nullptr == Scene)
	{
		GameEngineDebug::MsgBoxError("FBX 씬생성 실패.");
		return false;
	}

	if (false == Importer->Import(Scene))
	{
		GameEngineDebug::MsgBoxError("FBX 임포트 실패.");
		return false;
	}

	return true;
}
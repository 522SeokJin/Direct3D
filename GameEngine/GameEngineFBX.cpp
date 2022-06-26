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
		GameEngineDebug::MsgBoxError("FBX �Ŵ��� ������ �����߽��ϴ�.");
		return false;
	}

	// fbx������ �д� ����� �����Ѵ�.
	// these defines are used for hierarchical properties names �������� Ÿ�� �аڴ�.
	// IOSROOT -> ��Ʈ���� �аڴ�.
	IOSetting = fbxsdk::FbxIOSettings::Create(Manager, IOSROOT);
	
	// �������� ������ٵ� ������� �� �� �ִ�.
	Importer = fbxsdk::FbxImporter::Create(Manager, "");

	if (false == Importer->Initialize(GameEngineString::AnsiToUTF8Return(_Path).c_str(), -1, IOSetting))
	{
		Importer->Destroy();
		IOSetting->Destroy();
		GameEngineDebug::MsgBoxError("FBX �ε� �̴ϼȶ����� ����.");
		return false;
	}

	Scene = fbxsdk::FbxScene::Create(Manager, "");

	if (nullptr == Scene)
	{
		GameEngineDebug::MsgBoxError("FBX ������ ����.");
		return false;
	}

	if (false == Importer->Import(Scene))
	{
		GameEngineDebug::MsgBoxError("FBX ����Ʈ ����.");
		return false;
	}

	return true;
}
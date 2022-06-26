#include "PreCompile.h"
#include "GameEngineFBXMesh.h"

GameEngineFBXMesh::GameEngineFBXMesh()
{

}

GameEngineFBXMesh::~GameEngineFBXMesh()
{

}

void GameEngineFBXMesh::Load(const std::string& _Path)
{
	if (false == CreateFBXSystemInitialize(_Path))
	{
		GameEngineDebug::MsgBoxError("FBX �̴ϼȶ���� �����߽��ϴ�.");
		return;
	}

	FBXConvertScene();
}


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
		GameEngineDebug::MsgBoxError("FBX 이니셜라이즈에 실패했습니다.");
		return;
	}

	FBXConvertScene();
}


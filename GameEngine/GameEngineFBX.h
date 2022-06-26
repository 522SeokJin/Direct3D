#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>

#include "FBX/fbxsdk.h"

// ����� ����϶�.
//#pragma comment(lib, "zlib-mt.lib")
//#pragma comment(lib, "libxml2-mt.lib")
//#pragma comment(lib, "libfbxsdk-mt.lib")

// ������� ���̺귯��
#pragma comment(lib, "libfbxsdk.lib")

// ���� :
class GameEngineFBX : public GameEngineObjectNameBase
{
public:
	GameEngineFBX();
	~GameEngineFBX();

protected:
	GameEngineFBX(const GameEngineFBX& _Other) = delete;
	GameEngineFBX(GameEngineFBX&& _Other) noexcept = delete;
	GameEngineFBX& operator=(const GameEngineFBX& _Other) = delete;
	GameEngineFBX& operator=(GameEngineFBX&& _Other) noexcept = delete;

	bool CreateFBXSystemInitialize(const std::string& _Path);

private:

public:

protected:
	// �����Ϸ��� �̳༮�� �ʿ��ϴ�.
	fbxsdk::FbxManager* Manager;
	fbxsdk::FbxIOSettings* IOSetting;
	fbxsdk::FbxImporter* Importer;
	fbxsdk::FbxScene* Scene;

private:

};


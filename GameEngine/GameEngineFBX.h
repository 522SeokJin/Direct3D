#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>

#include "FBX/fbxsdk.h"

// 디버그 모드일때.
//#pragma comment(lib, "zlib-mt.lib")
//#pragma comment(lib, "libxml2-mt.lib")
//#pragma comment(lib, "libfbxsdk-mt.lib")

// 릴리즈용 라이브러리
#pragma comment(lib, "libfbxsdk.lib")

// 설명 :
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
	// 시작하려면 이녀석이 필요하다.
	fbxsdk::FbxManager* Manager;
	fbxsdk::FbxIOSettings* IOSetting;
	fbxsdk::FbxImporter* Importer;
	fbxsdk::FbxScene* Scene;

private:

};


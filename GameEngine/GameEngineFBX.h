#pragma once

#include "FBX/fbxsdk.h"

// ����� ����϶�.
//#pragma comment(lib, "zlib-mt.lib")
//#pragma comment(lib, "libxml2-mt.lib")
//#pragma comment(lib, "libfbxsdk-mt.lib")

// ������� ���̺귯��
#pragma comment(lib, "libfbxsdk.lib")

// ���� :
class GameEngineFBX
{
public:
	GameEngineFBX();
	~GameEngineFBX();

	void Load();

protected:
	GameEngineFBX(const GameEngineFBX& _Other) = delete;
	GameEngineFBX(GameEngineFBX&& _Other) noexcept = delete;
	GameEngineFBX& operator=(const GameEngineFBX& _Other) = delete;
	GameEngineFBX& operator=(GameEngineFBX&& _Other) noexcept = delete;

private:

public:

protected:
	fbxsdk::FbxManager* Manager;

private:

};


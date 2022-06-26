#pragma once
#include "GameEngineFBX.h"

// ���� : 
class GameEngineFBXMesh : public GameEngineFBX
{
public:
	GameEngineFBXMesh();
	~GameEngineFBXMesh();

protected:
	GameEngineFBXMesh(const GameEngineFBXMesh& _other) = delete; 
	GameEngineFBXMesh(GameEngineFBXMesh&& _other) noexcept = delete;
	GameEngineFBXMesh& operator=(const GameEngineFBXMesh& _other) = delete;
	GameEngineFBXMesh& operator=(const GameEngineFBXMesh&& _other) = delete;

private:

};


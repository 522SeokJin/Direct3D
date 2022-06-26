#pragma once

// Ό³Έν : 
class GameEngineFBX
{
public:
	GameEngineFBX();
	~GameEngineFBX();

protected:
	GameEngineFBX(const GameEngineFBX& _other) = delete; 
	GameEngineFBX(GameEngineFBX&& _other) noexcept = delete;
	GameEngineFBX& operator=(const GameEngineFBX& _other) = delete;
	GameEngineFBX& operator=(const GameEngineFBX&& _other) = delete;

private:

};


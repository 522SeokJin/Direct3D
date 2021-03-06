#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

// ???? :
class GameEngineLevel;
class GameEngineImageRenderer;
class Player : public GameEngineActor
{
public:
	static Player* MainPlayer;

public:
	// constrcuter destructer
	Player();
	~Player();

	// delete Function
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

protected:
	GameEngineImageRenderer* PlayerImageRenderer;
	GameEngineCollision* PlayerCollision;

	GameEngineFSM StateManager_;

	float Speed;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
};


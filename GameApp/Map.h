#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase\GameEngineRandom.h>

// Ό³Έν :
class GameEngineImageRenderer;
class Map : public GameEngineActor
{
public:
	// constrcuter destructer
	Map();
	~Map();

	// delete Function
	Map(const Map& _Other) = delete;
	Map(Map&& _Other) noexcept = delete;
	Map& operator=(const Map& _Other) = delete;
	Map& operator=(Map&& _Other) noexcept = delete;

	GameEngineImageRenderer* ImageRenderer;

	static float4 GetColor(GameEngineTransform* _Ptr, bool YRevers = true);

	static float4 GetColor(float4 _Postion);



protected:

	void Start() override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;
	
	GameEngineRandom Random;
	float4 RandomStart;

	void RandomRoad(int Count);
	void RandomRoad();

private:
	static Map* CurrentMap;
};
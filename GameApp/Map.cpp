#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include "Player.h"
#include "Map.h"

Map* Map::CurrentMap = nullptr;

Map::Map()
{
	RandomStart = float4::ZERO;
}

Map::~Map()
{
}

void Map::Start()
{
	{
		ImageRenderer = CreateTransformComponent<GameEngineImageRenderer>();
		ImageRenderer->GetTransform()->SetLocalPosition(float4{ 1200 * 0.5f, -720.0f * 0.5f, 100.0f });
		ImageRenderer->GetTransform()->SetLocalScaling(float4{ 1200, 720.0f, 1.0f });
		ImageRenderer->SetImage("Map.Png");
	}
}

void Map::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	CurrentMap = this;
}

float4 Map::GetColor(GameEngineTransform* _Ptr, bool YRevers /*= true*/)
{
	float4 Pos = _Ptr->GetWorldPosition();

	if (true == YRevers)
	{
		Pos.y *= -1.0f;
	}

	return GetColor(Pos);
}

float4 Map::GetColor(float4 _Postion)
{
	return CurrentMap->ImageRenderer->GetCurrentTexture()->GetPixel(_Postion.ix(), _Postion.iy());
}


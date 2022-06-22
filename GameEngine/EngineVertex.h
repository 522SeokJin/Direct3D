#pragma once
#include <GameEngineBase/GameEngineMath.h>


struct GameEngineVertex
{
public:
	float4 Postion;
	float4 Texcoord;
	float4 Color;

	// 회전행렬을 만들어 낼수 있다.
	// 빛만연산할거면 이것만
	float4 Normal; // y

	// 노말맵 혹은 범프연산을 할때는 보통 아래까지 필요하다.
	float4 Tangent; // z
	float4 BiNormal; // x
};
#pragma once
#include <GameEngineBase/GameEngineMath.h>


struct GameEngineVertex
{
public:
	float4 Postion;
	float4 Texcoord;
	float4 Color;

	// ȸ������� ����� ���� �ִ�.
	// ���������ҰŸ� �̰͸�
	float4 Normal; // y

	// �븻�� Ȥ�� ���������� �Ҷ��� ���� �Ʒ����� �ʿ��ϴ�.
	float4 Tangent; // z
	float4 BiNormal; // x
};
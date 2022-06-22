#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include "Player.h"
#include "Monster.h"
#include "Bullet.h"
#include "Map.h"

Player* Player::MainPlayer;

Player::Player()
	: Speed(300.0f)
{
	MainPlayer = this;
}

Player::~Player()
{
}

void Player::Start()
{
	GetTransform()->SetWorldPosition({ 0.0f, 0.0f });

	{
		GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
		Renderer->SetRenderingPipeLine("Color");
		Renderer->SetMesh("Box");

		Renderer->GetTransform()->SetLocalScaling({ 10.0f, 10.0f, 10.0f });
		Renderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
		Renderer->ShaderHelper.SettingConstantBufferSet("ResultColor", float4(1.0f, 0.0f, 0.0f));
	}

	if (false == GameEngineInput::GetInst().IsKey("PlayerMove"))
	{
		GameEngineInput::GetInst().CreateKey("MoveLeft", 'A');
		GameEngineInput::GetInst().CreateKey("MoveRight", 'D');
		GameEngineInput::GetInst().CreateKey("MoveForward", 'W');
		GameEngineInput::GetInst().CreateKey("MoveBack", 'S');
		GameEngineInput::GetInst().CreateKey("MoveUp", 'Q');
		GameEngineInput::GetInst().CreateKey("MoveDown", 'E');
		GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
	}
}

void Player::Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Down("FreeCameraOn"))
	{
		GetLevel()->GetMainCameraActor()->FreeCameraModeSwitch();
	}

	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}


	if (true == GameEngineInput::GetInst().Press("MoveLeft"))
	{
	}
	if (true == GameEngineInput::GetInst().Press("MoveRight"))
	{
	}
	if (true == GameEngineInput::GetInst().Press("MoveForward"))
	{
	}
	if (true == GameEngineInput::GetInst().Press("MoveBack"))
	{
	}


}

void Player::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	if (std::string::npos != _NextLevel->GetName().find("Title"))
	{
		return;
	}

	GetLevel()->SetLevelActorMove(_NextLevel, this);
	_NextLevel->GetMainCameraActor()->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + float4(0.0f, 0.0F, -100.0F));

}
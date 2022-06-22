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

void Player::TestTimeEvent() 
{
	static float Time = 0.5f;

	Bullet* NewBullet = GetLevel()->CreateActor<Bullet>();
	NewBullet->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());
	NewBullet->Release(1.0f);

	GetLevel()->AddTimeEvent(Time += 0.5f, std::bind(&Player::TestTimeEvent, this));
}

void Player::Start()
{
	// 정말 세팅해줘야할게 많은 녀석입니다.
	// 랜더러로서 뭐든지 다 그릴수있는 가능성을 가지고 있는 녀석.

	GetTransform()->SetWorldPosition({500.0f, -500.0f});

	{
		GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
		Renderer->SetRenderingPipeLine("Color");
		Renderer->GetTransform()->SetLocalScaling({ 100.0f, 20.0f, 1.0f });
		Renderer->GetTransform()->SetLocalPosition({ 0.0f, 80.0f, -20.0f });
		Renderer->ShaderHelper.SettingConstantBufferSet("ResultColor", float4(1.0f, 0.0f, 1.0f));
	}

	{
		PlayerImageRenderer = CreateTransformComponent<GameEngineImageRenderer>();
		PlayerImageRenderer->SetImage("Char.png");
		PlayerImageRenderer->GetTransform()->SetLocalScaling(float4{ 100.0f, 100.0f, 1.0f });
		PlayerImageRenderer->SetAlpha(0.5f);
	}

	{
		PlayerCollision = CreateTransformComponent<GameEngineCollision>(10);
		PlayerCollision->GetTransform()->SetLocalScaling(float4{ 200.0f, 200.0f, 1.0f });
		PlayerCollision->SetCollisionGroup(30);
	}


	if (false == GameEngineInput::GetInst().IsKey("PlayerMove"))
	{
		GameEngineInput::GetInst().CreateKey("MoveLeft", 'A');
		GameEngineInput::GetInst().CreateKey("MoveRight", 'D');
		GameEngineInput::GetInst().CreateKey("MoveUp", 'W');
		GameEngineInput::GetInst().CreateKey("MoveDown", 'S');
		GameEngineInput::GetInst().CreateKey("RotZ+", 'Q');
		GameEngineInput::GetInst().CreateKey("RotZ-", 'E');
		GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
		GameEngineInput::GetInst().CreateKey("Fire", VK_SPACE);
	}

	GetLevel()->AddTimeEvent(0.5f, std::bind(&Player::TestTimeEvent, this));

	StateManager_.CreateState("Idle",
	[&]()
	{
			if (
				true == GameEngineInput::GetInst().Press("MoveLeft") || 
				true == GameEngineInput::GetInst().Press("MoveRight") ||
				true == GameEngineInput::GetInst().Press("MoveUp") ||
				true == GameEngineInput::GetInst().Press("MoveDown") 
				)
			{
				StateManager_.ChangeState("Move");
			}


			float Time = StateManager_.GetCurrentState()->Time;

			if (1.0f >= Time)
			{
				PlayerImageRenderer->SetPlusColor(float4(Time, Time, Time, 0.0f));
			}
			else {
				StateManager_.GetCurrentState()->Time = 0.0f;
			}
			
			//float4 Color = Map::GetColor(GetTransform());

			//if (Color != float4::BLACK)
			//{
			//	GetTransform()->SetLocalDeltaTimeMove(float4::DOWN * 100.0f);
			//}
	}
	);


	StateManager_.CreateState("Move",
		[&]()
	{
		if (
			false == GameEngineInput::GetInst().Press("MoveLeft") &&
			false == GameEngineInput::GetInst().Press("MoveRight") &&
			false == GameEngineInput::GetInst().Press("MoveUp") &&
			false == GameEngineInput::GetInst().Press("MoveDown") 
			)
		{
			StateManager_.ChangeState("Idle");
			return;
		}


	}
	);

	PlayerImageRenderer->SetPlusColor(float4(1.0f, 1.0f, 1.0f, 0.0f));

	StateManager_.ChangeState("Idle");
}

void Player::Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Down("FreeCameraOn"))
	{
		GetLevel()->GetMainCameraActor()->FreeCameraModeSwitch();
	}


	if (true == GameEngineInput::GetInst().Press("MoveLeft"))
	{
		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldMove(float4::LEFT * 200.0f * _DeltaTime);

		// GetTransform()->SetLocalDeltaTimeMove(float4::LEFT * Speed);
	}
	if (true == GameEngineInput::GetInst().Press("MoveRight"))
	{
		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldMove(float4::RIGHT * 200.0f * _DeltaTime);

		// GetTransform()->SetLocalDeltaTimeMove(float4::RIGHT * Speed);
	}
	if (true == GameEngineInput::GetInst().Press("MoveUp"))
	{
		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldMove(float4::UP * 200.0f * _DeltaTime);

		// GetTransform()->SetLocalDeltaTimeMove(float4::UP * Speed);
	}
	if (true == GameEngineInput::GetInst().Press("MoveDown"))
	{
		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldMove(float4::DOWN * 200.0f * _DeltaTime);

		// GetTransform()->SetLocalDeltaTimeMove(float4::DOWN * Speed);
	}

	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{

		return;
	}

	StateManager_.Update(_DeltaTime);

	// StateManager_.IsCurrentState("Move");



	//static float Speed = 300.0f;

	//if (true == GameEngineInput::GetInst().Press("MoveLeft"))
	//{
	//	GetTransform()->SetLocalDeltaTimeMove(float4::LEFT * Speed);
	//}
	//if (true == GameEngineInput::GetInst().Press("MoveRight")) 
	//{
	//	GetTransform()->SetLocalDeltaTimeMove(float4::RIGHT * Speed);
	//}
	//if (true == GameEngineInput::GetInst().Press("MoveUp")) 
	//{
	//	GetTransform()->SetLocalDeltaTimeMove(float4::UP * Speed);
	//}
	//if (true == GameEngineInput::GetInst().Press("MoveDown")) 
	//{
	//	GetTransform()->SetLocalDeltaTimeMove(float4::DOWN * Speed);
	//}

	//float4 Color = Map::GetColor(GetTransform());

	//if (Color != float4::BLACK)
	//{
	//	GetTransform()->SetLocalDeltaTimeMove(float4::DOWN * 100.0f);
	//}



	//if (true == GameEngineInput::GetInst().Press("RotZ+"))
	//{
	//	// GetTransform()->SetLocalDeltaTimeRotation(float4{ 0.0f, 0.0f, 1.0f } *100.0f);
	//	PlayerCollision->GetTransform()->SetLocalDeltaTimeRotation(float4{ 0.0f, 0.0f, 1.0f } *100.0f);
	//	PlayerImageRenderer->GetTransform()->SetLocalDeltaTimeRotation(float4{0.0f, 0.0f, 1.0f} * 100.0f);
	//}

	//if (true == GameEngineInput::GetInst().Press("RotZ-"))
	//{
	//	// GetTransform()->SetLocalDeltaTimeRotation(float4{ 0.0f, 0.0f, -1.0f } *100.0f);
	//	PlayerCollision->GetTransform()->SetLocalDeltaTimeRotation(float4{ 0.0f, 0.0f, -1.0f } *100.0f);
	//	PlayerImageRenderer->GetTransform()->SetLocalDeltaTimeRotation(float4{ 0.0f, 0.0f, -1.0f } * 100.0f);
	//}

	//if (true == GameEngineInput::GetInst().Down("Fire"))
	//{
	//	Bullet* NewBullet = GetLevel()->CreateActor<Bullet>();
	//	NewBullet->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());
	//	NewBullet->Release(1.0f);
	//}

	//PlayerCollision->Collision(
	//	CollisionType::OBBBox3D, CollisionType::OBBBox3D, 20,
	//	[](GameEngineCollision* _OtherCollision
	//		)
	//{
	//	_OtherCollision->GetActor()->Death();
	//}
	//);


	//// GetLevel()->DebugOff();
	//
	//GetLevel()->PushDebugRender(PlayerCollision->GetTransform(), CollisionType::Rect);

	// GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

	//// GetTransform()->Collision(1);
}

void Player::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	if (std::string::npos != _NextLevel->GetName().find("Title"))
	{
		return;
	}

	//StageLevel* Stage = GetLevelConvert<StageLevel>();
	//Stage->GetStartPostion();

	//if (nullptr == Stage)
	//{
	//	return;
	//}

	GetLevel()->SetLevelActorMove(_NextLevel, this);
	// GetLevel()->SetLevelActorMove(_NextLevel, UIActor);
	_NextLevel->GetMainCameraActor()->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + float4(0.0f, 0.0F, -100.0F));

}
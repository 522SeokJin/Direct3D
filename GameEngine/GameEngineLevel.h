#pragma once
#include <list>
#include <map>
#include <GameEngine\Enums.h>
//#include <GameEngine\GameEngineRenderingPipeLine.h>
//
//// 설명 : 
//
//class DebugRenderData
//{
//	TransformData Data;
//	GameEngineRenderingPipeLine Data;
//	// 파이프라인
//	// 쉐이더 헬퍼가 한쌍이어야 한다.
//};

class CameraActor;
class CameraComponent;
class GameEngineActor;
class GameEngineRenderer;
class GameEngineTransform; 
class GameEngineCollision;
class GameEngineDebugRenderData;
class GameEnginePostProcessRender;
class GameEngineLevel : public GameEngineObjectNameBase
{
	friend class GameEngineLevelControlWindow;
	friend class GameEngineCore;
	friend class GameEngineRenderer;
	friend class GameEngineCollision;

private:
	class NextLevelActor 
	{
	public:
		GameEngineActor* Actor;
		GameEngineLevel* Level;
	};

public:
	// constrcuter destructer
	GameEngineLevel();
	~GameEngineLevel();

	// delete Function
	GameEngineLevel(const GameEngineLevel& _Other) = delete;
	GameEngineLevel(GameEngineLevel&& _Other) noexcept = delete;
	GameEngineLevel& operator=(const GameEngineLevel& _Other) = delete;
	GameEngineLevel& operator=(GameEngineLevel&& _Other) noexcept = delete;

	CameraActor* GetMainCameraActor();
	CameraComponent* GetMainCamera();

	CameraActor* GetUICameraActor();
	CameraComponent* GetUICamera();

	void AddTimeEvent(float _Time, std::function<void()> _Event);

protected:

private:
	std::vector<NextLevelActor> NextLevelActorsData_;
	std::map<std::string, GameEngineActor*> FindMap_;
	std::map<int, std::list<GameEngineActor*>> ActorList_;
	CameraActor* MainCameraActor_;
	CameraActor* UICameraActor_;

	std::list<TimeEvent*> AllEvent_;
	std::list<TimeEvent*> AddEvent_;

	void Init();

	void TimeEventUpdate();
public:
	template<typename ActorType>
	ActorType* CreateActor(int _UpdateOrder = 0)
	{
		GameEngineActor* NewActor = new ActorType();
		NewActor->SetLevel(this);
		NewActor->Start();
		NewActor->SetOrder(_UpdateOrder);

		// Insert + Find
		std::list<GameEngineActor*>& List = ActorList_[_UpdateOrder];
		List.push_back(NewActor);
		return dynamic_cast<ActorType*>(NewActor);
	}



	template<typename ActorType>
	ActorType* CreateActor(std::string _Name, bool _IsFind = false, int _UpdateOrder = 0)
	{
		GameEngineActor* NewActor = new ActorType();
		NewActor->IsFindObject_ = _IsFind;
		NewActor->SetName(_Name);
		NewActor->SetLevel(this);
		NewActor->Start();
		NewActor->SetOrder(_UpdateOrder);

		if (true == _IsFind)
		{
			FindMap_.insert(std::make_pair(_Name, NewActor));
		}

		// Insert + Find
		std::list<GameEngineActor*>& List = ActorList_[_UpdateOrder];
		List.push_back(NewActor);
		return dynamic_cast<ActorType*>(NewActor);
	}

	template<typename ActorType>
	ActorType* FindActor(std::string _Name)
	{
		if (FindMap_.end() == FindMap_.find(_Name))
		{
			return nullptr;
		}

		return dynamic_cast<ActorType>(FindMap_[_Name]);
	}

	void ActorUpdate(float _DeltaTime);
	void Render(float _DeltaTime);
	void Release(float _DeltaTime);

	virtual void LevelStart() = 0;
	virtual void LevelUpdate(float _DeltaTime) = 0;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) = 0;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) = 0;

	void SetLevelActorMove(GameEngineLevel* _NextLevel, GameEngineActor* _Actor);

	//////////////////////////////////////////////////////// collision:
private:

	std::map<int, std::list<GameEngineCollision*>> CollisionList_;


	inline std::list<GameEngineCollision*>& GetCollisionGroup(int _Group) 
	{
		return CollisionList_[_Group];
	}

	void ChangeCollisionGroup(int _Group, GameEngineCollision* _Collision);

	void LevelChangeEndActorEvent(GameEngineLevel* _NextLevel);
	void LevelChangeStartActorEvent(GameEngineLevel* _PrevLevel);

	void SetLevelActorMoveProcess();

	// 다른애가 이걸 가릴수 있나요?

public:
	void PushDebugRender(GameEngineTransform* _Transform, CollisionType _Type);

	template<typename UserEnumType>
	void PushCollision(GameEngineCollision* _Collision, UserEnumType _Group)
	{
		PushCollision(_Collision, static_cast<int>(_Group));
	}

	void PushCollision(GameEngineCollision* _Collision, int _Group);

/// ///////////////////////////GameEnginePostProcessRender
private:
	std::map<std::string, std::vector<GameEnginePostProcessRender*>> PostRender;

public:
	// FadeIn : public GameEnginePostProcessRender
	// FadeIn(float Speed)

	// ColorLerp : public GameEnginePostProcessRender
	// ColorLerp(Color _Start, Color _End)

	template<typename PostProcess, typename ... Parameter>
	PostProcess* AddPostProcessCameraMergePrev(Parameter ... _Arg)
	{
		return AddPostProcess<PostProcess>("CameraMergePrev", _Arg...);
	}

	template<typename PostProcess, typename ... Parameter >
	PostProcess* AddPostProcessCameraMergeNext(Parameter ... _Arg)
	{
		return AddPostProcess<PostProcess>("CameraMergeNext", _Arg...);
	}

	template<typename PostProcess, typename ... Parameter >
	PostProcess* AddPostProcess(const std::string& _Key, Parameter ... _Arg)
	{
		PostProcess* NewPost = new PostProcess(_Arg...);
		GameEnginePostProcessRender* ParentType = dynamic_cast<GameEnginePostProcessRender*>(NewPost);
		ParentType->Initialize();
		PostRender[_Key].push_back(NewPost);
		return NewPost;
	}

	void AllClear();

public:
	// 빛의 채널
	// 1, 2, 3
	// 각 랜더러들은 선택해야 합니다.
	// 빛을 안받겠다.
	// 1번 그룹빛을 받겠다.
	// 2번 그룹 빛을 받겠다.
	// 3번 그룹 빛을 받겠다.

	// main랜더러.
	// 빛이 10개 있어요 빛의 영향을 받는다와 안받는다만 있으면 될까요?
	// Forward light와
	// deferred light의 차이도 존재한다.



	// ui 빛은 따로 만들어야 한다.
	// UI 까지 영향을 미쳐야 하나요?

	// std::map<int, std::vector<GameEngineLight*>> Light;
};
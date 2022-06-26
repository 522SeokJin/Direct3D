#include "PreCompile.h"
#include "GameEngineFBX.h"

GameEngineFBX::GameEngineFBX()
	: Manager(nullptr)
	, IOSetting(nullptr)
	, Importer(nullptr)
	, Scene(nullptr)
	, RootNode(nullptr)
{

}

GameEngineFBX::~GameEngineFBX()
{
	if (nullptr != Scene)
	{
		Scene->Destroy();
		Scene = nullptr;
	}
	if (nullptr != Manager)
	{
		Importer->Destroy();
		Importer = nullptr;
	}
	if (nullptr != Manager)
	{
		IOSetting->Destroy();
		IOSetting = nullptr;
	}
	if (nullptr != Manager)
	{
		Manager->Destroy();
		Manager = nullptr;
	}
}

bool GameEngineFBX::CreateFBXSystemInitialize(const std::string& _Path)
{
	Manager = fbxsdk::FbxManager::Create();

	if (nullptr == Manager)
	{
		GameEngineDebug::MsgBoxError("FBX 매니저 생성에 실패했습니다.");
		return false;
	}

	// fbx파일을 읽는 방법을 정의한다.
	// these defines are used for hierarchical properties names 구조도를 타고 읽겠다.
	// IOSROOT -> 루트부터 읽겠다.
	IOSetting = fbxsdk::FbxIOSettings::Create(Manager, IOSROOT);
	
	// 마지막이 경로일텐데 만들고나서 할 수 있다.
	Importer = fbxsdk::FbxImporter::Create(Manager, "");

	if (false == Importer->Initialize(GameEngineString::AnsiToUTF8Return(_Path).c_str(), -1, IOSetting))
	{
		Importer->Destroy();
		IOSetting->Destroy();
		GameEngineDebug::MsgBoxError("FBX 로드 이니셜라이즈 실패.");
		return false;
	}

	Scene = fbxsdk::FbxScene::Create(Manager, "");

	if (nullptr == Scene)
	{
		GameEngineDebug::MsgBoxError("FBX 씬생성 실패.");
		return false;
	}

	if (false == Importer->Import(Scene))
	{
		GameEngineDebug::MsgBoxError("FBX 임포트 실패.");
		return false;
	}

	return true;
}

void GameEngineFBX::FBXConvertScene()
{
	fbxsdk::FbxAxisSystem::EUpVector UpVector = fbxsdk::FbxAxisSystem::eYAxis;
	fbxsdk::FbxAxisSystem::EFrontVector FrontVector = (fbxsdk::FbxAxisSystem::EFrontVector)-fbxsdk::FbxAxisSystem::eParityOdd;
	fbxsdk::FbxAxisSystem::ECoordSystem CooreSystem = fbxsdk::FbxAxisSystem::ECoordSystem::eRightHanded;

	fbxsdk::FbxAxisSystem EngineAxisSystem(UpVector, FrontVector, CooreSystem);
	// fbxsdk::FbxAxisSystem EngineAxisSystem(fbxsdk::FbxAxisSystem::EPreDefinedAxisSystem::eDirectX);

	fbxsdk::FbxAxisSystem SourceAxisSystem = Scene->GetGlobalSettings().GetAxisSystem();
	
	// Convert 작업을 해줘야 한다.
	if (SourceAxisSystem != EngineAxisSystem)
	{
		// 먼저 루트를 지워.
		fbxsdk::FbxRootNodeUtility::RemoveAllFbxRoots(Scene);
		// 루트가 새롭게 만들어진다.
		EngineAxisSystem.ConvertScene(Scene);

		// 매쉬의 기존 매트릭스를 가져온다.
		fbxsdk::FbxAMatrix SourceMatrix;
		SourceAxisSystem.GetMatrix(SourceMatrix);

		fbxsdk::FbxAMatrix EngineMatrix;
		EngineAxisSystem.GetMatrix(EngineMatrix);

		// 기존에 곱해진 행렬을 역행렬로 취소하고, 우리 기준의 행렬을 곱함
		ConvertMatrix = SourceMatrix.Inverse() * EngineMatrix;
	}

	int OriginUpSign = 0;
	int OriginFrontSign = 0;
	int EngineUpSign = 0;
	int EngineFrontSign = 0;

	fbxsdk::FbxAxisSystem::EFrontVector OriginFrontVector = SourceAxisSystem.GetFrontVector(OriginFrontSign);
	fbxsdk::FbxAxisSystem::EFrontVector EngineFrontVector = EngineAxisSystem.GetFrontVector(EngineFrontSign);

	fbxsdk::FbxAxisSystem::EUpVector OriginUpVector = SourceAxisSystem.GetUpVector(OriginUpSign);
	fbxsdk::FbxAxisSystem::EUpVector EngineUpVector = EngineAxisSystem.GetUpVector(EngineUpSign);

	fbxsdk::FbxAxisSystem::ECoordSystem  OriginCoordSystem = SourceAxisSystem.GetCoorSystem();
	fbxsdk::FbxAxisSystem::ECoordSystem  EngineCoordSystem = EngineAxisSystem.GetCoorSystem();

	if (OriginUpVector == EngineUpVector && OriginUpSign == EngineUpSign)
	{
		if (OriginFrontSign != EngineFrontSign && OriginCoordSystem == EngineCoordSystem)
		{
			AxisVector.mData[static_cast<int>(OriginUpVector) - 1] += 180;
		}
	}
	else if (OriginUpVector == EngineUpVector && OriginUpSign != EngineUpSign)
	{
		if (OriginUpVector == fbxsdk::FbxAxisSystem::EUpVector::eXAxis)
		{
			AxisVector.mData[1] += 180;
		}
		else
		{
			AxisVector.mData[0] += 180;
		}

		if (OriginFrontSign != EngineFrontSign && OriginCoordSystem == EngineCoordSystem)
		{
			AxisVector.mData[static_cast<int>(OriginUpVector) - 1] += 180;
		}
	}
	else if (OriginUpVector == fbxsdk::FbxAxisSystem::EUpVector::eXAxis)
	{
		//origin up vector 가 x up vector 일때를 아직 만나보지를 못했다.

		AxisVector.mData[1] += OriginUpSign * 90;
	}
	else
	{
		AxisVector.mData[0] += OriginUpSign * 90;
		if (OriginUpSign != EngineFrontSign)
		{
			AxisVector.mData[static_cast<int>(OriginUpVector) - 1] += OriginUpSign * 180;
		}
	}

	JointConvertMatrix.SetR(AxisVector);

	if (true == JointConvertMatrix.IsIdentity())
	{
		JointConvertMatrix = ConvertMatrix;
	}

	// 애니메이션도 행렬의 집합인데. 
	Scene->GetAnimationEvaluator()->Reset();

	RootNode = Scene->GetRootNode();

	if (nullptr == RootNode)
	{
		GameEngineDebug::MsgBoxError("루트노드생성에 실패했습니다.");
	}

	return;
}

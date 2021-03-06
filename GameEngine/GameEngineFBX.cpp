#include "PreCompile.h"
#include "GameEngineFBX.h"

// 릴리즈용 라이브러리
#pragma comment(lib, "libfbxsdk.lib")


GameEngineFBX::GameEngineFBX()
	: Manager(nullptr)
	, IOSetting(nullptr)
	, Importer(nullptr)
	, Scene(nullptr)
	, RootNode(nullptr)
{
}

void GameEngineFBX::Reset()
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

GameEngineFBX::~GameEngineFBX()
{
	Reset();
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
	IOSetting = fbxsdk::FbxIOSettings::Create(Manager, IOSROOT);

	// 마지막이 경로일텐데 만들고나서 할수 있다.
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
	// int a = 0;

	// fbxsdk::FbxAxisSystem::ECoordSystem CooreSystem = fbxsdk::FbxAxisSystem::ECoordSystem::eRightHanded;

	fbxsdk::FbxAxisSystem::EUpVector UpVector = fbxsdk::FbxAxisSystem::eYAxis;
	fbxsdk::FbxAxisSystem::EFrontVector FrontVector = (fbxsdk::FbxAxisSystem::EFrontVector)-fbxsdk::FbxAxisSystem::eParityOdd;
	fbxsdk::FbxAxisSystem::ECoordSystem CooreSystem = fbxsdk::FbxAxisSystem::ECoordSystem::eRightHanded;

	fbxsdk::FbxAxisSystem EngineAxisSystem(UpVector, FrontVector, CooreSystem);
	// fbxsdk::FbxAxisSystem EngineAxisSystem(fbxsdk::FbxAxisSystem::EPreDefinedAxisSystem::eDirectX);

	fbxsdk::FbxAxisSystem SourceAxisSystem = Scene->GetGlobalSettings().GetAxisSystem();

	// 이건 이제 컨버트 해줘야 하는겁니다.
	if (SourceAxisSystem != EngineAxisSystem)
	{
		// 먼저 루트를 지워.
		fbxsdk::FbxRootNodeUtility::RemoveAllFbxRoots(Scene);
		// 루트가 새롭게 만들어진다.
		EngineAxisSystem.ConvertScene(Scene);

		// 매쉬의 기존 매트릭스를 가져옵니다.
		fbxsdk::FbxAMatrix SourceMatrix;
		SourceAxisSystem.GetMatrix(SourceMatrix);

		fbxsdk::FbxAMatrix EngineMatrix;
		EngineAxisSystem.GetMatrix(EngineMatrix);

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

	fbxsdk::FbxGeometryConverter Con(Manager);

	if (false == Con.Triangulate(Scene, true))
	{
		GameEngineDebug::MsgBoxError("삼각화에 실패했습니다.");
	}

	// RecursiveAllNode(RootNode);

	return;
}

void GameEngineFBX::RecursiveAllNode(
	fbxsdk::FbxNode* _Node,
	std::function<int(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _InitCallBack,
	std::function<void(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _EndCallBack,
	int _ParentReturn
)
{
	// fbxsdk::FbxNodeAttribute* Info = _Node->GetNodeAttribute();
	fbxsdk::FbxNodeAttribute* Info = _Node->GetNodeAttribute();

	if (nullptr != Info)
	{
		fbxsdk::FbxNodeAttribute::EType Type = Info->GetAttributeType();

		switch (Type)
		{
		case fbxsdk::FbxNodeAttribute::eUnknown:
			break;
		case fbxsdk::FbxNodeAttribute::eNull:
			break;
		case fbxsdk::FbxNodeAttribute::eMarker:
			break;
		case fbxsdk::FbxNodeAttribute::eSkeleton:
			break;
		case fbxsdk::FbxNodeAttribute::eMesh:
			break;
		case fbxsdk::FbxNodeAttribute::eNurbs:
			break;
		case fbxsdk::FbxNodeAttribute::ePatch:
			break;
		case fbxsdk::FbxNodeAttribute::eCamera:
			break;
		case fbxsdk::FbxNodeAttribute::eCameraStereo:
			break;
		case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
			break;
		case fbxsdk::FbxNodeAttribute::eLight:
			break;
		case fbxsdk::FbxNodeAttribute::eOpticalReference:
			break;
		case fbxsdk::FbxNodeAttribute::eOpticalMarker:
			break;
		case fbxsdk::FbxNodeAttribute::eNurbsCurve:
			break;
		case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
			break;
		case fbxsdk::FbxNodeAttribute::eBoundary:
			break;
		case fbxsdk::FbxNodeAttribute::eNurbsSurface:
			break;
		case fbxsdk::FbxNodeAttribute::eShape:
			break;
		case fbxsdk::FbxNodeAttribute::eLODGroup:
			break;
		case fbxsdk::FbxNodeAttribute::eSubDiv:
			break;
		case fbxsdk::FbxNodeAttribute::eCachedEffect:
			break;
		case fbxsdk::FbxNodeAttribute::eLine:
			break;
		default:
			break;
		}
	}

	int StartReturn = 0;

	if (nullptr != _InitCallBack)
	{
		if (nullptr != Info)
		{
			fbxsdk::FbxNodeAttribute::EType Type = Info->GetAttributeType();
			StartReturn = _InitCallBack(Type, _Node, _ParentReturn);
		}
		else
		{
			StartReturn = _InitCallBack(fbxsdk::FbxNodeAttribute::EType::eUnknown, _Node, _ParentReturn);
		}
	}


	int ChildCount = _Node->GetChildCount();

	for (int i = 0; i < ChildCount; i++)
	{
		fbxsdk::FbxNode* ChildNode = _Node->GetChild(i);
		RecursiveAllNode(ChildNode, _InitCallBack, _EndCallBack, StartReturn);
	}

	if (nullptr != _EndCallBack)
	{
		if (nullptr != Info)
		{
			fbxsdk::FbxNodeAttribute::EType Type = Info->GetAttributeType();
			_EndCallBack(Type, _Node, StartReturn);
		}
		else
		{
			_EndCallBack(fbxsdk::FbxNodeAttribute::EType::eUnknown, _Node, StartReturn);
		}
	}

}

void GameEngineFBX::RecursiveAllNode(
	std::function<int(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _InitCallBack,
	std::function<void(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _EndCallBack,
	int _ParentReturn
)
{
	RecursiveAllNode(GetRootNode(), _InitCallBack, _EndCallBack, _ParentReturn);
}
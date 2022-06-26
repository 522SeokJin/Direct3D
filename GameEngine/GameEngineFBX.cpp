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
		GameEngineDebug::MsgBoxError("FBX �Ŵ��� ������ �����߽��ϴ�.");
		return false;
	}

	// fbx������ �д� ����� �����Ѵ�.
	// these defines are used for hierarchical properties names �������� Ÿ�� �аڴ�.
	// IOSROOT -> ��Ʈ���� �аڴ�.
	IOSetting = fbxsdk::FbxIOSettings::Create(Manager, IOSROOT);
	
	// �������� ������ٵ� ������� �� �� �ִ�.
	Importer = fbxsdk::FbxImporter::Create(Manager, "");

	if (false == Importer->Initialize(GameEngineString::AnsiToUTF8Return(_Path).c_str(), -1, IOSetting))
	{
		Importer->Destroy();
		IOSetting->Destroy();
		GameEngineDebug::MsgBoxError("FBX �ε� �̴ϼȶ����� ����.");
		return false;
	}

	Scene = fbxsdk::FbxScene::Create(Manager, "");

	if (nullptr == Scene)
	{
		GameEngineDebug::MsgBoxError("FBX ������ ����.");
		return false;
	}

	if (false == Importer->Import(Scene))
	{
		GameEngineDebug::MsgBoxError("FBX ����Ʈ ����.");
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
	
	// Convert �۾��� ����� �Ѵ�.
	if (SourceAxisSystem != EngineAxisSystem)
	{
		// ���� ��Ʈ�� ����.
		fbxsdk::FbxRootNodeUtility::RemoveAllFbxRoots(Scene);
		// ��Ʈ�� ���Ӱ� ���������.
		EngineAxisSystem.ConvertScene(Scene);

		// �Ž��� ���� ��Ʈ������ �����´�.
		fbxsdk::FbxAMatrix SourceMatrix;
		SourceAxisSystem.GetMatrix(SourceMatrix);

		fbxsdk::FbxAMatrix EngineMatrix;
		EngineAxisSystem.GetMatrix(EngineMatrix);

		// ������ ������ ����� ����ķ� ����ϰ�, �츮 ������ ����� ����
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
		//origin up vector �� x up vector �϶��� ���� ���������� ���ߴ�.

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

	// �ִϸ��̼ǵ� ����� �����ε�. 
	Scene->GetAnimationEvaluator()->Reset();

	RootNode = Scene->GetRootNode();

	if (nullptr == RootNode)
	{
		GameEngineDebug::MsgBoxError("��Ʈ�������� �����߽��ϴ�.");
	}

	return;
}

#include "PreCompile.h"
#include "GameEngineFBXMesh.h"

GameEngineFBXMesh::GameEngineFBXMesh()
{

}

GameEngineFBXMesh::~GameEngineFBXMesh()
{

}

void GameEngineFBXMesh::Load(const std::string& _Path)
{
	if (false == CreateFBXSystemInitialize(_Path))
	{
		GameEngineDebug::MsgBoxError("FBX 이니셜라이즈에 실패했습니다.");
		return;
	}

	FBXConvertScene();
}


fbxsdk::FbxNode* GameEngineFBXMesh::RecursiveFindParentLodGroup(fbxsdk::FbxNode* parentNode)
{
	if (nullptr == parentNode)
	{
		return nullptr;
	}
	fbxsdk::FbxNodeAttribute* attribute = parentNode->GetNodeAttribute();
	if (nullptr != attribute && attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eLODGroup)
	{
		return parentNode;
	}
	return RecursiveFindParentLodGroup(parentNode->GetParent());
}

void GameEngineFBXMesh::MeshNodeCheck()
{
	int geometryCount = Scene->GetGeometryCount();
	for (int i = 0; i < geometryCount; i++)
	{
		// 노드중에서 기하구조를 가진녀석들을 뽑아내는것이고.
		fbxsdk::FbxGeometry* geoMetry = Scene->GetGeometry(i);
		fbxsdk::FbxNode* geoMetryNode = geoMetry->GetNode();

		// 뽑아낸 애들중에서 그 타입이
		if (geoMetry->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh)
		{
			MeshInfos.push_back(FbxExMeshInfo());
			FbxExMeshInfo& Info = MeshInfos[MeshInfos.size() - 1];

			if (geoMetry->GetName()[0] != '\0')
			{
				Info.Name = GameEngineString::AnsiToUTF8Return(geoMetry->GetName());
			}
			else
			{
				Info.Name = nullptr != geoMetryNode ? GameEngineString::AnsiToUTF8Return(geoMetryNode->GetName()) : "None";
			}

			Info.Mesh = reinterpret_cast<fbxsdk::FbxMesh*>(geoMetry);
			Info.bTriangulated = Info.Mesh->IsTriangleMesh();
			Info.MaterialNum = geoMetryNode ? geoMetryNode->GetMaterialCount() : 0;
			Info.FaceNum = Info.Mesh->GetPolygonCount();
			Info.VertexNum = Info.Mesh->GetControlPointsCount();
			Info.UniqueId = Info.Mesh->GetUniqueID();

			// 나중에 정리할수 있을것 같다.
			Info.LODGroup = "";
			if (nullptr != geoMetryNode)
			{
				fbxsdk::FbxNode* ParentNode = RecursiveFindParentLodGroup(geoMetryNode->GetParent());
				if (ParentNode != nullptr && ParentNode->GetNodeAttribute() && ParentNode->GetNodeAttribute()->GetAttributeType() == fbxsdk::FbxNodeAttribute::eLODGroup)
				{
					Info.LODGroup = GameEngineString::AnsiToUTF8Return(ParentNode->GetName());
					Info.bIsLodGroup = true;
				}
			}

			if (Info.Mesh->GetDeformerCount(FbxDeformer::eSkin) > 0)
			{
				// meshInfo.bIsSkelMesh = true;

				int a = 0;

				//m_SceneInfo.SkinnedMeshNum++;
				//meshInfo.bIsSkelMesh = true;
				//meshInfo.MorphNum = mesh->GetShapeCount();
				//fbxsdk::FbxSkin* Skin = (fbxsdk::FbxSkin*)mesh->GetDeformer(0, FbxDeformer::eSkin);
				//int ClusterCount = Skin->GetClusterCount();
				//fbxsdk::FbxNode* Link = NULL;
				//for (int ClusterId = 0; ClusterId < ClusterCount; ++ClusterId)
				//{
				//	fbxsdk::FbxCluster* Cluster = Skin->GetCluster(ClusterId);
				//	Link = Cluster->GetLink();
				//	while (Link && Link->GetParent() && Link->GetParent()->GetSkeleton())
				//	{
				//		Link = Link->GetParent();
				//	}

				//	if (Link != NULL)
				//	{
				//		break;
				//	}
				//}

				//meshInfo.SkeletonRoot = nullptr != Link ? GameEngineString::ConvertMultiByteString(Link->GetName()) : L"None";
				//meshInfo.SkeletonElemNum = nullptr != Link ? Link->GetChildCount(true) : 0;

				//if (nullptr != Link)
				//{
				//	fbxsdk::FbxTimeSpan AnimTimeSpan(fbxsdk::FBXSDK_TIME_INFINITE, fbxsdk::FBXSDK_TIME_MINUS_INFINITE);
				//	Link->GetAnimationInterval(AnimTimeSpan);
				//	GlobalTimeSpan.UnionAssignment(AnimTimeSpan);
				//}
			}
			else
			{
				//m_SceneInfo.NonSkinnedMeshNum++;
				//meshInfo.bIsSkelMesh = false;
				//meshInfo.SkeletonRoot = L"";
			}
		}
	}

	int a = 0;
}

void GameEngineFBXMesh::Initialize()
{
}

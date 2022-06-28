#pragma once
#include "GameEngineFBX.h"
#include "EngineVertex.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineVertexBuffer.h"

class FbxExIW
{
public:
	int Index;
	double Weight;
};


class FbxExRenderingPipeLineSettingData
{
public:
	std::string Name;
	// 픽셀쉐이더에서 그냥 최종색상에 곱해주면 되는 색상.
	float4			 DifColor;
	float4			 AmbColor; // 빛
	float4			 SpcColor; // 빛
	float4			 EmvColor; // 빛
	float			 SpecularPower;		// 빛의 강도
	float			 TransparencyFactor;	// 빛의 강도
	float			 Shininess;			// 빛의 강도
	std::string DifTexturePath;	// 텍스처경로 
	std::string NorTexturePath; // 텍스처경로
	std::string SpcTexturePath; // 텍스처경로

	std::string DifTextureName;	// 텍스처경로 
	std::string NorTextureName; // 텍스처경로
	std::string SpcTextureName; // 텍스처경로

public:
	FbxExRenderingPipeLineSettingData() {}
	~FbxExRenderingPipeLineSettingData() {}
};


struct FbxMeshSet
{
public:
	int IsLodLv;
	bool IsLod;

	//       자신의 정보를 
	//       들고 있던 node
	//       의 mesh
	//       이매쉬에서  이점들을 빼냈다라고 보면 됩니다.
	//       얻어온 점들에 대한 모든 정보이고.
	//       만약 필요하다면 더 얻어야 할수도 있다.
	std::map<FbxMesh*, std::vector<GameEngineVertex>*> m_FbxVtxMap;

	//       애니메이션이 있다면 채워져 있을겁니다.
	std::map<FbxMesh*, std::map<int, std::vector<FbxExIW>>> MapWI;

	std::vector<std::vector<GameEngineVertex>> Vertexs;
	std::vector<std::vector<std::vector<UINT>>> Indexs;

	std::vector<GameEngineVertexBuffer*> GameEngineVertexBuffers;
	std::vector<std::vector<GameEngineIndexBuffer*>> GameEngineIndexBuffers;
	// std::vector<std::vector<std::shared_ptr<DirectMesh>>> m_Mesh;

	std::vector<std::vector<FbxExRenderingPipeLineSettingData>> m_MatialData;

	// FbxMeshSet(const FbxMeshSet& _Other) = delete;
	// FbxMeshSet(FbxMeshSet&& _Other) noexcept = delete;
	// FbxMeshSet& operator=(const FbxMeshSet& _Other) = delete;
	// FbxMeshSet& operator=(FbxMeshSet&& _Other) noexcept = delete;

	FbxMeshSet() :
		IsLod(false),
		IsLodLv(-1)
	{
	}

	~FbxMeshSet()
	{
		for (size_t i = 0; i < GameEngineVertexBuffers.size(); i++)
		{
			if (nullptr == GameEngineVertexBuffers[i])
			{
				continue;
			}
			delete GameEngineVertexBuffers[i];
			GameEngineVertexBuffers[i] = nullptr;
		}

		// 서브셋 때문에
		for (size_t i = 0; i < GameEngineIndexBuffers.size(); i++)
		{
			for (size_t j = 0; j < GameEngineIndexBuffers[i].size(); j++)
			{
				if (nullptr == GameEngineIndexBuffers[i][j])
				{
					continue;
				}
				delete GameEngineIndexBuffers[i][j];
				GameEngineIndexBuffers[i][j] = nullptr;
			}
		}

	}
};

struct FbxExMeshInfo
{
	std::string Name;
	fbxsdk::FbxMesh* Mesh;
	bool bTriangulated;

	unsigned __int64 UniqueId;
	int FaceNum;
	int VertexNum;
	int MaterialNum;

	// 애니메이션과 연결되는 매쉬인가?
	bool bIsSkelMesh;
	std::string SkeletonRoot;
	int SkeletonElemNum;

	bool bIsLodGroup;
	std::string LODGroup;
	int LODLevel;
	int MorphNum;

	FbxExMeshInfo()
	{
		Name = "";
		UniqueId = 0;
		FaceNum = 0;
		VertexNum = 0;
		bTriangulated = false;
		MaterialNum = 0;
		bIsSkelMesh = false;
		SkeletonRoot = "";
		SkeletonElemNum = 0;
		bIsLodGroup = false;
		LODGroup = "";
		LODLevel = 0;
		MorphNum = 0;
	}
};

// 설명 : 
class GameEngineFBXMesh : public GameEngineFBX
{
public:
	GameEngineFBXMesh();
	~GameEngineFBXMesh();

	void Load(const std::string& _Path);
	void MeshLoad();
	void CreateRenderingBuffer();

protected:
	GameEngineFBXMesh(const GameEngineFBXMesh& _other) = delete; 
	GameEngineFBXMesh(GameEngineFBXMesh&& _other) noexcept = delete;
	GameEngineFBXMesh& operator=(const GameEngineFBXMesh& _other) = delete;
	GameEngineFBXMesh& operator=(const GameEngineFBXMesh&& _other) = delete;

private:
	void MeshNodeCheck();
	void VertexBufferCheck();
	
	fbxsdk::FbxAMatrix ComputeTotalMatrix(fbxsdk::FbxNode* Node);

	bool IsOddNegativeScale(const fbxsdk::FbxAMatrix& TotalMatrix);

	// 기하 컨버트
	float4x4 FbxMatTofloat4x4(const fbxsdk::FbxAMatrix& _BaseTrans);
	fbxsdk::FbxAMatrix float4x4ToFbxAMatrix(const float4x4& _MATRIX);
	float4 FbxVecTofloat4(const fbxsdk::FbxVector4& _BaseVector);
	float4 FbxVecToTransform(const fbxsdk::FbxVector4& _BaseVector);
	float4 FbxQuaternionTofloat4(const fbxsdk::FbxQuaternion& _BaseVector);

	// 탄젠트 바이노말 계산

	void LoadBinormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index);
	void LoadTangent(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index);
	void LoadNormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index);

	void CreateVertexBuffer();
	void CreateIndexBuffer();
	// Lod
	// 부모쪽으로 계속 올라가면서 LOD 그룹이 나올때까지 찾는 함수
	fbxsdk::FbxNode* RecursiveFindParentLodGroup(fbxsdk::FbxNode* parentNode);

private:
	std::vector<FbxExMeshInfo> MeshInfos;

	// int lod
	// int 0 lod 매쉬
	// int 1 lod 매쉬
	// int 2 lod 매쉬
	std::map<int, FbxMeshSet> MeshLodMap;
};


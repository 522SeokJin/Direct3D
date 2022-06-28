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
	// �ȼ����̴����� �׳� �������� �����ָ� �Ǵ� ����.
	float4			 DifColor;
	float4			 AmbColor; // ��
	float4			 SpcColor; // ��
	float4			 EmvColor; // ��
	float			 SpecularPower;		// ���� ����
	float			 TransparencyFactor;	// ���� ����
	float			 Shininess;			// ���� ����
	std::string DifTexturePath;	// �ؽ�ó��� 
	std::string NorTexturePath; // �ؽ�ó���
	std::string SpcTexturePath; // �ؽ�ó���

	std::string DifTextureName;	// �ؽ�ó��� 
	std::string NorTextureName; // �ؽ�ó���
	std::string SpcTextureName; // �ؽ�ó���

public:
	FbxExRenderingPipeLineSettingData() {}
	~FbxExRenderingPipeLineSettingData() {}
};


struct FbxMeshSet
{
public:
	int IsLodLv;
	bool IsLod;

	//       �ڽ��� ������ 
	//       ��� �ִ� node
	//       �� mesh
	//       �̸Ž�����  �������� ���´ٶ�� ���� �˴ϴ�.
	//       ���� ���鿡 ���� ��� �����̰�.
	//       ���� �ʿ��ϴٸ� �� ���� �Ҽ��� �ִ�.
	std::map<FbxMesh*, std::vector<GameEngineVertex>*> m_FbxVtxMap;

	//       �ִϸ��̼��� �ִٸ� ä���� �����̴ϴ�.
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

		// ����� ������
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

	// �ִϸ��̼ǰ� ����Ǵ� �Ž��ΰ�?
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

// ���� : 
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

	// ���� ����Ʈ
	float4x4 FbxMatTofloat4x4(const fbxsdk::FbxAMatrix& _BaseTrans);
	fbxsdk::FbxAMatrix float4x4ToFbxAMatrix(const float4x4& _MATRIX);
	float4 FbxVecTofloat4(const fbxsdk::FbxVector4& _BaseVector);
	float4 FbxVecToTransform(const fbxsdk::FbxVector4& _BaseVector);
	float4 FbxQuaternionTofloat4(const fbxsdk::FbxQuaternion& _BaseVector);

	// ź��Ʈ ���̳븻 ���

	void LoadBinormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index);
	void LoadTangent(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index);
	void LoadNormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index);

	void CreateVertexBuffer();
	void CreateIndexBuffer();
	// Lod
	// �θ������� ��� �ö󰡸鼭 LOD �׷��� ���ö����� ã�� �Լ�
	fbxsdk::FbxNode* RecursiveFindParentLodGroup(fbxsdk::FbxNode* parentNode);

private:
	std::vector<FbxExMeshInfo> MeshInfos;

	// int lod
	// int 0 lod �Ž�
	// int 1 lod �Ž�
	// int 2 lod �Ž�
	std::map<int, FbxMeshSet> MeshLodMap;
};


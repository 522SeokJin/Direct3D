#include "PreCompile.h"
#include "GameEngineCore.h"
#include "GameEngineWindow.h"
#include "GameEngineResourcesManager.h"
#include "GameEngineCollision.h"
#include "GameEngineBase/GameEngineDirectory.h"
#include "GameEngineBase/GameEngineFile.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineDepthStencil.h"
#include "EngineVertex.h"


void GameEngineCore::EngineResourcesLoad()
{
	{
		GameEngineDirectory EngineDir;

		while (true)
		{
			if (EngineDir.IsRoot())
			{
				GameEngineDebug::MsgBoxError("���� ���ҽ� ������ �������� �ʽ��ϴ�.");
				return;
			}

			std::vector<GameEngineDirectory> AllDir = EngineDir.GetAllDirectory("EngineResources");

			if (0 == AllDir.size())
			{
				EngineDir.MoveParent();
				continue;
			}

			EngineDir.MoveChild("EngineResources");
			break;
		}

		EngineDir.MoveChild("Texture");

		std::vector<GameEngineFile> AllFile = EngineDir.GetAllFile();

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		}
	}

	{
		GameEngineDirectory EngineDir;

		while (true)
		{
			if (EngineDir.IsRoot())
			{
				GameEngineDebug::MsgBoxError("���� ���ҽ� ������ �������� �ʽ��ϴ�.");
				return;
			}

			std::vector<GameEngineDirectory> AllDir = EngineDir.GetAllDirectory("EngineResources");

			if (0 == AllDir.size())
			{
				EngineDir.MoveParent();
				continue;
			}

			EngineDir.MoveChild("EngineResources");
			break;
		}

		EngineDir.MoveChild("Shader");

		std::vector<GameEngineFile> AllShader = EngineDir.GetAllFile("fx");

		for (auto& ShaderFile : AllShader)
		{
			ShaderFile.Open("rt");

			std::string FileName = ShaderFile.GetFileNameWithOutExtension();
			std::string AllCode = ShaderFile.GetString();

			if (std::string::npos != AllCode.find(FileName + "_VS"))
			{
				GameEngineVertexShader* Ptr = GameEngineVertexShaderManager::GetInst().Load(FileName + "_VS", ShaderFile.GetFullPath(), FileName + "_VS");
			}

			if (std::string::npos != AllCode.find(FileName + "_PS"))
			{
				GameEnginePixelShader* Ptr = GameEnginePixelShaderManager::GetInst().Load(FileName + "_PS", ShaderFile.GetFullPath(), FileName + "_PS");
			}

		}
	}

	GameEngineSampler* NewRes = GameEngineSamplerManager::GetInst().Find("PointSmp");
	NewRes->Info_.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	NewRes->ReCreate();
}
void GameEngineCore::EngineResourcesCreate()
{

	{
		std::vector<GameEngineVertex> Vertex = std::vector<GameEngineVertex>(4 * 6);

		{
			Vertex[0] = { float4({ -0.5f, 0.5f, 0.5f }) };
			Vertex[1] = { float4({ 0.5f, 0.5f, 0.5f }) };
			Vertex[2] = { float4({ 0.5f, -0.5f, 0.5f }) };
			Vertex[3] = { float4({ -0.5f, -0.5f, 0.5f }) };

			Vertex[4] = { float4::RotateXDegree(Vertex[0].Postion, 180.0f) };
			Vertex[5] = { float4::RotateXDegree(Vertex[1].Postion, 180.0f) };
			Vertex[6] = { float4::RotateXDegree(Vertex[2].Postion, 180.0f) };
			Vertex[7] = { float4::RotateXDegree(Vertex[3].Postion, 180.0f) };
		}

		{
			Vertex[8] = { float4::RotateYDegree(Vertex[0].Postion, 90.0f) };
			Vertex[9] = { float4::RotateYDegree(Vertex[1].Postion, 90.0f) };
			Vertex[10] = { float4::RotateYDegree(Vertex[2].Postion, 90.0f) };
			Vertex[11] = { float4::RotateYDegree(Vertex[3].Postion, 90.0f) };

			Vertex[12] = { float4::RotateYDegree(Vertex[0].Postion, -90.0f) };
			Vertex[13] = { float4::RotateYDegree(Vertex[1].Postion, -90.0f) };
			Vertex[14] = { float4::RotateYDegree(Vertex[2].Postion, -90.0f) };
			Vertex[15] = { float4::RotateYDegree(Vertex[3].Postion, -90.0f) };
		}

		{
			Vertex[16] = { float4::RotateXDegree(Vertex[0].Postion, 90.0f) };
			Vertex[17] = { float4::RotateXDegree(Vertex[1].Postion, 90.0f) };
			Vertex[18] = { float4::RotateXDegree(Vertex[2].Postion, 90.0f) };
			Vertex[19] = { float4::RotateXDegree(Vertex[3].Postion, 90.0f) };

			Vertex[20] = { float4::RotateXDegree(Vertex[0].Postion, -90.0f) };
			Vertex[21] = { float4::RotateXDegree(Vertex[1].Postion, -90.0f) };
			Vertex[22] = { float4::RotateXDegree(Vertex[2].Postion, -90.0f) };
			Vertex[23] = { float4::RotateXDegree(Vertex[3].Postion, -90.0f) };
		}

		for (size_t i = 0; i < Vertex.size(); i += 4)
		{
			Vertex[i + 0].Texcoord = { 0.0f, 0.0f };
			Vertex[i + 1].Texcoord = { 1.0f, 0.0f };
			Vertex[i + 2].Texcoord = { 1.0f, 1.0f };
			Vertex[i + 3].Texcoord = { 0.0f, 1.0f };
		}

		GameEngineVertexBufferManager::GetInst().Create("Box", Vertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> BoxIndex;

		for (int i = 0; i < 6; i++)
		{
			BoxIndex.push_back(i * 4 + 0);
			BoxIndex.push_back(i * 4 + 1);
			BoxIndex.push_back(i * 4 + 2);

			BoxIndex.push_back(i * 4 + 0);
			BoxIndex.push_back(i * 4 + 2);
			BoxIndex.push_back(i * 4 + 3);
		}

		GameEngineIndexBufferManager::GetInst().Create("Box", BoxIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}



	{
		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(4);

		{
			// �ո�
			RectVertex[0] = { float4({ -0.5f, 0.5f, 0.0f }),  { 0.0f, 0.0f } };
			RectVertex[1] = { float4({ 0.5f, 0.5f, 0.0f }),  { 1.0f, 0.0f } };
			RectVertex[2] = { float4({ 0.5f, -0.5f, 0.0f }),  { 1.0f, 1.0f } };
			RectVertex[3] = { float4({ -0.5f, -0.5f, 0.0f }),  { 0.0f, 1.0f } };
		}

		GameEngineVertexBufferManager::GetInst().Create("Rect", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}


	{
		std::vector<UINT> RectIndex;

		RectIndex.push_back(0);
		RectIndex.push_back(1);
		RectIndex.push_back(2);

		RectIndex.push_back(0);
		RectIndex.push_back(2);
		RectIndex.push_back(3);

		GameEngineIndexBufferManager::GetInst().Create("Rect", RectIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}


	{
		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(5);

		{
			// �ո�
			RectVertex[0] = { float4({ -0.5f, 0.5f, 0.0f }),  { 0.0f, 0.0f } };
			RectVertex[1] = { float4({ 0.5f, 0.5f, 0.0f }),  { 1.0f, 0.0f } };
			RectVertex[2] = { float4({ 0.5f, -0.5f, 0.0f }),  { 1.0f, 1.0f } };
			RectVertex[3] = { float4({ -0.5f, -0.5f, 0.0f }),  { 0.0f, 1.0f } };
			RectVertex[4] = { float4({ -0.5f, 0.5f, 0.0f }),  { 0.0f, 0.0f } };
		}

		GameEngineVertexBufferManager::GetInst().Create("DebugRect", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> RectIndex;

		RectIndex.push_back(0);
		RectIndex.push_back(1);
		RectIndex.push_back(2);
		RectIndex.push_back(3);
		RectIndex.push_back(4);

		GameEngineIndexBufferManager::GetInst().Create("DebugRect", RectIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<GameEngineVertex> RectVertex = std::vector<GameEngineVertex>(4);

		{
			// �ո�
			RectVertex[0] = { float4({ -1.0f, 1.0f, 0.0f }),float4({ 0.0f, 0.0f }) };
			RectVertex[1] = { float4({ 1.0f, 1.0f, 0.0f }), float4({ 1.0f, 0.0f }) };
			RectVertex[2] = { float4({ 1.0f, -1.0f, 0.0f }), float4({ 1.0f, 1.0f }) };
			RectVertex[3] = { float4({ -1.0f, -1.0f, 0.0f }),  float4({ 0.0f, 1.0f })};
		}

		GameEngineVertexBufferManager::GetInst().Create("FullRect", RectVertex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}

	{
		std::vector<UINT> RectIndex;

		RectIndex.push_back(0);
		RectIndex.push_back(1);
		RectIndex.push_back(2);

		RectIndex.push_back(0);
		RectIndex.push_back(2);
		RectIndex.push_back(3);

		GameEngineIndexBufferManager::GetInst().Create("FullRect", RectIndex, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	}



	{
		D3D11_RASTERIZER_DESC Info = { D3D11_FILL_MODE::D3D11_FILL_SOLID, };
		Info.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		Info.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Info.AntialiasedLineEnable = true;
		Info.MultisampleEnable = true;
		GameEngineRasterizer* Ptr = GameEngineRasterizerManager::GetInst().Create("EngineBaseRasterizer", Info);
		Ptr->AddWindowSizeViewPort();
	}

	{
		D3D11_BLEND_DESC BlendInfo = { 0 };

		BlendInfo.AlphaToCoverageEnable = FALSE;
		BlendInfo.IndependentBlendEnable = FALSE;
		BlendInfo.RenderTarget[0].BlendEnable = true;
		BlendInfo.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		BlendInfo.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		BlendInfo.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		BlendInfo.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		BlendInfo.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		BlendInfo.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		BlendInfo.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;

		GameEngineBlendManager::GetInst().Create("AlphaBlend", BlendInfo);
	}

	{
		D3D11_BLEND_DESC BlendInfo = { 0 };

		BlendInfo.AlphaToCoverageEnable = FALSE;
		BlendInfo.IndependentBlendEnable = FALSE;
		BlendInfo.RenderTarget[0].BlendEnable = true;
		BlendInfo.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		BlendInfo.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_MAX;
		BlendInfo.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		BlendInfo.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_DEST_COLOR;
		BlendInfo.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		BlendInfo.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		BlendInfo.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;

		GameEngineBlendManager::GetInst().Create("Trans", BlendInfo);
	}


	{
		D3D11_DEPTH_STENCIL_DESC DepthInfo = {0};

		DepthInfo.DepthEnable = true;
		DepthInfo.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		DepthInfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		DepthInfo.StencilEnable = false;
		GameEngineDepthStencilManager::GetInst().Create("BaseDepthOn", DepthInfo);
	}

	{
		D3D11_DEPTH_STENCIL_DESC DepthInfo = { 0 };

		DepthInfo.DepthEnable = true;
		DepthInfo.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		DepthInfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		DepthInfo.StencilEnable = false;
		GameEngineDepthStencilManager::GetInst().Create("MergeDepth", DepthInfo);
	}


	{
		D3D11_DEPTH_STENCIL_DESC DepthInfo = { 0 };

		DepthInfo.DepthEnable = false;
		DepthInfo.StencilEnable = false;
		GameEngineDepthStencilManager::GetInst().Create("BaseDepthOff", DepthInfo);
	}

	{
		D3D11_SAMPLER_DESC Smp_Decs = {};

		memset(&Smp_Decs, 0, sizeof(D3D11_SAMPLER_DESC));
		Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		Smp_Decs.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		Smp_Decs.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		Smp_Decs.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		Smp_Decs.MipLODBias = 0.0f;
		Smp_Decs.MaxAnisotropy = 1;
		Smp_Decs.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		Smp_Decs.MinLOD = -FLT_MAX;
		Smp_Decs.MaxLOD = FLT_MAX;
		// Smp_Decs.BorderColor;
		// Smp_Decs.MaxAnisotropy;

		GameEngineSampler* NewRes = GameEngineSamplerManager::GetInst().CreateAndFind("LINEARSmp", Smp_Decs);
	}


	{
		D3D11_SAMPLER_DESC Smp_Decs = {};

		memset(&Smp_Decs, 0, sizeof(D3D11_SAMPLER_DESC));
		Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		Smp_Decs.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		Smp_Decs.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		Smp_Decs.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		Smp_Decs.MipLODBias = 0.0f;
		Smp_Decs.MaxAnisotropy = 1;
		Smp_Decs.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		Smp_Decs.MinLOD = -FLT_MAX;
		Smp_Decs.MaxLOD = FLT_MAX;
		// Smp_Decs.BorderColor;
		// Smp_Decs.MaxAnisotropy;

		GameEngineSampler* NewRes = GameEngineSamplerManager::GetInst().CreateAndFind("PointSmp", Smp_Decs);
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("DebugRect");
		Pipe->SetInputAssembler1VertexBufferSetting("DebugRect");
		Pipe->SetInputAssembler1InputLayOutSetting("Color_VS");
		Pipe->SetVertexShader("Color_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("DebugRect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		Pipe->SetPixelShader("Color_PS");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TargetMerge");
		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetInputAssembler1InputLayOutSetting("TargetMerge_VS");
		Pipe->SetVertexShader("TargetMerge_VS");
		Pipe->SetPixelShader("TargetMerge_PS");
		Pipe->SetOutputMergerDepthStencil("BaseDepthOff");
	}


	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("Color");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetInputAssembler1InputLayOutSetting("Color_VS");
		Pipe->SetVertexShader("Color_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizer");
		Pipe->SetPixelShader("Color_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("Texture");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetInputAssembler1InputLayOutSetting("Texture_VS");
		Pipe->SetVertexShader("Texture_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizer");
		Pipe->SetPixelShader("Texture_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}


	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TextureUI");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetInputAssembler1InputLayOutSetting("Texture_VS");
		Pipe->SetVertexShader("Texture_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizer");
		Pipe->SetPixelShader("Texture_PS");
		Pipe->SetOutputMergerBlend("AlphaBlend");
		Pipe->SetOutputMergerDepthStencil("BaseDepthOff");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("Fade");
		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetInputAssembler1InputLayOutSetting("Fade_VS");
		Pipe->SetVertexShader("Fade_VS");
		Pipe->SetPixelShader("Fade_PS");
		Pipe->SetOutputMergerDepthStencil("BaseDepthOff");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}


	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("Blur");
		Pipe->SetInputAssembler1VertexBufferSetting("FullRect");
		Pipe->SetInputAssembler2IndexBufferSetting("FullRect");
		Pipe->SetInputAssembler1InputLayOutSetting("Blur_VS");
		Pipe->SetVertexShader("Blur_VS");
		Pipe->SetPixelShader("Blur_PS");
		Pipe->SetOutputMergerDepthStencil("BaseDepthOff");
		Pipe->SetOutputMergerBlend("AlphaBlend");
	}

	{
		GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Create("TextureTrans");
		Pipe->SetInputAssembler1VertexBufferSetting("Rect");
		Pipe->SetInputAssembler1InputLayOutSetting("Texture_VS");
		Pipe->SetVertexShader("Texture_VS");
		Pipe->SetInputAssembler2IndexBufferSetting("Rect");
		Pipe->SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pipe->SetRasterizer("EngineBaseRasterizer");
		Pipe->SetPixelShader("Texture_PS");
		Pipe->SetOutputMergerBlend("Trans");
	}




}
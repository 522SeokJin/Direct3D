#pragma once
#include <GameEngine/GameEngineDevice.h>
#include <GameEngineBase/GameEngineMath.h>
#include "GameEngineShaderResHelper.h"



// �з� : 
// �뵵 : 
// ���� : 
class GameEngineBlend;
class GameEngineShader;
class GameEngineDepthStencil;
class GameEngineConstantBuffer;
class GameEngineIndexBuffer;
class GameEngineVertexBuffer;
class GameEngineVertexShader;
class GameEnginePixelShader;
class GameEngineRasterizer;
class GameEngineRenderingPipeLine : public GameEngineObjectNameBase
{
private:	// member Var
	// IA1
	GameEngineVertexBuffer* VertexBuffer_;
	// IA1
	GameEngineVertexShader* InputLayOutVertexShader_;
	// VS
	GameEngineVertexShader* VertexShader_;
	// IA2
	GameEngineIndexBuffer* IndexBuffer_;
	// IA2
	D3D11_PRIMITIVE_TOPOLOGY Topology_;
	// RS
	GameEngineRasterizer* Rasterizer_;
	// PS
	GameEnginePixelShader* PixelShader_;
	// Blend
	GameEngineBlend* Blend_;
	// OM
	GameEngineRenderTarget* RenderTarget_;

	GameEngineDepthStencil* DepthStencil_;

public:
	inline GameEngineRasterizer* GetRasterizer() const
	{
		return Rasterizer_;
	}


	inline GameEnginePixelShader* GetPixelShader() const
	{
		return PixelShader_;
	}

	inline GameEngineVertexShader* GetVertexShader() const
	{
		return VertexShader_;
	}

	void SetInputAssembler1InputLayOutSetting(const std::string& _Name);

	void SetInputAssembler1VertexBufferSetting(const std::string& _Name);

	void SetVertexShader(const std::string& _Name);

	void SetInputAssembler2IndexBufferSetting(const std::string& _Name);

	void SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY Topology_);

	void SetRasterizer(const std::string& _Name);

	void SetPixelShader(const std::string& _Name);

	void SetOutputMergerBlend(const std::string& _Name);

	void SetOutputMergerDepthStencil(const std::string& _Name);

	void RenderingPipeLineSetting();

	void Rendering();

	void Reset();

	void InstanceRendering();

	GameEngineRenderingPipeLine* Clone();


	void RasterizerClone();


public:
	GameEngineRenderingPipeLine(); // default constructer ����Ʈ ������
	~GameEngineRenderingPipeLine(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineRenderingPipeLine(const GameEngineRenderingPipeLine& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineRenderingPipeLine(GameEngineRenderingPipeLine&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

	void InputAssembler1();
	void InputAssembler2();
	void VertexShader();
	void Rasterizer();
	void PixelShader();
	void OutPutMerger();
};

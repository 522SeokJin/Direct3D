#include "CbufferHeader.fx"
#include "LightHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Normal : NORMAL;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float4 ViewNormal : NORMAL;
};

VertexOut Color_VS(VertexIn _In)
{
    VertexOut Out = (VertexOut) 0;
    
    Out.Position.w = 1.0f;
    Out.ViewNormal.w = 0.0f;
    // 픽셀을 건져내기 위한 포지션
    Out.Position = mul(_In.Position, WVP);
    Out.ViewNormal = mul(_In.Normal, WV);
    Out.ViewNormal = normalize(Out.ViewNormal);

    return Out;
}

cbuffer ResultColor : register(b0)
{
    float4 vColor;
};

float4 Color_PS(float4 pos : SV_POSITION) : SV_Target0
{
    float4 Color = Lights[0].ViewLightDir + vColor;
    
    return Color;
}
// 라이트 1개의 데이터
struct LightData 
{
	float4 LightDir; // 라이트의 포워드 벡터
	float4 NegLightDir; // 라이트의 포워드 -벡터
};

cbuffer LightsData : register(b11)
{
    int LightCount;
    LightData Lights[128];
};
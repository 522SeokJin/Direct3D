// 라이트 1개의 데이터
struct LightData 
{
	float4 ViewLightDir; // 라이트의 포워드 벡터
    float4 ViewNegLightDir; // 라이트의 포워드 -벡터
};

cbuffer LightsData : register(b11)
{
    int LightCount;
    LightData Lights[128];
};
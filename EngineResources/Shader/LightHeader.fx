// ����Ʈ 1���� ������
struct LightData 
{
	float4 LightDir; // ����Ʈ�� ������ ����
	float4 NegLightDir; // ����Ʈ�� ������ -����
};

cbuffer LightsData : register(b11)
{
    int LightCount;
    LightData Lights[128];
};
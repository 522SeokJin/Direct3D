// ����Ʈ 1���� ������
struct LightData 
{
	float4 ViewLightDir; // ����Ʈ�� ������ ����
    float4 ViewNegLightDir; // ����Ʈ�� ������ -����
};

cbuffer LightsData : register(b11)
{
    int LightCount;
    LightData Lights[128];
};
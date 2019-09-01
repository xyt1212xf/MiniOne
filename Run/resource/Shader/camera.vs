cbuffer CameraBuffer : register(b5)
{
    float3 cameraPosition;
	float padding;
}

struct CameraVSType
{
    float4 position : SV_POSITION;
};
////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
CameraVSType vMain()
{
	CameraVSType output;
	output.position = float4(0,0,0,0);
	
	return output;
}
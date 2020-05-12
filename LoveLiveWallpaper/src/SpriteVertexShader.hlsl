cbuffer matBuffer
{
	matrix tranMat;
	matrix viewMat;
	matrix projMat;
};

struct vtk
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float4 col : COLOR;
};

vtk main(float4 pos : POSITION, float2 texCoord : TEXCOORD0, float4 color : COLOR)
{
	vtk output;
	output.pos = pos;
	output.pos.w = 1.0;
	output.pos = mul(output.pos, tranMat);
	//output.pos = mul(output.pos, viewMat);
	//output.pos = mul(output.pos, projMat);
	output.texCoord = texCoord;
	output.col = color;
	return output;
}
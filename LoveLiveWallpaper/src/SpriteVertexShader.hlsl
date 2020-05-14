cbuffer matBuffer
{
	matrix tranMat;
	float4 vtkColor;
};

struct vtk
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float4 color : COLOR;
};

vtk main(float4 pos : POSITION, float2 texCoord : TEXCOORD0)
{
	vtk output;
	output.pos = pos;
	output.pos.w = 1.0;
	output.pos = mul(output.pos, tranMat);
	output.texCoord = texCoord;
	output.color = vtkColor;
	return output;
}
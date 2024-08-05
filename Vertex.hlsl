struct PSInput {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

PSInput main(float4 pos : POSITION, float4 color : COLOR) {
	PSInput input;
	input.pos = pos;
	input.color = color;
	return input;
}
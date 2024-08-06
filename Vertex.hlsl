struct PSInput {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

struct CB {
	float4x4 model;
	float4x4 view;
	float4x4 projection;
	float4x4 pad; // Add a padding matrix to align the constant buffer
};


ConstantBuffer<CB> buf : register(b0);
PSInput main(float4 pos : POSITION, float4 color : COLOR) {

	PSInput output;

	// Apply transformations in the correct order:
	output.pos = mul(pos, buf.model); // Model transformation
	output.pos = mul(output.pos, buf.view); // View transformation
	output.pos = mul(output.pos, buf.projection); // Projection transformation

	//output.pos = pos;
	output.color = color;
	//output.color = mul(pos, buf.model); // Model transformation
	//output.color = mul(output.color, buf.view); // View transformation
	//output.color = mul(output.color, buf.projection); // Projection transformation
	
	return output;
}
#pragma once
#include "Shader.h"

class PixelShader: public Shader
{
public:
	PixelShader(LPCWSTR path, ShaderCompiler* compiler);
};


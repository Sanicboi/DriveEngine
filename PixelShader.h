#pragma once
#include "Shader.h"

class PixelShader: public Shader
{
public:
	using Shader::Shader;
	PixelShader(LPCWSTR path, ShaderCompiler* compiler);
};


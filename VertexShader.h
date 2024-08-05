#pragma once
#include "Shader.h"
class VertexShader : public Shader
{
public:
	using Shader::Shader;
	VertexShader(LPCWSTR path, ShaderCompiler* compiler);
};


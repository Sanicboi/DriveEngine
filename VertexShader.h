#pragma once
#include "Shader.h"
class VertexShader : public Shader
{
public:
	VertexShader(LPCWSTR path, ShaderCompiler* compiler);
};


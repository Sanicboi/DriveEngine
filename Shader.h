#pragma once
#include "SinglePtr.h"
#include "ShaderCompiler.h"
class Shader : public SinglePtr<IDxcBlob>
{
public:
	Shader(LPCWSTR path, LPCWSTR model, ShaderCompiler* compiler);
};


#pragma once
#include "SinglePtr.h"
#include "ShaderCompiler.h"
class Shader : public SinglePtr<IDxcBlob>
{
public:
	using SinglePtr::SinglePtr;
	Shader(LPCWSTR path, LPCWSTR model, ShaderCompiler* compiler);
};


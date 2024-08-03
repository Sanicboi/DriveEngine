#pragma once
#include "Hresultable.h"
class ShaderCompiler : public Hresultable
{
private:
	ComPtr<IDxcUtils> utils;
	ComPtr<IDxcCompiler3> compiler;
	ComPtr<IDxcIncludeHandler> includeHandler;
public:
	ShaderCompiler();
	HRESULT Compile(LPCWSTR entry, LPCWSTR model, LPCWSTR source, IDxcBlob** out);
};


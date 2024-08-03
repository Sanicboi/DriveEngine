#include "Shader.h"

Shader::Shader(LPCWSTR path, LPCWSTR model, ShaderCompiler* compiler)
{
	hr = compiler->Compile(L"main", model, path, &ptr);
}

#include "ShaderCompiler.h"

ShaderCompiler::ShaderCompiler()
{
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
	if (SUCCEEDED(hr)) {
		hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));
		if (SUCCEEDED(hr)) {
			hr = utils->CreateDefaultIncludeHandler(&includeHandler);
		}
	}
}

HRESULT ShaderCompiler::Compile(LPCWSTR entry, LPCWSTR model, LPCWSTR source, IDxcBlob** out)
{
	std::vector<LPCWSTR> arguments;
	arguments.push_back(L"-E");
	arguments.push_back(entry);

	arguments.push_back(L"-T");
	arguments.push_back(model);

	
	arguments.push_back(L"-Qstrip_debug");
#ifdef _DEBUG
	arguments.push_back(L"-Od");
#endif // _DEBUG

	//arguments.push_back(L"-Qstrip_reflect");

	arguments.push_back(DXC_ARG_DEBUG);


	ComPtr<ID3DBlob> src;
	D3DReadFileToBlob(source, &src);



	if (SUCCEEDED(hr)) {
		DxcBuffer sourceBuffer;
		sourceBuffer.Encoding = 0;
		sourceBuffer.Ptr = src->GetBufferPointer();
		sourceBuffer.Size = src->GetBufferSize();

		ComPtr<IDxcResult> result;

		hr = compiler->Compile(&sourceBuffer, arguments.data(), arguments.size(), includeHandler.Get(), IID_PPV_ARGS(&result));

		if (SUCCEEDED(hr)) {
#ifdef _DEBUG
			ComPtr<IDxcBlobUtf8> errs;
			result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errs), NULL);
			if (errs->GetStringLength() > 0) {
				OutputDebugStringA((LPCSTR)errs->GetBufferPointer());
				OutputDebugStringA("\n");
			}
				ComPtr<ID3DBlob> pDebugData;
				ComPtr<IDxcBlobUtf16> pDebugDataPath;
				result->GetOutput(DXC_OUT_PDB, IID_PPV_ARGS(pDebugData.GetAddressOf()), pDebugDataPath.GetAddressOf());
				D3DWriteBlobToFile(pDebugData.Get(), (std::wstring(model) + L".pdb").c_str(), TRUE);
#endif
			result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(out), NULL);
		}


	}

	return hr;
}


#include "Output.h"

Output::Output(IDXGIAdapter4* adapter)
{
	ComPtr<IDXGIOutput> temp;
	hr = adapter->EnumOutputs(0, &temp);
	if (Ok()) {
		hr = temp.As<IDXGIOutput6>(&ptr);
	}
}

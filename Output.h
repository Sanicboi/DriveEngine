#pragma once
#include "SinglePtr.h"
class Output : public SinglePtr<IDXGIOutput6>
{
public:
	using SinglePtr::SinglePtr;
	Output(IDXGIAdapter4* adapter);
};


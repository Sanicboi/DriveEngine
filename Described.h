#pragma once
#include "SinglePtr.h"
template<class I, typename T>
class Described : public SinglePtr<I>
{
protected:
	T descriptor;
public:
	T GetDescriptor() const {
		return descriptor;
	};
};


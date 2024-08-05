#pragma once
#include "Hresultable.h"
template<class T>
class SinglePtr : public Hresultable
{
protected:
	ComPtr<T> ptr;

public:
	T** operator&() {
		return &ptr;
	}
	T* operator->() {
		return ptr.Get();
	}
	T** Ref() {
		return &ptr;
	}
	T* Get() {
		return ptr.Get();
	}
	~SinglePtr() {

	}
};

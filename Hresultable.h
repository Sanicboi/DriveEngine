#pragma once
#include "Main.h"
class Hresultable
{
protected:
	HRESULT hr;
public:
	HRESULT GetHr() const;
	bool Ok() const;
	Hresultable();
};


#include "Hresultable.h"


HRESULT Hresultable::GetHr() const {
	return hr;
}

bool Hresultable::Ok() const {
	return SUCCEEDED(hr);
}

Hresultable::Hresultable()
{
	hr = E_NOTIMPL;
}

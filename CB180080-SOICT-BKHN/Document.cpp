#include "pch.h"
#include "Document.h"
#include "Utils.h"
int Document::GetInterval()
{
	if (m_pDisp == nullptr)
		return 0;
	VARIANT result;
	VariantInit(&result);
	auto hr = Utils::OLEMethod(DISPATCH_PROPERTYGET, &result, m_pDisp, L"Interval", 0);
	if (FAILED(hr))
		return 0;
	return result.intVal;
}

#include "pch.h"
#include "Application.h"
#include "Utils.h"
#include "Document.h"
Application::Application()
{
}
Application* Application::GetInstance()
{
	static Application __instance;
	return &__instance;
}

Document* Application::GetActiveDocument()
{
	if (m_pDisp == nullptr)
		return nullptr;
	VARIANT result;
	VariantInit(&result);
	Utils::OLEMethod(DISPATCH_PROPERTYGET, &result, m_pDisp, L"ActiveDocument", 0);
	if (result.pdispVal)
		return new Document(result.pdispVal);
	return nullptr;
}

void Application::StartOLE()
{
	CoInitialize(NULL);
	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(L"Broker.Application", &clsid);
	// "Excel.Application" for MSExcel

	if (SUCCEEDED(hr))
	{
		hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER,
			IID_IDispatch, (void**)&m_pDisp);
	}
}

void Application::EndOLE()
{
	CoUninitialize();
}

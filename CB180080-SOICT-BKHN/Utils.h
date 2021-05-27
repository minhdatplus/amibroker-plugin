#pragma once
class Utils
{
public:
	static HRESULT OLEMethod(int nType, VARIANT* pvResult,
		IDispatch* pDisp, LPOLESTR ptName, int cArgs...);
	static std::wstring S2WS(const std::string& str);
	static std::string WS2S(const std::wstring& str);
};


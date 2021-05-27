#pragma once
class PluginManager
{
private:
	PluginManager() {}
public:
	static PluginManager* GetInstance();
	void SetHwnd(HWND hwnd);
protected:
	HWND m_hwndAmiBroker;
};


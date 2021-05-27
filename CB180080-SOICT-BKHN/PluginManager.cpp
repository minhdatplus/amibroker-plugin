#include "pch.h"
#include "PluginManager.h"

PluginManager* PluginManager::GetInstance()
{
    static PluginManager __instance;
    return &__instance;
}

void PluginManager::SetHwnd(HWND hwnd)
{
    m_hwndAmiBroker = hwnd;
}

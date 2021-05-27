#include "pch.h"
#include "COMObject.h"

CB180080::COMObject::COMObject()
{
}

CB180080::COMObject::COMObject(IDispatch* disp)
{
	m_pDisp = disp;
}

IDispatch* CB180080::COMObject::getDispatch()
{
	return m_pDisp;
}

CB180080::COMObject::~COMObject()
{
	//delete m_pDisp;
}

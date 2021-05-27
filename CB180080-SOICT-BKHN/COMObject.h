#pragma once
namespace CB180080 {
	class COMObject
	{
	public:
		COMObject();
		COMObject(IDispatch* disp);
		IDispatch* getDispatch();
		virtual ~COMObject();
	protected:
		IDispatch* m_pDisp;
	};
}


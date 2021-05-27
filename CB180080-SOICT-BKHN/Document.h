#pragma once
#include "COMObject.h"
using namespace CB180080;
namespace CB180080 {
    class Document :
        public COMObject
    {
    public:
        Document(IDispatch* pdisp) :COMObject(pdisp) {}
        int GetInterval();
    };
}

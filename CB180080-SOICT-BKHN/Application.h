#pragma once
#include "COMObject.h"
#include "Document.h"
using namespace CB180080;
namespace CB180080 {
    class Application :
        public COMObject
    {
    private:
        Application();
    public:
        static Application* GetInstance();
        Document* GetActiveDocument();
        void StartOLE();
        void EndOLE();
    protected:
        CLSID m_clsid;
    };
}

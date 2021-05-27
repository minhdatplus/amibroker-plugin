// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the {0}_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// {0}_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CB180080SOICTBKHN_EXPORTS
#define CB180080SOICTBKHN_API __declspec(dllexport)
#else
#define CB180080SOICTBKHN_API __declspec(dllimport)
#endif

// This class is exported from the dll
class CB180080SOICTBKHN_API CCB180080SOICTBKHN {
public:
	CCB180080SOICTBKHN(void);
	// TODO: add your methods here.
};

extern CB180080SOICTBKHN_API int nCB180080SOICTBKHN;

CB180080SOICTBKHN_API int fnCB180080SOICTBKHN(void);

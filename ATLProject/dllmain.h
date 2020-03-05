// dllmain.h: 模块类的声明。

class CATLProjectModule : public ATL::CAtlDllModuleT< CATLProjectModule >
{
public :
	DECLARE_LIBID(LIBID_ATLProjectLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLPROJECT, "{0656ea0a-b2c0-40f5-8859-bdadd11ed590}")
};

extern class CATLProjectModule _AtlModule;

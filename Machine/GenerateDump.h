#pragma once

#include <imagehlp.h>
#pragma comment(lib, "dbghelp.lib")

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* lpExceptionInfo);
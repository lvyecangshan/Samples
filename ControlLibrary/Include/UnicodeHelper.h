#pragma once

// unicode related
CString		__declspec(dllexport) GetUnicodeString(const char *pszText);
void		__declspec(dllexport) GetMultiByteString(char **ppszText, CString str);

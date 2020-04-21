#pragma once

#include "NonStdFontPropertySheet.h"

class __declspec(dllexport) CMasterPanel
{
public:
    CMasterPanel();
    virtual ~CMasterPanel();

    void Create(CWnd* pParentWnd, CRect rcLocation);
    void AddWindow(VOID *pWnd, BOOL bPropertyPage, enum USER_TYPE eUserLevel, int nGroup);
    BOOL ShowGroup(int nGroup, enum USER_TYPE eUserLevel);

private:
    void HideAll();

private:
    CNonStdFontPropertySheet *m_pSheet;
    
    typedef struct SWindowItem
    {
        VOID *pWnd;
        BOOL bPropertyPage; // false means other window, f.g dialog
        enum USER_TYPE eUserLevel;
        int  iGroup;
    };
    
    CList<SWindowItem, SWindowItem&> m_lstWindows;
    CWnd *m_pParentWnd;
    CRect m_rcLocation;
    int m_iPreviousGroup;
};

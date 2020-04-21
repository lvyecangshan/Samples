#pragma once

#include "output.h"
#include "RunCtrl.h"

enum TOWER_LIGHT_UNIT_STATE
{
    ALWAYS_OFF,
    ALWAYS_ON,
    SHORT_BLINK,
    LONG_BLINK,
};

enum TOWER_LIGHT_UNITS
{
    TOWER_LIGHT_RED,
    TOWER_LIGHT_AMBER,
    TOWER_LIGHT_GREEN,
    TOWER_LIGHT_BUZZER,
    NUM_OF_TOWER_LIGHT_UNITS,
};

#define TOWER_LIGHT_CTRL_TIMER 4

#define NUM_OF_TOWERLIGHT_STATE NUM_MC_STATE + 1
#define MC_NEED_ATTN NUM_MC_STATE

class __declspec(dllexport) CTowerLightCtrl : public CStatic
{
	DECLARE_DYNAMIC(CTowerLightCtrl)

public:
	CTowerLightCtrl();
	virtual ~CTowerLightCtrl();

    BOOL Initialize(CRunCtrl *pRunCtrl, COutput *pvRedLight, COutput *pvAmberLight, COutput *pvGreenLight, COutput *pvBuzzer);

public:
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

protected:
	DECLARE_MESSAGE_MAP()
    BOOL LoadTowerLightSetting();

protected:
    COutput *m_pvTowerLightUnit[NUM_OF_TOWER_LIGHT_UNITS];
    int m_nTimerCounter;

    CRunCtrl *m_pRunCtrl;

    enum TOWER_LIGHT_UNIT_STATE m_eDisplaySetting[NUM_OF_TOWERLIGHT_STATE][NUM_OF_TOWER_LIGHT_UNITS]; // 4 is R, A, B light and Buzzer
};



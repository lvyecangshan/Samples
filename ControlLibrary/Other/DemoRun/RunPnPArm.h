#pragma once
#include "Run.h"
class CRunPnPArm : public CRun
{
protected:
	enum POWER_UP_STEPS
	{
		PU_ARM_Z_HOME,
		PU_ARM_X_HOME,
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_ARM_X_TO_ONLOAD,
		AUTO_WAIT_FOR_ONLOAD,
		AUTO_ARM_Z_PICK_DOWN,
		AUTO_ARM_PICK_STRIP,
		AUTO_ARM_Z_PICK_UP,
		AUTO_ARM_X_TO_UNLOAD,
		AUTO_WAIT_FOR_UNLOAD,
		AUTO_ARM_Z_PLACE_DOWN,
		AUTO_ARM_RELASE_STRIP,
		AUTO_ARM_Z_BACK_UP,
		NUM_OF_AUTO_STEPS,
	};

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
public:
	CRunPnPArm(void);
	~CRunPnPArm(void);
};

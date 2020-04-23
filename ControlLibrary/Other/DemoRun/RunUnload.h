#pragma once
#include "Run.h"
class CRunUnload : public CRun
{
protected:
	enum POWER_UP_STEPS
	{
		PU_RUN_HOME,
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_MOVE_UP,
		AUTO_WAIT_PUT_END,
		AUTO_CHECK_MOVE_DOWN,
		AUTO_MOVE_DOWN_STEP,
		AUTO_UNLOAD_FINISHED,
		AUTO_EXCHANGE_MAG,
		NUM_OF_AUTO_STEPS,
	};

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	//BOOL MoveDown(BOOL bStep = TRUE);
public:
	CRunUnload(void);
	~CRunUnload(void);
};

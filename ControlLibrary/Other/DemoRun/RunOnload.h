#pragma once
#include "Run.h"
class CRunOnload : public CRun
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
		AUTO_WAIT_PICK_END,
		AUTO_SUPPLY_FINISHED,
		AUTO_EXCHANGE_MAG,
		NUM_OF_AUTO_STEPS,
	};

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
public:
	CRunOnload(void);
	~CRunOnload(void);
	BOOL m_bMagFirstMoveUp;
};

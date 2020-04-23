#pragma once

#include "maintenance.h"

class CMachineMaintenance : public CMaintenance
{
public:
	CMachineMaintenance();
	~CMachineMaintenance();

public:
	void MaintenanceOutput(COutput *pOutput, BOOL bOn);
	void MaintenanceMotorHome(CMotor *pMotor);
	void MaintenanceMotorMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
    void MaintenanceUpdateAxisData(CMotor *pMotor);
    void MaintenanceContinueMove(CMotor *pMotor,BOOL bPositive);
};

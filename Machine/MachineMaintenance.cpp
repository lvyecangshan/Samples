#include "StdAfx.h"
#include "MachineMaintenance.h"
#include "RunProcess.h"
CMachineMaintenance::CMachineMaintenance()
{
}

CMachineMaintenance::~CMachineMaintenance()
{
}

void CMachineMaintenance::MaintenanceOutput(COutput *pOutput, BOOL bOn)
{
	CMaintenance::MaintenanceOutput(pOutput,bOn);
	return;

	for (int i = 0; i < MACHINE.m_pRunCtrl->GetRunCount(); i++)
	{
		if (!((CRunProcess*)MACHINE.m_pRunCtrl->GetRun(i))->CheckOutputCanActive(pOutput))
		{
			return;
		}
	}
	CMaintenance::MaintenanceOutput(pOutput,bOn);
}

void CMachineMaintenance::MaintenanceMotorHome(CMotor *pMotor)
{
	CMaintenance::MaintenanceMotorHome(pMotor);
	return;
	for (int i = 0; i < MACHINE.m_pRunCtrl->GetRunCount(); i++)
	{
		if (!((CRunProcess*)MACHINE.m_pRunCtrl->GetRun(i))->CheckMotorCanMove(pMotor,0,0,FALSE))
		{
			return;
		}
	}
	CMaintenance::MaintenanceMotorHome(pMotor);
}

void CMachineMaintenance::MaintenanceMotorMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	CMaintenance::MaintenanceMotorMove(pMotor, iLoc, lfValue, bAbsMove);
	return;
	for (int i = 0; i < MACHINE.m_pRunCtrl->GetRunCount(); i++)
	{
		if (!((CRunProcess*)MACHINE.m_pRunCtrl->GetRun(i))->CheckMotorCanMove(pMotor,iLoc,lfValue,bAbsMove))
		{
			return;
		}
	}
	CMaintenance::MaintenanceMotorMove(pMotor, iLoc, lfValue, bAbsMove);
}

void CMachineMaintenance::MaintenanceUpdateAxisData(CMotor *pMotor)
{

}
void CMachineMaintenance::MaintenanceContinueMove(CMotor *pMotor,BOOL bPositive)
{
	CMaintenance::MaintenanceContinueMove(pMotor,bPositive);
	return;
    for (int i = 0; i < MACHINE.m_pRunCtrl->GetRunCount(); i++)
    {
        if (!((CRunProcess*)MACHINE.m_pRunCtrl->GetRun(i))->CheckMotorCanMove(pMotor,0,0,FALSE))
        {
            return;
        }
    }
    CMaintenance::MaintenanceContinueMove(pMotor,bPositive);
}
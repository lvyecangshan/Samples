#pragma once
#include "Output.h"
#include "Motor.h"
#include "run.h"

#define RUN_MAINTENANCE_ID 1000

class __declspec(dllexport) CMaintenance : public CRun
{
public:
	CMaintenance(void);
	~CMaintenance(void);

	BOOL IsIdle() { return !m_bBusy; }

	virtual void MaintenanceOutput(COutput *pOutput, BOOL bOn);
	virtual void MaintenanceMotorHome(CMotor *pMotor);
	virtual void MaintenanceMotorMove(CMotor *pMotor, int nLoc, double dValue, BOOL bAbsMove);
	virtual void MaintenanceStopMotor(CMotor *pMotor);
	virtual void MaintenanceContinueMove(CMotor *pMotor,BOOL bPositive);
	virtual void MaintenanceCustomOperation(int nOperation) {}
	virtual void MaintenanceUpdateAxisData(CMotor *pMotor){}
    virtual BOOL ExtJogStart(int axis){return FALSE;}
	virtual void ExtJogStop(int axis){}

	void IssueOutput(COutput *pOutput, BOOL bOn);
	void IssueMotorHome(CMotor *pMotor);
	void IssueContineMove(CMotor *pMotor,BOOL bPositive = TRUE);
	void IssueMotorMove(CMotor *pMotor, int nLoc, double dValue, BOOL bAbsMove);
	void IssueCustomOperation(int nCustomOperation);
protected:
	UINT RunThread();

	BOOL CheckBusy();

protected:
	BOOL	m_bBusy;

	BOOL	m_bHasMotorOperation;
	CMotor	*m_pMotor;
	BOOL	m_bMotorHome;
	BOOL	m_bMotorMove;
	int		m_nMotorLoc;
	double	m_dMotorPos;
	BOOL	m_bMotorAbsMove;

	BOOL    m_bContinueMove;
	BOOL    m_bContinueMoveDir;

	BOOL	m_bHasOutputOperation;
	COutput	*m_pOutput;
	BOOL	m_bOutputValue;

	BOOL	m_bHasCustomOperation;
	int		m_nCustomOperationCode;
};
/****************************************************************************************
�ļ�����RunprocessRunprocessShortEdgeclean.h
���ߣ�chenshengjin					�汾��A1						���ڣ�2019/7/12
��������: ���ǳ������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"
#include "..\parts\PressCoreCylinder.h"
#include "..\parts\Finger.h"
#include "..\parts\SCtrolCylinder.h"
#include "..\parts\Cylinder.h"

//���ǳ������
class CRunProcessRunProcessShortEdgeClean : public CRunProcess
{
protected:
    enum POWER_UP_STEPS
    {
		PU_CHECK_FINGER_HAVE_CORE,
		PU_FINGER_CY_OPEN,
		PU_JACKING_CY_UP,
		PU_RODLESS_CY_RETURN,
		PU_CLEAN_CY_RETURN,
        PU_FINISH,
    };

    enum AUTO_STEPS
    {
        AUTO_WAIT_BEGIN_WORK_EVEND,
		AUTO_CHECK_INDUCTOR,
		AUTO_JACKING_CY_DOWN,
		AUTO_FINGER_CY_CLOSE,
		AUTO_JACKING_CY_UP,
		AUTO_CHECK_HAVE_CORE,
		AUTO_RODLESS_CY_PUSH,
		AUTO_CLEAN_CY_PUSH,
		AUTO_JUDGE_FEEDER_READY,
		AUTO_CLEAN_CY_RETURN,
		AUTO_SEND_FEEDER_WORD_FINISH,
		AUTO_RODLESS_CY_RETURN,
		AUTO_JACKING_CY_REPEAT_DOWN,
		AUTO_FINGER_CY_OPEN,
		AUTO_SEND_CLEAN_FINISH_EVENT,
		AUTO_JACKING_CY_REPEAT_UP,
        AUTO_WORK_END,
    };
protected:
    void PowerUpOperation(void);
    void PowerUpRestart(void);
	void AutoOperation(void);
    BOOL CheckConfigValid(void);
public:
    CRunProcessRunProcessShortEdgeClean(int nID);
    virtual ~CRunProcessRunProcessShortEdgeClean();
public:
    BOOL InitializeConfig(CString strProcessModule);
    BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
    BOOL CheckOutputCanActive(COutput *pOutput);

	virtual void Anticollision();   //��צ��ײ�߳� 

	void Release();

	//��ʼ��ָ��
	void SetCleanOnLoad(CRunProcess* CleanOnLoad); 

public: 

	PressCoreCylinder     m_PressCoreCy;  //ѹ������
	CFinger               m_Finger;     //��צ
	CCylinder             m_JackingCylinder;   //��������  
	SCtrolCylinder        m_RodlessCylinder;  //�޸�����
	PressCoreCylinder    m_CleanPush;    //��ϴ�Ƴ�

private:

	CRunProcess* m_pCleanOnLoad;  //����λ

 };
 
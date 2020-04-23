/****************************************************************************************
�ļ�����RunProcessOnloadCorePNPEx.cpp
��������������ϵ�о��е��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"
#include "NetSenseClient.h"

class CRunProcessOnloadCore : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_PRESS_CYL_PUSH,  //��ֹ����������о�ƶ�
		PU_MOVE_Z_TO_HOME,
		PU_MOVE_Y_TO_HOME,
		PU_JUDGE_IF_WITH_PALLET,//�жϴ�������
		PU_CHECK_WITHOUT_PRODUCT,
		PU_PRESS_CYL_RETURN,
		PU_FINGER_CYL_OPEN,
		PU_ROTATE_CYL_RETURN,
		PU_MOVE_Z_TO_SAFE_POS,
		PU_MOVE_Y_TO_ONLOAD_POS,
		PU_BUFFER_CYL_BACK,//�ݴ����׻���

		PU_CHECK_BUFFER_PALLET_DATA,
		PU_CHECK_PALLET_DATA,
		PU_PRESS_CLY_ACTION_WITH_PALLET,
		PU_ROTATE_CYL_PUSH_WITH_PALLET,
		PU_JUDGE_BARCODE_EXIST,

		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_MOVE_Y_TO_ONLOAD_POS,
		AUTO_ROTATE_CLY_RETURN_INIT,
		AUTO_PRESS_CLY_RETURN_INIT,
		AUTO_FINER_CLY_OPEN_INIT,
		AUTO_WAIT_READY_ONLOAD_SIGNAL,
		AUTO_WAIT_START_EVENT_LINE,

		//onload
		AUTO_MOVE_Z_TO_ONLOAD_POS,
		AUTO_CHECK_AXIS_STATE,
		AUTO_FINGER_CYL_CLOSE,
		AUTO_PRESS_CYL_PUSH,
		AUTO_MOVE_Z_TO_SAFE_POS,
		AUTO_CHECK_HAVE_PRODUCT,
		AUTO_SEND_ONLOAD_FINISH,
		AUTO_WAIT_REPLY_SIGNAL,
		AUTO_ROTATE_CYL_PUSH,
		AUTO_JUDGE_HAVE_NG,

		//offload
		AUTO_MOVE_Y_TO_OFFLOAD_POS,
		AUTO_WAIT_NEXT_MOUDLE_REQUIRE,
		AUTO_MOVE_Z_TO_OFFLOAD_POS,
		AUTO_PRESS_CYL_RETURN_OK,
		AUTO_FINGER_CYL_OPEN_OK,
		AUTO_SEND_DATE,
		AUTO_MOVE_Z_TO_SAFE_POS_OK,

		//ng
		AUTO_MOVE_Y_TO_NG_POS,
		AUTO_WAIT_NG_MODULE_REQUIRE,
		AUTO_MOVE_Z_TO_NG_POS,
		AUTO_PRESS_CYL_RETURN_NG,
		AUTO_FINGER_OPEN_NG,
		AUTO_MOVE_Z_TO_SAFE_PO_NG,
		AUTO_RESET_NG_LINE_REQUIRE,

		// buf
		AUTO_MOVE_Y_TO_BUFFER_POS,
		AUTO_BUFFER_CYL_ACTION,
		AUTO_CHECK_HAVE_PRODUCT_BUFFER,
		AUTO_MOVE_Z_TO_BUFFER_POS,
		AUTO_PRESS_CYL_RETURN_BUF_EX,
		AUTO_PRESS_CYL_RETURN_BUF,
		AUTO_FINGER_CYL_OPEN_BUF,
		AUTO_MOVE_Z_TO_SAFE_POS_BUF,
		AUTO_CHECK_WITHOUT_PRODUCT_BUF,
		AUTO_JUDGE_BATTERY_ALL_PUT_FINISH,//�ж����е�ط����
		AUTO_WORK_END,
	};

	enum MOTOR_LOC
	{
		Y_MOTOR_PICK_LOC=0,      //ȡ��λ
		Y_MOTOR_PNP_OFFLOAD_LOC,//����Ʒλ
		Y_MOTOR_NG_POS,          //�Ų���λ
		Y_MOTOR_BUFFER_POS,      //����λ��


		Z_MOTOR_SAFE_LOC= 0,  //Z�ᰲȫλ
		Z_MOTOR_PICK_LOC,     //Z��ȡ��λ
		Z_MOTOR_PUT_OK_LOC,   //Z�����Ʒλ
		Z_MOTOR_PUT_NG_LOC,   //�Ų���λ
		Z_MOTOR_BUFFER_LOC,	  //����λ
	};

public:
	CRunProcessOnloadCore(int nID);
	virtual ~CRunProcessOnloadCore(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL FingerClose (int nIndex,BOOL bClose);//��צ��������
	BOOL RotatePush (int nIndex,BOOL bPush);  //��ת���׶�������
	BOOL PressPush (int nIndex,BOOL bPush);   //ѹ��о���׶�������
	BOOL BufferPush(BOOL bPush);               //��������

	BOOL CheckHasVaild(int nIndex, BOOL bHave);//����צ�Ƿ��в�Ʒ
	BOOL CheckZSafe();//���Z���Ƿ�ѹ��
	void EmgStop();  //��ѹ�ϼ�ͣ
    void ThreadRelease();//��ѹ�ϼ�ͣ�߳��ͷ�

	void  ReadStepInfo();
	BOOL  SaveStepInfo(int nWriteType);
	BOOL CheckStartWithPalletDataSame();  //��е�������ж�
	BOOL CheckStartWithPalletDataSameForBuffer(); //�������ж�

	double GetMotorZSafePos();//��ȡZ�ᰲȫλλ��

public:
	//��ץ����  01---��λ1  23---��λ2
	int m_IFingerClose[4];
	int m_IFingerOpen[4];
	int m_OFingerClose[2];
	int m_OFingerOpen[2];

	//ѹ������ ��Ӧ  01---��λ1 23---��λ2
	int m_IPressPush[4];
	int m_IPressReturn[4];
	int m_OPressPush[2];   //ץ��ѹ��
	int m_OPressReturn[2];   //ץ��ѹ��

	//��������
	int m_IBufferCylPush;
	int m_IBufferCylReturn;
	int m_OBufferCylPush;
	int m_OBufferCylReturn;

	// ���ϼ�� 0-1:��צ���ϼ��  2-3:�������ϼ��
	int m_IHasCore[4];
	int m_IZCheckAbnormal[4];		//ѹ�ϸ�Ӧ

	int m_nMotorY;
	int m_nMotorZ;

public:
	BOOL m_bThreadStart;
	BOOL m_bThreadEnd;
	BOOL m_bEnableDocking;
	int m_nWorkIndex;//��λ�������
	int m_nStartWithPalletData;//������������״̬����
	int m_nBufferWithPalletData;
	BOOL m_bHomeMoveSafePos;
	CPallet m_pBufferPallet;
	CPallet m_pTempPallet[2];


	int m_nWaitTime;
	// ����ǹ
	int m_nScanPort;
	CString m_strScanIp;
	CString m_strScanCmd;
	CNetSenseClient  m_ScanCodeClient;
	
	//�������Խ��ź� Yao  5.6
	int m_IReadyOnloadSignal;//����׼�����ź�
	int m_IPLCRunningSignal;//�������������ź�
	int m_ORequiredOnloadSignal;//��ǻ���������ź�
	int m_OPLCRunningSignal;//��ǻ����������ź�
};
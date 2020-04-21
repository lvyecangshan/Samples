/*******************************************************************
�ļ�����Motor.h
���ߣ�              �汾��A1        ���ڣ�
��������������ƽ̨�ŷ����������
��ʷ�޸ļ�¼��
*******************************************************************/
#pragma once
#include <vector>
using namespace std;

//includes
#include <afxmt.h>
#include "Input.h"

// invalid motor location
#define INVALID_MOTOR_LOC		-1

enum MOTOR_SPEED_MODE
{
    MOTOR_FAST_SPEED,
    MOTOR_MEDIUM_SPEED,
    MOTOR_SLOW_SPEED,
    NUM_OF_MOTOR_SPEED_MODES,
    MOTOR_AUTO_SPEED = NUM_OF_MOTOR_SPEED_MODES,
};

struct sMotorLocation
{
	CString m_strName;
    double  m_dPosition;
	int		m_numSubLocation;
	double	m_dSubLocationPitch;
	BOOL    m_bEnableEdit;
};

struct sAxisOffset
{
	sAxisOffset();
	double m_dStartPoint;
	double m_dStepSpace;
	int    m_nPointNums;
	vector<double> m_ptOffset;
	sAxisOffset&  operator = (const sAxisOffset &offsetAxis);
	BOOL LoadOffset(int nAxis);
	BOOL SaveOffset(int nAxis);
};

/////////////////////////////////////////////////////////////////////////////////
typedef int (*FUNMOTOR_EXT_HOME)(void); //return info reference M_CODE

class CDeviceManager;
class CCPModuleMotorPage;

class  CMotor
{
	friend CDeviceManager;
	friend CCPModuleMotorPage;

public:
	CMotor();
	~CMotor();

public:

	/*********************************************************************************
	//�����������Ḵλ
	//�����������
	//��������: M_OK��ʾ�ɹ��������ο�M_CODE
	**********************************************************************************/
	__declspec(dllexport) int Home();//Input output only used for home by IO;

	/*********************************************************************************
	//���������������е�ָ��λ��
	//���������dest_loc	Ŀ��λ�úţ�LibrarySupportTool.exe����λ�úţ�
	//			sub_loc		��λ�ñ�ţ���һ��λ���ж����λ��ʱʹ��. �������ڵ���ȡ�ϵ����Σ�ֻ���趨��һ����λֵ������λ�þ����Զ������
	//			Offset		ָ��λ�õĲ���ֵ.
    //          speedMode   �ƶ��ٶ�ģʽ��δָ�����Զ�����λ�Ƴ���ָ���ٶ�ģʽ. 
	//��������: TRUE�����гɹ�  FALSE������ʧ��
	**********************************************************************************/
	__declspec(dllexport) BOOL MoveLoc(int dest_loc, int sub_loc = 0, double offset = 0.0, int speedMode = -1);

	/*********************************************************************************
	//������������T�ͼӼ��پ����ƶ���ָ��λ��
	//���������dest_pos	Ŀ��λ��ֵ��
    //          loc			ָ���ƶ����Ŀ��λ�úţ�δָ�����Զ�����֮ǰ�Ľ������λ�úš�
    //          speedMode   �ٶ�ģʽ��δָ�����Զ�����λ�Ƴ���ָ���ٶ�ģʽ. 
	//��������: TRUE�����гɹ�  FALSE������ʧ��
	**********************************************************************************/
	__declspec(dllexport) BOOL TMoveAbs(double dest_pos, int loc = INVALID_MOTOR_LOC, int speedMode = -1); 

	/*********************************************************************************
	//������������T�ͼӼ�������ƶ�ָ������
	//���������dist		�ƶ�����ֵ��
    //          loc			ָ���ƶ����Ŀ��λ�úţ�δָ�����Զ�����֮ǰ�Ľ������λ�úš�
    //          speedMode   �ٶ�ģʽ��δָ�����Զ�����λ�Ƴ���ָ���ٶ�ģʽ. 
	//��������: TRUE�����гɹ�  FALSE������ʧ��
	**********************************************************************************/
	__declspec(dllexport) BOOL TMoveRel(double dist, int loc = INVALID_MOTOR_LOC, int speedMode = -1);

	/*********************************************************************************
	//������������S�ͼӼ��پ����ƶ���ָ��λ��
	//���������dest_pos	Ŀ��λ��ֵ��
	//			svARange	���ٷ�Χ����λΪ���ָ����λmm��deg��
    //          svDrange	���ٷ�Χ����λΪ���ָ����λmm��deg��
    //          loc			ָ���ƶ����Ŀ��λ�úţ�δָ�����Զ�����֮ǰ�Ľ������λ�úš�
    //          speedMode   �ٶ�ģʽ��δָ�����Զ�����λ�Ƴ���ָ���ٶ�ģʽ. 
	//��������: TRUE�����гɹ�  FALSE������ʧ��
	**********************************************************************************/
	__declspec(dllexport) BOOL SMoveAbs(double dest_pos, double svARange, double svDrange, int loc = INVALID_MOTOR_LOC, int speedMode = -1); 

	/*********************************************************************************
	//������������S�ͼӼ�������ƶ�ָ������
	//���������dist		�ƶ�����ֵ��
	//			svARange	���ٷ�Χ����λΪ���ָ����λmm��deg��
    //          svDrange	���ٷ�Χ����λΪ���ָ����λmm��deg��
    //          loc			ָ���ƶ����Ŀ��λ�úţ�δָ�����Զ�����֮ǰ�Ľ������λ�úš�
    //          speedMode   �ٶ�ģʽ��δָ�����Զ�����λ�Ƴ���ָ���ٶ�ģʽ. 
	//��������: TRUE�����гɹ�  FALSE������ʧ��
	**********************************************************************************/
	__declspec(dllexport) BOOL SMoveRel(double dist,  double svARange, double svDrange, int loc = INVALID_MOTOR_LOC, int speedMode = -1);

	/*********************************************************************************
	//�����������ᰴָ�����������ƶ�
	//���������bPositive	�ƶ�����
    //          nSpeedMode  �ٶ�ģʽ��δָ�����Թ����ٶ�ִ��. 
	//��������: TRUE�����гɹ�  FALSE������ʧ��
	**********************************************************************************/
	__declspec(dllexport) BOOL StartMove(BOOL bPositive, int nSpeedMode = -1);

	/*********************************************************************************
	//�����������ȴ���ǰ�����н���
	//���������timeout		��ʱʱ��
    //          pInExtAlm	�ⲿ��������㣬�ڵȴ����翴�����ź�ΪON,�������ز�����������Ϣ
    //          bState      �ⲿ������״̬
	//��������: �ɹ����� M_STOPPED�������ο�M_CODE
	**********************************************************************************/
	__declspec(dllexport) int  WaitMotionDone(DWORD timeout = INFINITE, CInput* pInExtAlm = NULL, BOOL bState = TRUE);

	/*********************************************************************************
	//������������ȡ��ǰ������״̬
	//�����������
	//��������: �ο�M_CODE
	**********************************************************************************/
	__declspec(dllexport) int  GetMotionStatus();

	/*********************************************************************************
	//������������ֹͣ����
	//���������tdec   �����ʱ��
	//��������: TRUE�����гɹ�  FALSE������ʧ��
	**********************************************************************************/
	__declspec(dllexport) BOOL Stop(double tdec = 0.1);

	/*********************************************************************************
	//����������������������źű�־
	//�����������
	//��������: ��
	**********************************************************************************/
	__declspec(dllexport) void ResetOriRefKnownFlag() { m_bOriRefIsKnown = FALSE; }

	/*********************************************************************************
	//������������ȡ�������ɱ�־״̬
	//�����������
	//��������: TRUE��������ɳ�ʼ����  FALSE����δ��ɳ�ʼ����
	**********************************************************************************/
	__declspec(dllexport) BOOL IsOriRefKnown() { return m_bOriRefIsKnown; }

	/*********************************************************************************
	//����������������ʹ��״̬
	//���������on_off  TRUE��ʹ��  FALSE����ֹ/�ر�
	//��������: TRUE��ʹ�����  FALSE��ʹ��ʧ��
	**********************************************************************************/
	__declspec(dllexport) BOOL SetSvOn(BOOL on_off);

	// IO related
	__declspec(dllexport) BOOL IOStatus(enum MOTOR_IO which_io);
	__declspec(dllexport) BOOL Save(int nIndex);

	// location related
    __declspec(dllexport) sMotorLocation & GetLocation(int loc);
	__declspec(dllexport) int GetCurLoc() { return m_nCurLoc; }
	__declspec(dllexport) int GetLocations(){return m_numLocations;}

	// 
	__declspec(dllexport) BOOL    IsRotaryType(){return m_nMoveType == MOVE_ROTARY;}
	__declspec(dllexport) int     GetMotorType(){return m_nMotorType;}
	__declspec(dllexport) double  GetPulsePerUint(){return m_dPulsePerUint;}
	__declspec(dllexport) double  GetVirtualHome(){return m_dVirtualHome;}
	__declspec(dllexport) double  GetHomeGoBackDist(){return m_dHomeGoBackDist;}
	__declspec(dllexport) void    SetHomeByExtFun(FUNMOTOR_EXT_HOME pHomeFun){m_pFunExtHome = pHomeFun;}

	// position related
	__declspec(dllexport) double GetCurPos();
	__declspec(dllexport) double GetCmdPos();
	__declspec(dllexport) double GetPosErr();
    __declspec(dllexport) double GetCurSpeed();
	__declspec(dllexport) BOOL   ChangeCurSpeed(double dSpeed);
	__declspec(dllexport) void   SetCurLoc(int loc);
	__declspec(dllexport) BOOL   SetCurPos(double dPos);

public:
    int	    m_nIndex;			// MotorX.cfg�������
	int		m_nSpeedMode;		// ���ٶ�ģʽ
	CString m_strName;			// ������
	CString	m_strForwardName;	// ���һ��ť����
	CString m_strBackwardName;	// ��ڶ���ť����
	DWORD   m_tHomeTimeout;		// ���ԭ��ʱʱ��
    DWORD   m_tMoveTimeout;		// ���ƶ���ʱʱ��

	double	m_dTolerance;								//used check if move pos error
	double	m_dStartup[NUM_OF_MOTOR_SPEED_MODES];		// start speed, in pulse/s, must be double
	double	m_dCruise[NUM_OF_MOTOR_SPEED_MODES];		// max speed, in pulse/s, must be double
	double	m_dTAcc[NUM_OF_MOTOR_SPEED_MODES];			// acceleration	time
	double	m_dTdec[NUM_OF_MOTOR_SPEED_MODES];			// deceleration time
	double  m_dAloneSpeedRatio;

	BOOL	m_bEnablePosOffsetParam;		//used to enable show dialog for offset or limit parameter
    static double  sm_dAllSpeedRatio;		// the whole machine speed ratio
	sAxisOffset m_sAxisOffset;
	vector<sMotorLocation> m_pLocations;

	FUNMOTOR_EXT_HOME m_pFunExtHome;		// only used in HOME_BY_EXT

protected:
	int		m_nAxisNo;		//ʵ�ʶ�ӦӲ�����
	int		m_nMotorType;	//������ͣ��ŷ��򲽽�
	int		m_nHomeDir;		//���ԭ�㷽��
	int     m_nHomeMode;	//���ԭ��ģʽ
	int 	m_nMoveType;	//���ƶ�����
	int		m_nPlsOpMode;	//����ģʽ OUT/DIR �� CW/CCW
	int		m_nEncoderType;	//��������, 1xAB, 2xAB, 4xAB, CW/CCW
	BOOL	m_bALM;			//�ᱨ���ź�
	BOOL	m_bINP;			//�ᵽλ�ź�
	BOOL	m_bEL;			//����λ�ź�
	BOOL	m_bORG;			//��ԭ���ź�
	BOOL	m_bRDY;			//��׼�����ź�
	BOOL	m_bSON;			//��ʹ���ź�

	// the speed ratio, which is used for machine speed tuning (normally value between 0 ~ 1.0)
	double  m_dVirtualHome;
	double  m_dHomeGoBackDist;		// leave steps for homing
	double  m_dMinStep;				//
	double	m_dPulsePerUint;		//1mm or 1deg
	double	m_dEncPulsePerUint;		//1mm or 1deg

	// common used
	double m_dSoftLimitP;			//reserve
	double m_dSoftLimitN;			//reserve
	double m_dBackLash;				//only used in card has this function
	double m_dForwardOffset;
	double m_dBackwardOffset;
    int    m_nFromLib;				// only used in dll

	// option only used for home by IO mode
	int    m_nHomeOut;				// only used in HOME_BY_IO
	int    m_nHomeIn;				// only used in HOME_BY_IO

protected:
	//////////////////////////////////////////////////
	//�ڲ�����
	int DetermineSpeedMode(double dDist);
	int HomeByLimit();
	int HomeByOrg();
	int HomeByIO();

protected:
	//////////////////////////////////////////////////
	//�ڲ�����
	int		m_nCurLoc;
	int		m_nSavedLoc;
	int		m_numLocations;
	BOOL	m_bOriRefIsKnown;
	BOOL    m_bMotionBreak;

protected:
	//////////////////////////////////////////////////
	//�ڲ�����
	BOOL    ApplyAxisConfig();
	BOOL    IsSavedLocChanged();
	BOOL    ReadConfig(int nIndex);
	int     GetSavedLoc();
	void    SaveLoc();
	void    MotionBreak(BOOL bBreak = TRUE);// break out in wait motion done
	static  BOOL ConfigModule(int nTotalMotor);
	double  CalcOffset(double destPos, double curPos);
};
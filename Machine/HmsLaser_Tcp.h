#pragma once
#include <WinSock2.h>

#define	SOCK_RECVBUF		10		// 接收Buf大小

class CHmsLaser_Tcp
{
public:
	CHmsLaser_Tcp(void);
	~CHmsLaser_Tcp(void);

public:
	//	connect
	BOOL ConnectServer(char* cIP, int nPort);
	BOOL DisConnectServer();
	BOOL IsConnectServer();

	// is Laser
	BOOL LaserMark(char* cEntityName, BOOL bIsBlock=FALSE);
 	BOOL StopMark();
 
 	// set laser text
 	BOOL SetEntityText(char* cEntityName, char* cText);
 	BOOL SetEntityTextOnly(char* cEntityName, char* cText);
 
 	// change laser pos
 	BOOL ReDraw();
 	BOOL SetEntityPos(char* cEntityName, double dX, double dY, BOOL bIsAbsolute = FALSE);
 	BOOL SetEntityRotate(char* cEntityName, double dAngle);
 
 	BOOL SetCurDocByName(char* cDocName);
 
 	char GetIOInput();
 	BOOL SetIOOutput(char btOutPutIOVal);
 
 	BOOL SetIsMarkByName(char* cEntityName, BOOL bIsMark);
 	BOOL IsMarking();
 
 	BOOL MoveRedToDot(double dX, double dY);
 
 	BOOL SetEntitySize(char* cEntityName, double dWidth, double dHeight);

 	BOOL OpenFile(char * cFileName);
 	BOOL LoadFile(char * cFilePath);
 	BOOL ShowEditWindow(BOOL bIsVisible);
 	BOOL SaveFile(char * cFilePath);


private:
	struct sockaddr_in	m_sockAddr;
	SOCKET m_pSocket;
};
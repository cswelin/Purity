#ifndef _CSOCKET_H_
#define _CSOCKET_H_

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib") 


class CSocket
{
public:
	CSocket();
	virtual ~CSocket();
	virtual bool create(int afInet = AF_INET);
	virtual bool connect(const unsigned long lHost, int nPort);
	virtual bool connect(const char *szHost, unsigned short usPort);
	virtual int disconnect();
	virtual bool bind(int nLocalPort);
	virtual bool accept(CSocket *pSocket);
	//virtual bool listen(
	virtual int send(const char *szMsg, int nLen, int nFlags =0);
	virtual int recv(char *pzMsg, int nLen, int nFlags = 0);
	virtual bool close();
	static unsigned long resolveAddress(const char *szHost);
	
protected:
	SOCKET		m_hSocket;
	bool		m_bConnected;

private:
	static int m_tSockets;
};

#endif
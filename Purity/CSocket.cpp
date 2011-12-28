#include "Purity.h"

int CSocket::m_tSockets = 0;

CSocket::CSocket():m_hSocket(0), m_bConnected(false)
{
	WSADATA info;

	if( ++m_tSockets == 1 )
		::WSAStartup(MAKEWORD(2,1), &info);
}

CSocket::~CSocket()
{
	close();
	if( --m_tSockets == 0)
		::WSACleanup();
}

bool CSocket::create(int afInet)
{
	m_hSocket = ::socket(afInet, SOCK_STREAM, IPPROTO_TCP );

	if(m_hSocket == INVALID_SOCKET)
	{
		#ifdef DEBUG_CONSOLE
			*g_log << "CSocket::create() - Failed to create socket" << endl;
		#endif
		return false;
	} 
	else
		return true;
}

bool CSocket::bind(int _localPort)
{
	SOCKADDR_IN addr;

	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(_localPort);
	memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

	if(::bind(m_hSocket, (const sockaddr*)&addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		return false;
	else
		return true;
}

bool CSocket::accept( CSocket *_socket )
{
	if( _socket == 0 )
		return false;
	
	SOCKADDR_IN addr;
	int len = sizeof( SOCKADDR_IN );
	memset(&addr, 0, sizeof( SOCKADDR_IN ) );

	_socket->m_hSocket  = ::accept( m_hSocket, (sockaddr*)&addr, &len );

	if( _socket->m_hSocket == INVALID_SOCKET )
		return false;
	else
		return true;
}

bool CSocket::close()
{
	return ::closesocket( m_hSocket ) == SOCKET_ERROR ? false : true;
}


bool CSocket::connect(const unsigned long lHost, int nPort) 
{
	if(this->m_bConnected) return false;

	sockaddr_in ssin;

	memset(&ssin, 0, sizeof(ssin)); 
	ssin.sin_family=AF_INET;
	ssin.sin_port=htons(nPort); 
	ssin.sin_addr.s_addr=lHost;

	if(::connect(m_hSocket, (sockaddr*)&ssin, sizeof(sockaddr_in)) < 0)
	{
		if(WSAGetLastError() == 10038) // Socket operation on nonsocket
			create();
		return false;
	}

	this->m_bConnected = true;
	return true;
}

unsigned long CSocket::resolveAddress(const char *szHost)
{
	unsigned long IP = inet_addr(szHost);

	if (IP==INADDR_NONE)
	{
		hostent *pHE = gethostbyname(szHost);

		if (pHE == 0) 
			return INADDR_NONE;

		IP = *((unsigned long *)pHE->h_addr_list[0]);	
	}
	return IP;
}


bool CSocket::connect(const char *szhost, unsigned short usPort)
{
	return connect(resolveAddress(szhost), usPort);
}

int CSocket::recv(char *szMsg,int nLen, int nFlags)
{
	int bytesRecv = ::recv(m_hSocket, szMsg, nLen, nFlags);
	
 	if(m_hSocket==SOCKET_ERROR || !this->m_bConnected || bytesRecv == 0) 
	{
		this->m_bConnected = false;	
		return SOCKET_ERROR;
	}
	return bytesRecv;
}

int CSocket::send(const char *szMsg,int nLen,int nFlags)
{
	int bytesSent = ::send(m_hSocket,szMsg,nLen,nFlags);

	#ifdef DEBUG_CONSOLE
		*g_log << "[O] " << szMsg;
	#endif

	return bytesSent;
}

int CSocket::disconnect()
{
	m_bConnected = false;
	return closesocket(this->m_hSocket);
}
#ifndef __CIRC_H__
#define __CIRC_H__


class CIrc :  public CSocket, public CThread
{

private:
	
	CConfig*		m_config;
	CHandler*		m_chand;
	THandler<CIrc>* m_ircJoin;
	THandler<CIrc>* m_ircPong;
	THandler<CIrc>* m_irc001;
	THandler<CIrc>* m_irc005;
	THandler<CIrc>* m_ircExit;
	THandler<CIrc>*	m_botShtD;
	bool			bConnected;
	bool			bShutdown;
	
public:
	CIrc();
	virtual ~CIrc();
	virtual unsigned int run();
	
	bool ircConnect(const char* remoteHost, const unsigned short iPort, const char* password);
	void ircDisconnect();
	bool isConnected() { return bConnected; }
	void ircConStatus(bool status) { bConnected = status; }
	int ircRaw(const char *szbuffer,...);

	string			getChannel();
	string			ircReceive();
	vector<string>	strSplit(string receivedLine, string delim);
	msgBuffer*		strParse(string strBuffer);

	void cmdPong(const msgBuffer *msgReceived);
	void cmdJoin(const msgBuffer *msgReceived);
	void cmdS001(const msgBuffer *msgReceived);
	void cmdExit(const msgBuffer *msgReceived);
	void cmdShtD(const msgBuffer *msgReceived);




};

#endif
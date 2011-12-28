#ifndef __CIRC_H__
#define __CIRC_H__


class CIrc :  public: CSocket, public CThread
{

private:
	CConfig*		m_config;
	THandler<CIrc>*	m_ircPong; 

public:
	CIrc();
	virtual ~CIrc();
	virtual unsigned int run();

	bool ircConnect(const char* remoteHost, const unsigned short iPort, const char* password);
	bool ircDisconnect(const char* quitMsg);

	int ircRaw(const char *szbuffer,...);

	string			ircReceive();
	vector<string>	strSplit(string receivedLine, string *delim);
	msgBuffer*		strParse(string *strBuffer);
	void	cmdPong(const msgBuffer *msgReceived);
};

#endif
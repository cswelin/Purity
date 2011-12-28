#include "Purity.h"


CIrc::CIrc()
{
	m_config = new CConfig;
	m_ircPong = new msgBuffer;
}

CIrc::~CIrc()
{
	DELETE_OBJECT(m_ircPong);
	DELETE_OBJECT(m_config);
}

CIrc::ircConnect(const char* remoteHost, const unsigned short iPort, const char* password)
{
	if(connect(serverIP, port))
	{
		sendRaw("PASS %s\r\n", password);
		sendRaw("NICK %s\r\n", m_conf->getNick().c_str());
		sendRaw("USER %s %s %s :%s\r\n", m_conf->getNick().c_str(),
										 m_conf->getNick().c_str(), 
										 m_conf->getNick().c_str(),
										 m_conf->getNick().c_str());
		return true;
	}
	return false;
}

string CIrc::ircReceive()
{
	char buffer[512]="";
	memset(buffer, 0, sizeof(buffer));
	int bReceived = this->recv(buffer,sizeof(buffer));
	if(bReceived == SOCKET_ERROR)
		this->m_hSocket = NULL;
	return buffer;
}

vector<string> CIrc::strSplit(string receivedLine, string delim)
{
	vector<string>  parsedLine;	
	unsigned int prevpos = 0;	
	if(!receivedLine.empty()) {
		while(true)
		{
			unsigned int curPosition = (unsigned int)receivedLine.find(delim, prevpos);
			if (curPosition == -1) 
				break;
			parsedLine.push_back(receivedLine.substr(prevpos, curPosition-prevpos));
			prevpos = curPosition+delim.length();
		}
		parsedLine.push_back(receivedLine.substr(prevpos, receivedLine.length()-prevpos-delim.length()+1));
	}
	return parsedLine;
}

msgBuffer* strParse(string strBuffer)
{
/*
[I] PING :A77ACE13

args[0] ->PING
args[1] ->:A77ACE13
[O] PONG :A77ACE13

[I] :irc.Purity.net 001 Purity123 :Enjoy your short visit
args[0] ->:irc.Purity.net
args[1] ->001
args[2] ->Purity123
args[3] ->:Enjoy
args[4] ->your
args[5] ->short
args[6] ->visit

[I] :Purity123 MODE Purity123 :+iwx
args[0] ->:Purity123
args[1] ->MODE
args[2] ->Purity123
args[3] ->:+iwx

[I] :Purity123!Purity123@purity-A9791EAF JOIN :#Purity
args[0] ->:Purity123!Purity123@purity-A9791EAF
args[1] ->JOIN
args[2] ->:#Purity


//<-
	addiction.bluehell.org NOTICE Asniper|Laptop :LAGCHK 2156929481
	:Digerati!digerati@netadmin.bluehell.org QUIT :Quit: I'll be back tomorrow.
	:degen!~user@light.dat.shit  PRIVMSG #ryan :pn
	<message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
	<prefix>   ::= <servername> | <nick> [ '!' <username> ] [ '@' <host> ]
	<command>  ::= <letter> { <letter> } | <number> <number> <number>
	<SPACE>    ::= ' ' { ' ' }
	<params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]
	<middle>   ::= <Any *non-empty* sequence of octets not including SPACE or NUL or CR or LF, the first of which may not be ':'>
	<trailing> ::= <Any, possibly *empty*, sequence of octets not including NUL or CR or LF>
	<crlf>     ::= CR LF
*/
	msgBuffer* parsedLine = new msgBuffer;

	if(strBuffer)
	{
		unsigned int pos = strBuffer.find(" ", 0);
		
		msgBuffer->isClient = true;
		msgBuffer->ircProtocol = this;
				
		if(strBuffer.at(0) != ':')
		{
			msgBuffer->isClient = false;
			msgBuffer->command = strBuffer.substr(0, pos);
		} else {
			msgBuffer->user = strBuffer.substr(0,pos);
			msgBuffer->command = strBuffer.substr(pos, pos = strBuffer.find(" ", pos));
		}
		msgBuffer->args = this->strSplit(strBuffer.substr(pos,strBuffer.length());
	}
	return msgBuffer;
}


int CIrc::ircRaw(const char *szbuffer,...)
{
	va_list va_alist; 
	char outputBuffer[512];  //max message length
	va_start(va_alist, szbuffer);

	unsigned int size = _vsnprintf_s(outputBuffer, sizeof(outputBuffer), sizeof(outputBuffer), szbuffer, va_alist); 
	va_end(va_alist);

	return this->send(outputBuffer,size,0);
}

unsigned int CIrc::run()
{
	msgBuffer* parsedLine;
	vector<string> lines;
	string receivedLine;
	
	while(m_config->ircReconnect) // TODO: Make while ircReconnected
	{
		ircConnect(m_conf->getHost(), m_conf->getPort(), m_conf->getServerPassword())
		while(this->ircConnected) // TODO: Make while m_bConnected
		{

			if(receivedLine = this->recReceive())
			{
				lines = strSplit(receivedLine, "\r\n");
				for(vector<string>::iterator iter = lines.begin(); iter != lines.end(); iter++)
				{
					#ifdef DEBUG_CONSOLE
						*g_log << "[I] " << iter->c_str() << endl;
					#endif

					parsedLine = strParse(iter->c_str());
					m_chand->getFuncPntr(parsedLine->command,parsedLine->isClient)->call(parsedLine));


					DELETE_OBJECT(parsedLine);
				}
			}
		}
		Sleep(10000);
		this->m_config->nextServer();
	}
	this->running = false;*/
	return NULL;
}
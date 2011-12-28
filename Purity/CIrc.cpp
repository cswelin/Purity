#include "Purity.h"


CIrc::CIrc() : CSocket(), CThread()
{
	//create Socket Handler
	this->create();

	//Connection states
	bConnected = false;
	bShutdown = false;

	//Module Objects
	m_chand = new CHandler;
	m_config = new CConfig;

	//Register bot member function pointers
	REGISTERCMD(this,&CIrc::cmdPong,"","PING",m_ircPong, CIrc);
	REGISTERCMD(this,&CIrc::cmdJoin,"irc","join", m_ircJoin, CIrc); 
	REGISTERCMD(this,&CIrc::cmdExit,"irc","exit", m_ircExit, CIrc);
	REGISTERCMD(this,&CIrc::cmdS001,"","001", m_irc001, CIrc);
	REGISTERCMD(this,&CIrc::cmdShtD,"base","shutdown", m_botShtD, CIrc);

}

CIrc::~CIrc()
{
	DELETE_OBJECT(m_ircPong);
	DELETE_OBJECT(m_chand);
	DELETE_OBJECT(m_config);
	DELETE_OBJECT(m_ircJoin);
	DELETE_OBJECT(m_ircExit);
	DELETE_OBJECT(m_irc001);
	DELETE_OBJECT(m_botShtD);
}

bool CIrc::ircConnect(const char* remoteHost, const unsigned short iPort, 
					  const char* password)
{
	
	if(connect(remoteHost, iPort))
	{
		ircRaw("PASS %s\r\n", password);
		ircRaw("NICK %s\r\n", m_config->getNick().c_str());
		ircRaw("USER %s %s %s :%s\r\n", m_config->getNick().c_str(),
										 m_config->getNick().c_str(), 
										 m_config->getNick().c_str(),
										 m_config->getNick().c_str());

		bConnected = true;
	}
	return bConnected;
}

string CIrc::ircReceive()
{
	char buffer[512];
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
			unsigned int curPosition = (unsigned int)receivedLine.find(delim.c_str(), 
																		prevpos);
			if (curPosition == -1) 
				break;
			parsedLine.push_back(receivedLine.substr(prevpos, curPosition-prevpos));
			prevpos = curPosition+delim.length();
		}
		parsedLine.push_back(receivedLine.substr(prevpos, 
							 receivedLine.length()-prevpos-delim.length()+1));
	}
	return parsedLine;
}

//:Asniper!Asniper@purity-A9791EAF PRIVMSG Purity123 :.irc.join #Purity
msgBuffer* CIrc::strParse(string strBuffer)
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
	<middle>   ::= <Any *non-empty* sequence of octets not including SPACE 
				  or NUL or CR or LF, the first of which may not be ':'>
	<trailing> ::= <Any, possibly *empty*, sequence of octets not including NUL or CR or LF>
	<crlf>     ::= CR LF
*/
	msgBuffer* parsedLine = new msgBuffer;
	string::size_type currentPos = 0;
	string strFrom;
	if(!strBuffer.empty())
	{
		parsedLine->isClient = true;
		parsedLine->ircProtocol = this;
		
		string::size_type pos = strBuffer.find(" ", 0);
		strFrom = strBuffer.substr(0, pos);
		
		if(strFrom.find('!',0) == string::npos) 
		{
			parsedLine->isClient = false;
			if(strFrom.at(0) != ':')
				parsedLine->command = strFrom;
			else 
			{
				pos = strBuffer.find(" ", pos+1);
				parsedLine->command = strBuffer.substr(strFrom.length()+1, (pos-strFrom.length())-1);
			}
		} else {
			parsedLine->from = strFrom;

			currentPos = strBuffer.find(" ", pos+1);
			parsedLine->servCmd = strBuffer.substr(pos, currentPos-pos);
			
			pos = strBuffer.find(" ", currentPos+1);
			parsedLine->servCmdArgs = strBuffer.substr(currentPos, pos-currentPos);
			
			currentPos = strBuffer.find(" ", pos+1);
			pos+=2;
			parsedLine->command = strBuffer.substr(pos, currentPos-pos);
		}
		if(currentPos < strBuffer.length())
			parsedLine->args = strSplit((strBuffer.substr(currentPos,strBuffer.length())), " ");
	}
	return parsedLine;
}

string CIrc::getChannel()
{
	return m_config->getChannel();
}

int CIrc::ircRaw(const char *szbuffer,...)
{
	va_list va_alist; 
	char outputBuffer[512];  //max message length
	va_start(va_alist, szbuffer);

	unsigned int size = _vsnprintf_s(outputBuffer, 
									 sizeof(outputBuffer), 
									 sizeof(outputBuffer),
									 szbuffer, 
									 va_alist); 
	va_end(va_alist);

	return this->send(outputBuffer,size,0);
}

unsigned int CIrc::run()
{
	msgBuffer* parsedLine;
	vector<string> lines;
	string receivedLine;
	
	while(m_config->getReconnect() && !bShutdown)
	{
		this->ircConnect(m_config->getHost(), m_config->getPort(), m_config->getServerPassword());
		while(this->isConnected())
		{
			receivedLine = this->ircReceive();

			lines = strSplit(receivedLine, "\r\n");
			if(!lines.empty())
			{
				for(vector<string>::iterator iter = lines.begin(); iter != lines.end(); iter++)
				{
					if(strcmp(iter->c_str(),""))
					{
						#ifdef DEBUG_CONSOLE
							*g_log << "[I] " << iter->c_str() << endl;
						#endif

						parsedLine = strParse(iter->c_str());
						if(!parsedLine->command.empty() && 
							(parsedLine->command.at(0) == m_config->getCmdPrefix() || 
							!parsedLine->isClient))
						
						m_chand->getFuncPntr(parsedLine->command,parsedLine->isClient)->call(parsedLine);


						DELETE_OBJECT(parsedLine);
					}
				}
			}
		}
		if(!bShutdown) {
			Sleep(10000);
			this->m_config->nextServer();
		}
	}
	ircConStatus(false);
	return NULL;
}
/*
	User & Server Responce Functions
*/
void CIrc::cmdS001(const msgBuffer* args)
{
	this->ircRaw("USERHOST %s\r\n",this->m_config->getNick().c_str());
	this->ircRaw("MODE %s +i\r\n", this->m_config->getNick().c_str());
	this->ircRaw("JOIN %s\r\n", this->m_config->getChannel().c_str(), 
								this->m_config->getChanPass().c_str());
}

void CIrc::cmdPong(const msgBuffer* args)
{	
	this->ircRaw("PONG %s\r\n", args->args.at(1).c_str());
}

void CIrc::cmdJoin(const msgBuffer* args)
{
	if(args->args.size() > 0 )
	{
		if(args->args.size() > 2)
			this->ircRaw("JOIN %s %s\r\n", args->args.at(1).c_str(), args->args.at(2).c_str());
		else
			this->ircRaw("JOIN %s\r\n", args->args.at(1).c_str());
	}
}

void CIrc::ircDisconnect()
{
	this->send("QUIT\n",strlen("QUIT\n"),0);
	ircConStatus(false);
	this->disconnect();
}

void CIrc::cmdExit(const msgBuffer* args)
{
	ircDisconnect();
}

void CIrc::cmdShtD(const msgBuffer* args)
{
	ircDisconnect();
	this->bShutdown = true;
}
#include "Purity.h"


CConfig::CConfig()
{
	curServIndex=0;

	//Autoreconnect
	this->bReconnect = true;

	//Generic Bot Settings
	this->cmdPrefix = '.';

	//Server Information
	this->addServer("127.0.0.1", "22876560", 2243, false, "Purity", "#Purity", "");

	//User Information
	this->addUser("Asniper","9292194","*",1);
}
void CConfig::addServer(std::string host, std::string password, int port, 
						bool randomNick, std::string nick, string channel, string chanPass)
{
	serverConfig _server;

	//if(randomNick)
		_server.nick = nick; //TODO: Random nick
	_server.host = host;
	_server.password = password;
	_server.port = port;
	_server.channel = channel;
	_server.chanPass = chanPass;

	servers.push_back(_server);
}


void CConfig::addUser(std::string userName, std::string password, std::string authHost, int maxLogins)
{
	usersConfig _users;
	
	_users.userName = userName;
	_users.password = password;
	_users.authHost = authHost;
	_users.maxLogins = maxLogins;
	
	users.push_back(_users);
}
void CConfig::removeServer(std::string host, int port)
{

}

void CConfig::nextServer()
{
	//if(curServIndex >= servers.size())
		curServIndex=0;
	//else
	//	curServIndex++;
}
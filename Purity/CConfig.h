#ifndef __CCONFIG_H__
#define __CCONFIG_H__
#include "Purity.h"

class serverConfig
{
public:
	std::string			nick;
	std::string			host;
	std::string			password;
	std::string			channel;
	std::string			chanPass;
	int					port;
};

class usersConfig
{
public:
	std::string		userName;
	std::string		password;
	std::string		authHost;
	int				maxLogins;
};


class CConfig
{
private:
	bool						bReconnect;
	char						cmdPrefix;
	unsigned short int			curServIndex;
	std::vector<serverConfig>	servers;
	std::vector<usersConfig>	users;
	
public:
	CConfig();

	void nextServer();
	void addServer(std::string host, std::string password, int port, 
					bool randomNick, std::string nick, std::string channel, string chanPass);

	void addUser(std::string userName, std::string password, std::string authHost,
					int maxLogins);

	void removeServer(std::string host, int port);

	int getPort() { return servers[curServIndex].port; }
	const char* getHost() { return servers[curServIndex].host.c_str(); }
	const char*  getServerPassword() { return servers[curServIndex].password.c_str(); }
	std::string getNick() { return servers[curServIndex].nick; }
	std::string getChannel() { return servers[curServIndex].channel; }
	std::string getChanPass() { return servers[curServIndex].chanPass; }
	const char getCmdPrefix() { return '.'; }
	bool getReconnect() { return bReconnect; }
};
#endif	
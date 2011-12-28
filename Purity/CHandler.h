#ifndef __CHANDLER_H__
#define __CHANDLER_H__

class CIrc;

struct msgBuffer
{
	bool			isClient;
	bool			isUserCmd;
	string			from;
	string			servCmd;
	string			servCmdArgs;
	string			command;
	vector<string>	args;
	CIrc*			ircProtocol;
};

class TCHandler
{
public:
	char *cat;    //Category
	char *cmd;    //Command
	TCHandler();
	~TCHandler(){ if(cat)  delete [] cat; cat = NULL; \
				  if(cmd)  delete [] cmd; cmd = NULL;}
	virtual void call(const msgBuffer* args)=0;
};

template <class TClass> class THandler : public TCHandler
{

private:
	void (TClass::*fpt)(const msgBuffer* args);	//pointer to member function
	TClass* pt2Object;								//pointer to object

public:
	void setPointers(TClass* _pt2Object, void(TClass::*_fpt)(const msgBuffer* args))
	{
		pt2Object = _pt2Object;
		fpt=_fpt;
	}
	virtual void call(const msgBuffer* args) { (*pt2Object.*fpt)(args); }
};

class CHandler
{
private:
	map<std::string, TCHandler*> cmds;
	THandler<CHandler> noCmdFound;			//noCmd() -- no command found error output
	THandler<CHandler> listCommands;

public:
	CHandler();

	bool registerCommand(string cat, string cmd,TCHandler* func);
	void removeCommand(string _cmd);

	TCHandler* getFuncPntr(string _cmd, bool isClient);
	//void listCommands();
	void noCmd(const msgBuffer* args); //no command found error output -invoked by getFuncPntr()
	void listCmds(const msgBuffer* args); // lists all availble registered commands
};
#endif // #ifndef __CHANDLER_H__
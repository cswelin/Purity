#ifndef __CHANDLER_H__
#define __CHANDLER_H__

class CIrc;

class TCHandler
{
public:
	string cat;    //Category
	string cmd;    //Command
	string desc;   //Description
	virtual void call(const vector <string> args, CIrc *irc=NULL);
};

template <class TClass> class THandler : public TCHandler
{
private:
	void (TClass::*fpt)(const vector <string>, CIrc *irc);	//pointer to member function
	TClass* pt2Object;								//pointer to object

public:
	THandler(){}
	~THandler(){}
	void setPointers(TClass* _pt2Object, void(TClass::*_fpt)(const vector <string>, CIrc *irc))
	{
		pt2Object = _pt2Object;
		fpt=_fpt;
	}
	virtual void call(const vector <string> args, CIrc *irc) { (*pt2Object.*fpt)(args, irc); }
};

class CHandler
{
private:
	map<string, TCHandler*> cmds;
	//vector<TCHandler*> cmds;				//list of member function pointers
	THandler<CHandler> noCmdFound;			//noCmd() -- no command found error output
	

public:
	CHandler();
	bool registerCommand(string cat, string cmd, string desc, TCHandler* func);
	void removeCommand(string _cmd);
	TCHandler* getFuncPntr(string _cmd);
	void listCommands();
	void noCmd(const vector <string> args, CIrc *irc=NULL); //no command found error output -invoked by getFuncPntr()
};
#endif // #ifndef __CHANDLER_H__
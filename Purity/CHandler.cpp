#include "Purity.h"


TCHandler::TCHandler()
{
	cat = NULL;
	cmd = NULL;
}


CHandler::CHandler()
{
	//TODO: Rewrite registration of commands
	//TODO: cleanup code
	noCmdFound.setPointers(this, &CHandler::noCmd);
	listCommands.setPointers(this, &CHandler::listCmds);
	if(!this->registerCommand("noCmdFound","err",&noCmdFound))
	{
		#ifdef DEBUG_CONSOLE
			DEBUGERROR("CHandler::registerCommand()  \t-> Failed","");
		#endif
	}
	if(!this->registerCommand("listCmds", "base", &listCommands))
	{
		#ifdef DEBUG_CONSOLE
			DEBUGERROR("CHandler::listCommands() \t-> Failed","");
		#endif
	}
}

bool CHandler::registerCommand(string _cmd, string _cat, TCHandler* func)
{
	DELETE_OBJECT(func->cmd);
	func->cmd = new char[_cmd.length()+1];
	strcpy(func->cmd, _cmd.c_str());

	DELETE_OBJECT(func->cat);
	func->cat = new char[_cat.length()+1];
	strcpy(func->cat, _cat.c_str());

	if(!_cat.empty())
		_cat.append(COMMAND_CATEGORY);

	string fullCmd = _cat.append(_cmd);
	cmds[fullCmd] = func;

	#ifdef DEBUG_CONSOLE
		*g_log << "CHandler::resgisterCommand() \t-> " << _cat <<  " successfully added" << endl;
	#endif

	return true;
}

TCHandler* CHandler::getFuncPntr(string _cmd, bool isClient)
{
	if(isClient)
		_cmd = _cmd.substr(1, _cmd.size());

	map<std::string, TCHandler*>::iterator cmdIte = cmds.find(_cmd);
	if(cmdIte != cmds.end()) 
		return cmdIte->second;

	#ifdef DEBUG_CONSOLE
		DEBUGERROR("CHandler::getFuncPntr() \t-> Unable to find command: [", _cmd.c_str() << "]");
	#endif*/
	return cmds["err.noCmdFound"];
}

void CHandler::listCmds(const msgBuffer *args)
{
	for(map<string, TCHandler*>::iterator iter = cmds.begin(); iter != cmds.end(); iter++)
	{
		args->ircProtocol->ircRaw("PRIVMSG %s %s.%s\n\r", args->servCmdArgs.c_str(), iter->second->cat, iter->second->cmd);	
		Sleep(300);
	}
}

void CHandler::removeCommand(std::string _cmd)
{
	cmds.erase(_cmd);
}

void CHandler::noCmd(const msgBuffer* args)
{
	
}

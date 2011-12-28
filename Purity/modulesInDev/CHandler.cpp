#include "Purity.h"

CHandler::CHandler()
{
	noCmdFound.setPointers(this, &CHandler::noCmd);
	if(!this->registerCommand("noCmdFound","err","Spews no command found",&noCmdFound))
	{
		#ifdef DEBUG_CONSOLE
			DEBUGERROR("CHandler::registerCommand()  \t-> Failed","");
		#endif
	}
}

bool CHandler::registerCommand(string _cmd, string _cat, string _desc, TCHandler* func)
{

	if(!_cat.empty()) {
		_cat.append(COMMAND_CATEGORY);
	}
	
	func->cmd = _cat.append(_cmd);
	func->desc = _desc;
		
	//cmds[(_cat.append(_cmd))] = func;
	//cmds.push_back(func);
	#ifdef DEBUG_CONSOLE
		*g_log << "CHandler::resgisterCommand() \t-> " << _cmd.c_str() << " successfully added" << endl;
	#endif

	return true;
}

TCHandler* CHandler::getFuncPntr(std::string _cmd)
{
	if(_cmd.at(0) == ':')
		_cmd = _cmd.substr(1,_cmd.size());
	/*for(unsigned int x=0; x < cmds.size(); x++) 
	{
		if(_cmd.compare(cmds[x]->cmd)==0)
		{
			#ifdef DEBUG_CONSOLE
				//*g_log << "CHandler::getFuncPntr() \t-> " << _cmd.c_str() << " found" << endl;
			#endif
			return cmds[x];
		}
	}
	#ifdef DEBUG_CONSOLE
		DEBUGERROR("CHandler::getFuncPntr() \t-> Unable to find command: ",_cmd.c_str());
	#endif*/

	return cmds[_cmd];//cmds[0];

}

void CHandler::listCommands()
{
	//TODO: rewrite for loop with vector.being()
	/*for(unsigned int curFunc=0; curFunc < cmds.size(); curFunc++)
	{
		if(cmds[curFunc]) 
		{
			#ifdef DEBUG_CONSOLE
				*g_log << cmds[curFunc]->cmd.c_str() << " " << cmds[curFunc]->desc.c_str() << " "  << endl;
			#endif
		}
	
	}*/
}

void CHandler::removeCommand(std::string _cmd)
{
	//cmds.erase(_cmd);
	/*
	for(unsigned int curFunc=0; curFunc < cmds.size(); curFunc++)
	{
		//removes registered command based on passed cmd string
		//if(_cmd.compare(cmds[curFunc]->cmd)==0)
			//cmds.erase(cmds.begin()+curFunc,cmds.begin()+(curFunc+1));
	}*/
}

void CHandler::noCmd(const vector <string> args, CIrc *irc)
{
	#ifdef DEBUG_CONSOLE
		//*g_log << "CIrc::ping() :: Ping received" << endl;
	#endif
}

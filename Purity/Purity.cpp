#include "Purity.h"

#ifdef DEBUG_CONSOLE
 CDebug* g_log = NULL;
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow)
{
	#ifdef DEBUG_CONSOLE
	 g_log = new CDebug;
	#endif
	
	CIrc *irc = new CIrc();

	//Module Objects
#ifdef ENABLE_SYSTEM
//	CSystem sys(cmdHandler);
#endif
		
	irc->start();
	irc->waitThread();


	DELETE_OBJECT(irc);
	
	#ifdef DEBUG_CONSOLE
		DELETE_OBJECT(g_log);
	#endif
	_CrtDumpMemoryLeaks();
	return 0;
}



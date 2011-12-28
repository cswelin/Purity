#ifndef _PURITY_H_
#define _PURITY_H_

#include "Defines.h"



#include <string>
#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <vector>
#include <map>
//#include <tlhelp32.h>
#include <process.h>
using namespace std;

#ifdef DEBUG_CONSOLE
 //Memory leak detection
 #define _CRTDBG_MAPALLOC
 #include <stdlib.h>
 #include <crtdbg.h>
 #include "CDebug.h"
#endif


#include "CHandler.h"
#include "CConfig.h"
#include "CSocket.h"
#include "CThread.h"
#include "CIrc.h"

#ifdef ENABLE_ENCRYPTION
	#include "CCrypt.h"
#endif // ENABLE_ENCRYPTION

#ifdef ENABLE_SYSTEM
	#include "CSystem.h"
#endif
#ifdef BOT_KILLER
	#include "CBotKiller.h"
#endif

#endif
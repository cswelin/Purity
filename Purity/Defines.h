#ifndef __DEFINES__
#define __DEFINES__


// -- Defines
#define WIN32_LEAN_AND_MEAN
#pragma warning(disable : 4996) //disable deprecated warnings


// -- Modules
#define DEBUG_CONSOLE //Windows Debug Console
//#define WINDOWS_SERVICES //Windows Service
#define ENABLE_SYSTEM //System information, cpu speed, hdspace, etc..
//#define BOT_KILLER //Bot Killer
//#define ENABLE_ENCRYPTION //string encryption

// -- Command Defines
#define COMMAND_DELIM		'.'
#define COMMAND_CATEGORY	"." // Category delimiter


// --- Macros
#ifdef DEBUG_CONSOLE
 #define DEBUGERROR(wf,cause) \
		 g_log->setColour(FOREGROUND_RED); \
         *g_log << wf << cause << endl; \
		 g_log->setColour(); 
#endif

#define REGISTERCMD(curObject, classFunction, cmdName, \
					cat, variable, objectType) \
		variable = new THandler<objectType>; \
		variable->setPointers(curObject, classFunction); \
		m_chand->registerCommand(cat, cmdName, variable);

#define DELETE_OBJECT(object) if(object) { delete object; object = NULL; }
#define	DELETE_ARRAY(object) if(object) { delete [] object; object = NULL; }
#endif
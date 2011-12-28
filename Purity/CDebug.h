#ifndef _CDEBUG_H_
#define _CDEBUG_H_

#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

class CDebug : public basic_ostream<char>
{
public:
	CDebug();
	void setColour(WORD wColour=NULL);
	virtual ~CDebug();
protected:
	HANDLE m_hConsole;

	basic_filebuf<char> *m_FileBuf;    
    basic_filebuf<char> m_Nul;  

    FILE *m_fNul;                     
    FILE *m_fConsole;   
};

extern CDebug* g_log;
#endif

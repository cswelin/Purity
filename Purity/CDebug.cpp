#include "Defines.h"
#ifdef DEBUG_CONSOLE
#include <io.h>
#include <fcntl.h>
#include "CDebug.h"

#pragma warning(disable : 4311) //WARNING: disable pointer truncation from 'HANDLE' to 'long' 
CDebug::CDebug(): m_Nul( m_fNul = fopen( "nul", "w" ) ), basic_ostream<char>( &m_Nul )
{
	AllocConsole();
	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int handle = _open_osfhandle((long) m_hConsole, _O_TEXT);
	m_fConsole = _fdopen(handle, "w");
	m_FileBuf = new basic_filebuf<char>(m_fConsole);
	init( m_FileBuf );
	setf(ios::unitbuf);
}

CDebug::~CDebug()
{

	if(m_FileBuf)
		delete m_FileBuf;
	if( m_hConsole != INVALID_HANDLE_VALUE){
		FreeConsole();
		fclose(m_fConsole);
	}
	fclose( m_fNul );
}

void CDebug::setColour(WORD wColour)
{
	// no color specified, reset to defaults (white font on black background)
	if(wColour != NULL)
		SetConsoleTextAttribute(m_hConsole, wColour );
	// change font and/or background color
	else
		SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN 
									| FOREGROUND_BLUE); // white text on black bg
};
#endif
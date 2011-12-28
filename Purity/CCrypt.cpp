#include "Purity.h"


/*CCrypt::CCrypt()
{
	szMagicBuffer = new char[sizeof(ENCRYPTION_BUFFER)];
	szMagicBuffer = ENCRYPTION_BUFFER;
}


CCrypt::~CCrypt()
{
	if(szMagicBuffer)
		delete [] szMagicBuffer;
	szMagicBuffer = NULL;
}

string CCrypt::encrypt(string phrase)
{
	char *encPhrase = new char[phrase.length()+1];

	encPhrase[0] = (char)(rand() % 255);

	for(unsigned int iCurPos = 0, iTemp=0; iCurPos < phrase.length(); iCurPos++, iTemp++)
	{
		encPhrase[iCurPos] = ((phrase.at(iTemp) & szMagicBuffer[iCurPos]) + encPhrase[iCurPos]);
		encPhrase[iCurPos+1] = '\0';
		
		if(iTemp == phrase.length())
			iTemp=0;
	}
	return encPhrase;
}*/
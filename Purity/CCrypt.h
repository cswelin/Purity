#ifndef __CCRYPT_H__
#define __CCRYPT_H__

class CCrypt
{
private:
	char *szMagicBuffer;
public:
	CCrypt();
	~CCrypt();
	string encrypt(string phrase);
	char* decrypt(string encString);
};


#endif //ifndef __CCRYPT_H__
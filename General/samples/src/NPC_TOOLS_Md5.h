

#ifndef _NPC_TOOLS_MD5_H
#define _NPC_TOOLS_MD5_H

#include "NPC_TypeDef.h"

int MD5_enc(unsigned char id, unsigned char *pwd, unsigned char pwdlen, unsigned char *hash);
int MD5_Xinkaili(unsigned char * user_realm_pwd, int len1,  unsigned char * cmd_url, int len2, unsigned char * nonce, unsigned char * md5_32);

NPC_API_INTER int MDString (unsigned char* string, int nLen, unsigned char* digest);

//int MD5Encode(unsigned char* szEncoded, const unsigned char* szData, int nSize, unsigned char* szKey, int nKeyLen);

int md5_packages_string(char *desc_str, char *src_str);

NPC_API_INTER	void NPC_TOOLS_MD5_MD5Encrypt(char *strOutput, char *strInput);
NPC_API_INTER	void NPC_TOOLS_MD5_EncodeString(char *dest, char *src);

#endif



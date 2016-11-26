#include "stdafx.h"
#include "Util.h"

map<uint64_t, CCamera*> Mac_CCamera_Map;
CUtil::CUtil()
{
}


CUtil::~CUtil()
{
}


void CUtil::LoadOrder(uint8_t * pOrder, uint8_t Header, uint8_t ScrType, uint8_t destType, uint8_t Cmd, uint8_t Prm1, uint8_t Parm2, CCamera * pCamear)
{
	memset(pOrder, 0, 17 * sizeof(uint8_t));
	pOrder[0] = Header;
	pOrder[1] = ScrType;
	pOrder[2] = destType;
	pOrder[3] = Cmd;
	pOrder[4] = Prm1;
	pOrder[5] = Parm2;
	if (pCamear)
	{
		pOrder[6] = CharToUint8(pCamear->mCommonNetConfig.sMac[0]) * 16 + CharToUint8(pCamear->mCommonNetConfig.sMac[1]);
		pOrder[7] = CharToUint8(pCamear->mCommonNetConfig.sMac[3]) * 16 + CharToUint8(pCamear->mCommonNetConfig.sMac[4]);
		pOrder[8] = CharToUint8(pCamear->mCommonNetConfig.sMac[6]) * 16 + CharToUint8(pCamear->mCommonNetConfig.sMac[7]);
		pOrder[9] = CharToUint8(pCamear->mCommonNetConfig.sMac[9]) * 16 + CharToUint8(pCamear->mCommonNetConfig.sMac[10]);
		pOrder[10] = CharToUint8(pCamear->mCommonNetConfig.sMac[12]) * 16 + CharToUint8(pCamear->mCommonNetConfig.sMac[13]);
		pOrder[11] = CharToUint8(pCamear->mCommonNetConfig.sMac[15]) * 16 + CharToUint8(pCamear->mCommonNetConfig.sMac[16]);
	}
	pOrder[12] = 0x0;
	pOrder[13] = 0x0;
	pOrder[14] = 0x0;
	pOrder[15] = 0x0;
	pOrder[16] = 0xfd;
}

uint8_t CUtil::CharToUint8(char ch)
{
	uint8_t tmp = 0;
	if (ch >= '0' && ch <= '9')
	{
		tmp += ch - '0';
	}
	else  if (ch >= 'a' && ch <= 'z')
	{
		tmp += ch - 'a' + 10;
	}
	return tmp;
}

uint64_t CUtil::CharToUint64(char * pch)
{
	uint64_t mac = 0;

	mac += CharToUint8(pch[0]);
	mac *= 16;
	mac += CharToUint8(pch[1]);
	mac *= 16;
	mac += CharToUint8(pch[3]);
	mac *= 16;
	mac += CharToUint8(pch[4]);
	mac *= 16;
	mac += CharToUint8(pch[6]);
	mac *= 16;
	mac += CharToUint8(pch[7]);
	mac *= 16;
	mac += CharToUint8(pch[9]);
	mac *= 16;
	mac += CharToUint8(pch[10]);
	mac *= 16;
	mac += CharToUint8(pch[12]);
	mac *= 16;
	mac += CharToUint8(pch[13]);
	mac *= 16;
	mac += CharToUint8(pch[15]);
	mac *= 16;
	mac += CharToUint8(pch[16]);

	return uint64_t(mac);
}

void CUtil::LoadMap(CCamera * pCamera)
{
	uint64_t uint64mac;
	uint64mac = CharToUint64(pCamera->mCommonNetConfig.sMac);
	printf("LoadMap %llu\n", uint64mac);
	Mac_CCamera_Map.insert(pair<uint64_t, CCamera*>(uint64mac, pCamera));
}

void CUtil::RemoveDev(CCamera * pCamera)
{
	uint64_t uint64mac;
	uint64mac = CharToUint64(pCamera->mCommonNetConfig.sMac);
	map<uint64_t, CCamera*>::iterator iter;
	iter = Mac_CCamera_Map.find(uint64mac);
	if (iter != Mac_CCamera_Map.end())
	{
		printf("RemoveDev %llu\n", uint64mac);
		Mac_CCamera_Map.erase(iter);
	}
}

uint64_t CUtil::ArrayToUint64(uint8_t * pch)
{
	uint64_t macuint64 = 0;

	macuint64 += pch[0];
	macuint64 *= 256;
	macuint64 += pch[1];
	macuint64 *= 256;
	macuint64 += pch[2];
	macuint64 *= 256;
	macuint64 += pch[3];
	macuint64 *= 256;
	macuint64 += pch[4];
	macuint64 *= 256;
	macuint64 += pch[5];

	return uint64_t(macuint64);
}

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
	uint32_t crc = CRC32Software(pOrder, 12);
	pOrder[12] = (uint8_t) (crc>>24);
	pOrder[13] = (uint8_t) ((crc&0x00ffffff)>>16);
	pOrder[14] = (uint8_t) ((crc&0x0000ffff)>>8);
	pOrder[15] = (uint8_t) (crc&0x000000ff);
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
	if (pCamera)
	{
		uint64_t uint64mac;
		uint64mac = CharToUint64(pCamera->mCommonNetConfig.sMac);
		Mac_CCamera_Map.insert(pair<uint64_t, CCamera*>(uint64mac, pCamera));
	}
}

void CUtil::RemoveDev(CCamera * pCamera)
{
	if (pCamera)
	{
		uint64_t uint64mac;
		uint64mac = CharToUint64(pCamera->mCommonNetConfig.sMac);
		map<uint64_t, CCamera*>::iterator iter;
		iter = Mac_CCamera_Map.find(uint64mac);
		if (iter != Mac_CCamera_Map.end())
		{
			Mac_CCamera_Map.erase(iter);
		}
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



void CUtil::MacNumberToStr(uint8_t* pNumber, char* pMac)
{
	int tmp  = 0;
	int i  = 0;
	for (; i < 6; i++) {
		tmp  = pNumber[i]/16;
		if (tmp < 10) {
			pMac[i*3]  = tmp + '0';
		}
		else {
			pMac[i*3]  = tmp - 10 + 'a';
		}

		tmp  = pNumber[i] % 16;
		if (tmp < 10) {
			pMac[i*3+1] = tmp + '0';
		}
		else {
			pMac[i*3+1] = tmp - 10 + 'a';
		}
	}

	pMac[2] = ':';
	pMac[5] = ':';
	pMac[8] = ':';
	pMac[11] = ':';
	pMac[14] = ':';
	pMac[17] = 0;
}

uint32_t CUtil::CRC32Software(uint8_t *pData, uint16_t Length)
{
	DWORD dwPolynomial = 0x04c11db7;
	DWORD    xbit;
	DWORD    data;
	DWORD    CRC = 0xFFFFFFFF;    // init
	//Length = 1;
	while (Length--) {
		xbit = 1 << 31;
		data = *pData++;
		data &= 0x000000ff;
		//data = 0x44434241;
		//Print("%32x", data);
		for (int bits = 0; bits < 32; bits++) {
			if (CRC & 0x80000000) {
				CRC <<= 1;
				CRC ^= dwPolynomial;
			}
			else
				CRC <<= 1;
			if (data & xbit)
				CRC ^= dwPolynomial;

			xbit >>= 1;
		}
	}
	return CRC;
}

bool CUtil::CheckCRC32(uint8_t * pData, uint16_t Length)
{
	uint32_t crc = CRC32Software(pData, Length);
	if (pData[Length] != (uint8_t)(crc >> 24))
		return false;
	if (pData[Length + 1] != (uint8_t)((crc & 0x00ffffff) >> 16))
		return false;
	if (pData[Length + 2] != (uint8_t)((crc & 0x0000ffff) >> 8))
		return false;
	if (pData[Length + 3] != (uint8_t)(crc & 0x000000ff))
		return false;
	return true;
}

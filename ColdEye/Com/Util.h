#pragma once
#include "stdint.h"
#include "Device\Camera.h"
#include <map>
using namespace std;
extern map<uint64_t, CCamera*> Mac_CCamera_Map;
static char DevMac[6][32];
class CUtil
{
public:
	CUtil();
	~CUtil();
	static void LoadOrder(uint8_t *pOrder, uint8_t Header, uint8_t ScrType, uint8_t destType, uint8_t Cmd, uint8_t Prm1, uint8_t Parm2, CCamera *pCamear);
	static uint8_t CharToUint8(char ch);
    static uint64_t CharToUint64(char *pch);
	static void LoadMap(CCamera *pCamera);
	static void RemoveDev(CCamera *pCamera);
	static uint64_t ArrayToUint64(uint8_t *pch);
	static void     MacNumberToStr(uint8_t * pNum, char* pMac);
	static uint32_t CRC32Software(uint8_t *pData, uint16_t Length);
	static bool CheckCRC32(uint8_t *pData, uint16_t Length);
};


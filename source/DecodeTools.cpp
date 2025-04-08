#include "stdafx.h"

#include "DecodeTools.h"


bool ParseProtocol(const BYTE* rawData, DWORD rawLen, CByteArray& parsedData) {
    // ʾ���������򵥵� [AA 55][����][����][У��] Э��
    if (rawLen < 4 || rawData[0] != 0xAA || rawData[1] != 0x55) {
        return false;
    }
    BYTE dataLen = rawData[2];
    if (rawLen < 3 + dataLen + 1) {  // ��ͷ2 + ����1 + ����N + У��1
        return false;
    }
    // У�����֤�����ۼӺͣ�
    BYTE checksum = 0;
    for (DWORD i = 0; i < 3 + dataLen; i++) {
        checksum += rawData[i];
    }
    if (checksum != rawData[3 + dataLen]) {
        return false;
    }
    // ��ȡ��Ч����
    parsedData.RemoveAll();
    parsedData.SetSize(dataLen);
    memcpy(parsedData.GetData(), &rawData[3], dataLen);
    return true;
}
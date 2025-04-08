#include "stdafx.h"

#include "DecodeTools.h"


bool ParseProtocol(const BYTE* rawData, DWORD rawLen, CByteArray& parsedData) {
    // 示例：解析简单的 [AA 55][长度][数据][校验] 协议
    if (rawLen < 4 || rawData[0] != 0xAA || rawData[1] != 0x55) {
        return false;
    }
    BYTE dataLen = rawData[2];
    if (rawLen < 3 + dataLen + 1) {  // 包头2 + 长度1 + 数据N + 校验1
        return false;
    }
    // 校验和验证（简单累加和）
    BYTE checksum = 0;
    for (DWORD i = 0; i < 3 + dataLen; i++) {
        checksum += rawData[i];
    }
    if (checksum != rawData[3 + dataLen]) {
        return false;
    }
    // 提取有效数据
    parsedData.RemoveAll();
    parsedData.SetSize(dataLen);
    memcpy(parsedData.GetData(), &rawData[3], dataLen);
    return true;
}
#pragma once
#include <afxtempl.h>  // ֧�� CByteArray

bool ParseProtocol(const BYTE* rawData, DWORD rawLen, CByteArray& parsedData);
#pragma once
#include <afxtempl.h>  // Ö§³Ö CByteArray

bool ParseProtocol(const BYTE* rawData, DWORD rawLen, CByteArray& parsedData);
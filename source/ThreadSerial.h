#pragma once

#include <string>
#include "afxwin.h"
#include "afxstr.h"
#include "winbase.h"
#include "mysscom.h"

/*************************************************************************************************/
/*                                     串口设备参数定义                                          */
/*************************************************************************************************/
typedef struct {
	unsigned long    baudrate;                                                 /* 波特率 */
	unsigned char    databits;                                                 /* 数据位 */
	unsigned char    stopbits;                                                 /* 停止位 */
	unsigned char    checkbit;                                                 /* 校验位 */
} SRL_DEV_PARA_T;

// 消息类型枚举
enum ComMsgType {
    MSG_RAW_DATA,     // 原始数据
    MSG_FRAME_DATA,   // 完整帧数据
    MSG_ERROR         // 错误
};

// 消息数据结构体
struct ComMsgData {
    ComMsgType type;    // 消息类型
    CByteArray* pData;  // 数据指针（动态分配，主线程负责释放）
};

/*************************************************************************************************/
/*                                     串口设备参数定义                                          */
/*************************************************************************************************/
#define  MAX_RECV_BYTE                 1024                                    /* 最大接收字节数 */
#define  MAX_SEND_BYTE                 2048                                    /* 最大发送字节数 */

/*************************************************************************************************/
/*                                     串口设备参数定义                                          */
/*************************************************************************************************/
#define  XON                           0x11
#define  XOFF                          0x13


bool OpenSerialDevice(LPCSTR dname, SRL_DEV_PARA_T *spara);
bool CloseSerialDevice(void);
bool SerialDevisOpened(void);
int  SendSerialData(unsigned char *sbuf, int slen);

bool CreateDeviceThread(void);
bool CheckCompleteFrame(const CByteArray& frame);


#pragma once

#include <string>
#include "afxwin.h"
#include "afxstr.h"
#include "winbase.h"
#include "mysscom.h"
#include <vector>
#include <afxmt.h>  // MFC线程同步类的头文件

/*************************************************************************************************/
/*                                     串口设备参数定义                                          */
/*************************************************************************************************/
typedef struct {
	unsigned long    baudrate;                                                 /* 波特率 */
	unsigned char    databits;                                                 /* 数据位 */
	unsigned char    stopbits;                                                 /* 停止位 */
	unsigned char    checkbit;                                                 /* 校验位 */
} SRL_DEV_PARA_T;

static struct {
    bool hasPartialFrame = false;  // 是否已找到帧头但未收齐
    CByteArray partialData;       // 已接收的部分数据（包括帧头）
} g_frameState;


class CircularBuffer {
private:
    std::vector<BYTE> m_buffer;
    size_t m_capacity;
    size_t m_head = 0;  // 写指针
    size_t m_tail = 0;  // 读指针
    CCriticalSection m_cs;

public:
    // 构造函数
    CircularBuffer(size_t size) : m_capacity(size) {
        m_buffer.resize(size);
    }

    // 清空缓冲区
    void Reset() {
        CSingleLock lock(&m_cs, TRUE);
        m_head = m_tail = 0;
    }

    // 写入数据
    bool Push(const BYTE* data, size_t len) {
        CSingleLock lock(&m_cs, TRUE);
        if (len > m_capacity - (m_head - m_tail)) return false;

        for (size_t i = 0; i < len; i++) {
            m_buffer[(m_head + i) % m_capacity] = data[i];
        }
        m_head += len;
        return true;
    }

    // 读取并移除数据
    bool Pop(BYTE* output, size_t len) {
        CSingleLock lock(&m_cs, TRUE);
        if (m_head - m_tail < len) return false;

        for (size_t i = 0; i < len; i++) {
            output[i] = m_buffer[(m_tail + i) % m_capacity];
        }
        m_tail += len;
        return true;
    }

    //// 查看数据（不移除）
    //bool Peek(BYTE* output, size_t len) const {
    //    CSingleLock lock(&m_cs, TRUE);
    //    if (m_head - m_tail < len) return false;

    //    for (size_t i = 0; i < len; i++) {
    //        output[i] = m_buffer[(m_tail + i) % m_capacity];
    //    }
    //    return true;
    //}

    // 获取可读数据量
    size_t Available() const {
        return m_head - m_tail;
    }
};

static CircularBuffer g_rxBuffer(1024);  // 全局环形缓冲区

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

//串口通信数据传递的结构体
struct USART_MSG {
    DWORD length;
    BYTE* data;

    // 显式构造函数
    USART_MSG(DWORD len, const BYTE* buf)
        : length(len),
        data(new BYTE[len])
    {
        memcpy(data, buf, len);
    }

    // 析构函数
    ~USART_MSG() {
        delete[] data;
    }
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

void ProcessCompleteFrame(const BYTE* frame);


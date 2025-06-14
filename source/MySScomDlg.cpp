#include "stdafx.h"
#include "MySScom.h"
#include "MySScomDlg.h"
#include <cstdint>
#include <string>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static const char* THIS_FILE = __FILE__;
#endif


#pragma comment(lib, "version")

static const int    Combo_Baud[12] = {600,  1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
static const int    Combo_Data[4]  = {5,    6,    7,    8};
static const double Combo_Stop[4]  = {1,    1.5,  2};
static const int    Combo_Check[5] = {0,    1,    2,    3,   4};


CMySScomDlg::CMySScomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySScomDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON3);

	AfxInitRichEdit();                                                         /* 初始化RichEdit控件 */
}

void CMySScomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_SENDFILE, m_Progs_SendFile);
	DDX_Control(pDX, IDC_COMBO_COMMPORT, m_Combo_CommPort);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_Combo_BaudRate);
	DDX_Control(pDX, IDC_COMBO_DATABITS, m_Combo_DataBits);
	DDX_Control(pDX, IDC_COMBO_CHECKBIT, m_Combo_CheckBit);
	DDX_Control(pDX, IDC_COMBO_STOPBITS, m_Combo_StopBits);

	DDX_Check(pDX, IDC_CHECK_SHOWTIME, m_Check_ShowTime);
	DDX_Check(pDX, IDC_CHECK_HEXDISPL, m_Check_HexDispl);
	DDX_Check(pDX, IDC_CHECK_HEXFRAME, m_Check_HexFrame);
	DDX_Check(pDX, IDC_CHECK_PROTOACK, m_Check_ProtoACK);
	DDX_Check(pDX, IDC_CHECK_AUTOCLER, m_Check_AutoCler);
	DDX_Check(pDX, IDC_CHECK_AUTOSAVE, m_Check_AutoSave);
	DDX_Check(pDX, IDC_CHECK_AUTOFILT, m_Check_AutoFilt);
	DDX_Check(pDX, IDC_CHECK_KEYWORDS, m_Check_Keywords);
	DDX_Check(pDX, IDC_CHECK_HEXSSEND, m_Check_HexsSend);
	DDX_Check(pDX, IDC_CHECK_CRLFSEND, m_Check_CRLFSend);
	DDX_Check(pDX, IDC_CHECK_AUTOSEND, m_Check_AutoSend);
	
	DDX_Text(pDX, IDC_EDIT_RECVCSTR, m_Edit_RecvCstr);
	DDX_Text(pDX, IDC_EDIT_SENDCSTR, m_Edit_SendCstr);

	DDX_Text(pDX, IDC_EDIT_SENDTIME, m_Edit_SendTime);
	DDV_MaxChars(pDX, m_Edit_SendTime, 5);

	DDX_Text(pDX, IDC_EDIT_AUTCLRKB, m_Edit_AutClrKB);
	DDV_MaxChars(pDX, m_Edit_AutClrKB, 5);

	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_Edit_FilePath);

	DDX_Control(pDX, IDC_EDIT_RECVCSTR, m_RichEdit_Recv);
}

BEGIN_MESSAGE_MAP(CMySScomDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_CLOSE()

	ON_COMMAND(IDC_MENU_TRAY_ABOUT, OnMenuTrayAbout)
	ON_COMMAND(IDC_MENU_TRAY_EXIT, OnMenuTrayExit)
	ON_COMMAND(IDC_MENU_TRAY_HIDE, OnMenuTrayHide)
	ON_COMMAND(IDC_MENU_TRAY_SHOW, OnMenuTrayShow)

	ON_COMMAND(IDC_MENU_EDIT_COPY, OnMenuEditCopy)
	ON_COMMAND(IDC_MENU_EDIT_CLEAR, OnMenuEditClear)
	ON_COMMAND(IDC_MENU_EDIT_FONT, OnMenuEditFont)
	ON_COMMAND(IDC_MENU_EDIT_ABOUT, OnMenuEditAbout)
	ON_COMMAND(IDC_MENU_EDIT_COLOR_WHITE, OnMenuEditColorWhite)
	ON_COMMAND(IDC_MENU_EDIT_COLOR_GRAY, OnMenuEditColorGray)
	ON_COMMAND(IDC_MENU_EDIT_COLOR_GREEN, OnMenuEditColorGreen)
	ON_COMMAND(IDC_MENU_EDIT_COLOR_BLUE, OnMenuEditColorBlue)
	ON_COMMAND(IDC_MENU_EDIT_COLOR_PINK, OnMenuEditColorPink)
	ON_COMMAND(IDC_MENU_EDIT_COLOR_PURPLE, OnMenuEditColorPurple)

	ON_BN_CLICKED(IDC_BUTTON_ONOFF, OnButtonPortCtrl)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnButtonRecvPause)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClearAll)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSaveFile)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSendData)
	ON_BN_CLICKED(IDC_BUTTON_SRSEND, OnButtonSuperSend)
	ON_BN_CLICKED(IDC_BUTTON_EXFUNCT, OnButtonExtrafunc)
	ON_BN_CLICKED(IDC_BUTTON_KEYWORD, OnButtonConfigKeywords)
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE, OnButtonOpenFile)
	ON_BN_CLICKED(IDC_BUTTON_SENDFILE, OnButtonSendFile)
	ON_BN_CLICKED(IDC_BUTTON_SETFILTER, OnButtonConfigDsplFilt)
	ON_BN_CLICKED(IDC_BUTTON_SETREPLY, OnButtonConfigAutoReply)

	ON_BN_CLICKED(IDC_CHECK_HEXDISPL, OnCheckHexDisplay)
	ON_BN_CLICKED(IDC_CHECK_AUTOSAVE, OnCheckAutoSave)
	ON_BN_CLICKED(IDC_CHECK_HEXSSEND, OnCheckHexSend)
	ON_BN_CLICKED(IDC_CHECK_AUTOSEND, OnCheckAutoSend)
	ON_BN_CLICKED(IDC_CHECK_CRLFSEND, OnCheckAddReturn)
	ON_BN_CLICKED(IDC_CHECK_SHOWTIME, OnCheckShowTime)
	ON_BN_CLICKED(IDC_CHECK_AUTOCLER, OnCheckAutoClear)
	ON_BN_CLICKED(IDC_CHECK_HEXFRAME, OnCheckFrameDspl)
	ON_BN_CLICKED(IDC_CHECK_AUTOFILT, OnCheckDsplFilt)
	ON_BN_CLICKED(IDC_CHECK_PROTOACK, OnCheckAutoReply)
	ON_BN_CLICKED(IDC_CHECK_KEYWORDS, OnCheckKeyword)
	
	ON_MESSAGE(WM_USERMSG_NOTIFYICON, OnUsrMsgHdlIconNotify)
	ON_MESSAGE(WM_USERMSG_COMDEVLIST, OnUsrMsgHdlComDevList)
	ON_MESSAGE(WM_USERMSG_COMDEVWAIT, OnUsrMsgHdlComDevWait)
	ON_MESSAGE(WM_USERMSG_DATARECVED, OnUsrMsgHdlDataRecved)
	ON_MESSAGE(WM_USERMSG_DATATOSEND, OnUsrMsgHdlDatatoSend)
	ON_MESSAGE(WM_USERMSG_ARDLGCLOSE, OnUsrMsgHdlARDlgClose)
	ON_MESSAGE(WM_USERMSG_DFDLGCLOSE, OnUsrMsgHdlDFDlgClose)
	ON_MESSAGE(WM_USERMSG_EFDLGCLOSE, OnUsrMsgHdlEFDlgClose)
	ON_MESSAGE(WM_USERMSG_SSDLGCLOSE, OnUsrMsgHdlSSDlgClose)
	ON_MESSAGE(WM_USERMSG_KWDLGCLOSE, OnUsrMsgHdlKWDlgClose)

	ON_WM_SYSCOMMAND()
	ON_WM_CONTEXTMENU()
	ON_EN_CHANGE(IDC_EDIT_SENDCSTR, &CMySScomDlg::OnChangeEditSendcstr)
	ON_STN_CLICKED(IDC_STATIC_MS, &CMySScomDlg::OnStnClickedStaticMs)
	ON_BN_CLICKED(IDC_BUTTON1, &CMySScomDlg::SendPacketData)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS_SENDFILE, &CMySScomDlg::OnNMCustomdrawProgressSendfile)
	ON_BN_CLICKED(IDC_BUTTON2, &CMySScomDlg::OnButtonSendUnPackFile)
	ON_BN_CLICKED(IDC_BUTTON3, &CMySScomDlg::StrongTest)
	ON_EN_CHANGE(IDC_EDIT_RECVCSTR, &CMySScomDlg::OnEnChangeEditRecvcstr)
	ON_BN_CLICKED(IDC_STATIC_RECEIVE, &CMySScomDlg::OnBnClickedStaticReceive)
	ON_BN_CLICKED(IDC_BUTTON4, &CMySScomDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMySScomDlg::StopSending)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CMySScomDlg, CDialog)
	//{{AFX_EVENTSINK_MAP(CMySScomDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BEGIN_EASYSIZE_MAP(CMySScomDlg)
	EASYSIZE(IDC_STATIC_CONTROL,    ES_BORDER,           ES_BORDER,          ES_KEEPSIZE,        ES_BORDER,        0)
	EASYSIZE(IDC_STATIC_RECEIVE,    IDC_STATIC_CONTROL,  ES_BORDER,          ES_BORDER,          ES_BORDER,        0)
	EASYSIZE(IDC_STATIC_SEND,       IDC_STATIC_CONTROL,  ES_KEEPSIZE,        ES_BORDER,          ES_BORDER,        0)
	EASYSIZE(IDC_STATIC_FILE,       IDC_STATIC_CONTROL,  ES_KEEPSIZE,        ES_BORDER,          ES_BORDER,        0)
	EASYSIZE(IDC_EDIT_RECVCSTR,     ES_BORDER,           ES_BORDER,          ES_BORDER,          ES_BORDER,        0)
	EASYSIZE(IDC_EDIT_SENDCSTR,     ES_BORDER,           ES_KEEPSIZE,        ES_BORDER,          ES_BORDER,        0)
	EASYSIZE(IDC_PROGRESS_SENDFILE, ES_BORDER,           ES_KEEPSIZE,        ES_BORDER,          ES_BORDER,        0)
	EASYSIZE(IDC_BUTTON_SEND,       ES_KEEPSIZE,         ES_KEEPSIZE,        ES_BORDER,          IDC_STATIC_SEND,  0)
	EASYSIZE(IDC_BUTTON1,			ES_KEEPSIZE,         ES_KEEPSIZE,        ES_BORDER,          IDC_STATIC_SEND,  0)
	EASYSIZE(IDC_EDIT_FILEPATH,     ES_BORDER,           ES_KEEPSIZE,        ES_BORDER,          ES_BORDER,        0)
	EASYSIZE(IDC_BUTTON_OPENFILE,   ES_KEEPSIZE,         ES_KEEPSIZE,        ES_BORDER,          IDC_STATIC_SEND,  0)
	EASYSIZE(IDC_BUTTON_SENDFILE,   ES_KEEPSIZE,         ES_KEEPSIZE,        ES_BORDER,          IDC_STATIC_SEND,  0)
	EASYSIZE(IDC_BUTTON2,			ES_KEEPSIZE,         ES_KEEPSIZE,        ES_BORDER,          IDC_STATIC_SEND,  0)
END_EASYSIZE_MAP


/**************************************************************************************************
**  函数名称:  EnumCommPortList
**  功能描述:  本函数用来枚举电脑上存在可用的串口
**************************************************************************************************/
BOOL CMySScomDlg::EnumCommPortList(void)
{
	HKEY    hSERIALCOMM;
	BOOL    bSuccess = FALSE;
	CString comstr;
	bool    newone;
	
	s_PortNumber.RemoveAll();
	
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_QUERY_VALUE, &hSERIALCOMM) == ERROR_SUCCESS) {

		DWORD dwMaxValueNameLen;
		DWORD dwMaxValueLen;
		DWORD dwQueryInfo = RegQueryInfoKey(hSERIALCOMM, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &dwMaxValueNameLen, &dwMaxValueLen, NULL, NULL);
		
		if (dwQueryInfo == ERROR_SUCCESS) {

			DWORD dwMaxValueNameSizeInChars = dwMaxValueNameLen + 1;
			DWORD dwMaxValueNameSizeInBytes = dwMaxValueNameSizeInChars * sizeof(TCHAR);
			DWORD dwMaxValueDataSizeInChars = dwMaxValueLen / sizeof(TCHAR) + 1;
			DWORD dwMaxValueDataSizeInBytes = dwMaxValueDataSizeInChars * sizeof(TCHAR);
				
			TCHAR *szValueName;
			BYTE  *byValue;

			if ((szValueName = (TCHAR *)malloc(dwMaxValueNameSizeInChars)) && (byValue = (BYTE *)malloc(dwMaxValueDataSizeInBytes))) {
				
				bSuccess = TRUE;
				
				DWORD dwIndex = 0;
				DWORD dwType;
				DWORD dwValueNameSize = dwMaxValueNameSizeInChars;
				DWORD dwDataSize = dwMaxValueDataSizeInBytes;

				memset(szValueName, 0, dwMaxValueNameSizeInBytes);
				memset(byValue, 0, dwMaxValueDataSizeInBytes);

				LONG nEnum = RegEnumValue(hSERIALCOMM, dwIndex, szValueName, &dwValueNameSize, NULL, &dwType, byValue, &dwDataSize);
				
				while (nEnum == ERROR_SUCCESS) {

					if (dwType == REG_SZ) {
						TCHAR* szPort = (TCHAR *)(byValue);

						newone = TRUE;

						for (int i = 0; i < s_PortNumber.GetSize(); i++) {     /* 这段话是用来剔除名称一样的重复项 */
							comstr = s_PortNumber.GetAt(i);
							if (comstr == szPort) {
								newone = FALSE;
							}
						}
						if (newone == TRUE) {
							s_PortNumber.Add(szPort);
						}
					}
					
					dwValueNameSize = dwMaxValueNameSizeInChars;
					dwDataSize = dwMaxValueDataSizeInBytes;
					memset(szValueName, 0, dwMaxValueNameSizeInBytes);
					memset(byValue, 0, dwMaxValueDataSizeInBytes);
					++dwIndex;
					nEnum = RegEnumValue(hSERIALCOMM, dwIndex, szValueName, &dwValueNameSize, NULL, &dwType, byValue, &dwDataSize);
				}

				free(szValueName);
				free(byValue);
			} else {
				SetLastError(ERROR_OUTOFMEMORY);
			}
		}
		
		RegCloseKey(hSERIALCOMM);
		
		if (dwQueryInfo != ERROR_SUCCESS) {
			SetLastError(dwQueryInfo);
		}
	}
	
	return bSuccess;
}

/* ============================================================================================= */
/* ====================================                    ===================================== */
/* ==================================== 传说中华丽的分割线 ===================================== */
/* ====================================                    ===================================== */
/* ============================================================================================= */

/* ==================================== 以下为控件状态设置 ===================================== */

/**************************************************************************************************
**  函数名称:  SetControlStatus
**  功能描述:  设置控件的状态
**************************************************************************************************/
void CMySScomDlg::SetControlStatus(bool Enable)
{
    GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(Enable);
    GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON_OPENFILE)->EnableWindow(Enable);
    GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(Enable);
    GetDlgItem(IDC_CHECK_HEXDISPL)->EnableWindow(Enable);
    GetDlgItem(IDC_CHECK_AUTOCLER)->EnableWindow(Enable);
    GetDlgItem(IDC_CHECK_AUTOSEND)->EnableWindow(Enable);
    GetDlgItem(IDC_CHECK_HEXSSEND)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON_SETREPLY)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON_SETFILTER)->EnableWindow(Enable);
	GetDlgItem(IDC_CHECK_SHOWTIME)->EnableWindow(Enable);
	GetDlgItem(IDC_CHECK_AUTOFILT)->EnableWindow(Enable);
	GetDlgItem(IDC_CHECK_KEYWORDS)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON_KEYWORD)->EnableWindow(Enable);
	GetDlgItem(IDC_STATIC_OVER)->EnableWindow(Enable);
	GetDlgItem(IDC_STATIC_LINES)->EnableWindow(Enable);
	GetDlgItem(IDC_EDIT_AUTCLRKB)->EnableWindow(Enable);
	GetDlgItem(IDC_EDIT_SENDTIME)->EnableWindow(Enable);
	GetDlgItem(IDC_STATIC_MS)->EnableWindow(Enable);
	GetDlgItem(IDC_CHECK_CRLFSEND)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON_SRSEND)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(Enable);

    if ((IsDlgButtonChecked(IDC_CHECK_AUTOCLER) == FALSE) || (Enable == FALSE)) {
        GetDlgItem(IDC_CHECK_AUTOSAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_HEXFRAME)->EnableWindow(FALSE);
    } else {
        GetDlgItem(IDC_CHECK_AUTOSAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_HEXFRAME)->EnableWindow(TRUE);
    }

	if (IsDlgButtonChecked(IDC_CHECK_HEXDISPL) == FALSE) {
		GetDlgItem(IDC_CHECK_HEXFRAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_PROTOACK)->EnableWindow(FALSE);
	} else {
		GetDlgItem(IDC_CHECK_HEXFRAME)->EnableWindow(Enable);
		GetDlgItem(IDC_CHECK_PROTOACK)->EnableWindow(Enable);
	}

	if (Enable == TRUE) {
		if (IsDlgButtonChecked(IDC_CHECK_PROTOACK) == TRUE) {
			GetDlgItem(IDC_BUTTON_SETREPLY)->EnableWindow(TRUE);
		} else {
			GetDlgItem(IDC_BUTTON_SETREPLY)->EnableWindow(FALSE);
		}

		if (IsDlgButtonChecked(IDC_CHECK_AUTOFILT) == TRUE) {
			GetDlgItem(IDC_BUTTON_SETFILTER)->EnableWindow(TRUE);
		} else {
			GetDlgItem(IDC_BUTTON_SETFILTER)->EnableWindow(FALSE);
		}

		if (IsDlgButtonChecked(IDC_CHECK_KEYWORDS) == TRUE) {
			GetDlgItem(IDC_BUTTON_KEYWORD)->EnableWindow(TRUE);
		} else {
			GetDlgItem(IDC_BUTTON_KEYWORD)->EnableWindow(FALSE);
		}
	}
}

/**************************************************************************************************
**  函数名称:  SetSendCtrlArea
**  功能描述:  在发送文件的时候设置各个发送区的状态
**************************************************************************************************/
void CMySScomDlg::SetSendCtrlArea(bool Enable)
{
	GetDlgItem(IDC_CHECK_HEXSSEND)->EnableWindow(Enable);
	GetDlgItem(IDC_CHECK_AUTOSEND)->EnableWindow(Enable);
	GetDlgItem(IDC_EDIT_SENDTIME)->EnableWindow(Enable);
	GetDlgItem(IDC_STATIC_MS)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON_SRSEND)->EnableWindow(Enable);
	GetDlgItem(IDC_CHECK_CRLFSEND)->EnableWindow(Enable);
	GetDlgItem(IDC_EDIT_FILEPATH)->EnableWindow(Enable);
	GetDlgItem(IDC_BUTTON_OPENFILE)->EnableWindow(Enable);
	SetDlgItemText(IDC_BUTTON_SENDFILE, (Enable == TRUE) ? "开始发送" : "停止发送");
	GetDlgItem(IDC_EDIT_SENDCSTR)->ShowWindow((Enable == TRUE) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BUTTON_SEND)->ShowWindow((Enable == TRUE) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_PROGRESS_SENDFILE)->ShowWindow((Enable == TRUE) ? SW_HIDE : SW_SHOW);

	if (Enable == TRUE) {
		SetDlgItemText(IDC_STATIC_SEND, "发送区");
	}
}


/* ============================================================================================= */
/* ====================================                    ===================================== */
/* ==================================== 传说中华丽的分割线 ===================================== */
/* ====================================                    ===================================== */
/* ============================================================================================= */

/* ==================================== 以下为消息处理函数 ===================================== */

/**************************************************************************************************
**  函数名称:  OnUsrMsgHdlIconNotify
**  功能描述:  处理托盘图标消息
**************************************************************************************************/
LRESULT CMySScomDlg::OnUsrMsgHdlIconNotify(WPARAM wParam, LPARAM lParam)
{
	UINT    uMouseMsg = LOWORD(lParam);
	CMenu   oMenu;
	CPoint  oPoint;

	switch (uMouseMsg)
	{
		case WM_LBUTTONDBLCLK:                                                 /* 如果是左键双击 */
			ShowWindow(IsWindowVisible() ? SW_HIDE : SW_SHOWNORMAL);
			break;

		case WM_RBUTTONDOWN:                                                   /* 如果是右键 */
			if (oMenu.LoadMenu(IDR_MENU_TRAY)) {
				CMenu* pPopup = oMenu.GetSubMenu(0);
				ASSERT(pPopup != NULL);
				GetCursorPos(&oPoint);                                         /* 确定鼠标位置以便在该位置附近显示菜单 */
				SetForegroundWindow();
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, oPoint.x, oPoint.y, this); 
			}
			break;
	}

	return 0;
}

/**************************************************************************************************
**  函数名称:  OnUsrMsgHdlComDevList
**  功能描述:  处理更新串口号下拉列表内容的消息 （串口设备监听线程监听到串口设备发生变化时，发送本消息给主窗体通知其立即刷新列表显示）
**************************************************************************************************/
LRESULT CMySScomDlg::OnUsrMsgHdlComDevList(WPARAM wParam, LPARAM lParam)
{
	UpdateComboComs();
	
	return true;
}

/**************************************************************************************************
**  函数名称:  OnUsrMsgHdlComDevWait
**  功能描述:  处理稍候更新串口设备列表的消息 （串口设备监听线程监听到串口设备发生变化时，发送本消息给主窗体通知其稍候刷新列表显示）
**************************************************************************************************/
LRESULT CMySScomDlg::OnUsrMsgHdlComDevWait(WPARAM wParam, LPARAM lParam)
{
	s_DevNeedUpd = TRUE;

	return true;
}

/**************************************************************************************************
**  函数名称:  OnUsrMsgHdlDataRecved
**  功能描述:  处理接收到了串口数据的消息 （串口数据监听线程收到串口数据时，发送本消息给主窗体进行数据解析处理）
**************************************************************************************************/
LRESULT CMySScomDlg::OnUsrMsgHdlDataRecved(WPARAM wParam, LPARAM lParam)
{
	unsigned char buff[MAX_SEND_BYTE];

	memcpy(buff, (unsigned char *)lParam, wParam);

	HandleUSARTData(buff, wParam);

	return true;
}

/**************************************************************************************************
**  函数名称:  OnUsrMsgHdlDatatoSend
**  功能描述:  处理有串口数据需要发送的消息
**************************************************************************************************/
LRESULT CMySScomDlg::OnUsrMsgHdlDatatoSend(WPARAM wParam, LPARAM lParam)
{
	unsigned char buff[MAX_SEND_BYTE];
	
	memcpy(buff, (unsigned char *)lParam, wParam);

	SendDatatoComm(buff, wParam, FALSE);

	return true;
}

/**************************************************************************************************
**  函数名称:  OnUsrMsgHdlARDlgClose
**  功能描述:  处理自动回复窗口关闭的消息
**************************************************************************************************/
LRESULT CMySScomDlg::OnUsrMsgHdlARDlgClose(WPARAM wParam, LPARAM lParam)
{
	//GetDlgItem(IDC_BUTTON_EXFUNCT)->EnableWindow(TRUE);

	return true;
}

/**************************************************************************************************
**  函数名称:  OnUsrMsgHdlDFDlgClose
**  功能描述:  处理显示过滤窗口关闭的消息
**************************************************************************************************/
LRESULT CMySScomDlg::OnUsrMsgHdlDFDlgClose(WPARAM wParam, LPARAM lParam)
{
	return true;
}

/**************************************************************************************************
**  函数名称:  OnUsrMsgHdlKWDlgClose
**  功能描述:  处理关键字匹配窗口关闭的消息
**************************************************************************************************/
LRESULT CMySScomDlg::OnUsrMsgHdlKWDlgClose(WPARAM wParam, LPARAM lParam)
{
	return true;
}

/**************************************************************************************************
**  函数名称:  OnUsrMsgHdlEFDlgClose
**  功能描述:  处理附加功能窗口关闭的消息
**************************************************************************************************/
LRESULT CMySScomDlg::OnUsrMsgHdlEFDlgClose(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_BUTTON_EXFUNCT)->EnableWindow(TRUE);

	return true;
}

/**************************************************************************************************
**  函数名称:  OnUsrMsgHdlSSDlgClose
**  功能描述:  处理高级发送窗口关闭的消息
**************************************************************************************************/
LRESULT CMySScomDlg::OnUsrMsgHdlSSDlgClose(WPARAM wParam, LPARAM lParam)
{
	SetDlgItemText(IDC_BUTTON_SRSEND, "高级发送");
	GetDlgItem(IDC_CHECK_HEXSSEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_AUTOSEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SENDTIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_MS)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OPENFILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(TRUE);

	return true;
}


/* ============================================================================================= */
/* ====================================                    ===================================== */
/* ==================================== 传说中华丽的分割线 ===================================== */
/* ====================================                    ===================================== */
/* ============================================================================================= */


/* ==================================== 以下为自定义处理函数 =================================== */

/**************************************************************************************************
**  函数名称:  ShowAboutMeDialog
**  功能描述:  显示“关于我”的对话框
**************************************************************************************************/
void CMySScomDlg::ShowAboutMeDialog(void)
{
	ShellExecute(NULL, _T("open"), README_FILE_PATH, NULL, NULL, SW_SHOW);
}

/**************************************************************************************************
**  函数名称:  KeyWordMatchOK
**  功能描述:  关键字匹配成功
**************************************************************************************************/
bool CMySScomDlg::KeyWordMatchOK(CString showstr)
{
	CString  tempstr;
	
	for (int i = 1; i < s_PDlgKeyWords->GetKeywordMax(); i++) {                /* 逐条分析，关键字是否匹配成功 */
		tempstr = s_PDlgKeyWords->GetKeywordX(i);
		if ((tempstr != "") && (tempstr.GetLength() > 0)) {
			if (showstr.Find(tempstr, 0) != -1) {
				s_PDlgKeyWords->AddNewMatchRec(i);                             /* 将匹配结果通知到该页面进行处理 */
				return TRUE;
			}
		}
	}

	return FALSE;
}

/**************************************************************************************************
**  函数名称:  ShakeMainWindow
**  功能描述:  抖动窗体
**************************************************************************************************/
void CMySScomDlg::ShakeMainWindow(void)
{
	CRect rect;  
	int m_move = SHAKE_DISTANCE;

	GetWindowRect(&rect);  

	for (int i = 1; i < 11; i++)  {                                            /* 为了使窗口抖动后能回到原来的位置，这里设置i必须为5的倍数并且i<5*n+1 */
		rect.OffsetRect(0, m_move);                                            /* OffsetRect函数将指定的矩形移动到指定的位置,这里是将矩形向上移动10像素 */
		MoveWindow(&rect);  
		Sleep(40);
		rect.OffsetRect(m_move,0);
		MoveWindow(&rect);
		Sleep(40);
		m_move = (m_move == SHAKE_DISTANCE) ? (-SHAKE_DISTANCE) : SHAKE_DISTANCE;
	}
}

/**************************************************************************************************
**  函数名称:  AttractAttention
**  功能描述:  吸引用户的注意
**************************************************************************************************/
void CMySScomDlg::AttractAttention(void)
{
	FlashWindowEx(FLASHW_ALL | FLASHW_TIMERNOFG, 10, 0);

	if (IsIconic() == TRUE) {                                                  /* 如果窗体已经最小化，将其最大化显示 */
		ShowWindow(SW_SHOWMAXIMIZED);
	} else {                                                                   /* 否则，将其按原来的状态显示出来即可 */
		ShowWindow(SW_SHOW);
	}

	ShakeMainWindow();

	if (s_PDlgKeyWords->IsWindowVisible() == FALSE) {
		s_PDlgKeyWords->ShowHideDlgWindow(TRUE);                               /* 弹出窗体，用于展示匹配成功的结果 */
	}
}

/**************************************************************************************************
**  函数名称:  CloseAllChildWin
**  功能描述:  关闭所有子窗口
**************************************************************************************************/
void CMySScomDlg::CloseAllChildWin(void)
{
	s_PDlgSuprSend->ShowHideDlgWindow(FALSE);                                  /* 关闭高级发送窗口 */
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_USERMSG_SSDLGCLOSE, 0, 0); 

	s_PDlgExtrfunc->ShowHideDlgWindow(FALSE);                                  /* 关闭附加功能窗口 */
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_USERMSG_EFDLGCLOSE, 0, 0);   

	s_PDlgDsplFilt->ShowHideDlgWindow(FALSE);                                  /* 关闭过滤设置窗口 */
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_USERMSG_DFDLGCLOSE, 0, 0);

	s_PDlgKeyWords->ShowHideDlgWindow(FALSE);                                  /* 关闭关键字符窗口 */
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_USERMSG_KWDLGCLOSE, 0, 0);

	s_PDlgAutoRply->ShowHideDlgWindow(FALSE);                                  /* 关闭自动回复窗口 */
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_USERMSG_ARDLGCLOSE, 0, 0);
}

/**************************************************************************************************
**  函数名称:  UserFnKeyHdl
**  功能描述:  用户自定义Fn功能键消息处理
**************************************************************************************************/
bool CMySScomDlg::UserFnKeyHdl(WPARAM key)
{
	unsigned char dirbuf[512];
	CString       pathstr;
	
	if (key == VK_F1) {                                                        /* 按下F1键：弹出系统帮助的对话框 */
		ShowAboutMeDialog();
		return true;
	}

	if (key == VK_F2) {                                                        /* 按下F2键：自动打开系统保存文件的目录 */
		GetCurrentDirectory(512, (LPSTR)dirbuf);
		pathstr = dirbuf;
		pathstr = pathstr + "\\" + REC_DIR_PATH;
		ShellExecute(NULL, "open", pathstr, NULL, NULL, SW_SHOWNORMAL);
		return true;
	}

	if (key == VK_F5) {                                                        /* 按下F5键：清空数据窗口的全部内容 */
		OnButtonClearAll();
		return true;
	}

	if (key == VK_F8) {                                                        /* 按下F8键：开始发送文件数据内容 */
		if (SerialDevisOpened() == TRUE) {
			OnButtonSendFile();
		} else {
			MessageBox("串口都没打开，您叫我把文件往哪里发送？", "提示", MB_OK + MB_ICONERROR);
		}
		return true;
	}

	if (key == VK_F12) {                                                       /* 按下F12键：保存数据窗口的全部内容 */
		OnButtonSaveFile();
		return true;
	}

	MessageBox("此热键的功能尚未定义......\r\n阁下是否有很好的建议？欢迎随时出谋划策！", "提示", MB_OK + MB_ICONINFORMATION);
	return false;
}

/**************************************************************************************************
**  函数名称:  GetHighExactTime
**  功能描述:  获取精确的时间
**************************************************************************************************/
CString CMySScomDlg::GetHighExactTime(void)
{
	CString       timestr;
	LARGE_INTEGER litmp;
	CTime         NowTime;
	LONGLONG      NowTick;
	double        difFreq, difTime;

	QueryPerformanceFrequency(&litmp);
	difFreq = (double)litmp.QuadPart;                                          /* 获得计数器的时钟频率 */

	QueryPerformanceCounter(&litmp);
	NowTick = litmp.QuadPart;                                                  /* 获得中止值 */

	difTime = (1000 * ((double)(NowTick - s_StartdTcik))) / difFreq;           /* 获得对应的时间值，单位为ms */

	NowTime = CTime::GetCurrentTime();
	timestr.Format("[%.2d:%.2d:%.2d:%.3d] ", NowTime.GetHour(), NowTime.GetMinute(), NowTime.GetSecond(), ((int)difTime % 1000));

	return timestr;
}

/**************************************************************************************************
**  函数名称:  SaveEditContent
**  功能描述:  保存编辑框的内容到文本文件
**************************************************************************************************/
bool CMySScomDlg::SaveEditContent(void)
{
	CFile rawFile, decodedFile;  // 分别用于原始数据和解码数据
	CString commStr, rawFileName, decodedFileName;
	CTime nowTime = CTime::GetCurrentTime();

	// 1. 获取当前串口号
	int comNumber = m_Combo_CommPort.GetCurSel();
	m_Combo_CommPort.GetLBText(comNumber, commStr);

	// 2. 生成文件名（原始数据和解码数据）
	CString timeStr = nowTime.Format("_%y-%m-%d_%H-%M-%S");
	rawFileName = "All_Rec_" + commStr + timeStr + ".txt";
	decodedFileName = "Decoded_" + commStr + timeStr + ".txt";  

	// 3. 获取编辑框内容（原始数据）
	GetDlgItemText(IDC_EDIT_RECVCSTR, m_Edit_RecvCstr);

	// 4. 保存原始数据
	if (!rawFile.Open(REC_DIR_PATH + rawFileName, CFile::modeCreate | CFile::modeWrite)) {
		AfxMessageBox(_T("原始文件保存失败！"));
		return FALSE;
	}
	rawFile.Write(m_Edit_RecvCstr, m_Edit_RecvCstr.GetLength());
	rawFile.Close();
	//DecodeEditContent();
	//// 5. 解码并保存处理后的数据
	//if (DecodeAndSaveFile(REC_DIR_PATH + rawFileName, REC_DIR_PATH + decodedFileName)) {
	//	AfxMessageBox(_T("解码文件保存成功！"));
	//}
	//else {
	//	AfxMessageBox(_T("解码失败！"));
	//	return FALSE;
	//}
	CString decodedData;
	if (DecodeAndSaveToVariable(m_Edit_RecvCstr, decodedData)) {
		// 保存解码后的CSV数据
		CStdioFile file;
		if (file.Open(REC_DIR_PATH + decodedFileName, CFile::modeCreate | CFile::modeWrite)) {
			// 写入CSV文件头
			// 表头（严格对齐数据列的宽度）
			file.WriteString(_T("            时间                     \t纬度		\t经度	       椭球高	海平面高	北向速度	 东向速度	  地速	水平速度	方位角  方位精度	仰角	  仰角精度	\n"));
			file.WriteString(decodedData);
			file.Close();
		}
		else {
			AfxMessageBox(_T("解码文件保存失败！"));
			return FALSE;
		}
	}
	else {
		AfxMessageBox(_T("解码失败！"));
		return FALSE;
	}
	return TRUE;
}

// 解析协议函数
bool CMySScomDlg::DecodeAndSaveToVariable(const CString& rawData, CString& outDecodedData)
{
	// 1. 查找帧头 "EE CC" 的位置
	int headerPos = rawData.Find(_T("EE CC"));
	if (headerPos == -1) {
		TRACE(_T("错误：未找到帧头 EE CC\n"));
		return false;
	}

	// 2. 提取协议部分（从EE CC开始）
	CString hexProtocol = rawData.Mid(headerPos);
	hexProtocol.Remove(' '); // 移除所有空格
	
	// 3. 检查最小长度（至少能读取数据长度字段）
	if (hexProtocol.GetLength() < 8) { // 需要至少4字节（8字符）
		TRACE(_T("错误：数据太短，无法读取长度字段\n"));
		return false;
	}


// 定义临时变量存储解析后的字节值
	unsigned int year_high, year_low, month, day, hour, minute, second, millis_high, millis_low;
	int longitude, latitude, height, hMSL, velN, velE, velD, gSpeed, relPosHeading, relPosEvl;
	uint32_t accHeading, accEvl;
	uint8_t checksum,checktemp;
	// 按协议字段偏移量解析字节（每2字符为1字节）
	sscanf_s(hexProtocol.Mid(4 * 2, 2), "%2x", &year_low);    // 索引4（第5字节）：低位
	sscanf_s(hexProtocol.Mid(5 * 2, 2), "%2x", &year_high);   // 索引5（第6字节）：高位
	sscanf_s(hexProtocol.Mid(6 * 2, 2), "%2x", &month);       // 索引6（第7字节）
	sscanf_s(hexProtocol.Mid(7 * 2, 2), "%2x", &day);         // 索引7（第8字节）
	sscanf_s(hexProtocol.Mid(8 * 2, 2), "%2x", &hour);        // 索引8（第9字节）
	sscanf_s(hexProtocol.Mid(9 * 2, 2), "%2x", &minute);      // 索引9（第10字节）
	sscanf_s(hexProtocol.Mid(10 * 2, 2), "%2x", &second);     // 索引10（第11字节）
	sscanf_s(hexProtocol.Mid(11 * 2, 2), "%2x", &millis_low); // 索引11（第12字节）：低位
	sscanf_s(hexProtocol.Mid(12 * 2, 2), "%2x", &millis_high);// 索引12（第13字节）：高位

	// 小端序合并
	uint16_t year = (year_high << 8) | year_low;
	uint16_t millis = (millis_high << 8) | millis_low;

	// 定义4字节小端序解析函数
	auto parseU32LittleEndian = [&](int bytePos) -> uint32_t {
		uint32_t b0, b1, b2, b3;

		// 安全解析每个字节（每2字符为1字节）
		sscanf_s(hexProtocol.Mid(bytePos * 2, 2), "%2x", &b0);
		sscanf_s(hexProtocol.Mid((bytePos + 1) * 2, 2), "%2x", &b1);
		sscanf_s(hexProtocol.Mid((bytePos + 2) * 2, 2), "%2x", &b2);
		sscanf_s(hexProtocol.Mid((bytePos + 3) * 2, 2), "%2x", &b3);

		// 小端序组合
		return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
		};

	// 修正后的字段解析
	longitude     = parseU32LittleEndian(13);  // 字节13-16 (小端序)
	latitude      = parseU32LittleEndian(17);  // 字节17-20
	height        = parseU32LittleEndian(21);  // 字节21-24
	hMSL          = parseU32LittleEndian(25);  // 字节25-28
	velN          = parseU32LittleEndian(29);  // 字节29-32
	velE          = parseU32LittleEndian(33);  // 字节33-36
	velD          = parseU32LittleEndian(37);  // 字节37-40
	gSpeed        = parseU32LittleEndian(41);  // 字节41-44
	relPosHeading = parseU32LittleEndian(45);  // 字节45-48
	accHeading    = parseU32LittleEndian(49);  // 字节49-52
	relPosEvl     = parseU32LittleEndian(53);  // 字节53-56
	accEvl        = parseU32LittleEndian(57);  // 字节57-60

	// 校验和（最后1字节）
	sscanf_s(hexProtocol.Mid(61 * 2, 2) ,"%hhx", &checksum);

	// 4. 检查校验和
	height = parseU32LittleEndian(21);  // 字节21-24
	checktemp = 0xFF & (year + year_low + month + day + hour + minute + second + millis + longitude + latitude + height+
		hMSL+ velN + velE + velD + gSpeed + relPosHeading + accHeading + relPosEvl + accEvl);
	if (checksum == checktemp) {
		TRACE(_T("校验和正确\n"));
	}
	else
	{
		TRACE(_T("校验和错误\n"));
		//return false;
	}
	// 数据格式化（添加固定宽度）
	outDecodedData.Format(
		_T("%04u-%02u-%02u %02u:%02u:%02u.%03u ")  // 时间（固定19字符）
		_T("%11.6f\t%11.6f\t")                      // 纬度,经度（保留6位小数）
		_T("%6.2f\t%6.2f\t")                        // 椭球高,海平面高（2位小数）
		_T("%6.2f\t%6.2f\t%6.2f\t%6.2f\t")          // 速度（北/东/地/水平）
		_T("%5.2f\t%5.2f\t%5.2f\t%5.2f"),           // 角度（方位/仰角及精度）

		// 时间字段
		year, month, day, hour, minute, second, millis,

		// 位置和高程（转换为浮点）
		latitude / 1000000.0, longitude / 1000000.0,  // 假设原始数据为1e-7度
		height / 1000.0, hMSL / 1000.0,          // 高程单位转换为米

		// 速度（转换为m/s）
		velN / 1000.0, velE / 1000.0, velD / 1000.0, gSpeed / 1000.0,

		// 角度
		relPosHeading / 100000.0,  // 方位角（假设单位0.001度）
		accHeading / 100000.0,     // 方位精度
		relPosEvl / 100000.0,      // 仰角
		accEvl / 100000.0          // 仰角精度
	);

	return true;
}

/**************************************************************************************************
**  函数名称:  ExcuteAutoReply
**  功能描述:  处理协议数据的自动回复
**************************************************************************************************/
void CMySScomDlg::ExcuteAutoReply(CString instr)
{
	CString TempStr;
	unsigned char sbuf[MAX_SEND_BYTE];

	for (int i = 0; i < s_PDlgAutoRply->GetTotalStringNo(); i++) {
		TempStr = s_PDlgAutoRply->GetRecvString(i);
		if ((TempStr.GetLength() > 0) && (instr.Find(TempStr, 0) != -1)) {
			TempStr = s_PDlgAutoRply->GetSendString(i);
			if (TempStr.GetLength() > 0) {
				strncpy_s((char *)&sbuf[0], sizeof(sbuf), (LPCTSTR)TempStr, TempStr.GetLength());
				SendDatatoComm(sbuf, TempStr.GetLength(), TRUE);
			}
		}
	}
}

/**************************************************************************************************
**  函数名称:  UpdateEditStr
**  功能描述:  将更新部分的字符显示出来
**************************************************************************************************/
void CMySScomDlg::UpdateEditStr(CString showstr)
{
    /* 如果开启了显示过滤功能，则进行字符串匹配，不需要显示的字符串内容不予显示，只是更新一下状态栏的统计信息 */
	if ((m_Check_AutoFilt == TRUE) && (s_PDlgDsplFilt->StringCanDisplay(showstr) == FALSE)) {
		UpdateStatusBarNow();
		return;
	}

	/* 如果开启了关键字自动匹配的功能，则进行字符串匹配。匹配成功时，自动进行窗体抖动及弹出对话框提示，来吸引用户的注意 */
	if ((m_Check_Keywords == TRUE) && (KeyWordMatchOK(showstr) == TRUE)) {
		AttractAttention();
	}

	/* 如果在16进制显示模式下，同时开启了自动回复的功能。则判断接收到的字符中是否有满足条件的字段，并进行自动回复 */
	if ((m_Check_HexDispl == TRUE) && (m_Check_ProtoACK == TRUE)) {
		ExcuteAutoReply(showstr);
	}

	#if 1                                                                      /* ◆◆这种方法会导致中文乱码◆◆ */
	{
		s_Edit_Recv->SetSel(-1, -1);                                           /* 添加本次的内容显示 */
		s_Edit_Recv->ReplaceSel((LPCTSTR)showstr);
		s_Edit_Recv->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);                    /* 让编辑框内容滚动到最后一行 */
	}
	#else                                                                      /* ◆◆这种方法会导致CPU占用率过高◆◆ */
	{
		m_Edit_RecvCstr += showstr;                                            /* 添加本次的内容显示 */
		SetDlgItemText(IDC_EDIT_RECVCSTR, m_Edit_RecvCstr);
        s_Edit_Recv->SetSel(-1, -1);
		s_Edit_Recv->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);                    /* 让编辑框内容滚动到最后一行 */
	}
	#endif

	if (showstr.Right(1) == "\n") {                                            /* 如果接收到了回车符 */
		if (m_Check_AutoCler == TRUE) {                                        /* 如果需要自动清空内容 */
			GetDlgItemText(IDC_EDIT_AUTCLRKB, m_Edit_AutClrKB);                /* 读取数据并保存 */
			if (s_RecvedByte >= (atoi((LPSTR)(LPCTSTR)m_Edit_AutClrKB) * 1024)) {        /* 判断是否满足清空条件*/
				if (m_Check_AutoSave == TRUE) {                                /* 如果需要自动保存 */
					SaveEditContent();                                         /* 保存编辑框的数据 */
				}
				s_RecvedByte = 0;
				m_Edit_RecvCstr = "";
				SetDlgItemText(IDC_EDIT_RECVCSTR, m_Edit_RecvCstr);            /* 清空编辑框内容 */
			}
		}
	}

	UpdateStatusBarNow();                                                      /* 更新状态栏统计数据的显示 */
}

/**************************************************************************************************
**  函数名称:  HandleUSARTData
**  功能描述:  接收串口数据
**************************************************************************************************/
void CMySScomDlg::HandleUSARTData(unsigned char *sbuf, DWORD len)
{
    DWORD   i;
    CString ShowStr, TempStr, TimeStr;

	TempStr = "";
	ShowStr = "";

	if (s_RecvPaused == TRUE) return;                                          /* 暂停接收时，不进行处理 */

    for (i = 0; i < len; i++) {                                                /* 将数组转换为Cstring型变量 */

		if (m_Check_HexDispl == TRUE) {                                        /* 当前处于16进制显示模式 */

			/* 考虑到00字符的特殊性，需要对其进行转义才能存储。转义规则如下：00转义成FF 01，FF转义成FF 02，其他字符不转义 */

			if (sbuf[i] == 0) {                                                /* 00 转义成 FF 01 */
				TempStr.Format("%c%c", 0xFF, 0x01);
			} else if ((unsigned char)(sbuf[i]) == 0xFF) {                     /* FF 转义成 FF 02 */
				TempStr.Format("%c%c", 0xFF, 0x02);
			} else {
				TempStr.Format("%c", sbuf[i]);
			}

			TempStr = TransformtoHex(TempStr);                                 /* 转换结果为16进制显示 */

			if ((s_NeedChgLne == TRUE) && (m_Check_ShowTime == TRUE)) {        /* 如果需要换行显示 */
				ShowStr +=  GetHighExactTime() + TempStr; 
				s_NeedChgLne = FALSE;
			} else {
				ShowStr += TempStr; 
			}
			
			if (m_Check_HexFrame == TRUE) {                                    /* 这里判断接下来一段时间内是否没有再收到其他数据 */
				KillTimer(Timer_No_FrameDspl);                                 /* 以实现16进制下，按帧换行显示的功能 */
				SetTimer(Timer_No_FrameDspl, CHNGLINE_INTERVAL, NULL);         /* 这里重新启动定时器判断是否没有再收到其他数据 */
			}
			
		} else {                                                               /* 当前处于字符显示模式 */

			if (s_NeedChgLne == TRUE) {                                        /* 如果接收完一整行 */

				if (m_Check_ShowTime == TRUE) {                                /* 如果启用了时间显示功能 */
					ShowStr = ShowStr + GetHighExactTime();
				}
				
				s_NeedChgLne = FALSE;
			}

			TempStr.Format("%c", sbuf[i]);                                     /* 处理接收到的数据 */
			ShowStr += TempStr;                                                /* 保存数据内容 */

			if (TempStr == "\n") {                                             /* 本次接收到回车符 */
			    s_NeedChgLne = TRUE;                                           /* 标记需要换行显示 */
			}
        }
    }

	s_RecvedByte += len;                                                       /* 接收字节数累加 */
    s_DataRecved  = TRUE;
	s_RecvString += ShowStr;                                                   /* 注意这里要用加号，不然会造成之前的数据丢失 */
}

/**************************************************************************************************
**  函数名称:  NeedAutoSendData
**  功能描述:  启动定时发送数据的功能
**************************************************************************************************/
void CMySScomDlg::NeedAutoSendData(void)
{
    int Timer;
    CString TempStr;

    GetDlgItemText(IDC_EDIT_SENDTIME, TempStr);
    
    Timer = atoi((LPSTR)(LPCTSTR)TempStr);
    
    if (Timer > 0) {

        SetTimer(Timer_No_AutoSend, Timer, NULL);                              /* 启动定时器 */

        GetDlgItem(IDC_EDIT_SENDTIME)->EnableWindow(FALSE);
        GetDlgItem(IDC_STATIC_MS)->EnableWindow(FALSE);

        m_Edit_SendTime = TempStr;                                             /* 更新编辑框内容 */
        SetDlgItemText(IDC_EDIT_SENDTIME, m_Edit_SendTime);

        GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);                      /* 禁用发送按钮 */
		GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE);

    } else {

        MessageBox("发送时间间隔不能为0，请确认！  ", "提示", MB_OK + MB_ICONEXCLAMATION);

        SetDlgItemText(IDC_EDIT_SENDTIME, m_Edit_SendTime);                    /* 还原编辑框内容 */

        m_Check_AutoSend = FALSE;
        UpdateData(FALSE);                                                    /* 取消复选框被选中的状态 */

        return;
    }
}

/**************************************************************************************************
**  函数名称:  UpdateStatusBarNow
**  功能描述:  更新状态栏的显示
**************************************************************************************************/
void CMySScomDlg::UpdateStatusBarNow(void)
{
    CTime   Nowtime;
    CString TempStr, DisplayStr;
    CRect   DialogMain;

    this->GetWindowRect(&DialogMain);                                          /* 获取主界面在屏幕上的位置 */

#if VERSION_CTRL == VERSION_YAXON
	if (DialogMain.Width() > 1150) {
		DisplayStr = " 欢迎使用MySScom ※ 这是MySSCom的魔改版本 ※ ";
	} else if (DialogMain.Width() > 700) {
		DisplayStr = " 欢迎使用";
	} else {
		DisplayStr = "";
	}
#else
    if (DialogMain.Width() > 1200) {
        DisplayStr = " 欢迎使用MySScom ※ Designed By LEON (LEON1741@126.com) ※ Welcome to Email me!!";
    } else if (DialogMain.Width() > 1100) {
        DisplayStr = " 欢迎使用MySScom ※ Designed By LEON (LEON1741@126.com)";
    } else if (DialogMain.Width() > 1000) {
        DisplayStr = " 欢迎使用MySScom ※ Designed By LEON";
    } else if (DialogMain.Width() > 850) {
        DisplayStr = " 欢迎使用MySScom ※ LEON";
    } else if (DialogMain.Width() > 800) {
        DisplayStr = " 欢迎使用MySScom";
    } else if (DialogMain.Width() > 720) {
        DisplayStr = " 欢迎使用";
    } else {
        DisplayStr = "";
    }
#endif

	s_CStatusBar.SetPaneText(0, DisplayStr);
        
    DisplayStr = (SerialDevisOpened() == TRUE) ? " 串口已打开" : " 串口未打开";  
    s_CStatusBar.SetPaneText(1, DisplayStr);
        
	DisplayStr.Format(" 接收: %.3d, %.3d, %.3d", (s_RecvedByte / 1000000), ((s_RecvedByte % 1000000)  / 1000), (s_RecvedByte % 1000));
    s_CStatusBar.SetPaneText(2, DisplayStr);

	DisplayStr.Format(" 发送: %.3d, %.3d, %.3d", (s_SendedByte / 1000000), ((s_SendedByte % 1000000)  / 1000), (s_SendedByte % 1000));
    s_CStatusBar.SetPaneText(3, DisplayStr);

    Nowtime = CTime::GetCurrentTime();
    DisplayStr = " 当前时间: " + Nowtime.Format("%Y-%m-%d") + " " + Nowtime.Format("%H:%M:%S");
    s_CStatusBar.SetPaneText(4, DisplayStr);

	UpdateMainStatic();                                                        /* 更新提示框的显示 */
}

/*测试Lara USR-LG207的缓存大小是否为240字节*/
void CMySScomDlg::SendTestUSRLG207(void) {
	unsigned char data[MAX_SEND_BYTE] = { 0 };	
	SendDatatoComm(data, MAX_SEND_BYTE, FALSE);
}
/*测试结果：252个字节*/

/**************************************************************************************************
**  函数名称:  SendDatatoComm
**  功能描述:  将指定的字符串发送出去
**************************************************************************************************/
bool CMySScomDlg::SendDatatoComm(unsigned char *sbuf, int slen, BOOL needhex)
{
	int j, len;
	CString  tempstr;
	unsigned char temp[MAX_SEND_BYTE];
	CByteArray hexdata;	
	
    if (needhex) {                                                             /* 如果需要以16进制发送 */

		if (slen > MAX_SEND_BYTE * 3) {                                        /* 数据长度过大 */
			return FALSE;
		}

		tempstr.Format("%s", sbuf);
		if (ParseCStrisLegal(tempstr) == FALSE) {                              /* 字符串内容非法 */
			return FALSE;
		}

        len = String2Hex(tempstr, hexdata);                                    /* 将字符串转换为实际对应的数据 */
		
        for (j = 0; j < len; j++) {                                            /* 将实际数据保存至数组 */
            temp[j] = hexdata.GetAt(j);
        }
        
        s_SendedByte += SendSerialData(temp, len);                             /* 写入串口发送，发送字节数累加 */
        
    } else {                                                                   /* 字符模式下，直接发送 */

		if (slen > MAX_SEND_BYTE) {                                            /* 数据长度过大 */
			return FALSE;
		}

        s_SendedByte += SendSerialData(sbuf, slen);                            /* 写入串口发送，发送字节数累加 */
    }
	
    if (m_Check_CRLFSend == TRUE) {                                            /* 补发回车换行符 */
		
        temp[0] = '\r';
        temp[1] = '\n';
        
        s_SendedByte += SendSerialData(temp, 2);
    }

    UpdateStatusBarNow();                                                      /* 更新状态栏统计数据的显示 */
	
	return TRUE;
}

/**************************************************************************************************
**  函数名称:  SendFileDatatoComm
**  功能描述:  将文件数据发送到串口
**************************************************************************************************/
bool CMySScomDlg::SendFileDatatoComm(void)
{
	CFile          filename;
	int            baudrate, sendbyte;
	unsigned long  filetlen;
	unsigned char *filebuff;                                                   /* 用于存放文件数据 */
	double         tempbyte;
	CString        dsplystr;
	unsigned int   totltime, lefttime;

	if (filename.Open(m_Edit_FilePath, CFile::modeReadWrite | CFile::typeBinary) == 0) {
		MessageBox("读取文件失败，请确认路径正确且文件未处于打开状态！    ", "提示", MB_OK + MB_ICONINFORMATION);
		return FALSE;
	}

	filetlen = (unsigned long)filename.GetLength();                            /* 获取文件长度 */
	
	baudrate = m_Combo_BaudRate.GetCurSel();
	baudrate = Combo_Baud[baudrate];

	tempbyte = ((double)baudrate / FILESEND_BYTE) * 1.1;
	sendbyte = (int)tempbyte;

	if ((s_FileDatPos + sendbyte) > filetlen) {                                /* 这里需要对最后一包进行特殊判断 */
		sendbyte = filetlen - s_FileDatPos;
	}
	
    filebuff = new unsigned char[sendbyte];                                    /* 分配内存空间 */
	filename.Seek(s_FileDatPos, CFile::begin);                                 /* 定位到上次发送的位置 */
    filename.Read(filebuff, sendbyte);                                         /* 读取文件内容 */

	sendbyte = SendSerialData(filebuff, sendbyte);                             /* 发送数据，并统计长度 */
	s_FileDatPos += sendbyte;
	s_SendedByte += sendbyte;

	totltime = (filetlen * 10) / baudrate + 1;                                 /* 计算发送整个文件的耗时 */
	lefttime = (filetlen - s_FileDatPos) * 10 / baudrate + 1;                  /* 计算发送剩余部分的耗时 */

	m_Progs_SendFile.SetPos(s_FileDatPos * PROGRESS_POS / filetlen);           /* 更新进度条显示 */

	dsplystr.Format("发送区：预计耗时%0.2d分%0.2d秒，当前进度%0.2d%%，剩余%0.2d分%0.2d秒", 
		            totltime / 60, totltime % 60, s_FileDatPos * 100 / filetlen, lefttime / 60, lefttime % 60);
	SetDlgItemText(IDC_STATIC_SEND, dsplystr);
	
	filename.Close();                                                          /* 关闭文件 */
	delete []filebuff;                                                         /* 释放空间 */

	if (s_FileDatPos >= filetlen) {                                            /* 全部数据发送完毕 */
		s_FileDatPos = 0;
		KillTimer(Timer_No_SendFile);
		SetSendCtrlArea(TRUE);                                                 /* 启用其他发送控件 */
		MessageBox("文件发送完成！", "提示", MB_OK + MB_ICONINFORMATION);
	}

	return TRUE;
}

bool CMySScomDlg::SendFileDataPackstoComm()
{
	LARGE_INTEGER freq, start, end;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	CFile filename;
	const int FIXED_PACKET_SIZE = 200; // 固定每次发送200字节数据部分
	const int HEADER_SIZE = 7;        // 包头长度
	const int CHECKSUM_SIZE = 1;      // 校验和长度
	const int MAX_PACKET_SIZE = HEADER_SIZE + FIXED_PACKET_SIZE + CHECKSUM_SIZE;

	unsigned long filetlen = 0;
	unsigned char filebuff[FIXED_PACKET_SIZE] = { 0 };
	unsigned char txBuffer[MAX_PACKET_SIZE] = { 0 };   // 完整数据包缓冲区 

	if (!filename.Open(m_Edit_FilePath, CFile::modeRead | CFile::typeBinary)) {
		MessageBox("读取文件失败，请确认路径正确且文件未处于打开状态！", "提示", MB_OK + MB_ICONINFORMATION);
		return FALSE;
	}

	filetlen = (unsigned long)filename.GetLength();

	// 计算本次实际发送量（不超过200字节）
	int sendbyte = min(FIXED_PACKET_SIZE, (int)(filetlen - s_FileDatPos));

	filename.Seek(s_FileDatPos, CFile::begin);
	filename.Read(filebuff, sendbyte);

	//添加包头
	static uint16_t packetCounter = 1; // 数据包计数器
	uint16_t TotalPackets = (filetlen + FIXED_PACKET_SIZE - 1) / FIXED_PACKET_SIZE;
	txBuffer[0] = 0xEE;   // 包头标识1
	txBuffer[1] = 0xCC;   // 包头标识2
	txBuffer[2] = (packetCounter >> 8) & 0xFF; // 序号高字节
	txBuffer[3] = packetCounter & 0xFF;        // 序号低字节
	txBuffer[4] = (TotalPackets >> 8) & 0xFF;  // 总包数高字节
	txBuffer[5] = TotalPackets & 0xFF;         // 总包数低字节
	txBuffer[6] = static_cast<BYTE>(sendbyte);					   // 数据位数


	// 2. 拷贝数据
	memcpy(&txBuffer[HEADER_SIZE], filebuff, sendbyte);

	// 3. 计算校验和
	unsigned char checksum = 0;
	for (int i = 0; i < HEADER_SIZE + sendbyte; i++) {
		checksum ^= txBuffer[i];
	}
	txBuffer[HEADER_SIZE + sendbyte] = checksum; // 追加校验和

	// ===== 发送完整数据包 =====
	int actualSent = SendSerialData(txBuffer, HEADER_SIZE + sendbyte + CHECKSUM_SIZE);
	if (actualSent <= 0) {
		MessageBox("发送失败！", "错误", MB_OK + MB_ICONERROR);
		filename.Close();
		return FALSE;
	}
	// 计算时间间隔
	QueryPerformanceCounter(&end);
	double intervalMs = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
	start = end;

	// 调试输出
	CString debugMsg;
	debugMsg.Format(_T("time=%.3fms\n"), intervalMs);
	OutputDebugString(debugMsg);

	packetCounter++;  // 更新包计数器
	// 更新状态
	s_FileDatPos += sendbyte;
	s_SendedByte += actualSent;

	// 计算剩余时间（基于波特率）
	// 剩余数据字节数
	int remainingData = filetlen - s_FileDatPos;
	// 剩余包数（处理最后一包不满200字节的情况）
	int remainingPackets = (remainingData + FIXED_PACKET_SIZE - 1) / FIXED_PACKET_SIZE;
	// 剩余总字节数（含包头）
	int remainingTotalBytes = remainingPackets * MAX_PACKET_SIZE;

	int baudrate = Combo_Baud[m_Combo_BaudRate.GetCurSel()];
	double secondsPerByte = 10.0 / baudrate; // 每个字节的传输时间(s)
	double remainingTime = remainingTotalBytes * secondsPerByte + remainingPackets * 0.15;

	// 更新进度条（PROGRESS_POS=100）
	m_Progs_SendFile.SetPos((int)(s_FileDatPos * 100.0 / filetlen));

	// 更新状态文本
	CString statusText;
	statusText.Format(_T("进度: %d/%d字节 (%.1f%%) | 剩余时间: %.4f秒"),
		s_FileDatPos, filetlen,
		s_FileDatPos * 100.0 / filetlen,
		remainingTime);
	SetDlgItemText(IDC_STATIC_SEND, statusText);

	filename.Close();

	// 检查是否发送完成
	if (s_FileDatPos >= filetlen) {
		s_FileDatPos = 0;
		packetCounter = 1;
		KillTimer(Timer_No_SendUnPackFile);
		SetSendCtrlArea(TRUE);

		

		CString completeMsg;
		completeMsg.Format(_T("数据发送完成，共%hu包"), TotalPackets);
		MessageBox(completeMsg, _T("发送完成"), MB_OK | MB_ICONINFORMATION);

		
	}

	return TRUE;
}

/**************************************************************************************************
**  函数名称:  ConfigFileCanUse
**  功能描述:  分析当前版本的程序是否能够兼容使用目标版本下的配置文件
**************************************************************************************************/
bool CMySScomDlg::ConfigFileCanUse(CString target)
{
	return TRUE;
}

/**************************************************************************************************
**  函数名称:  CreateSettingFile
**  功能描述:  创建配置文件，并写入默认参数
**  注意事项:  若配置文件已经存在，则需校验其中的版本号，从而分析出是否兼容该版本的文件格式
**************************************************************************************************/
void CMySScomDlg::CreateSettingFile(void)
{
	char TempChar[256];
	CString oldver, showstr;
	
	// 以下语句判断目录下是否存在INI文件，若不存在则创建该文件并写入默认值

    if ((::GetPrivateProfileInt(FLAG_SYSRUNREC, SYSRUNREC_RUNTIMES, 0, CONFIGFILENAME)) == FALSE) {
		::WritePrivateProfileString(FLAG_SYSRUNREC, SYSRUNREC_PROGMVER, GetProgramVersion(), CONFIGFILENAME);
        ::WritePrivateProfileString(FLAG_SYSRUNREC, SYSRUNREC_RUNTIMES, "1",  CONFIGFILENAME);
		return;
	}

	::GetPrivateProfileString(FLAG_SYSRUNREC, SYSRUNREC_PROGMVER, "", TempChar, sizeof(TempChar), CONFIGFILENAME);
	oldver.Format("%s", TempChar);

	if (ConfigFileCanUse(oldver) == FALSE) {                                   /* 配置文件版本不能兼容 */
		showstr = "检测到目录下现有的配置文件版本过旧，无法使用！\r\n系统即将自动创建一个全新的配置文件\r\n并将原来的配置文件更名为\"Settings_old.ini\"\r\n若旧文件中存在重要信息，请手动拷贝其中的内容到新文件中！";
		MessageBox(showstr, "抱歉", MB_OK + MB_ICONINFORMATION);
		rename(CONFIGFILENAME, CONFIGBACKNAME);                                /* 将原有的配置文件更名，然后新建一个，并写入关键信息 */
		::WritePrivateProfileString(FLAG_SYSRUNREC, SYSRUNREC_PROGMVER, GetProgramVersion(), CONFIGFILENAME);
		::WritePrivateProfileString(FLAG_SYSRUNREC, SYSRUNREC_RUNTIMES, "1",  CONFIGFILENAME);
	}
}

/**************************************************************************************************
**  函数名称:  InitiateAllParas
**  功能描述:  读取配置文件的各个参数内容并初始化对话框的值
**************************************************************************************************/
void CMySScomDlg::InitiateAllParas(void)
{
    char TempChar[MAX_SEND_BYTE];

	s_DialogPos_X = (::GetPrivateProfileInt(FLAG_MAINFRAME, MAINFRAME_POSTIONX, 0, CONFIGFILENAME));
	s_DialogPos_Y = (::GetPrivateProfileInt(FLAG_MAINFRAME, MAINFRAME_POSTIONY, 0, CONFIGFILENAME));

    m_Combo_CommPort.SetCurSel(::GetPrivateProfileInt(FLAG_MAINFRAME, MAINFRAME_COMMPORT, 0, CONFIGFILENAME));
    m_Combo_BaudRate.SetCurSel(::GetPrivateProfileInt(FLAG_MAINFRAME, MAINFRAME_BAUDRATE, 4, CONFIGFILENAME));
    m_Combo_DataBits.SetCurSel(::GetPrivateProfileInt(FLAG_MAINFRAME, MAINFRAME_DATABITS, 3, CONFIGFILENAME));
    m_Combo_CheckBit.SetCurSel(::GetPrivateProfileInt(FLAG_MAINFRAME, MAINFRAME_CHECKBIT, 0, CONFIGFILENAME));
    m_Combo_StopBits.SetCurSel(::GetPrivateProfileInt(FLAG_MAINFRAME, MAINFRAME_STOPBITS, 0, CONFIGFILENAME));

	m_Check_ShowTime = (::GetPrivateProfileInt(FLAG_MAINFRAME, MAINFRAME_SHOWTIME, 1, CONFIGFILENAME)) ? TRUE : FALSE;
    m_Check_HexDispl = (::GetPrivateProfileInt(FLAG_MAINFRAME, MAINFRAME_HEXDISPL, 0, CONFIGFILENAME)) ? TRUE : FALSE;
    m_Check_HexFrame = (::GetPrivateProfileInt(FLAG_MAINFRAME, MAINFRAME_HEXFRAME, 0, CONFIGFILENAME)) ? TRUE : FALSE;
	m_Check_ProtoACK = (::GetPrivateProfileInt(FLAG_MAINFRAME, MAINFRAME_PROTOACK, 0, CONFIGFILENAME)) ? TRUE : FALSE;
	m_Check_HexsSend = (::GetPrivateProfileInt(FLAG_MAINFRAME, MAINFRAME_HEXSSEND, 0, CONFIGFILENAME)) ? TRUE : FALSE;
	m_Check_CRLFSend = (::GetPrivateProfileInt(FLAG_MAINFRAME, MAINFRAME_CRLFSEND, 0, CONFIGFILENAME)) ? TRUE : FALSE;

    ::GetPrivateProfileString(FLAG_MAINFRAME, MAINFRAME_AUTCLRKB, "1000", TempChar, 5, CONFIGFILENAME);
    m_Edit_AutClrKB.Format("%s", TempChar);
    
    ::GetPrivateProfileString(FLAG_MAINFRAME, MAINFRAME_SENDTIME, "1000", TempChar, 5, CONFIGFILENAME);
    m_Edit_SendTime.Format("%s", TempChar);

    ::GetPrivateProfileString(FLAG_MAINFRAME, MAINFRAME_SENDSTRS, "", TempChar, MAX_SEND_BYTE, CONFIGFILENAME);
    s_SendStr_Chr.Format("%s", TempChar);
	s_SendStr_Chr = FormatQuotesStrRead(s_SendStr_Chr);

	::GetPrivateProfileString(FLAG_MAINFRAME, MAINFRAME_SENDHEXS, "", TempChar, MAX_SEND_BYTE, CONFIGFILENAME);
	s_SendStr_Hex.Format("%s", TempChar);

	m_Edit_SendCstr = (m_Check_HexsSend == TRUE) ? s_SendStr_Hex : s_SendStr_Chr;

	::GetPrivateProfileString(FLAG_MAINFRAME, MAINFRAME_FILEPATH, "", TempChar, 1024, CONFIGFILENAME);
	m_Edit_FilePath.Format("%s", TempChar);
    
	UpdateData(FALSE);                                                         /* 更新编辑框内容 */
}

/**************************************************************************************************
**  函数名称:  RecordAllParas
**  功能描述:  将对话框中各个参数的值记录到配置文件中
**************************************************************************************************/
void CMySScomDlg::RecordAllParas(void)
{
    int TempData;                                                              /* 需要注意的是：自动发送使能和循环发送使能两项无须保存 */
    CString ParaStr, TempStr;
	CRect rect;

	TempData = (::GetPrivateProfileInt(FLAG_SYSRUNREC, SYSRUNREC_RUNTIMES, 0, CONFIGFILENAME));
	ParaStr.Format("%d", TempData + 1);
	::WritePrivateProfileString(FLAG_SYSRUNREC, SYSRUNREC_RUNTIMES, ParaStr, CONFIGFILENAME);
    
	GetWindowRect(&rect);
	s_DialogPos_X = (rect.left < 0) ? 0 : rect.left;                           /* 这里要防止出现负值，因此要做限定 */
	s_DialogPos_Y = (rect.top  < 0) ? 0 : rect.top;

	ParaStr.Format("%d", s_DialogPos_X);
	::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_POSTIONX, ParaStr, CONFIGFILENAME);

	ParaStr.Format("%d", s_DialogPos_Y);
	::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_POSTIONY, ParaStr, CONFIGFILENAME);
	
    ParaStr.Format("%d", m_Combo_CommPort.GetCurSel());
    ::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_COMMPORT, ParaStr, CONFIGFILENAME);

    ParaStr.Format("%d", m_Combo_BaudRate.GetCurSel());
    ::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_BAUDRATE, ParaStr, CONFIGFILENAME);

    ParaStr.Format("%d", m_Combo_DataBits.GetCurSel());
    ::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_DATABITS, ParaStr, CONFIGFILENAME);

    ParaStr.Format("%d", m_Combo_CheckBit.GetCurSel());
    ::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_CHECKBIT, ParaStr, CONFIGFILENAME);

    ParaStr.Format("%d", m_Combo_StopBits.GetCurSel());
    ::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_STOPBITS, ParaStr, CONFIGFILENAME);

    ::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_SHOWTIME, m_Check_ShowTime  ?  "1" : "0", CONFIGFILENAME);
	::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_HEXDISPL, m_Check_HexDispl  ?  "1" : "0", CONFIGFILENAME);
	::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_HEXFRAME, m_Check_HexFrame  ?  "1" : "0", CONFIGFILENAME);
	::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_HEXSSEND, m_Check_HexsSend  ?  "1" : "0", CONFIGFILENAME);
	::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_CRLFSEND, m_Check_CRLFSend  ?  "1" : "0", CONFIGFILENAME);

    GetDlgItemText(IDC_EDIT_AUTCLRKB, m_Edit_AutClrKB);
    ::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_AUTCLRKB, m_Edit_AutClrKB, CONFIGFILENAME);

	::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_AUTOSEND, m_Check_AutoSend  ?  "1" : "0", CONFIGFILENAME);
    GetDlgItemText(IDC_EDIT_SENDTIME, m_Edit_SendTime);
    ::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_SENDTIME, m_Edit_SendTime, CONFIGFILENAME);

	::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_SENDHEXS, s_SendStr_Hex, CONFIGFILENAME);
	::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_SENDSTRS, FormatQuotesStrWrite(s_SendStr_Chr), CONFIGFILENAME);

	GetDlgItemText(IDC_EDIT_FILEPATH, m_Edit_FilePath);
	::WritePrivateProfileString(FLAG_MAINFRAME, MAINFRAME_FILEPATH, m_Edit_FilePath, CONFIGFILENAME);
}

/**************************************************************************************************
**  函数名称:  InitiateStatusBar
**  功能描述:  初始化状态栏
**************************************************************************************************/
void CMySScomDlg::InitiateStatusBar(void)
{
    static UINT indicators[] =
    {
        ID_SEPARATOR,
        ID_INDICATOR_CAPS,
        ID_INDICATOR_NUM,
        ID_INDICATOR_SCRL,
        ID_INDICATOR_OVR
    };

    CTime   time;
    CString m_strTime;
    UINT    nID = 0;
    
    if (!s_CStatusBar.Create(this) || !s_CStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT))) {
        TRACE0("Failed to create status bar!");
        return;
    }

    time = CTime::GetCurrentTime();
    m_strTime = " 当前时间: " + time.Format("%Y-%m-%d") + " " + time.Format("%H:%M:%S");

    s_CStatusBar.SetPaneInfo(0, nID, SBPS_STRETCH, 1);
    s_CStatusBar.SetPaneText(0, " 欢迎使用MySScom");

    s_CStatusBar.SetPaneInfo(1, nID, SBPS_NORMAL, 90);
    s_CStatusBar.SetPaneText(1, " 串口未打开");
    
    s_CStatusBar.SetPaneInfo(2, nID, SBPS_NORMAL, 180);
    s_CStatusBar.SetPaneText(2, " 接收: 000, 000, 000");
    
    s_CStatusBar.SetPaneInfo(3, nID, SBPS_NORMAL, 180);
    s_CStatusBar.SetPaneText(3, " 发送: 000, 000, 000");

    s_CStatusBar.SetPaneInfo(4, nID, SBPS_NORMAL, 230);
    s_CStatusBar.SetPaneText(4, m_strTime);

    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}

/**************************************************************************************************
**  函数名称:  InitiateComboComs
**  功能描述:  初始化串口号组合框
**************************************************************************************************/
void CMySScomDlg::InitiateComboComs(void)
{
    EnumCommPortList();                                                        /* 枚举可用的串口 */

	m_Combo_CommPort.ResetContent();
	m_Combo_CommPort.AddString("请选择");                                      /* 第一行为提示行 */

    for (int i = 0; i < s_PortNumber.GetSize(); i++) {
        m_Combo_CommPort.AddString(s_PortNumber.GetAt(i));
    }
}

/**************************************************************************************************
**  函数名称:  InitiateComboBaud
**  功能描述:  初始化波特率组合框
**************************************************************************************************/
void CMySScomDlg::InitiateComboBaud(void)
{
    CString TempStr;
    
    for (int i = 0; i < (sizeof(Combo_Baud) / sizeof(Combo_Baud[0])); i++) {
        TempStr.Format("%d bps", Combo_Baud[i]);
        m_Combo_BaudRate.AddString(TempStr);
    }
}

/**************************************************************************************************
**  函数名称:  InitiateComboData
**  功能描述:  初始化数据位组合框
**************************************************************************************************/
void CMySScomDlg::InitiateComboData(void)
{
    CString TempStr;
    
    for (int i = 0; i < (sizeof(Combo_Data) / sizeof(Combo_Data[0])); i++) {
        TempStr.Format("%d 位", Combo_Data[i]);
        m_Combo_DataBits.AddString(TempStr);
    }
}

/**************************************************************************************************
**  函数名称:  InitiateComboCheck
**  功能描述:  初始化校验位组合框
**************************************************************************************************/
void CMySScomDlg::InitiateComboCheck(void)
{
    m_Combo_CheckBit.AddString("None 校验");
    m_Combo_CheckBit.AddString("Odd  校验");
    m_Combo_CheckBit.AddString("Even 校验");
    m_Combo_CheckBit.AddString("Mark 校验");
    m_Combo_CheckBit.AddString("Space校验");
}

/**************************************************************************************************
**  函数名称:  InitiateComboStop
**  功能描述:  初始化停止位组合框
**************************************************************************************************/
void CMySScomDlg::InitiateComboStop(void)
{
    m_Combo_StopBits.AddString("1  位");
    m_Combo_StopBits.AddString("1.5位");
    m_Combo_StopBits.AddString("2  位");
}

/**************************************************************************************************
**  函数名称:  InitiateChildWins
**  功能描述:  初始化各个子窗体页面
**************************************************************************************************/
bool CMySScomDlg::InitiateChildWins(void)
{
	s_PDlgSuprSend = new CDialogSuperSend();                                   /* 以下语句创建高级发送窗体，并隐藏待用 */

	if (s_PDlgSuprSend != NULL){

		int Result = s_PDlgSuprSend->Create(IDD_DIALOG_SRSEND, this);

		if (!Result) {
			MessageBox("系统资源不足，创建对话框失败......   ", "抱歉", MB_OK + MB_ICONERROR);
			return FALSE;
		}

		s_PDlgSuprSend->ShowWindow(SW_HIDE);
		s_PDlgSuprSend->InitiateAllParas();
	}

	s_PDlgExtrfunc = new CDialogExtrafunct();                                  /* 以下语句创建附加功能窗体，并隐藏待用 */

	if (s_PDlgExtrfunc != NULL){

		int Result = s_PDlgExtrfunc->Create(IDD_DIALOG_EXFUNCT, this);

		if (!Result) {
			MessageBox("系统资源不足，创建对话框失败......   ", "抱歉", MB_OK + MB_ICONERROR);
			return FALSE;
		}

		s_PDlgExtrfunc->ShowWindow(SW_HIDE);
		s_PDlgExtrfunc->InitiateAllParas();
	}

	s_PDlgDsplFilt = new CDialogDsplFilt();                                    /* 以下语句创建显示过滤设置窗体，并隐藏待用 */

	if (s_PDlgDsplFilt != NULL){

		int Result = s_PDlgDsplFilt->Create(IDD_DIALOG_FILTER, this);

		if (!Result) {
			MessageBox("系统资源不足，创建对话框失败......   ", "抱歉", MB_OK + MB_ICONERROR);
			return FALSE;
		}

		s_PDlgDsplFilt->ShowWindow(SW_HIDE);
		s_PDlgDsplFilt->InitiateAllParas();
	}

	s_PDlgKeyWords = new CDialogKeywords();                                    /* 以下语句创建关键字监听窗体，并隐藏待用 */

	if (s_PDlgKeyWords != NULL){

		int Result = s_PDlgKeyWords->Create(IDD_DIALOG_KEYWORDS, this);

		if (!Result) {
			MessageBox("系统资源不足，创建对话框失败......   ", "抱歉", MB_OK + MB_ICONERROR);
			return FALSE;
		}

		s_PDlgKeyWords->ShowWindow(SW_HIDE);
		s_PDlgKeyWords->InitiateAllParas();
	}

	s_PDlgAutoRply = new CDialogAutoReply();                                   /* 以下语句创建自动回复窗体，并隐藏待用 */

	if (s_PDlgAutoRply != NULL){

		int Result = s_PDlgAutoRply->Create(IDD_DIALOG_AUTOREPLY, this);

		if (!Result) {
			MessageBox("系统资源不足，创建对话框失败......   ", "抱歉", MB_OK + MB_ICONERROR);
			return FALSE;
		}

		s_PDlgAutoRply->ShowWindow(SW_HIDE);
		s_PDlgAutoRply->InitiateAllParas();
	}

	return TRUE;
}

/**************************************************************************************************
**  函数名称:  InitiateToolsTip
**  功能描述:  初始化主界面下各个控件的悬浮提示信息
**************************************************************************************************/
void CMySScomDlg::InitiateToolsTip(void)
{
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);

	m_tooltip.AddTool(GetDlgItem(IDC_COMBO_COMMPORT),  IDS_STRING_001);
	m_tooltip.AddTool(GetDlgItem(IDC_COMBO_BAUDRATE),  IDS_STRING_002);
	m_tooltip.AddTool(GetDlgItem(IDC_COMBO_DATABITS),  IDS_STRING_003);
	m_tooltip.AddTool(GetDlgItem(IDC_COMBO_STOPBITS),  IDS_STRING_004);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_ONOFF),    IDS_STRING_005);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_PAUSE),    IDS_STRING_006);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_CLEAR),    IDS_STRING_007);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_SAVE),     IDS_STRING_008);
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_HEXDISPL),  IDS_STRING_009);
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_AUTOCLER),  IDS_STRING_010);
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_AUTOSAVE),  IDS_STRING_011);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_SEND),     IDS_STRING_015);
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_HEXSSEND),  IDS_STRING_016);
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_AUTOSEND),  IDS_STRING_017);
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_SENDTIME),   IDS_STRING_018);
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_CRLFSEND),  IDS_STRING_020);
	m_tooltip.AddTool(GetDlgItem(IDC_COMBO_CHECKBIT),  IDS_STRING_021);
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_AUTCLRKB),   IDS_STRING_024);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_EXFUNCT),  IDS_STRING_025);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_SRSEND),   IDS_STRING_026);
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_SENDCSTR),   IDS_STRING_027);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_OPENFILE), IDS_STRING_028);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_SENDFILE), IDS_STRING_029);
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_FILEPATH),   IDS_STRING_030);
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_SHOWTIME),  IDS_STRING_031);
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_HEXFRAME),  IDS_STRING_032);
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_AUTOFILT),  IDS_STRING_033);
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_PROTOACK),  IDS_STRING_034);
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_KEYWORDS),  IDS_STRING_035);
}

/**************************************************************************************************
**  函数名称:  InitiateMainStatic
**  功能描述:  初始化主界面帮助提示框的显示
**************************************************************************************************/
void CMySScomDlg::InitiateMainStatic(void)
{
	CFont* font = new CFont;

	s_MainStatic = new CStatic;
	CRect rect1(STATIC_LEFT, STATIC_TOP, STATIC_LEFT + STATIC_WIDTH, STATIC_TOP + STATIC_HEIGHT);

	s_MainStatic->CreateEx(NULL, "static", STATIC_TEXT, WS_CHILD | WS_VISIBLE, rect1, this, IDS_MAINSTATIC, NULL);
	
	font->CreateFont(14, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "arial");
	s_MainStatic->SetFont(font);

	s_MainStatic->ShowWindow(FALSE);
}

/**************************************************************************************************
**  函数名称:  UpdateMainStatic
**  功能描述:  更新主界面的信息提示框的显示
**************************************************************************************************/
void CMySScomDlg::UpdateMainStatic(void)
{
	CRect rectDlg;
	GetClientRect(rectDlg);

	if (rectDlg.Height() > MAINWIN_HEIGHT) {                                   /* 根据窗体当前的高度来判断 */
		s_MainStatic->ShowWindow(TRUE);
	} else {
		s_MainStatic->ShowWindow(FALSE);
	}
}

/**************************************************************************************************
**  函数名称:  UpdateComboComs
**  功能描述:  更新串口号组合框
**************************************************************************************************/
void CMySScomDlg::UpdateComboComs(void)
{
    int      pindex;
	CString  comstr;
	
	pindex = m_Combo_CommPort.GetCurSel();                                     /* 首先获取当前选择的串口号 */

	if (pindex == 0) {                                                         /* 0表示没有选中有效的串口号 */
		comstr = "";
	} else {                                                                   /* 根据串口号获取其COM字符串 */
		comstr = s_PortNumber.GetAt(pindex - 1);
	}

	InitiateComboComs();

	if (comstr == "") {                                                        /* 表示之前没有选择有效串口号 */
		m_Combo_CommPort.SetCurSel(0);                                         /* 直接选中提示栏 */
		return;
	}

	for (pindex = 0; pindex < s_PortNumber.GetSize(); pindex++) {              /* 之前曾经选中过某个串口号 */
		if (s_PortNumber.GetAt(pindex) == comstr) {                            /* 逐个比对，定位出该串口的序号 */
			m_Combo_CommPort.SetCurSel(pindex + 1);                            /* 自动选中该串口号 */
			return;
		}
	}

	m_Combo_CommPort.SetCurSel(0);                                             /* 比对失败，说明之前选中的串口已消失，则恢复到提示栏 */
}

/**************************************************************************************************
**  函数名称:  TaskBarAddIcon
**  功能描述:  在任务栏增加图标显示
**************************************************************************************************/
BOOL CMySScomDlg::TaskBarAddIcon(HWND hwnd, UINT uID, HICON hicon, LPSTR lpszTip)
{
    NOTIFYICONDATA d;
    
    d.cbSize = sizeof(NOTIFYICONDATA);
    d.hWnd   = hwnd;
    d.uID    = uID;
    d.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    d.hIcon  = hicon;
    d.uCallbackMessage = WM_USERMSG_NOTIFYICON;
    
    if (lpszTip) {
        lstrcpy(d.szTip, lpszTip);
    } else {
        d.szTip[0] = '\0';
    }
    
    return Shell_NotifyIcon(NIM_ADD, &d);
}

/**************************************************************************************************
**  函数名称:  TaskBarDeleteIcon
**  功能描述:  在任务栏删除图标显示
**************************************************************************************************/
BOOL CMySScomDlg::TaskBarDeleteIcon(HWND hwnd, UINT uID)
{
    NOTIFYICONDATA d;
    
    d.cbSize = sizeof(NOTIFYICONDATA);
    d.hWnd   = hwnd;
    d.uID    = uID;
    
    return Shell_NotifyIcon(NIM_DELETE, &d);
}

/* ============================================================================================= */
/* ====================================                    ===================================== */
/* ==================================== 传说中华丽的分割线 ===================================== */
/* ====================================                    ===================================== */
/* ============================================================================================= */

/* ================================== 各个控件消息处理--开始 =================================== */

/**************************************************************************************************
**  函数名称:  OnButtonPortCtrl
**  功能描述:  串口开关
**************************************************************************************************/
void CMySScomDlg::OnButtonPortCtrl() 
{
    CString TempStr;
    SRL_DEV_PARA_T spara;
    
    if (SerialDevisOpened() == TRUE) {                                         /* 如果串口已经打开，那么执行关闭操作 */

        if ((m_Check_AutoSend == TRUE) || (s_PDlgSuprSend->s_issenddata == TRUE)) {
            MessageBox("请首先停用自动发送功能再尝试关闭串口...  ", "提示", MB_OK + MB_ICONINFORMATION);
            return;
        }

		if (s_FileDatPos != 0) {
            MessageBox("正在发送文件，请先停止文件的发送，再尝试关闭串口...  ", "提示", MB_OK + MB_ICONINFORMATION);
            return;
        }
        
		CloseSerialDevice();                                                   /* 关闭串口设备 */
		CloseAllChildWin();                                                    /* 关闭各个子窗体 */

		SetControlStatus(FALSE);

        SetDlgItemText(IDC_BUTTON_ONOFF, "打开串口");
        GetDlgItem(IDC_COMBO_COMMPORT)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO_BAUDRATE)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO_DATABITS)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO_CHECKBIT)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO_STOPBITS)->EnableWindow(TRUE);

        return;
    }

    int ComNumber = m_Combo_CommPort.GetCurSel();                              /* 得到串口号 */

    if (ComNumber == 0) {
        MessageBox("连串口号都没有选择，你叫我打开什么东东...？   ", "提示", MB_OK + MB_ICONINFORMATION);
        return;
    }

	m_Combo_CommPort.GetLBText(ComNumber, TempStr);                            /* 获取串口号选择项 */
    CString ComDevFile = "\\\\.\\" + TempStr;

    TempStr.Format("%d", Combo_Baud[m_Combo_BaudRate.GetCurSel()]);            /* 获取波特率选择项 */
    spara.baudrate = atoi(TempStr);
    
    TempStr.Format("%d", Combo_Data[m_Combo_DataBits.GetCurSel()]);            /* 获取数据位选择项 */
    spara.databits = atoi(TempStr);
    
    TempStr.Format("%d", Combo_Check[m_Combo_CheckBit.GetCurSel()]);           /* 获取校验位选择项 */
    spara.checkbit = atoi(TempStr);

    TempStr.Format("%d", Combo_Stop[m_Combo_StopBits.GetCurSel()]);            /* 获取停止位选择项 */
    spara.stopbits = atoi(TempStr);

	if (OpenSerialDevice(ComDevFile, &spara) == FALSE) {                       /* 打开串口并初始化 */
		return;
	}

    SetControlStatus(TRUE);                                                    /* 启用各个按钮控件 */
    SetDlgItemText(IDC_BUTTON_ONOFF, "关闭串口");
    GetDlgItem(IDC_COMBO_COMMPORT)->EnableWindow(FALSE);
    GetDlgItem(IDC_COMBO_BAUDRATE)->EnableWindow(FALSE);
    GetDlgItem(IDC_COMBO_DATABITS)->EnableWindow(FALSE);
    GetDlgItem(IDC_COMBO_CHECKBIT)->EnableWindow(FALSE);
    GetDlgItem(IDC_COMBO_STOPBITS)->EnableWindow(FALSE);
}

/**************************************************************************************************
**  函数名称:  OnButtonRecvPause
**  功能描述:  暂停接收/恢复接收
**************************************************************************************************/
void CMySScomDlg::OnButtonRecvPause() 
{
    if (s_RecvPaused == FALSE) {
        s_RecvPaused = TRUE;
        SetDlgItemText(IDC_BUTTON_PAUSE, "恢复接收");
        GetDlgItem(IDC_BUTTON_ONOFF)->EnableWindow(FALSE);
    } else {
        s_RecvPaused = FALSE;
        SetDlgItemText(IDC_BUTTON_PAUSE, "暂停接收");
        GetDlgItem(IDC_BUTTON_ONOFF)->EnableWindow(TRUE);
    }
}

/**************************************************************************************************
**  函数名称:  OnButtonClearAll
**  功能描述:  清除窗口内已经接收的数据
**************************************************************************************************/
void CMySScomDlg::OnButtonClearAll()
{
    m_Edit_RecvCstr = "";
    SetDlgItemText(IDC_EDIT_RECVCSTR, m_Edit_RecvCstr);

    s_RecvedByte = 0;
    s_SendedByte = 0;
    s_RecvString = "";
    s_NeedChgLne = TRUE;                                                       /* 下次需要换行显示 */

    UpdateStatusBarNow();                                                      /* 更新状态栏的统计数据显示 */
}

/**************************************************************************************************
**  函数名称:  OnButtonSaveFile
**  功能描述:  将接收到的数据保存到文件中
**************************************************************************************************/
void CMySScomDlg::OnButtonSaveFile()
{
    CTime   NowTime  = CTime::GetCurrentTime();                                /* 获取现在时间 */
    CString FileName = NowTime.Format("%y-%m-%d %H-%M-%S") + ".txt";           /* 指定文件名 */
    
    GetDlgItemText(IDC_EDIT_RECVCSTR, m_Edit_RecvCstr);
    
    if (m_Edit_RecvCstr.GetLength() <= 0) {
        MessageBox("尚未接收到任何内容，无须保存！   ", "提示", MB_OK + MB_ICONINFORMATION);
        return;
    }

	if (SaveEditContent() == TRUE) {
		MessageBox("窗口数据已经成功保存至指定文件!      ", "恭喜", MB_OK + MB_ICONINFORMATION);
	} else {
		MessageBox("文件创建失败!         ", "抱歉", MB_OK + MB_ICONWARNING);
	}
}

/**************************************************************************************************
**  函数名称:  OnButtonSendData
**  功能描述:  发送窗口内输入的数据
**************************************************************************************************/
void CMySScomDlg::OnButtonSendData()
{
    unsigned char buff[MAX_SEND_BYTE];
	
	GetDlgItemText(IDC_EDIT_SENDCSTR, m_Edit_SendCstr);
    
    if (m_Edit_SendCstr.GetLength() <= 0) {
        MessageBox("发送窗口内容为空，未发送任何数据！ ", "提示", MB_OK + MB_ICONINFORMATION);
        return;
    }
	
	strncpy_s((char *)&buff[0], sizeof(buff), (LPCTSTR)m_Edit_SendCstr, m_Edit_SendCstr.GetLength());

	if (SendDatatoComm(buff, m_Edit_SendCstr.GetLength(), m_Check_HexsSend) == FALSE) {
		MessageBox("您输入的数据帧内容过长，或者存在非法字符，请确认！......       ", "提醒", MB_OK + MB_ICONEXCLAMATION);
		return;
	}

	MessageBox("数据发送完成！", "提示", MB_OK + MB_ICONINFORMATION);

}

/**************************************************************************************************
**  函数名称:  SendPacketData
**  功能描述:  分包发送窗口内输入的数据
**************************************************************************************************/
//void CMySScomDlg::SendPacketData()
//{
//	const int PACKET_SIZE = 240; // 每包240字节
//	unsigned char buff[MAX_SEND_BYTE];
//
//	GetDlgItemText(IDC_EDIT_SENDCSTR, m_Edit_SendCstr);
//
//	if (m_Edit_SendCstr.GetLength() <= 0) {
//		MessageBox("发送窗口内容为空，未发送任何数据！ ", "提示", MB_OK + MB_ICONINFORMATION);
//		return;
//	}
//
//	// 将字符串数据复制到缓冲区
//	strncpy_s((char*)&buff[0], sizeof(buff), (LPCTSTR)m_Edit_SendCstr, m_Edit_SendCstr.GetLength());
//
//	// 获取数据总长度
//	int totalLength = m_Edit_SendCstr.GetLength();
//
//	// 计算分包数量
//	int packetCount = (totalLength + PACKET_SIZE - 1) / PACKET_SIZE;
//
//	// 分包发送
//	for (int i = 0; i < packetCount; i++)
//	{
//		// 计算当前包起始位置和长度
//		int offset = i * PACKET_SIZE;
//		int packetLength = min(PACKET_SIZE, totalLength - offset);
//
//		// 添加包头信息（每个字段后加空格）
//		unsigned char packet[PACKET_SIZE + 7]; // 预留包头空间（原4字节+3个空格）
//		packet[0] = i;                // 包序号
//		packet[1] = ' ';              // 空格
//		packet[2] = packetCount;      // 总包数
//		packet[3] = ' ';              // 空格
//		packet[4] = packetLength;     // 当前包长度
//		packet[5] = ' ';              // 空格
//
//		// 拷贝数据
//		memcpy(packet + 6, buff + offset, packetLength);
//
//		// 计算校验和（包含新增的空格）
//		unsigned char checksum = 0;
//		for (int j = 0; j < packetLength + 6; j++) {
//			checksum ^= packet[j];
//		}
//		packet[packetLength + 6] = checksum;
//
//		// 发送数据包
//		if (SendDatatoComm(packet, packetLength + 7, m_Check_HexsSend) == FALSE)
//		{
//			CString errorMsg;
//			errorMsg.Format("发送第%d/%d包失败！", i + 1, packetCount);
//			MessageBox(errorMsg, "错误", MB_OK + MB_ICONERROR);
//			return;
//		}
//
//		// 添加小延迟，防止串口缓冲区溢出
//		Sleep(10);
//	}
//
//	// 发送完成提示
//	CString completeMsg;
//	completeMsg.Format(_T("数据发送完成，共%d包"), packetCount);
//	MessageBox(completeMsg, _T("发送完成"), MB_OK | MB_ICONINFORMATION);
//
//}

//void CMySScomDlg::SendPacketData()

void CMySScomDlg::SendPacketData()
{
	LARGE_INTEGER freq, start, end;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	const int PACKET_SIZE = 200; // 每包200字节
	const int HEADER_SIZE = 7;   // 包头大小
	const int CHECKSUM_SIZE = 1; // 校验和大小
	unsigned char buff[MAX_SEND_BYTE] = { 0 };

	GetDlgItemText(IDC_EDIT_SENDCSTR, m_Edit_SendCstr);

	if (m_Edit_SendCstr.GetLength() <= 0) {
		MessageBox("发送窗口内容为空，未发送任何数据！ ", "提示", MB_OK + MB_ICONINFORMATION);
		return;
	}

	// 将字符串数据复制到缓冲区
	strncpy_s((char*)&buff[0], sizeof(buff), (LPCTSTR)m_Edit_SendCstr, m_Edit_SendCstr.GetLength());

	int totalLength = m_Edit_SendCstr.GetLength();
	uint16_t packetCount = (totalLength + PACKET_SIZE - 1) / PACKET_SIZE;

	
	for (uint16_t i = 0; i < packetCount; i++)
	{
		
		int offset = i * PACKET_SIZE;
		int dataLength = min(PACKET_SIZE, totalLength - offset); // 当前包有效数据长度
		int packetLength = HEADER_SIZE + dataLength + CHECKSUM_SIZE; // 总包长=包头+数据+校验和

		// 创建带包头的缓冲区（初始化为0）
		unsigned char packet[PACKET_SIZE + HEADER_SIZE + CHECKSUM_SIZE] = { 0 };

		// ============= 包头 =============
		packet[0] = 0xEE;                   // 包头标识
		packet[1] = 0xCC;                   // 包头标识
		packet[2] = (packetCount >> 8) & 0xFF;  
		packet[3] = packetCount & 0xFF;			
		packet[4] = (packetCount >> 8) & 0xFF;    // 总包数高字节
		packet[5] = packetCount & 0xFF;			  // 总包数低字节
		packet[6] = static_cast<BYTE>(dataLength);  // 本包数据长度

		// 复制有效数据（放在包头后面）
		memcpy(packet + HEADER_SIZE, buff + offset, dataLength);

		// 计算范围：包头（5字节） + 数据（dataLength字节）
		BYTE checksum = 0;
		for (int j = 0; j < HEADER_SIZE + dataLength; j++) {
			checksum ^= packet[j]; // 使用简单的异或校验
		}
		// 将校验和放在数据之后（即整个包的最后）
		packet[HEADER_SIZE + dataLength] = checksum;

		if (SendDatatoComm(packet, packetLength, m_Check_HexsSend) == FALSE)
		{
			CString errorMsg;
			errorMsg.Format("发送第%d/%hu包失败！", i + 1, packetCount);
			MessageBox(errorMsg, "错误", MB_OK + MB_ICONERROR);
			return;
		}

		Sleep(150);
	}
	// 计算时间间隔
	QueryPerformanceCounter(&end);
	double intervalMs = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
	start = end;

	// 调试输出
	CString debugMsg;
	debugMsg.Format(_T("time=%.3fms\n"), intervalMs);
	OutputDebugString(debugMsg);

	CString completeMsg;
	completeMsg.Format(_T("数据发送完成，共%hu包"), packetCount);
	MessageBox(completeMsg, _T("发送完成"), MB_OK | MB_ICONINFORMATION);

	
}

/**************************************************************************************************
**  函数名称:  OnButtonSuperSend
**  功能描述:  显示高级发送窗口
**************************************************************************************************/
void CMySScomDlg::OnButtonSuperSend() 
{
	if (s_PDlgSuprSend->IsWindowVisible()) {                                   /* 窗口已经打开，尝试关闭 */

		if ((s_PDlgSuprSend->s_issenddata) == TRUE) {
			MessageBox("请先停止自动循环发送功能，再尝试关闭高级发送窗口......   ", "提示", MB_OK + MB_ICONINFORMATION);
			return;
		} else {
			s_PDlgSuprSend->ShowHideDlgWindow(FALSE);
			SetDlgItemText(IDC_BUTTON_SRSEND, "高级发送");
			
			GetDlgItem(IDC_CHECK_HEXSSEND)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_AUTOSEND)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SENDTIME)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_MS)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_OPENFILE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(TRUE);
		}
		
	} else {                                                                   /* 窗口尚未打开，尝试打开 */

		if (m_Check_AutoSend) {
			MessageBox("请先停止单条数据的自动发送功能，再尝试打开高级发送窗口......   ", "提示", MB_OK + MB_ICONINFORMATION);
			return;
		} else {
			s_PDlgSuprSend->ShowHideDlgWindow(TRUE);
			SetDlgItemText(IDC_BUTTON_SRSEND, "正常发送");

			GetDlgItem(IDC_CHECK_HEXSSEND)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_AUTOSEND)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SENDTIME)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_MS)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_OPENFILE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE);
		}
	}
}

/**************************************************************************************************
**  函数名称:  OnButtonExtrafunc
**  功能描述:  显示附加功能窗口
**************************************************************************************************/
void CMySScomDlg::OnButtonExtrafunc() 
{
	if (s_PDlgExtrfunc->IsWindowVisible() == FALSE) {                          /* 窗口尚未打开，尝试打开 */
		s_PDlgExtrfunc->ShowHideDlgWindow(TRUE);
	}
}

/**************************************************************************************************
**  函数名称:  OnButtonOpenFile
**  功能描述:  打开文件
**************************************************************************************************/
void CMySScomDlg::OnButtonOpenFile() 
{
	CFile myFile;
	
	CFileDialog dlg(TRUE, "*.bin", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
					"所有支持的文件(*.bin;*.hex;*.dat)|*.bin;*.hex;*.dat|\
					bin文件(*.bin)|*.bin|\
					hex文件(*.hex)|*.hex|\
					dat文件(*.dat)|*.dat|\
					所有文件(*.*)|*.*||");
	
	if (dlg.DoModal() != IDOK) {                                               /* 未选择任何文件 */
		return;
	}
	
	m_Edit_FilePath = dlg.GetPathName();						               /* 保存文件的路径 */
	
	SetDlgItemText(IDC_EDIT_FILEPATH, m_Edit_FilePath);
}

/**************************************************************************************************
**  函数名称:  OnButtonSendFile
**  功能描述:  发送文件
**************************************************************************************************/
void CMySScomDlg::OnButtonSendFile() 
{
	CFile myFile;
	
	if (s_FileDatPos == 0) {                                                   /* 尚未开始发送，则开始发送 */

		GetDlgItemText(IDC_EDIT_FILEPATH, m_Edit_FilePath);
		
		if (m_Edit_FilePath == "") {
			MessageBox("您尚未指定需要发送的文件的路径！    ", "提示", MB_OK + MB_ICONINFORMATION);
			return;
		}
		
		if (myFile.Open(m_Edit_FilePath, CFile::modeReadWrite | CFile::typeBinary) == 0) {
			MessageBox("读取文件失败，请确认路径正确且文件未处于打开状态！    ", "提示", MB_OK + MB_ICONINFORMATION);
			return;
		} else {
			if (myFile.GetLength() <= 0) {
				MessageBox("文件内容为空，发送终止！    ", "提示", MB_OK + MB_ICONINFORMATION);
				return;
			} else {
				s_FileDatPos = 0;
				SetTimer(Timer_No_SendFile, FILESEND_BYTE, NULL);              /* 开启定时器 */
				SetSendCtrlArea(FALSE);                                        /* 禁用其他发送控件 */
				m_Progs_SendFile.SetPos(0);
			}
		}
	} else {                                                                   /* 正在发送过程中，则停止发送 */
		s_FileDatPos = 0;
		KillTimer(Timer_No_SendFile);                                          /* 关闭定时器 */
		SetSendCtrlArea(TRUE);                                                 /* 恢复其他发送控件 */
	}
}

void CMySScomDlg::OnButtonSendUnPackFile()
{
	CFile myFile;

	if (s_FileDatPos == 0) {                                                   /* 尚未开始发送，则开始发送 */

		GetDlgItemText(IDC_EDIT_FILEPATH, m_Edit_FilePath);

		if (m_Edit_FilePath == "") {
			MessageBox("您尚未指定需要发送的文件的路径！    ", "提示", MB_OK + MB_ICONINFORMATION);
			return;
		}

		if (myFile.Open(m_Edit_FilePath, CFile::modeReadWrite | CFile::typeBinary) == 0) {
			MessageBox("读取文件失败，请确认路径正确且文件未处于打开状态！    ", "提示", MB_OK + MB_ICONINFORMATION);
			return;
		}
		else {
			if (myFile.GetLength() <= 0) {
				MessageBox("文件内容为空，发送终止！    ", "提示", MB_OK + MB_ICONINFORMATION);
				return;
			}
			else {
				s_FileDatPos = 0;
				SetTimer(Timer_No_SendUnPackFile, FILESEND_BYTE, NULL);              /* 开启定时器 */
				SetSendCtrlArea(FALSE);                                        /* 禁用其他发送控件 */
				m_Progs_SendFile.SetPos(0);
			}
		}
	}
	else {                                                                   /* 正在发送过程中，则停止发送 */
		s_FileDatPos = 0;
		KillTimer(Timer_No_SendUnPackFile);                                          /* 关闭定时器 */
		SetSendCtrlArea(TRUE);                                                 /* 恢复其他发送控件 */
	}
}

/**************************************************************************************************
**  函数名称:  OnButtonConfigAutoReply
**  功能描述:  打开自动回复功能的设置窗口
**************************************************************************************************/
void CMySScomDlg::OnButtonConfigAutoReply()
{
	if (s_PDlgAutoRply->IsWindowVisible() == FALSE) {                          /* 窗口尚未打开，尝试打开 */
		s_PDlgAutoRply->ShowHideDlgWindow(TRUE);
	}
}

/**************************************************************************************************
**  函数名称:  OnButtonConfigDsplFilt
**  功能描述:  打开显示过滤法则的设置窗口
**************************************************************************************************/
void CMySScomDlg::OnButtonConfigDsplFilt()
{
	if (s_PDlgDsplFilt->IsWindowVisible() == FALSE) {                          /* 窗口尚未打开，尝试打开 */
		s_PDlgDsplFilt->ShowHideDlgWindow(TRUE);
	}
}

/**************************************************************************************************
**  函数名称:  OnButtonConfigKeywords
**  功能描述:  打开关键字符匹配的设置窗口
**************************************************************************************************/
void CMySScomDlg::OnButtonConfigKeywords()
{
	if (s_PDlgKeyWords->IsWindowVisible() == FALSE) {                          /* 窗口尚未打开，尝试打开 */
		s_PDlgKeyWords->ShowHideDlgWindow(TRUE);
	}
}

/**************************************************************************************************
**  函数名称:  OnCheckHexDisplay
**  功能描述:  显示16进制字符复选框状态切换
**************************************************************************************************/
void CMySScomDlg::OnCheckHexDisplay()
{
    m_Check_HexDispl = !m_Check_HexDispl;

	GetDlgItem(IDC_CHECK_HEXFRAME)->EnableWindow(m_Check_HexDispl);
	GetDlgItem(IDC_CHECK_PROTOACK)->EnableWindow(m_Check_HexDispl);
	GetDlgItem(IDC_BUTTON_SETREPLY)->EnableWindow(m_Check_HexDispl);

    if (m_Check_HexDispl == TRUE) {
		if (IsDlgButtonChecked(IDC_CHECK_PROTOACK) == TRUE) {
			GetDlgItem(IDC_BUTTON_SETREPLY)->EnableWindow(TRUE);
		} else {
			GetDlgItem(IDC_BUTTON_SETREPLY)->EnableWindow(FALSE);
		}
    } else {
		if (s_PDlgAutoRply->IsWindowVisible() == TRUE) {
			s_PDlgAutoRply->ShowHideDlgWindow(FALSE);
		}
    }
}

/**************************************************************************************************
**  函数名称:  OnCheckAutoClear
**  功能描述:  自动清除功能复选框状态切换
**************************************************************************************************/
void CMySScomDlg::OnCheckAutoClear() 
{
    int     TempData;
    CString TempStr;
    
    m_Check_AutoCler = !m_Check_AutoCler;
    
    GetDlgItemText(IDC_EDIT_AUTCLRKB, TempStr);

    TempData = atoi((LPSTR)(LPCTSTR)TempStr);
    
    if (TempData < 100) {        
        MessageBox("您设置的行数值过小，不建议使用。请设置一个大于100的值！     ", "提示", MB_OK + MB_ICONINFORMATION);
        SetDlgItemText(IDC_EDIT_AUTCLRKB, m_Edit_AutClrKB);                    /* 还原编辑框内容 */
        m_Check_AutoCler = FALSE;        
        UpdateData(FALSE);
        return;
    }

    if (m_Check_AutoCler == TRUE) {
        GetDlgItem(IDC_STATIC_OVER)->EnableWindow(FALSE);
        GetDlgItem(IDC_STATIC_LINES)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_AUTCLRKB)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK_AUTOSAVE)->EnableWindow(TRUE);
    } else {
        GetDlgItem(IDC_STATIC_OVER)->EnableWindow(TRUE);
        GetDlgItem(IDC_STATIC_LINES)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_AUTCLRKB)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_AUTOSAVE)->EnableWindow(FALSE);
    }
}

/**************************************************************************************************
**  函数名称:  OnCheckAutoSave
**  功能描述:  自动保存功能复选框状态切换
**************************************************************************************************/
void CMySScomDlg::OnCheckAutoSave()
{
    m_Check_AutoSave = !m_Check_AutoSave;
}

/**************************************************************************************************
**  函数名称:  OnCheckHexSend
**  功能描述:  发送16进制数据复选框状态切换
**************************************************************************************************/
void CMySScomDlg::OnCheckHexSend()
{
    m_Check_HexsSend = !m_Check_HexsSend;
	
	GetDlgItemText(IDC_EDIT_SENDCSTR, m_Edit_SendCstr);                        /* 首先保存当前内容，然后更新显示 */

	if (m_Check_HexsSend == FALSE) {                                           /* 之前是16进制模式，则现在是字符模式 */
		s_SendStr_Hex = m_Edit_SendCstr;
		SetDlgItemText(IDC_EDIT_SENDCSTR, s_SendStr_Chr);
	} else {                                                                   /* 之前是字符模式，则现在是16进制模式 */
		s_SendStr_Chr = m_Edit_SendCstr;
		SetDlgItemText(IDC_EDIT_SENDCSTR, s_SendStr_Hex);
	}
	
	UpdateData(TRUE);
}

/**************************************************************************************************
**  函数名称:  OnCheckAutoSend
**  功能描述:  自动发送功能复选框状态切换
**************************************************************************************************/
void CMySScomDlg::OnCheckAutoSend()
{   
    m_Check_AutoSend = !m_Check_AutoSend;

    GetDlgItemText(IDC_EDIT_SENDCSTR, m_Edit_SendCstr);

    if (m_Edit_SendCstr.GetLength() <= 0) {
        MessageBox("什么数据都不填，是想要发送一串空气么......   ", "提示", MB_OK + MB_ICONINFORMATION);
        m_Check_AutoSend = FALSE;
        UpdateData(FALSE);
        return;
    }

    if (m_Check_AutoSend) {
        if (m_Edit_SendCstr.GetLength() >= MAX_SEND_BYTE) {                    /* 确保输入的数据不会过长 */
            MessageBox("您输入的数据过长，提交发送请求失败......   ", "提示", MB_OK + MB_ICONINFORMATION);
            m_Check_AutoSend = FALSE;
            UpdateData(FALSE);
            return;
        }

        NeedAutoSendData();                                                    /* 开始尝试自动发送数据 */
		GetDlgItem(IDC_BUTTON_SRSEND)->EnableWindow(FALSE);

    } else {

        KillTimer(Timer_No_AutoSend);                                          /* 否则，用户取消了自动发送的功能 */
        GetDlgItem(IDC_EDIT_SENDTIME)->EnableWindow(TRUE);
        GetDlgItem(IDC_STATIC_MS)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);                       /* 重新启用发送按钮 */
		GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SRSEND)->EnableWindow(TRUE);
    }
}

/**************************************************************************************************
**  函数名称:  OnCheckAddReturn
**  功能描述:  发送时补加回车符复选框状态切换
**************************************************************************************************/
void CMySScomDlg::OnCheckAddReturn()
{
    m_Check_CRLFSend = !m_Check_CRLFSend;
}

/**************************************************************************************************
**  函数名称:  OnCheckShowTime
**  功能描述:  行头位置显示时间复选框状态切换
**************************************************************************************************/
void CMySScomDlg::OnCheckShowTime()
{
    m_Check_ShowTime = !m_Check_ShowTime;
}

/**************************************************************************************************
**  函数名称:  OnCheckFrameDspl
**  功能描述:  帧格式显示自动换行功能复选框状态切换
**************************************************************************************************/
void CMySScomDlg::OnCheckFrameDspl() 
{
	m_Check_HexFrame = !m_Check_HexFrame;
}

/**************************************************************************************************
**  函数名称:  OnCheckAutoReply
**  功能描述:  自动回复复选框状态切换
**************************************************************************************************/
void CMySScomDlg::OnCheckAutoReply()
{
	m_Check_ProtoACK = !m_Check_ProtoACK;

	GetDlgItem(IDC_BUTTON_SETREPLY)->EnableWindow(m_Check_ProtoACK);

	if (m_Check_ProtoACK == FALSE) {
		if (s_PDlgAutoRply->IsWindowVisible() == TRUE) {                       /* 窗口尚未关闭 */
			s_PDlgAutoRply->ShowHideDlgWindow(FALSE);
		}
	}
}

/**************************************************************************************************
**  函数名称:  OnCheckDsplFilt
**  功能描述:  显示内容过滤复选框状态切换
**************************************************************************************************/
void CMySScomDlg::OnCheckDsplFilt()
{
	m_Check_AutoFilt = !m_Check_AutoFilt;

	GetDlgItem(IDC_BUTTON_SETFILTER)->EnableWindow(m_Check_AutoFilt);

	if (m_Check_AutoFilt == FALSE) {
		if (s_PDlgDsplFilt->IsWindowVisible() == TRUE) {                       /* 窗口尚未关闭 */
			s_PDlgDsplFilt->ShowHideDlgWindow(FALSE);
		}
	}
}

/**************************************************************************************************
**  函数名称:  OnCheckKeyword
**  功能描述:  关键字监听开关复选框状态切换
**************************************************************************************************/
void CMySScomDlg::OnCheckKeyword()
{
	m_Check_Keywords = !m_Check_Keywords;

	GetDlgItem(IDC_BUTTON_KEYWORD)->EnableWindow(m_Check_Keywords);

	if (m_Check_Keywords == FALSE) {
		if (s_PDlgKeyWords->IsWindowVisible() == TRUE) {                       /* 窗口尚未关闭 */
			s_PDlgKeyWords->ShowHideDlgWindow(FALSE);
		}
	}
}

/**************************************************************************************************
**  函数名称:  OnChangeEditSendcstr
**  功能描述:  发送编辑框内容变化
**************************************************************************************************/
void CMySScomDlg::OnChangeEditSendcstr()
{
	GetDlgItemText(IDC_EDIT_SENDCSTR, m_Edit_SendCstr);
	
	if (m_Check_HexsSend == FALSE) {                                           /* 字符模式下更新s_SendStr_Chr */
		s_SendStr_Chr = m_Edit_SendCstr;
	} else {                                                                   /* 16进制模式更新s_SendStr_Hex */
		s_SendStr_Hex = m_Edit_SendCstr;
	}
}

/**************************************************************************************************
**  函数名称:  OnMenuEditCopy
**  功能描述:  编辑框菜单 - 复制所选内容
**************************************************************************************************/
void CMySScomDlg::OnMenuEditCopy()
{
	CString selstr;
	HGLOBAL clipbuffer;
	char    *buffer;

	selstr = m_RichEdit_Recv.GetSelText();

	if (OpenClipboard() == TRUE) {
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, selstr.GetLength() + 1);
		buffer = (char *)GlobalLock(clipbuffer);
		strcpy_s(buffer, selstr.GetLength() + 1, LPCSTR(selstr));
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, buffer);
		CloseClipboard();
	}
}

/**************************************************************************************************
**  函数名称:  OnMenuEditClear
**  功能描述:  编辑框菜单 - 清除全部内容
**************************************************************************************************/
void CMySScomDlg::OnMenuEditClear()
{
	if (MessageBox("确认要将所接收到的全部内容清空？", "提示", MB_OKCANCEL + MB_ICONQUESTION) != IDOK) {
		return;
	}
	
	OnButtonClearAll();
}

/**************************************************************************************************
**  函数名称:  OnMenuEditFont
**  功能描述:  编辑框菜单 - 显示字体设置
**************************************************************************************************/
void CMySScomDlg::OnMenuEditFont()
{
	CHARFORMAT cf;
	LOGFONT lf;

	memset(&cf, 0, sizeof(CHARFORMAT));
	memset(&lf, 0, sizeof(LOGFONT));

	m_RichEdit_Recv.GetDefaultCharFormat(cf);

	BOOL bIsBold = cf.dwEffects & CFE_BOLD;
	BOOL bIsItalic = cf.dwEffects & CFE_ITALIC;
	BOOL bIsUnderline = cf.dwEffects & CFE_UNDERLINE;
	BOOL bIsStrickout = cf.dwEffects & CFE_STRIKEOUT;

	lf.lfCharSet        = cf.bCharSet;
	lf.lfHeight         = cf.yHeight / 15;
	lf.lfPitchAndFamily = cf.bPitchAndFamily;
	lf.lfItalic         = bIsItalic;
	lf.lfWeight         = (bIsBold ? FW_BOLD : FW_NORMAL);
	lf.lfUnderline      = bIsUnderline;
	lf.lfStrikeOut      = bIsStrickout;
	sprintf_s(lf.lfFaceName, cf.szFaceName);

	CFontDialog dlg(&lf);

	dlg.m_cf.rgbColors = cf.crTextColor;

	if (dlg.DoModal() == IDOK) {
		dlg.GetCharFormat(cf);
		m_RichEdit_Recv.SetDefaultCharFormat(cf);
	}
}

/**************************************************************************************************
**  函数名称:  OnMenuEditAbout
**  功能描述:  编辑框菜单 - 程序介绍说明
**************************************************************************************************/
void CMySScomDlg::OnMenuEditAbout()
{
	ShowAboutMeDialog();
}

/**************************************************************************************************
**  函数名称:  OnMenuEditColorWhite
**  功能描述:  编辑框菜单 - 背景颜色设置 - 雪花白
**************************************************************************************************/
void CMySScomDlg::OnMenuEditColorWhite()
{
	m_RichEdit_Recv.SetBackgroundColor(FALSE, RGB_WHITE);
}

/**************************************************************************************************
**  函数名称:  OnMenuEditColorGray
**  功能描述:  编辑框菜单 - 背景颜色设置 - 科技灰
**************************************************************************************************/
void CMySScomDlg::OnMenuEditColorGray()
{
	m_RichEdit_Recv.SetBackgroundColor(FALSE, RGB_GRAY);
}

/**************************************************************************************************
**  函数名称:  OnMenuEditColorGreen
**  功能描述:  编辑框菜单 - 背景颜色设置 - 薄荷绿
**************************************************************************************************/
void CMySScomDlg::OnMenuEditColorGreen()
{
	m_RichEdit_Recv.SetBackgroundColor(FALSE, RGB_LIGHT_GREEN);
}

/**************************************************************************************************
**  函数名称:  OnMenuEditColorBlue
**  功能描述:  编辑框菜单 - 背景颜色设置 - 碧空蓝
**************************************************************************************************/
void CMySScomDlg::OnMenuEditColorBlue()
{
	m_RichEdit_Recv.SetBackgroundColor(FALSE, RGB_LIGHT_BLUE);
}

/**************************************************************************************************
**  函数名称:  OnMenuEditColorPink
**  功能描述:  编辑框菜单 - 背景颜色设置 - 浪漫粉
**************************************************************************************************/
void CMySScomDlg::OnMenuEditColorPink()
{
	m_RichEdit_Recv.SetBackgroundColor(FALSE, RGB_LIGHT_PINK);
}

/**************************************************************************************************
**  函数名称:  OnMenuEditColorPurple
**  功能描述:  编辑框菜单 - 背景颜色设置 - 轻纱紫
**************************************************************************************************/
void CMySScomDlg::OnMenuEditColorPurple()
{
	m_RichEdit_Recv.SetBackgroundColor(FALSE, RGB_LIGHT_PURPLE);
}

/**************************************************************************************************
**  函数名称:  OnMenuTrayAbout
**  功能描述:  托盘菜单 - 关于程序
**************************************************************************************************/
void CMySScomDlg::OnMenuTrayAbout()
{
	ShowAboutMeDialog();
}

/**************************************************************************************************
**  函数名称:  OnMenuTrayShow
**  功能描述:  托盘菜单 - 显示界面
**************************************************************************************************/
void CMySScomDlg::OnMenuTrayShow()
{
    ShowWindow(SW_SHOW);
}

/**************************************************************************************************
**  函数名称:  OnMenuTrayHide
**  功能描述:  托盘菜单 - 隐藏界面
**************************************************************************************************/
void CMySScomDlg::OnMenuTrayHide()
{
    CloseAllChildWin();                                                        /* 关闭各个子窗体 */
	ShowWindow(SW_HIDE);
}

/**************************************************************************************************
**  函数名称:  OnMenuTrayExit
**  功能描述:  托盘菜单 - 退出程序
**************************************************************************************************/
void CMySScomDlg::OnMenuTrayExit()
{
    CloseAllChildWin();                                                        /* 关闭各个子窗体 */
	
	RecordAllParas();                                                          /* 保存各个参数数据 */

	s_PDlgSuprSend->RecordAllParas();
	s_PDlgExtrfunc->RecordAllParas();
	s_PDlgDsplFilt->RecordAllParas();
	s_PDlgAutoRply->RecordAllParas();
	s_PDlgKeyWords->RecordAllParas();

	CloseSerialDevice();                                                       /* 关闭串口设备 */
    
    TaskBarDeleteIcon(GetSafeHwnd(), IDI_ICON3);                           /* 删除任务栏的图标 */
    
    ::PostQuitMessage(0);                                                      /* 程序退出的唯一方式 */
}


/* ============================================================================================= */
/* ====================================                    ===================================== */
/* ==================================== 传说中华丽的分割线 ===================================== */
/* ====================================                    ===================================== */
/* ============================================================================================= */

/* ==================================== 以下为系统消息处理 ===================================== */


/**************************************************************************************************
**  函数名称:  OnPaint
**  功能描述:  处理窗体重绘消息
**************************************************************************************************/
void CMySScomDlg::OnPaint() 
{
    if (IsIconic()) {

        CPaintDC dc(this);
        
        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
        
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);

        CRect rect;

        GetClientRect(&rect);

        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        
        dc.DrawIcon(x, y, m_hIcon);

    } else {

        CDialog::OnPaint();
    }
}

/**************************************************************************************************
**  函数名称:  OnQueryDragIcon
**  功能描述:  获取窗体图标
**************************************************************************************************/
HCURSOR CMySScomDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

/**************************************************************************************************
**  函数名称:  OnInitDialog
**  功能描述:  窗口初始化
**************************************************************************************************/
BOOL CMySScomDlg::OnInitDialog()
{
    LARGE_INTEGER litmp;
	
	CDialog::OnInitDialog();

	s_RecvPaused = FALSE;
	s_DevNeedUpd = FALSE;
	s_NeedChgLne = TRUE;
	s_RecvString = "";
	s_LopSendCnt = 0;
	s_RecvedByte = 0;
	s_SendedByte = 0;
	s_FileDatPos = 0;
	m_bButtonsVisible = false;
	
	QueryPerformanceCounter(&litmp);
	s_StartdTcik = litmp.QuadPart;                                             /* 获得初始值 */

	CreateDirectory(REC_DIR_PATH, NULL);                                       /* 创建Record文件夹，用于保存数据 */
	CreateSettingFile();                                                       /* 创建程序配置参数文件并初始化各个参数 */

	if (InitiateChildWins() == FALSE) {                                        /* 创建各个子窗体的页面 */
		return FALSE;
	}
	
	GetDlgItem(IDC_PROGRESS_SENDFILE)->ShowWindow(SW_HIDE);                    /* 隐藏进度条，并初始化配置 */
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_HIDE);
	m_Progs_SendFile.SetRange(0, PROGRESS_POS);
	m_Progs_SendFile.SetPos(0);

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

	#if RELEASE_VERSION == 1
	SetWindowText("MySScom V" + GetProgramVersion());                          /* 设置对话框标题为程序版本号 */
	#else
	SetWindowText("MySScom V" + GetProgramVersion() + " <Debug Version> <Built @ " + __DATE__ + " " + __TIME__ + ">");
	#endif

    s_Edit_Recv = (CEdit*)GetDlgItem(IDC_EDIT_RECVCSTR);
    s_Edit_Send = (CEdit*)GetDlgItem(IDC_EDIT_SENDCSTR);

    InitiateStatusBar();                                                       /* 初始化状态栏各个区域位置 */
    InitiateComboComs();                                                       /* 初始化选择串口号的列表框 */
    InitiateComboBaud();                                                       /* 初始化选择波特率的列表框 */
    InitiateComboData();                                                       /* 初始化选择数据位的列表框 */
    InitiateComboCheck();                                                      /* 初始化选择校验位的列表框 */
    InitiateComboStop();                                                       /* 初始化选择停止位的列表框 */
	InitiateAllParas();                                                        /* 初始化主界面下各控件参数 */
	InitiateToolsTip();                                                        /* 初始化控件的悬浮提示信息 */
	InitiateMainStatic();                                                      /* 初始化主界面提示框的显示 */

    SetControlStatus(FALSE);                                                   /* 首先禁用各个按钮控件 */

    SetTimer(Timer_No_UpdateDsp, UPDATEEDIT_TIME, NULL);
    SetTimer(Timer_No_StatusBar, STATUSBAR_TIME, NULL);

    INIT_EASYSIZE;                                                             /* 初始化各个控件的位置 */

	if (CreateDeviceThread() == FALSE) {                                       /* 如果线程创建失败 */
        MessageBox("系统资源异常，串口设备监听线程创建失败！请重启程序！", "提示", MB_OK + MB_ICONERROR);
        return FALSE;
    }

	if (CreateUpdateThread() == FALSE) {                                       /* 如果线程创建失败 */
		MessageBox("系统资源异常，后台升级管理线程创建失败！请重启程序！", "提示", MB_OK + MB_ICONERROR);
		return FALSE;
	}

	s_PDlgSuprSend->InitateSrDlgPos();                                         /* 这句话一定要放在最后面 */
	
    //TaskBarAddIcon(GetSafeHwnd(), IDI_ICON3, AfxGetApp()->LoadIcon(IDI_ICON3), "MySScom");

	CRect rect;                                                                /* 初始化主窗体位置，并限制高度和宽度 */
	GetWindowRect(&rect);
	SetWindowPos(NULL, s_DialogPos_X, s_DialogPos_Y, MIN_WIN_WIDTH, MIN_WIN_HIGHT, SWP_NOSIZE);

    return TRUE;
}

/**************************************************************************************************
**  函数名称:  OnTimer
**  功能描述:  定时器消息处理
**************************************************************************************************/
void CMySScomDlg::OnTimer(UINT nIDEvent) 
{
    unsigned char buff[MAX_SEND_BYTE];
	
	switch (nIDEvent)
    {
        case Timer_No_UpdateDsp:                                              /* 更新编辑框内容显示 */
            if (s_DataRecved == TRUE) {
				UpdateEditStr(s_RecvString);
				s_RecvString = "";
                s_DataRecved = FALSE;
            }
            break;
        
        case Timer_No_StatusBar:                                               /* 状态栏定时更新 */
            UpdateStatusBarNow();
			if (s_DevNeedUpd == TRUE) {                                        /* 顺便更新一下串口设备列表 */
				s_DevNeedUpd = FALSE;
				UpdateComboComs();
			}
            break;
            
        case Timer_No_AutoSend:                                                /* 自动发送数据 */
            GetDlgItemText(IDC_EDIT_SENDCSTR, m_Edit_SendCstr);
			strncpy_s((char *)&buff[0], sizeof(buff), (LPCTSTR)m_Edit_SendCstr, m_Edit_SendCstr.GetLength());
			if (SendDatatoComm(buff, m_Edit_SendCstr.GetLength(), m_Check_HexsSend) == FALSE) {
				OnCheckAutoSend();
				UpdateData(FALSE);
				MessageBox("您输入的数据帧内容过长，或者存在非法字符，请确认！......       ", "提醒", MB_OK + MB_ICONEXCLAMATION);
			}
            break;
            
		case Timer_No_FrameDspl:                                               /* 16进制下按帧换行显示判定 */
			KillTimer(Timer_No_FrameDspl);
			UpdateEditStr("\r\n");                                             /* 立即换行显示 */
			s_NeedChgLne = TRUE;                                               /* 标志下次需要换行显示 */
			break;
			
		case Timer_No_SendFile:                                                /* 发送文件数据 */
			if (SendFileDatatoComm() == FALSE) {                               /* 本次发送数据失败 */
				s_FileDatPos = 0;
				KillTimer(Timer_No_SendFile);                                  /* 停止发送 */
				SetSendCtrlArea(TRUE);                                         /* 恢复其他发送控件 */
			}
			UpdateStatusBarNow();
			break;
			
		case Timer_No_SendUnPackFile:                                                /* 发送文件数据 */
			if (SendFileDataPackstoComm() == FALSE) {                               /* 本次发送数据失败 */
				s_FileDatPos = 0;
				KillTimer(Timer_No_SendUnPackFile);                                  /* 停止发送 */
				SetSendCtrlArea(TRUE);                                         /* 恢复其他发送控件 */
			}
			UpdateStatusBarNow();
			break;

        default:
            return;
    }
        
    CDialog::OnTimer(nIDEvent);
}

/**************************************************************************************************
**  函数名称:  PreTranslateMessage
**  功能描述:  系统消息预处理函数
**************************************************************************************************/
BOOL CMySScomDlg::PreTranslateMessage(MSG* pMsg) 
{
    unsigned char keyvalue;
    
    m_tooltip.RelayEvent(pMsg);

    if (pMsg->message == WM_KEYDOWN) {

		if ((pMsg->wParam >= VK_F1) && (pMsg->wParam <= VK_F12)) {             /* 此处处理用户自定义的Fn功能键 */
			UserFnKeyHdl(pMsg->wParam);
			return true;
		}

		if (SerialDevisOpened() == TRUE) {                                     /* 此处将用户按键键值发送到串口 */
			if ((GetFocus() == GetDlgItem(IDC_EDIT_INPUT)) || GetFocus() == GetDlgItem(IDC_EDIT_RECVCSTR)) {
				keyvalue = GetKeyValue(pMsg->wParam);                          /* 只提取那些有意义的按键键值 */
				if (keyvalue > 0) {
					s_SendedByte += SendSerialData(&keyvalue, 1);              /* 将按键键值通过串口发送出去 */
				}
			}
		}

        if ((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN)) {      /* 返回键和确认键需要进行预处理 */
            return true;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

/**************************************************************************************************
**  函数名称:  OnClose
**  功能描述:  处理窗口关闭消息
**************************************************************************************************/
void CMySScomDlg::OnClose() 
{
    if ((m_Check_AutoSend == TRUE) || (s_PDlgSuprSend->s_issenddata == TRUE)) {
		MessageBox("请首先停用自动发送功能再尝试关闭本窗口...  ", "提示", MB_OK + MB_ICONINFORMATION);
		return;
    }
	
	if (MessageBox(_T("确定要关闭窗口吗？"), _T("确认"), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		OnMenuTrayExit();
	}
	
	
	//ShowWindow(SW_HIDE);                                                       /* 隐藏主窗口但是不退出 */
}

/**************************************************************************************************
**  函数名称:  OnSize
**  功能描述:  处理窗口大小缩放消息 （变动后，此时窗体大小已经确定）
**************************************************************************************************/
void CMySScomDlg::OnSize(UINT nType, int cx, int cy) 
{
    CDialog::OnSize(nType, cx, cy);

    UPDATE_EASYSIZE;

    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);      /* 同步状态栏的位置 */
}

/**************************************************************************************************
**  函数名称:  OnSizing
**  功能描述:  处理窗口大小缩放消息 （改变中，此时窗体大小尚未定型）
**************************************************************************************************/
void CMySScomDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
    CDialog::OnSizing(fwSide, pRect);

	UpdateStatusBarNow();

    EASYSIZE_MINSIZE(MIN_WIN_WIDTH, MIN_WIN_HIGHT, fwSide, pRect);             /* 限制窗体的最小尺寸 */
}

/**************************************************************************************************
**  函数名称:  OnContextMenu
**  功能描述:  处理上下文菜单消息
**************************************************************************************************/
void CMySScomDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu popMenu, *pPopup;
	
	if (pWnd->m_hWnd == m_RichEdit_Recv.m_hWnd) {                              /* 如果在编辑框内右击 */

		popMenu.LoadMenu(IDR_MENU_EDIT);                                       /* 载入菜单 */
		pPopup = popMenu.GetSubMenu(0);                                        /* 获得菜单指针 */

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		pPopup->Detach();
		popMenu.DestroyMenu();
	}
}

/**************************************************************************************************
**  函数名称:  WinHelp
**  功能描述:  重载系统帮助函数 - 用于屏蔽系统帮助功能
**************************************************************************************************/
void CMySScomDlg::WinHelp(DWORD dwData, UINT nCmd)
{
	return;
}

/**************************************************************************************************
**  函数名称:  OnSysCommand
**  功能描述:  处理系统消息
**************************************************************************************************/
void CMySScomDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE) {                                                  /* 如果是要窗体最小化 */
		CloseAllChildWin();                                                    /* 关闭所有子窗口 */
	}

	UpdateStatusBarNow();

	CDialog::OnSysCommand(nID, lParam);
}



void CMySScomDlg::OnStnClickedStaticMs()
{
	// TODO: 在此添加控件通知处理程序代码
}




void CMySScomDlg::OnNMCustomdrawProgressSendfile(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	if (pNMCD->dwDrawStage == CDDS_PREPAINT) {
		*pResult = CDRF_NOTIFYITEMDRAW; // 请求子项绘制通知
	}
	else if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT) {
		// 修改进度条颜色（示例：蓝色进度）
		if (pNMCD->dwItemSpec == PP_CHUNK) { // 进度部分
			HBRUSH hBrush = CreateSolidBrush(RGB(0, 120, 215));
			FillRect(pNMCD->hdc, &pNMCD->rc, hBrush);
			DeleteObject(hBrush);
			*pResult = CDRF_SKIPDEFAULT;
		}
		// 修改背景色（示例：浅灰色背景）
		else if (pNMCD->dwItemSpec == PP_BAR) {
			HBRUSH hBrush = CreateSolidBrush(RGB(240, 240, 240));
			FillRect(pNMCD->hdc, &pNMCD->rc, hBrush);
			DeleteObject(hBrush);
			*pResult = CDRF_SKIPDEFAULT;
		}
	}
	else {
		*pResult = 0;
	}
}





void CMySScomDlg::StrongTest()
{
	const int PACKET_SIZE = 200; // 每包200字节

	LARGE_INTEGER freq, start, end;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
	// 直接按包生成和发送数据，避免拼接大字符串
	for (int packetIndex = 0; packetIndex < 234000; ++packetIndex)
	{

		Sleep(150);
		
		// 每包填充200个相同的数字字符（0-9）
		char digit = ('0' + packetIndex)%10; // 当前数字对应的ASCII字符
		unsigned char packet[PACKET_SIZE] = { 0 };

		memset(packet, digit, PACKET_SIZE);

		// 发送数据包
		if (!SendDatatoComm(packet, PACKET_SIZE, m_Check_HexsSend))
		{
			CString errorMsg;
			errorMsg.Format(_T("发送第%d包失败！"), packetIndex + 1);
			MessageBox(errorMsg, _T("错误"), MB_OK | MB_ICONERROR);
			return;
		}

	}
	// 计算时间间隔
	QueryPerformanceCounter(&end);
	double intervalMs = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
	start = end;

	// 调试输出
	CString debugMsg;
	debugMsg.Format(_T("%.3fms\n"), intervalMs);
	OutputDebugString(debugMsg);
}

void CMySScomDlg::OnEnChangeEditRecvcstr()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CMySScomDlg::OnBnClickedStaticReceive()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CMySScomDlg::OnBnClickedButton4()
{
	// 切换状态
	m_bButtonsVisible = !m_bButtonsVisible;

	// 根据新状态设置显示/隐藏
	int nCmd = m_bButtonsVisible ? SW_SHOW : SW_HIDE;

	GetDlgItem(IDC_BUTTON1)->ShowWindow(nCmd);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(nCmd);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(nCmd);
	GetDlgItem(IDC_BUTTON5)->ShowWindow(nCmd);
}

void CMySScomDlg::StopSending()
{
	// TODO: 在此添加控件通知处理程序代码
	CFile myFile;

	if (s_FileDatPos == 0) {                                                   /* 尚未开始发送，则开始发送 */

		GetDlgItemText(IDC_EDIT_FILEPATH, m_Edit_FilePath);

		if (m_Edit_FilePath == "") {
			MessageBox("您尚未指定需要发送的文件的路径！    ", "提示", MB_OK + MB_ICONINFORMATION);
			return;
		}

		if (myFile.Open(m_Edit_FilePath, CFile::modeReadWrite | CFile::typeBinary) == 0) {
			MessageBox("读取文件失败，请确认路径正确且文件未处于打开状态！    ", "提示", MB_OK + MB_ICONINFORMATION);
			return;
		}
		else {
			if (myFile.GetLength() <= 0) {
				MessageBox("文件内容为空，发送终止！    ", "提示", MB_OK + MB_ICONINFORMATION);
				return;
			}
			else {
				s_FileDatPos = 0;
				SetTimer(Timer_No_SendUnPackFile, FILESEND_BYTE, NULL);              /* 开启定时器 */
				SetSendCtrlArea(FALSE);                                        /* 禁用其他发送控件 */
				m_Progs_SendFile.SetPos(0);
			}
		}
	}
	else {                                                                   /* 正在发送过程中，则停止发送 */
		s_FileDatPos = 0;
		KillTimer(Timer_No_SendUnPackFile);                                          /* 关闭定时器 */
		SetSendCtrlArea(TRUE);                                                 /* 恢复其他发送控件 */
	}
}

/*=======================================================================
模块名      : CNS控制台
文件名      : CncMsgDriver.cpp
相关文件    : 
文件实现功能: 消息驱动器
作者        : 
版本        : V3.0  Copyright(C) 1997-2003 KDC, All rights reserved.
-------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2011/4/30   1.0         肖楚然        创建
=======================================================================*/
#include "StdAfx.h"
#include "cncmsgdriver.h"
#include "cncmsgrecver.h"
#include "cnclib.h"

////////////////////////////////////////////////////////////////////////////
// CMsgDriver

CCncMsgDriver* CCncMsgDriver::s_pMsgDriver = NULL;

CCncMsgDriver::CCncMsgDriver()
{
    // 构造事件名称表
    MappingEventName();
    // 创建消息接收窗口
    m_hWndOsp = ::CreateWindowEx(0, _T("Static"), _T("CncMsgDriver"), WS_DISABLED,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, 0, 0);

	if ( m_hWndOsp != NULL )
	{
		m_wndprocOld = reinterpret_cast<WNDPROC>(GetWindowLong(m_hWndOsp, GWL_WNDPROC));
		SetWindowLong(m_hWndOsp, GWL_WNDPROC, reinterpret_cast<s32>(WndProc));
	} 
	else
	{
		m_wndprocOld = NULL;
	}
    m_nTimer = 0;
    m_pCurDispEvent = NULL;
    m_wCurSendEvent = 0;
    s_pMsgDriver = this;
    m_byTransferStatus = FREE;
    m_pbyFile = NULL;
    m_dwFileLength = 0;         // 文件长度
    m_dwFileFragIndex = 0;      // 文件块序号
    m_dwFileCurPos = 0;
	m_wInsID = 0;
    m_wAppID = 0;
    m_dwDesIID = MAKEIID(AID_CN_UE, 1);
}

CCncMsgDriver::~CCncMsgDriver()
{
    if (m_pbyFile != NULL)
    {
        free(m_pbyFile);
        m_pbyFile = NULL;
    }
    ::KillTimer(m_hWndOsp, m_nTimer);
    // 关闭消息接收窗口

	::DestroyWindow( m_hWndOsp );
	m_hWndOsp = NULL;
	s_pMsgDriver = NULL;
}


/*=======================================================================
函 数 名:   GetMtMsgPtr
功    能:   得到发送消息指针(发送TKdvMsg)
参    数:   BOOL bClear = TRUE         [in]返回消息指针时是否清空消息体
返 回 值:   发送消息的消息指针
相关消息:
-------------------------------------------------------------------------
修改纪录:
日      期  版本  修改人  修改内容
2004/09/30  4.0   liuruifei  创建
=======================================================================*/
CTpMsg* CCncMsgDriver::GetKdvMsgPtr(BOOL32 bClear)
{
    if (bClear) 
    {
        ZeroMemory(&m_cTpMsg, sizeof(CTpMsg));
    }

    return &m_cTpMsg;
}
/*=======================================================================
函 数 名:   GetCMsgPtr
功    能:   得到发送消息指针(发送TMtMsg)
参    数:   BOOL bClear = TRUE         [in]返回消息指针时是否清空消息体
返 回 值:   发送消息的消息指针
相关消息:
-------------------------------------------------------------------------
修改纪录:
日      期  版本  修改人  修改内容
2004/09/30  4.0   liuruifei  修改
=======================================================================*/
CMessage* CCncMsgDriver::GetCMsgPtr(BOOL32 bClear /* = TRUE */)
{
	if (bClear)
	{
		ZeroMemory(&m_cMsg, sizeof(CMessage));
	}

	return &m_cMsg;
}

/*=======================================================================
函 数 名:   PostMsg
功    能:   发送命令型消息，不需要终端有回应。调用接口前要要通过GetPostMsgPtr
            来设置消息体
参    数:   u32 dwType = TYPE_CMESSAGE   [in]消息类型TYPE_CMESSAGE或TYPE_MTMSG
返 回 值:
相关消息:
-------------------------------------------------------------------------
修改纪录:
日      期  版本  修改人  修改内容
2004/08/25  3.5   李洪强  创建
=======================================================================*/
u16 CCncMsgDriver::PostMsg(u32 dwType)
{  
    u32 dwDesIID = GetDesIID();//MAKEIID(AID_CN_UE, 1/*m_wInsID*/);	 // 目的：CNS
	u32 dwSrcIID = MAKEIID(GetAppID());	         // 源  ：CNS控制台
	
	u16 wRet = NO_ERROR;
	u16 wEvent = 0;
	if (TYPE_TPMSG == dwType) 
	{  
		//发送CMtMsg消息体
		{
			wRet = m_cTpMsg.Post(dwDesIID, g_CncApp.GetNodeId(), dwSrcIID);
		}
		wEvent = m_cTpMsg.GetEvent();
    }
	else
	{
		//发送Osp CMessage消息
		if(m_cMsg.dstid != INVALID_NODE)
		{
			dwDesIID = m_cMsg.dstid;
		}
		if(m_cMsg.srcid != INVALID_NODE)
		{
			dwSrcIID = m_cMsg.srcid;
		}

		wRet = OspPost(dwDesIID, m_cMsg.event, m_cMsg.content, m_cMsg.length,
        g_CncApp.GetNodeId(), dwSrcIID);

		wEvent = m_cMsg.event;
	}
	//打印输出   
	if(wRet == NO_ERROR)
	{  
		if( IS_PRINT_OUT_MSG )
		{
			CString strEvent = CPrintCtrl::GetPrintCtrl()->GetEventDescribe(wEvent);
	        PrtMsg( "[CncLib]: 发送消息:%s(%u).\n", strEvent,wEvent);
		}
	}
	else
	{   
		CString strEvent = CPrintCtrl::GetPrintCtrl()->GetEventDescribe(wEvent);
	    PrtMsg( "[CncLib]: 发送消息:%s(%u)失败,错误码：%d\n", strEvent, wEvent, wRet ); 
	}
	
    return wRet;
}



/*=======================================================================
函 数 名:   PostCommand
功    能:   发送请求型消息，一般会需要终端有回应，如无回应则报超时。调用接口前
要通过GetPostMsgPtr来设置消息体
参    数:   CDispEvent *pDispEvent     [in]响应消息的处理指针
u16 *pwWaitEvent           [in]期待响应的消息数组
s32 nEventNum = 1          [in]期待响应的消息数量
u32 dwTimeOut = 1000       [in]期待响应的消息超时时间
返 回 值:
相关消息:
-------------------------------------------------------------------------
修改纪录:
日      期  版本  修改人  修改内容
2004/08/25  3.5   李洪强  创建
=======================================================================*/
u16 CCncMsgDriver::PostCommand(CKdvDispEvent *pDispEvent, u16 *pwWaitEvent,
                            s32 nEventNum, u32 dwType, u32 dwTimeOut)
{
    // 检测是否有前一个命令正在执行中
    if(IsHandleCmd())
    {
        m_cCommandQueue.PushCommand(m_cMsg, m_cTpMsg, pDispEvent, pwWaitEvent,
                                    nEventNum, dwType, dwTimeOut);

        return ERR_CNC_WAIT_FOR_SEND_CMD;
    }
    // 保存当前命令参数
    if(TYPE_CMESSAGE == dwType)
    {
        m_wCurSendEvent = m_cMsg.event;
    }
    else
    {
        m_wCurSendEvent = m_cTpMsg.GetEvent();
    }
    m_pCurDispEvent = pDispEvent;
    
    for(s32 nIndex = 0; nIndex < nEventNum; nIndex++)
    {
        m_vWaitingEvents.push_back(pwWaitEvent[nIndex]);
    }
    // 超时计时器
    m_nTimer = SetTimer(m_hWndOsp, 1, dwTimeOut, NULL);
    // 发送
    u16 wRet = PostMsg(dwType);
    if(wRet != NO_ERROR)
    {
        // 发送失败
        ::KillTimer(m_hWndOsp, m_nTimer);
        ClearWaiting();
        PostNextCommand();
    }
    return wRet;
}

/*=======================================================================
函 数 名:   PostCommand
功    能:   发送请求型消息，一般会需要终端有回应，如无回应则报超时。调用接口前
            要要通过GetPostMsgPtr来设置消息体（单个等待消息）
参    数:   CDispEvent *pDispEvent     [in]响应消息的处理指针
            u16 wWaitEvent             [in]期待响应的消息
            u32 dwTimeOut = 1000       [in]期待响应的消息超时时间
返 回 值:
相关消息:
-------------------------------------------------------------------------
修改纪录:
日      期  版本  修改人  修改内容
2004/08/25  3.5   李洪强  创建
2004/10/28  3.5   王慧琛   修改
=======================================================================*/
u16 CCncMsgDriver::PostCommand(CKdvDispEvent *pDispEvent, u16 wWaitEvent, u32 dwType, u32 dwTimeOut)
{
    return PostCommand(pDispEvent, &wWaitEvent, 1, dwType, dwTimeOut);
}


/*=======================================================================
函 数 名:   RegHandler
功    能:   注册一个处理消息的类的指针
参    数:   CDispEvent* const pDispEvnet       [in]处理消息的类指针
返 回 值:
相关消息:
-------------------------------------------------------------------------
修改纪录:
日      期  版本  修改人  修改内容
2004/08/23  3.5   李洪强  创建
=======================================================================*/
void CCncMsgDriver::RegHandler(CKdvDispEvent* const pDispEvnet)
{
    m_vpDispEvent.push_back(pDispEvnet);
}

/*=======================================================================
函 数 名:   RegHandler
功    能:   注销一个处理消息的类的指针
参    数:   CDispEvent* const pDispEvnet       [in]处理消息的类指针
返 回 值:
相关消息:
-------------------------------------------------------------------------
修改纪录:
日      期  版本  修改人  修改内容
2004/08/23  3.5   李洪强  创建
=======================================================================*/
void CCncMsgDriver::UnregHandler(CKdvDispEvent* const pDispEvnet)
{
    vector<CKdvDispEvent*>::iterator itr = m_vpDispEvent.begin();
    for(; itr != m_vpDispEvent.end(); itr++)
    {
        if (*itr == pDispEvnet) 
        {
            m_vpDispEvent.erase(itr);
            break;
        }
    }
}

/*=======================================================================
函 数 名:   ClearHandler
功    能:   清空所有注册的类的指针
参    数:
返 回 值:
相关消息:
-------------------------------------------------------------------------
修改纪录:
日      期  版本  修改人  修改内容
2004/08/23  3.5   李洪强  创建
=======================================================================*/
void CCncMsgDriver::ClearHandler()
{
    m_vpDispEvent.clear();
}

void CCncMsgDriver::ClearAllCommand()
{
    m_cCommandQueue.ClearCommand();
}

/*=============================================================================
  函 数 名： OnOspMessageComing
  功    能： 从OSP回调线程提取消息，打印消息分发消息
  算法实现： 
  全局变量： 
  参    数： WPARAM wParam  0
             LPARAM lParam  0
  返 回 值： void 
  -----------------------------------------------------------------------------
  修改记录：
  日  期		版本		修改人		走读人    修改内容
  2004/10/13     3.5		王慧琛                  规范化
=============================================================================*/
void CCncMsgDriver::OnOspMessageComing(WPARAM wParam, LPARAM lParam)
{
    // 从OSP回调线程提取消息
    CCncMsgHandler *pHandler = CCncMsgHandler::GetMsgHandler();
    if (!pHandler) 
    {
        return;
    }
    CCncMsgQueue cMsgQue;
    pHandler->PopAllMsg(cMsgQue);
    // 分发消息
    while(!cMsgQue.IsEmpty())
    {
        const CMessage &cMsg = cMsgQue.Front();

		//打印输出
		CString strEvent = CPrintCtrl::GetPrintCtrl()->GetEventDescribe(cMsg.event);
		if( IS_PRINT_OUT_MSG )
		{   	 
			PrtMsg( "[CncLib]: 收到消息:%s(%u).\n", strEvent,cMsg.event);
		}

        // 检索命令应答消息
        if(IsHandleCmd())
        {	
            vector<u16>::iterator itr = m_vWaitingEvents.begin();
            for(; itr != m_vWaitingEvents.end(); itr++)
            {
                if(*itr == cMsg.event)
                {
                    // 2004/08/24 李洪强 得到回应，清除定时器
                    ::KillTimer(m_hWndOsp, m_nTimer);
                    ClearWaiting();
                    PostNextCommand();
                    break;
                }
            }
        }
        // 检索消息分发表
        vector<CKdvDispEvent*>::iterator itr = m_vpDispEvent.begin();
        BOOL bFind = FALSE;
        for(; itr != m_vpDispEvent.end(); itr++)
        {
            if ((*itr)->HasRegEvent(cMsg.event)) 
            {
                (*itr)->DispEvent(cMsg);
                bFind = TRUE;
            }
        }
        if (!bFind) 
        {	  	 
			PrtMsg( "[CncLib]: 消息:%s(%d)被抛弃,因为没有找到匹配的消息处理器.\n", strEvent, cMsg.event);		
        }

        cMsgQue.Pop();
    }
}

/*=============================================================================
  函 数 名： OnOspSyncMsgComing
  功    能： 同步消息处理
  算法实现： 
  全局变量： 
  参    数： WPARAM wParam 消息指针
             LPARAM lParam 0
  返 回 值： void 
  -----------------------------------------------------------------------------
  修改记录：
  日  期		版本		修改人		走读人    修改内容
  2004/11/30     3.5		    王慧琛                   创建
=============================================================================*/
void CCncMsgDriver::OnOspSyncMsgComing(WPARAM wParam, LPARAM lParam)
{    
    CMessage *pMsg = (CMessage *)wParam;
   
	//打印输出
	CString strEvent = CPrintCtrl::GetPrintCtrl()->GetEventDescribe(pMsg->event);
	if( IS_PRINT_OUT_MSG )
	{   	 
		PrtMsg( "[CncLib]: 收到消息:%s(%u).\n", strEvent, pMsg->event);
	}

    // 检索消息分发表
    vector<CKdvDispEvent*>::iterator itr = m_vpDispEvent.begin();
    BOOL bFind = FALSE;
    for(; itr != m_vpDispEvent.end(); itr++)
    {
        if ((*itr)->HasRegEvent(pMsg->event)) 
        {
            (*itr)->DispEvent(*pMsg);
            bFind = TRUE;
        }
    }
    if (!bFind) 
    {
        // 没有注册处理函数的消息
//         map<u16, CString>::iterator itrName = m_mapEventsName.find(pMsg->event);
//         if(itrName != m_mapEventsName.end())
//         {
//             OspLog(1, "[CncLib]: 消息:%s(%d)被抛弃,因为没有找到匹配的消息处理器.\n",
//                 (LPCTSTR)((*itrName).second), pMsg->event);
//         }
//         else
//         {
//             OspLog(1, "[CncLib]: 消息:%d被抛弃,因为没有找到匹配的消息处理器.\n",
//                 pMsg->event);
//         }
		PrtMsg( "[CncLib]: 消息:%s(%d)被抛弃,因为没有找到匹配的消息处理器.\n", strEvent, pMsg->event);
    }

}

/*=============================================================================
  函 数 名： OnTimer
  功    能： 超时计时器消息，等待的消息超时回应，清空等待消息数组
  算法实现： 
  全局变量： 
  参    数： WPARAM wParam
             LPARAM lParam
  返 回 值： void 
  -----------------------------------------------------------------------------
  修改记录：
  日  期		版本		修改人		走读人    修改内容
  2004/10/13     3.5		王慧琛                  规范化
=============================================================================*/
void CCncMsgDriver::OnTimer(WPARAM wParam, LPARAM lParam)
{
    if (wParam == m_nTimer)
    {
        // 超时计时器消息
        ::KillTimer(m_hWndOsp, m_nTimer);
        if(IsHandleCmd())
        {
            if (m_pCurDispEvent) 
            {
                m_pCurDispEvent->OnTimeOut(m_wCurSendEvent);
            }
            ClearWaiting();
            PostNextCommand();
        }
    }
	//发送文件超时,复位,即重头开始重新发送
    else if (wParam == m_nFileTransferTimer)
    {
        if (m_pbyFile == NULL)
        {
            EndTransFile();
            ::KillTimer(m_hWndOsp, m_nFileTransferTimer);
            return;
        }
        if (m_dwFileCurPos < m_dwFileLength)
        //未传送完文件，继续传送，每包30K字节
        {
            u32 dwLeft = m_dwFileLength - m_dwFileCurPos;
            if (dwLeft > 1024*30)
            {
                dwLeft = 1024*30;
            }
            PostFile(&m_pbyFile[m_dwFileCurPos], dwLeft, m_dwFileFragIndex);
            m_dwFileCurPos = m_dwFileCurPos + 1024 * 30;
            m_dwFileFragIndex++;
        }
        //传送完文件，发送停止传送请求
        else
        {
            if (m_pbyFile != NULL)
            {
                free(m_pbyFile);
                m_pbyFile = NULL;
            }
            m_dwFileCurPos = 0;
            m_dwFileFragIndex = 0;
            EndTransFile();
            ::KillTimer(m_hWndOsp, m_nFileTransferTimer);
        }
    }
    
}

/*=============================================================================
  函 数 名： WndProc
  功    能： 自定义的窗口处理函数，来区分windows消息和OSP消息以及超时消息
  算法实现： 
  全局变量： 
  参    数： HWND hWnd
             UINT uMsg
             WPARAM wParam
             LPARAM lParam
  返 回 值： LRESULT CALLBACK 
  -----------------------------------------------------------------------------
  修改记录：
  日  期		版本		修改人		走读人    修改内容
  2004/10/13     3.5		王慧琛                  规范化
=============================================================================*/
LRESULT CALLBACK CCncMsgDriver::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // 拦截OSP消息和超时计时器消息
    if (CCncMsgDriver::s_pMsgDriver) 
    {
        if (uMsg == CCncMsgHandler::WM_OSPMESSAGECOMING) 
        {
            CCncMsgDriver::s_pMsgDriver->OnOspMessageComing(wParam, lParam);
        }
        else if (uMsg == CCncMsgHandler::WM_OSPSYNCMSGCOMING)
        {
            CCncMsgDriver::s_pMsgDriver->OnOspSyncMsgComing(wParam, lParam);            
        }
        else if (uMsg == WM_TIMER) 
        {
            CCncMsgDriver::s_pMsgDriver->OnTimer(wParam, lParam);
        }
        else if (uMsg == CCncMsgHandler::WM_REQMTCSTATUS)
        {
            CCncMsgDriver::s_pMsgDriver->Dump();
        }
		else if (uMsg == CCncMsgHandler::WM_REQIKEY)
		{
			CCncMsgDriver::s_pMsgDriver->ReqIKey(wParam);
		}
        else
        {
			if ( CCncMsgDriver::s_pMsgDriver->m_wndprocOld != NULL )
			{
				return CallWindowProc(CCncMsgDriver::s_pMsgDriver->m_wndprocOld,
                                hWnd, uMsg, wParam, lParam);
			}
            
        }
    }
    return 0;
}

/*=============================================================================
  函 数 名： ClearWaiting
  功    能： 清空等待消息数组，以便调用IsHandleCmd时为FALSE，使消息得以继续发送
  算法实现： 
  全局变量： 
  参    数： 
  返 回 值： void 
  -----------------------------------------------------------------------------
  修改记录：
  日  期		版本		修改人		走读人    修改内容
  2004/10/13     3.5		王慧琛                  规范化
=============================================================================*/
void CCncMsgDriver::ClearWaiting()
{
    m_vWaitingEvents.clear();
    m_pCurDispEvent = NULL;
    m_wCurSendEvent = 0;
}

void CCncMsgDriver::PostNextCommand()
{
    ClearWaiting();

    // 没有待发送消息时直接返回
    if(TRUE == m_cCommandQueue.IsEmpty())
    {
        return;
    }

    // 取出下一条待发送的命令
    TCOMMAND tCommand;
    BOOL32 bSucess = m_cCommandQueue.GetNextCommand(tCommand);
    if(FALSE == bSucess)
    {
        return;
    }
    
    u16* pwWaitEvent = new u16[tCommand.m_nEventNum];

    ASSERT(tCommand.m_nEventNum == tCommand.m_vctWaitEvent.size());
    
    for(u32 nIndex = 0; nIndex < tCommand.m_nEventNum; nIndex++)
    {
        pwWaitEvent[nIndex] = tCommand.m_vctWaitEvent[nIndex];
    }

    if(TYPE_CMESSAGE == tCommand.m_dwType)
    {
        m_cMsg = tCommand.m_cMsg;
    }
    else
    {
        m_cTpMsg = tCommand.m_cMtMsg;
    }

    // 将该条命令发送出去
    PostCommand(tCommand.m_pCDispEvent, pwWaitEvent, tCommand.m_nEventNum,
                tCommand.m_dwType, tCommand.m_dwTimeout);

    delete []pwWaitEvent;
}

/*=============================================================================
  函 数 名： CanBeTraceOut
  功    能： 信息输出裁判，判断改事件是否需要输出打印
  算法实现： 
  全局变量： 
  参    数： u16 wEvent  需要判断的事件ID
  返 回 值： BOOL        TRUE需要输出打印
  -----------------------------------------------------------------------------
  修改记录：
  日  期		版本		修改人		走读人    修改内容
  2004/10/13     3.5		王慧琛                  规范化
=============================================================================*/
BOOL CCncMsgDriver::CanBeTraceOut(u16 wEvent) const
{
//    static u32 dwSN = 0;
//    static u32 dwCatalog[MTC_MSGCATALOG_COUNT];
//   //yjj CMtcLib::GetTraceStatus(dwSN, dwCatalog);
//    
//    map<u16, u32>::const_iterator itr = m_mapEventType.find(wEvent);
//    if(itr != m_mapEventType.end())
//    {
//        return dwCatalog[itr->second];
//    }
//    else
//    {
//        return dwCatalog[0];
//    }

	return TRUE;
}

/*=============================================================================
  函 数 名： TransferFile
  功    能： 开启发送定时器
  算法实现： 
  全局变量： 
  参    数： 
  返 回 值： u16 
  -----------------------------------------------------------------------------
  修改记录：
  日  期		版本		修改人		走读人    修改内容
  2004/12/27     3.5		    王慧琛                   创建
=============================================================================*/
u16 CCncMsgDriver::TransferFile()
{
    if (m_pbyFile == NULL) return 1;
    //先发送第一包
    m_dwFileCurPos = 0;
    m_dwFileFragIndex = 0;
	u16 wResult = 0;
    if (m_dwFileCurPos < m_dwFileLength)
    {
        u32 dwLeft = m_dwFileLength - m_dwFileCurPos;
        if (dwLeft > m_wMaxPackageLen)
        {
            dwLeft = m_wMaxPackageLen;
        }
		TRACE("\n开始传输文件");
        wResult = PostFile(&m_pbyFile[m_dwFileCurPos], dwLeft, m_dwFileFragIndex);
        m_dwFileCurPos = m_dwFileCurPos + m_wMaxPackageLen;
        m_dwFileFragIndex++;
    }

	return wResult;
}

void CCncMsgDriver::StopTransferFile()
{
    m_byTransferStatus = FREE;
	if (m_pbyFile != NULL)
    {
        free(m_pbyFile);
        m_pbyFile = NULL;
    }
    
}

/*函数名： PostFile
功    能： 发送文件段
参    数： u8 *pbyBuf      文件段指针
           u32 dwBufLenth  文件段长度
           u32 nIndex      文件段序号
返 回 值： u16 
说    明： */
u16 CCncMsgDriver::PostFile(u8 *pbyBuf, u32 dwBufLenth, u32 nIndex)
{
/*    //包头必须附带：传输协议版本号，文件标识符，文件类型，文件分片号，包长度。
	//包体：文件数据
    ZeroMemory(&m_cTpMsg, sizeof(CTpMsg));
    //传输文件
    TFileTransfer tMsgHead;
    memset( &tMsgHead, 0, sizeof (tMsgHead) );
	tMsgHead.wProtocolIdentifer = MTC_MT_FILE_VER;
	tMsgHead.wFileSliceNo = MTC_MT_MSG_BGN + nIndex;
	tMsgHead.byType = m_wFileType;
	tMsgHead.wDataLen = dwBufLenth + sizeof(TFileTransfer);

	//设置消息体
//	m_cKdvMsg.SetEvent(ev_FTSendFileCmd);
	m_cTpMsg.SetBody(&tMsgHead, sizeof( TFileTransfer ));
	m_cTpMsg.CatBody( pbyBuf, dwBufLenth );
 
//	OspPrintf(TRUE, FALSE, "传送的数据长度%d\n", dwBufLenth);
	TRACE("传送的数据长度%d\n", dwBufLenth);
*/
    u16 wResult = PostMsg(TYPE_TPMSG);
    
    return wResult;
}

/*=============================================================================
  函 数 名： MappingHelper
  功    能： 映射事件名称，同时注册到事件类别数组里面
  算法实现： 
  全局变量： 
  参    数： u16 wEvent                 事件ID
             const CString& strName     事件名称
             CATALOG catalog            类别
  返 回 值： void 
  -----------------------------------------------------------------------------
  修改记录：
  日  期		版本		修改人		走读人    修改内容
  2004/10/13     3.5		王慧琛                  规范化
=============================================================================*/
void CCncMsgDriver::MappingHelper(u16 wEvent, const CString& strName, CATALOG catalog)
{
    m_mapEventsName[wEvent] = strName;
    m_mapEventType[wEvent] = catalog;
}

/*=============================================================================
  函 数 名： MappingEventName
  功    能： 注册事件
  算法实现： 
  全局变量： 
  参    数： 
  返 回 值： void 
  -----------------------------------------------------------------------------
  修改记录：
  日  期		版本		修改人		走读人    修改内容
 2011/05/10     1.0		    肖楚然                  创建
=============================================================================*/
void CCncMsgDriver::MappingEventName()
{
    //登陆
	MAPPING_HELPER( OSP_DISCONNECT, _T("OSP_DISCONNECT") );
	MAPPING_HELPER( ev_CnLogin_Req, _T("ev_CnLogin_Req") );
	MAPPING_HELPER( ev_CnLogin_Rsp, _T("ev_CnLogin_Rsp") );
	MAPPING_HELPER( ev_CnList_Nty, _T("ev_CnList_Nty") );
	MAPPING_HELPER( ev_CnLoginIns_Req, _T("ev_CnLoginIns_Req") );
	MAPPING_HELPER( ev_CnLoginIns_Rsp, _T("ev_CnLoginIns_Rsp") );
	MAPPING_HELPER( ev_CNSLoginByOther_Notify, _T("ev_CNSLoginByOther_Notify") );
		
	//配置
	MAPPING_HELPER( ev_tppRegisterRs_Req, _T("ev_tppRegisterRs_Req") );
	MAPPING_HELPER( ev_tppRegisterRs_Rsp, _T("ev_tppRegisterRs_Rsp") );
	MAPPING_HELPER( ev_tpRegisterResult_Nty, _T("ev_tpRegisterResult_Nty") );
	MAPPING_HELPER( ev_tpRegisterResultFail_Nty, _T("ev_tpRegisterResultFail_Nty") );	
	MAPPING_HELPER( ev_CnCfgCnsInfo_Cmd, _T("ev_CnCfgCnsInfo_Cmd") );
	MAPPING_HELPER( ev_CnCfgCnsInfo_Ind, _T("ev_CnCfgCnsInfo_Ind") );
	MAPPING_HELPER( ev_CnCfgEthnet_Cmd, _T("ev_CnCfgEthnet_Cmd") );
	MAPPING_HELPER( ev_CnCfgEthnet_Ind, _T("ev_CnCfgEthnet_Ind") );
	MAPPING_HELPER( ev_CnSetMainRoom_Cmd, _T("ev_CnSetMainRoom_Cmd") );
	MAPPING_HELPER( ev_CnSetMainRoom_Ind, _T("ev_CnSetMainRoom_Ind") );

    //IPv6
    MAPPING_HELPER( ev_CnCfgEthnetIPV6_Cmd, _T("ev_CnCfgEthnetIPV6_Cmd") );
    MAPPING_HELPER( ev_CnCfgEthnetIPV6_Ind, _T("ev_CnCfgEthnetIPV6_Ind") );
    MAPPING_HELPER( ev_CnCfgIPVtpye_Cmd, _T("ev_CnCfgIPVtpye_Cmd") );
    MAPPING_HELPER( ev_CnCfgIPVtpye_Ind, _T("ev_CnCfgIPVtpye_Ind") );

	//NAT配置
	MAPPING_HELPER( ev_tppNatReg_Nty, _T("ev_tppNatReg_Nty"));
	MAPPING_HELPER( ev_tppNatReg_Cmd, _T("ev_tppNatReg_Cmd"));
	//画面裁剪配置（无缝屏与有缝屏兼容显示）
	MAPPING_HELPER( ev_TppSetVidCutLine_Cmd, _T("ev_TppSetVidCutLine_Cmd"));
	MAPPING_HELPER( ev_TppSetVidCutLine_Ind, _T("ev_TppSetVidCutLine_Ind"));
	//注册信息
	MAPPING_HELPER( ev_tppGetRegInfo_Req, _T("ev_tppGetRegInfo_Req") );
	MAPPING_HELPER( ev_tppGetRegInfo_Rsp, _T("ev_tppGetRegInfo_Rsp") );
	MAPPING_HELPER( ev_tppGetRegInfo_Nty, _T("ev_tppGetRegInfo_Nty") );
    MAPPING_HELPER( ev_tppGetRegGKInfo_Nty, _T("ev_tppGetRegGKInfo_Nty") );
	MAPPING_HELPER( ev_tppGetRegInfoCNS_Rsp, _T("ev_tppGetRegInfoCNS_Rsp") );
	MAPPING_HELPER( ev_tppGetRegInfoUMS_Rsp, _T("ev_tppGetRegInfoUMS_Rsp") );
	//白板
	MAPPING_HELPER( ev_tppSetDataServerInfo_Nty, _T("ev_tppSetDataServerInfo_Nty") );
	MAPPING_HELPER( ev_tppSetDataServerInfo_Ind, _T("ev_tppSetDataServerInfo_Ind") );
	MAPPING_HELPER( ev_tppSetDataServerInfo_Cmd, _T("ev_tppSetDataServerInfo_Cmd") );

	MAPPING_HELPER( ev_CnsGKReg_Cmd, _T("ev_CnsGKReg_Cmd") );
	MAPPING_HELPER( ev_CnsGKReg_Nty, _T("ev_CnsGKReg_Nty") );
	MAPPING_HELPER( ev_CnsGKReg_Ret, _T("ev_CnsGKReg_Ret") );
		
	//会控
	MAPPING_HELPER( ev_CnConfState_Nty, _T("ev_CnConfState_Nty") );

    MAPPING_HELPER( ev_TppHangupConf_Cmd, _T("ev_TppHangupConf_Cmd") );

    MAPPING_HELPER( ev_TppConfApplySpeaker_Cmd, _T("ev_TppConfApplySpeaker_Cmd") );

    MAPPING_HELPER( ev_TppConfApplySpeaker_Nty, _T("ev_TppConfApplySpeaker_Nty") );

    MAPPING_HELPER( ev_TppChairConfInfo_Nty, _T("ev_TppChairConfInfo_Nty") );

    MAPPING_HELPER( ev_TppAudMixStatus_Nty, _T("ev_TppAudMixStatus_Nty") );

    MAPPING_HELPER( ev_TppHangupCns_Ind, _T("ev_TppHangupCns_Ind") );
    
    MAPPING_HELPER( ev_TppHangupCns_Cmd, _T("ev_TppHangupCns_Cmd") );

    MAPPING_HELPER( ev_TppInviteCnsByEpId_Ind, _T("ev_TppInviteCnsByEpId_Ind") );

    MAPPING_HELPER( ev_TppInviteCnsByEpId_Cmd, _T("ev_TppInviteCnsByEpId_Cmd") );

    MAPPING_HELPER( ev_TppInviteCnsByAlias_Ind, _T("ev_TppInviteCnsByAlias_Ind") );

    MAPPING_HELPER( ev_TppInviteCnsByAlias_Cmd, _T("ev_TppInviteCnsByAlias_Cmd") );

    MAPPING_HELPER( ev_TppConfQuiet_Nty, _T("ev_TppConfQuiet_Nty") );

    MAPPING_HELPER( ev_TppConfQuiet_Ind, _T("ev_TppConfQuiet_Ind") );

    MAPPING_HELPER( ev_TppConfQuiet_Cmd, _T("ev_TppConfQuiet_Cmd") );

    MAPPING_HELPER( ev_TppConfMute_Nty, _T("ev_TppConfMute_Nty") );

    MAPPING_HELPER( ev_TppConfMute_Ind, _T("ev_TppConfMute_Ind") );

    MAPPING_HELPER( ev_TppConfMute_Cmd, _T("ev_TppConfMute_Cmd") );
    
    MAPPING_HELPER( ev_TppConfApplySpeaker_Ind, _T("ev_TppConfApplySpeaker_Ind") );
    
	MAPPING_HELPER( ev_TppModuleterminalLeftConf, _T("ev_TppModuleterminalLeftConf") );
	MAPPING_HELPER( ev_TppModuleTerminalJoinConf, _T("ev_TppModuleTerminalJoinConf") );

    MAPPING_HELPER( ev_TppConfStopDual_Cmd, _T("ev_TppConfStopDual_Cmd") );

    MAPPING_HELPER( ev_TppConfStartDual_Cmd, _T("ev_TppConfStartDual_Cmd") );

    MAPPING_HELPER( ev_TppConfStartDual_Ind, _T("ev_TppConfStartDual_Ind") );

    MAPPING_HELPER( ev_TppConfStopDual_Ind, _T("ev_TppConfStopDual_Ind") );

    MAPPING_HELPER( ev_TppConfDualEp_Nty, _T("ev_TppConfDualEp_Nty") );
 
    MAPPING_HELPER( ev_TppUmsCommonReasonToUI_Ind, _T("ev_TppUmsCommonReasonToUI_Ind") );  

	MAPPING_HELPER( ev_tppInputVolumeCmd , _T("ev_tppInputVolumeCmd ") );
	MAPPING_HELPER( ev_tppOutputVolumeCmd , _T("ev_tppOutputVolumeCmd ") );
	MAPPING_HELPER( ev_tppInputVolumeInd, _T("ev_tppInputVolumeInd") );
	MAPPING_HELPER( ev_tppOutputVolumeInd, _T("ev_tppOutputVolumeInd") );
	MAPPING_HELPER( ev_tppMute_Cmd , _T("ev_tppMute_Cmd ") );
	MAPPING_HELPER( ev_tppQuiet_Cmd , _T("ev_tppQuiet_Cmd ") );
	MAPPING_HELPER( ev_tppQuietInd, _T("ev_tppQuietInd") );
	MAPPING_HELPER( ev_tppMuteInd, _T("ev_tppMuteInd") );
	MAPPING_HELPER( ev_CnSetInnerSpeaker_Cmd, _T("ev_CnSetInnerSpeaker_Cmd") );
	MAPPING_HELPER( ev_CnSetInnerSpeaker_Ind, _T("ev_CnSetInnerSpeaker_Ind") );
	MAPPING_HELPER( ev_CnYouAreSeeing_Ind, _T("ev_CnYouAreSeeing_Ind"), 3 );
	MAPPING_HELPER( ev_tpRoomOnline_Nty, _T("ev_tpRoomOnline_Nty") );
	MAPPING_HELPER( ev_tpRoomOffline_Nty, _T("ev_tpRoomOffline_Nty") );
	
	MAPPING_HELPER( ev_CnStartPoll_Cmd, _T("ev_CnStartPoll_Cmd") );
	MAPPING_HELPER( ev_CnStartPoll_Ind, _T("ev_CnStartPoll_Ind") );	
	MAPPING_HELPER( ev_CnStopPoll_Cmd, _T("ev_CnStopPoll_Cmd") );
	MAPPING_HELPER( ev_CnStopPoll_Ind, _T("ev_CnStopPoll_Ind") ); 
	MAPPING_HELPER( ev_CnPollStatus_Nty, _T("ev_CnPollStatus_Nty") ); 
	MAPPING_HELPER( ev_CnSingleView_Cmd, _T("ev_CnSingleView_Cmd") );
	MAPPING_HELPER( ev_CnSingleView_Ind, _T("ev_CnSingleView_Ind") );
	MAPPING_HELPER( ev_CnCancelSingleView_Cmd, _T("ev_CnCancelSingleView_Cmd") );
	MAPPING_HELPER( ev_CnCancelSingleView_Ind, _T("ev_CnCancelSingleView_Ind") );
	MAPPING_HELPER( ev_TppStartDual_Cmd, _T("ev_TppStartDual_Cmd") );
	MAPPING_HELPER( ev_TppStartDual_Ind, _T("ev_TppStartDual_Ind") );
	MAPPING_HELPER( ev_TppStopDual_Cmd, _T("ev_TppStopDual_Cmd") );
	MAPPING_HELPER( ev_TppStopDual_Ind, _T("ev_TppStopDual_Ind") );
	MAPPING_HELPER( ev_TppIsDual_Nty, _T("ev_TppIsDual_Nty") );	
    MAPPING_HELPER( ev_TppIsRcvDual_Nty, _T("ev_TppIsRcvDual_Nty") );
	MAPPING_HELPER( ev_CnActivePos_Nty, _T("ev_CnActivePos_Nty") );
	MAPPING_HELPER( ev_tpCnRoomDualOffline_Nty, _T("ev_tpCnRoomDualOffline_Nty") );

	MAPPING_HELPER( ev_TppMakeCall_Cmd, _T("ev_TppMakeCall_Cmd") );
	MAPPING_HELPER( ev_TppHangUp_Cmd, _T("ev_TppHangUp_Cmd") );
	MAPPING_HELPER( ev_TppHangUp_Ind, _T("ev_TppHangUp_Ind") );
	MAPPING_HELPER( ev_CnConfEpList_Nty, _T("ev_CnConfEpList_Nty") );

    MAPPING_HELPER( ev_CnGetTempPwd_Cmd, _T("ev_CnGetTempPwd_Cmd") );
    MAPPING_HELPER( ev_CnGetTempPwd_Ind, _T("ev_CnGetTempPwd_Ind") );

    MAPPING_HELPER( ev_CnPosAllStatus_Nty, _T("ev_CnPosAllStatus_Nty"), 3 );

    MAPPING_HELPER( ev_TppDualRcvInterrupt_Nty, _T("ev_TppDualRcvInterrupt_Nty") );

    MAPPING_HELPER( ev_CnSetAudioInfo_Ind, _T("ev_CnSetAudioInfo_Ind") );

	//双流
	MAPPING_HELPER( ev_TppAddDualRcvAddr_Cmd, _T("ev_TppAddDualRcvAddr_Cmd") );
    MAPPING_HELPER( ev_TppDelDualRcvAddr_Cmd, _T("ev_TppDelDualRcvAddr_Cmd") );
    MAPPING_HELPER( ev_TppPadDualKeyFrame_Req, _T("ev_TppPadDualKeyFrame_Req") );
    MAPPING_HELPER( ev_TppAddDualRcvAddr_Ind, _T("ev_TppAddDualRcvAddr_Ind") );
    MAPPING_HELPER( ev_TppDelDualRcvAddr_Ind, _T("ev_TppDelDualRcvAddr_Ind") );
	
    MAPPING_HELPER( ev_TppSelDualView_Cmd, _T("ev_TppSelDualView_Cmd") );
    MAPPING_HELPER( ev_TppSelDualView_Ind, _T("ev_TppSelDualView_Ind") );
	MAPPING_HELPER( ev_TppCancelSelDualView_Cmd, _T("ev_TppCancelSelDualView_Cmd") );
    MAPPING_HELPER( ev_TppCancelSelDualView_Ind, _T("ev_TppCancelSelDualView_Ind") );
    	
    MAPPING_HELPER( ev_CnViewLocal_Cmd, _T("ev_CnViewLocal_Cmd") );
    MAPPING_HELPER( ev_CnViewLocal_Ind, _T("ev_CnViewLocal_Ind") );
	
    MAPPING_HELPER( ev_CnCancelViewLocal_Cmd, _T("ev_CnCancelViewLocal_Cmd") );
    MAPPING_HELPER( ev_CnCancelViewLocal_Ind, _T("ev_CnCancelViewLocal_Ind") );

    //双流源设置	
    MAPPING_HELPER( ev_TppSetDualInputPortName_Cmd, _T("ev_TppSetDualInputPortName_Cmd") );
    MAPPING_HELPER( ev_TppSetDualInputPortName_Ind, _T("ev_TppSetDualInputPortName_Ind") );
    MAPPING_HELPER( ev_TppSelDualInputPort_Cmd, _T("ev_TppSelDualInputPort_Cmd") );
    MAPPING_HELPER( ev_TppSelDualInputPort_Ind, _T("ev_TppSelDualInputPort_Ind") );
    MAPPING_HELPER( ev_TppCurrentVgaInfo_Nty, _T("ev_TppCurrentVgaInfo_Nty") );
    MAPPING_HELPER( ev_TppConfVgaInfo_Nty, _T("ev_TppConfVgaInfo_Nty") );

	//空闲状态双流设置
	MAPPING_HELPER( ev_tppSetShowDual_Cmd, _T("ev_tppSetShowDual_Cmd") );
	MAPPING_HELPER( ev_tppSetShowDual_Ind, _T("ev_tppSetShowDual_Ind") );
	MAPPING_HELPER( ev_tppSetShowDual_Nty, _T("ev_tppSetShowDual_Nty") );

	//系统待机设置
	MAPPING_HELPER( ev_Cn_SetSleepInfoCmd, _T("ev_Cn_SetSleepInfoCmd") );
	MAPPING_HELPER( ev_Cn_SetSleepInfoInd, _T("ev_Cn_SetSleepInfoInd") );
	MAPPING_HELPER( ev_TppSetDualInputPortName_Cmd, _T("ev_TppSetDualInputPortName_Cmd") );

	//混音
	MAPPING_HELPER( ev_TppConfAMixUpdate_Cmd, _T("ev_TppConfAMixUpdate_Cmd") );
	MAPPING_HELPER( ev_TppConfAMixUpdate_Ind, _T("ev_TppConfAMixUpdate_Ind") );	
	MAPPING_HELPER( ev_TppConfAMixUpdate_Nty, _T("ev_TppConfAMixUpdate_Nty") );
	MAPPING_HELPER( ev_TppAudMixDisListOpr_Cmd, _T("ev_TppAudMixDisListOpr_Cmd") );
    MAPPING_HELPER( ev_TppAudMixDisListOpr_Ind, _T("ev_TppAudMixDisListOpr_Ind") );
	//坐席激励
	MAPPING_HELPER( ev_Cn_ConfAMixVacSwitch_Ind, _T("ev_Cn_ConfAMixVacSwitch_Ind") );
	MAPPING_HELPER( ev_Cn_ConfAMixVacSwitch_Cmd, _T("ev_Cn_ConfAMixVacSwitch_Cmd") );
	//单独控制坐席激励
	MAPPING_HELPER( ev_CnMixIsUsed_Cmd, _T("ev_CnMixIsUsed_Cmd") );
	MAPPING_HELPER( ev_CnMixIsUsed_Ind, _T("ev_CnMixIsUsed_Ind") );
    //多点语音激励
    MAPPING_HELPER( ev_tppVoiceMotivation_Nty, _T("ev_tppVoiceMotivation_Nty") );
    MAPPING_HELPER( ev_tppVoiceMotivation_Cmd, _T("ev_tppVoiceMotivation_Cmd") );
    MAPPING_HELPER( ev_tppVoiceMotivation_Ind, _T("ev_tppVoiceMotivation_Ind") );
    //设置系统时间
    MAPPING_HELPER( ev_CnSetSysTime_Cmd, _T("ev_CnSetSysTime_Cmd") );
    MAPPING_HELPER( ev_CnGetSysTime_Req, _T("ev_CnGetSysTime_Req") );
    MAPPING_HELPER( ev_CnGetSysTime_Rsp, _T("ev_CnGetSysTime_Rsp") );  
	//电视墙
	MAPPING_HELPER( ev_TpUMS_CNS_HduPlan_Nty, _T("ev_TpUMS_CNS_HduPlan_Nty") );  
	MAPPING_HELPER( ev_TpHduChangeChanMode_Req, _T("ev_TpHduChangeChanMode_Req") ); 
	MAPPING_HELPER( ev_TpHduChangeChanMode_Ind, _T("ev_TpHduChangeChanMode_Ind") );  
	MAPPING_HELPER( ev_TpCNS_UMS_HduSetVol_Req, _T("ev_TpCNS_UMS_HduSetVol_Req") );  
	MAPPING_HELPER( ev_TpUMS_CNS_HduSetVol_Ind, _T("ev_TpUMS_CNS_HduSetVol_Ind") );  
	MAPPING_HELPER( ev_TpCNS_UMS_HduStopPlay_Req, _T("ev_TpCNS_UMS_HduStopPlay_Req") ); 
	MAPPING_HELPER( ev_TpUMS_CNS_HduStopPlay_Ind, _T("ev_TpUMS_CNS_HduStopPlay_Ind") );  
	MAPPING_HELPER( ev_TpCNS_UMS_HduPlay_Req, _T("ev_TpCNS_UMS_HduPlay_Req") );  
	MAPPING_HELPER( ev_TpUMS_CNS_HduPlay_Ind, _T("ev_TpUMS_CNS_HduPlay_Ind") );  
	
	//地址簿
	MAPPING_HELPER( ADDRBOOK_REGISTER_REQ, _T("ADDRBOOK_REGISTER_REQ") );
    MAPPING_HELPER( ADDRBOOK_REGISTER_ACK, _T("ADDRBOOK_REGISTER_ACK") );
    MAPPING_HELPER( ADDRBOOK_REGISTER_NACK, _T("ADDRBOOK_REGISTER_NACK"));
    MAPPING_HELPER( ADDRBOOK_UNREG_REQ, _T("ADDRBOOK_UNREG_REQ") );
	MAPPING_HELPER( ADDRBOOK_UNREG_CMD, _T("ADDRBOOK_UNREG_CMD") );
    MAPPING_HELPER( ADDRBOOK_SAVE_ACK, _T("ADDRBOOK_SAVE_ACK") );
    MAPPING_HELPER( ADDRBOOK_SAVE_NACK, _T("ADDRBOOK_SAVE_NACK") );
    MAPPING_HELPER( ADDRBOOK_ADD_ENTRY_REQ, _T("ADDRBOOK_ADD_ENTRY_REQ") );
    MAPPING_HELPER( ADDRBOOK_ADD_ENTRY_ACK, _T("ADDRBOOK_ADD_ENTRY_ACK") );
    MAPPING_HELPER( ADDRBOOK_ADD_ENTRY_NACK, _T("ADDRBOOK_ADD_ENTRY_NACK") );
    MAPPING_HELPER( ADDRBOOK_ADD_MULSETENTRY_REQ, _T("ADDRBOOK_ADD_MULSETENTRY_REQ") );
    MAPPING_HELPER( ADDRBOOK_ADD_MULSETENTRY_ACK, _T("ADDRBOOK_ADD_MULSETENTRY_ACK") );
    MAPPING_HELPER( ADDRBOOK_ADD_MULSETENTRY_NACK, _T("ADDRBOOK_ADD_MULSETENTRY_NACK") );
    MAPPING_HELPER( ADDRBOOK_DEL_ENTRY_REQ, _T("ADDRBOOK_DEL_ENTRY_REQ") );
    MAPPING_HELPER( ADDRBOOK_DEL_ENTRY_ACK, _T("ADDRBOOK_DEL_ENTRY_ACK") );
    MAPPING_HELPER( ADDRBOOK_DEL_ENTRY_NACK, _T("ADDRBOOK_DEL_ENTRY_NACK") );
    MAPPING_HELPER( ADDRBOOK_DEL_MULSETENTRY_REQ, _T("ADDRBOOK_DEL_MULSETENTRY_REQ") );
    MAPPING_HELPER( ADDRBOOK_DEL_MULSETENTRY_ACK, _T("ADDRBOOK_DEL_MULSETENTRY_ACK") );
    MAPPING_HELPER( ADDRBOOK_DEL_MULSETENTRY_NACK, _T("ADDRBOOK_DEL_MULSETENTRY_NACK") );
    MAPPING_HELPER( ADDRBOOK_ADD_ENTRYTOMULSET_REQ, _T("ADDRBOOK_ADD_ENTRYTOMULSET_REQ") );
    MAPPING_HELPER( ADDRBOOK_ADD_ENTRYTOMULSET_ACK, _T("ADDRBOOK_ADD_ENTRYTOMULSET_ACK") );
    MAPPING_HELPER( ADDRBOOK_ADD_ENTRYTOMULSET_NACK, _T("ADDRBOOK_ADD_ENTRYTOMULSET_NACK") );
    MAPPING_HELPER( ADDRBOOK_DEL_ENTRYFRMULSET_REQ, _T("ADDRBOOK_DEL_ENTRYFRMULSET_REQ") );
    MAPPING_HELPER( ADDRBOOK_DEL_ENTRYFRMULSET_ACK, _T("ADDRBOOK_DEL_ENTRYFRMULSET_ACK") );
    MAPPING_HELPER( ADDRBOOK_DEL_ENTRYFRMULSET_NACK, _T("ADDRBOOK_DEL_ENTRYFRMULSET_NACK") );
    MAPPING_HELPER( ADDRBOOK_MODIFYENTRY_REQ, _T("ADDRBOOK_MODIFYENTRY_REQ") );
    MAPPING_HELPER( ADDRBOOK_MODIFYENTRY_ACK, _T("ADDRBOOK_MODIFYENTRY_ACK") );
    MAPPING_HELPER( ADDRBOOK_MODIFYENTRY_NACK, _T("ADDRBOOK_MODIFYENTRY_NACK") );
    MAPPING_HELPER( ADDRBOOK_MODIFYMULSETENTRY_REQ, _T("ADDRBOOK_MODIFYMULSETENTRY_REQ") );
    MAPPING_HELPER( ADDRBOOK_MODIFYMULSETENTRY_ACK, _T("ADDRBOOK_MODIFYMULSETENTRY_ACK") );
    MAPPING_HELPER( ADDRBOOK_MODIFYMULSETENTRY_NACK, _T("ADDRBOOK_MODIFYMULSETENTRY_NACK") );
	MAPPING_HELPER( ADDRBOOK_ADD_MULSETTOMULSET_REQ, _T("ADDRBOOK_ADD_MULSETTOMULSET_REQ") );
	MAPPING_HELPER( ADDRBOOK_ADD_MULSETTOMULSET_ACK, _T("ADDRBOOK_ADD_MULSETTOMULSET_ACK") );
	MAPPING_HELPER( ADDRBOOK_ADD_MULSETTOMULSET_NACK, _T("ADDRBOOK_ADD_MULSETTOMULSET_NACK") );
	MAPPING_HELPER( ADDRBOOK_DEL_MULSETFROMMULSET_REQ, _T("ADDRBOOK_DEL_MULSETFROMMULSET_REQ") );
	MAPPING_HELPER( ADDRBOOK_DEL_MULSETFROMMULSET_ACK, _T("ADDRBOOK_DEL_MULSETFROMMULSET_ACK") );
	MAPPING_HELPER( ADDRBOOK_DEL_MULSETFROMMULSET_NACK, _T("ADDRBOOK_DEL_MULSETFROMMULSET_NACK") );
	
    MAPPING_HELPER( ADDRBOOK_GETENTRYNUM_REQ, _T("ADDRBOOK_GETENTRYNUM_REQ") );
    MAPPING_HELPER( ADDRBOOK_GETENTRYNUM_ACK, _T("ADDRBOOK_GETENTRYNUM_ACK") );
    MAPPING_HELPER( ADDRBOOK_GETENTRYNUM_NACK, _T("ADDRBOOK_GETENTRYNUM_NACK") );
    MAPPING_HELPER( ADDRBOOK_GETENTRY_REQ, _T("ADDRBOOK_GETENTRY_REQ"), 3 );
    MAPPING_HELPER( ADDRBOOK_GETENTRY_ACK, _T("ADDRBOOK_GETENTRY_ACK"), 3 );
    MAPPING_HELPER( ADDRBOOK_GETENTRY_NACK, _T("ADDRBOOK_GETENTRY_NACK") );
    MAPPING_HELPER( ADDRBOOK_GETMULSETENTRYNUM_REQ, _T("ADDRBOOK_GETMULSETENTRYNUM_REQ") );
    MAPPING_HELPER( ADDRBOOK_GETMULSETENTRYNUM_ACK, _T("ADDRBOOK_GETMULSETENTRYNUM_ACK") );
    MAPPING_HELPER( ADDRBOOK_GETMULSETENTRYNUM_NACK, _T("ADDRBOOK_GETMULSETENTRYNUM_NACK") );
    MAPPING_HELPER( ADDRBOOK_GETMULSETENTRY_REQ, _T("ADDRBOOK_GETMULSETENTRY_REQ") );
    MAPPING_HELPER( ADDRBOOK_GETMULSETENTRY_ACK, _T("ADDRBOOK_GETMULSETENTRY_ACK") );
    MAPPING_HELPER( ADDRBOOK_GETMULSETENTRY_NACK, _T("ADDRBOOK_GETMULSETENTRY_NACK") );
	MAPPING_HELPER( ADDRBOOK_GETMULSETRELATIONNUM_REQ, _T("ADDRBOOK_GETMULSETRELATIONNUM_REQ") );
	MAPPING_HELPER( ADDRBOOK_GETMULSETRELATIONNUM_ACK, _T("ADDRBOOK_GETMULSETRELATIONNUM_ACK") );
	MAPPING_HELPER( ADDRBOOK_GETMULSETRELATIONNUM_NACK, _T("ADDRBOOK_GETMULSETRELATIONNUM_NACK") );
	MAPPING_HELPER( ADDRBOOK_GETMULSETRELATION_REQ, _T("ADDRBOOK_GETMULSETRELATION_REQ") );
	MAPPING_HELPER( ADDRBOOK_GETMULSETRELATION_ACK, _T("ADDRBOOK_GETMULSETRELATION_ACK") );
	MAPPING_HELPER( ADDRBOOK_GETMULSETRELATION_NACK, _T("ADDRBOOK_GETMULSETRELATION_NACK") );
    MAPPING_HELPER( ADDRBOOK_REFRESHENTRY_NOTIF, _T("ADDRBOOK_REFRESHENTRY_NOTIF") );
    MAPPING_HELPER( ADDRBOOK_DELENTRY_NOTIF, _T("ADDRBOOK_DELENTRY_NOTIF") );
    MAPPING_HELPER( ADDRBOOK_REFRESHMULSETENTRY_NOTIF, _T("ADDRBOOK_REFRESHMULSETENTRY_NOTIF") );
    MAPPING_HELPER( ADDRBOOK_DELMULSETENTRY_NOTIF, _T("ADDRBOOK_DELMULSETENTRY_NOTIF") );
	MAPPING_HELPER( ADDRBOOK_REFRESHMULSETRELATION_NOTIF, _T("ADDRBOOK_REFRESHMULSETRELATION_NOTIF") );	
    MAPPING_HELPER( ADDRBOOK_UPDATEINMT_CMD, _T("ADDRBOOK_UPDATEINMT_CMD") );
    MAPPING_HELPER( ADDRBOOK_UPDATEOUTMT_CMD, _T("ADDRBOOK_UPDATEOUTMT_CMD") );
	MAPPING_HELPER( ev_cns_cncAddrBookNty, _T("ev_cns_cncAddrBookNty") );

  
	//用户管理
	MAPPING_HELPER( ev_CNSAddUserReq, _T("ev_CNSAddUserReq") );
	MAPPING_HELPER( ev_CNSAddUserRet, _T("ev_CNSAddUserRet") );
	MAPPING_HELPER( ev_CNSDelUserReq, _T("ev_CNSDelUserReq") );
	MAPPING_HELPER( ev_CNSDelUserRet, _T("ev_CNSDelUserRet") );
	MAPPING_HELPER( ev_CNSChangePWDReq, _T("ev_CNSChangePWDReq") );
	MAPPING_HELPER( ev_CNSChangePWDRet, _T("ev_CNSChangePWDRet") );
	MAPPING_HELPER( ev_CNSAddUser_Notify, _T("ev_CNSAddUser_Notify") );
	MAPPING_HELPER( ev_CNSDelUser_Notify, _T("ev_CNSDelUser_Notify") );
	MAPPING_HELPER( ev_CNSUpdateUser_Notify, _T("ev_CNSUpdateUser_Notify") );

	//中控
    MAPPING_HELPER( ev_Cn_Sch_Power_Cmd, _T("ev_Cn_Sch_Power_Cmd") );
    MAPPING_HELPER( ev_Cn_Sch_Power_Ind, _T("ev_Cn_Sch_Power_Ind") );
    MAPPING_HELPER( ev_Cn_Sch_Power_FB_Notify, _T("ev_Cn_Sch_Power_FB_Notify") );
    MAPPING_HELPER( ev_Cn_Sch_Light_Cmd, _T("ev_Cn_Sch_Light_Cmd") );
    MAPPING_HELPER( ev_Cn_Sch_Light_Ind, _T("ev_Cn_Sch_Light_Ind") ); 
    MAPPING_HELPER( ev_Cn_Sch_Light_FB_Notify, _T("ev_Cn_Sch_Light_FB_Notify") );
//    MAPPING_HELPER( ev_Cn_Sch_MidLight_Cmd, _T("ev_Cn_Sch_MidLight_Cmd") );
    MAPPING_HELPER( ev_Cn_Sch_MidLight_Ind, _T("ev_Cn_Sch_MidLight_Ind") );
    MAPPING_HELPER( ev_Cn_Sch_ScrLight_Cmd, _T("ev_Cn_Sch_ScrLight_Cmd") ); 
    MAPPING_HELPER( ev_Cn_Sch_ScrLight_Ind, _T("ev_Cn_Sch_ScrLight_Ind") );
    MAPPING_HELPER( ev_Cn_Sch_Temp_FB_Notify, _T("ev_Cn_Sch_Temp_FB_Notify") );
    MAPPING_HELPER( ev_cns_centreSchStateNty, _T("ev_cns_centreSchStateNty") ); 
    
    MAPPING_HELPER( ev_cns_centreselectTV_cmd, _T("ev_cns_centreselectTV_cmd") );
    MAPPING_HELPER( ev_cns_centreselectTV_Ind, _T("ev_cns_centreselectTV_Ind") );
    MAPPING_HELPER( ev_cns_centreTVMode_cmd, _T("ev_cns_centreTVMode_cmd") ); 
    MAPPING_HELPER( ev_cns_centreTVMode_Ind, _T("ev_cns_centreTVMode_Ind") );   
    MAPPING_HELPER( ev_cns_centreTVAudVisMode_cmd, _T("ev_cns_centreTVAudVisMode_cmd") );
    MAPPING_HELPER( ev_cns_centreTVAudVisMode_Ind, _T("ev_cns_centreTVAudVisMode_Ind") );
    MAPPING_HELPER( ev_cns_centreTVDisMode_cmd, _T("ev_cns_centreTVDisMode_cmd") );
    MAPPING_HELPER( ev_cns_centreTVDisMode_Ind, _T("ev_cns_centreTVDisMode_Ind") );
    MAPPING_HELPER( ev_cns_centreTVInpSelect_cmd, _T("ev_cns_centreTVInpSelect_cmd") );
    MAPPING_HELPER( ev_cns_centreTVInpSelect_Ind, _T("ev_cns_centreTVInpSelect_Ind") );
    MAPPING_HELPER( ev_cns_centreTVStateNty, _T("ev_cns_centreTVStateNty") );
	MAPPING_HELPER( ev_Cn_centreTVModleInd, _T("ev_Cn_centreTVModleInd") );

    MAPPING_HELPER( ev_Cns_DCamSelectCmd, _T("ev_Cns_DCamSelectCmd") );
    MAPPING_HELPER( ev_Cn_DCamPower_Cmd, _T("ev_Cn_DCamPower_Cmd") );
    MAPPING_HELPER( ev_Cn_DCamAFMode_Cmd, _T("ev_Cn_DCamAFMode_Cmd") );
    MAPPING_HELPER( ev_Cn_DCamWBMode_Cmd, _T("ev_Cn_DCamWBMode_Cmd") );
    MAPPING_HELPER( ev_Cn_DCamZoom_Cmd, _T("ev_Cn_DCamZoom_Cmd") );
    MAPPING_HELPER( ev_Cn_DCamPreset_Cmd, _T("ev_Cn_DCamPreset_Cmd") );
    MAPPING_HELPER( ev_Cn_DCamRecall_Cmd, _T("ev_Cn_DCamRecall_Cmd") );
    MAPPING_HELPER( ev_Cns_DCamSelectInd, _T("ev_Cns_DCamSelectInd") );
    MAPPING_HELPER( ev_Cns_DCamCurIdxNty, _T("ev_Cns_DCamCurIdxNty") );
    MAPPING_HELPER( ev_Cn_DCamPower_Ind, _T("ev_Cn_DCamPower_Ind") );
    MAPPING_HELPER( ev_Cn_DCamAFMode_Ind, _T("ev_Cn_DCamAFMode_Ind") );
    MAPPING_HELPER( ev_Cn_DCamWBMode_Ind, _T("ev_Cn_DCamWBMode_Ind") );
    MAPPING_HELPER( ev_Cn_DCamZoom_Ind, _T("ev_Cn_DCamZoom_Ind") );
    MAPPING_HELPER( ev_Cn_DCamPreset_Ind, _T("ev_Cn_DCamPreset_Ind") );
    MAPPING_HELPER( ev_Cn_DCamRecall_Ind, _T("ev_Cn_DCamRecall_Ind") );
    MAPPING_HELPER( ev_cns_centreDCamStateNty, _T("ev_cns_centreDCamStateNty") );

    MAPPING_HELPER( ev_cns_centreACStateNty, _T("ev_cns_centreACStateNty") );
    MAPPING_HELPER( ev_Cn_CentreSetACTempCmd, _T("ev_Cn_CentreSetACTempCmd") );
    MAPPING_HELPER( ev_Cn_CentreSetACTempInd, _T("ev_Cn_CentreSetACTempInd") );
    MAPPING_HELPER( ev_Cn_CentreSetACModeCmd, _T("ev_Cn_CentreSetACModeCmd") );
    MAPPING_HELPER( ev_Cn_CentreSetACModeInd, _T("ev_Cn_CentreSetACModeInd") );
    MAPPING_HELPER( ev_Cn_CentreSetACAirVolCmd, _T("ev_Cn_CentreSetACAirVolCmd") );
    MAPPING_HELPER( ev_Cn_CentreSetAirVolInd, _T("ev_Cn_CentreSetAirVolInd") );
	MAPPING_HELPER( ev_Cn_CentreSetACPowerCmd, _T("ev_Cn_CentreSetACPowerCmd") );
	MAPPING_HELPER( ev_Cn_CentreSetACPowerInd, _T("ev_Cn_CentreSetACPowerInd") );
	MAPPING_HELPER( ev_cns_centreSelectAC_Cmd, _T("ev_cns_centreSelectAC_Cmd"));
	MAPPING_HELPER( ev_cns_centreSelectAC_Ind, _T("ev_cns_centreSelectAC_Ind"));
	MAPPING_HELPER( ev_cns_centreACCopyCmd, _T("ev_cns_centreACCopyCmd"));
	MAPPING_HELPER( ev_cns_centreACCopyInd, _T("ev_cns_centreACCopyInd"));
	MAPPING_HELPER( ev_Cn_CamPowerStateNty, _T("ev_Cn_CamPowerStateNty") );
	MAPPING_HELPER( ev_CnCfgDisplay_Cmd, _T("ev_CnCfgDisplay_Cmd") );
	MAPPING_HELPER( ev_CnCfgDisplay_Ind, _T("ev_CnCfgDisplay_Ind") );
	MAPPING_HELPER( ev_Cn_DocSetTypeInd, _T("ev_Cn_DocSetTypeInd") );
    MAPPING_HELPER( ev_CnCfgDisplayShow_Ind, _T("ev_CnCfgDisplayShow_Ind") );
    MAPPING_HELPER( ev_CnCfgDisplayShow_Nty, _T("ev_CnCfgDisplayShow_Nty") );

	//系统信息
	MAPPING_HELPER( ev_tppGetSysInfo_Req, _T("ev_tppGetSysInfo_Req") );
	MAPPING_HELPER( ev_tppGetSysInfo_Rsp, _T("ev_tppGetSysInfo_Rsp") );
	MAPPING_HELPER( ev_tppGetSysInfo_Nty, _T("ev_tppGetSysInfo_Nty"), 3 );	

	//TPad列表
	MAPPING_HELPER( ev_tppGetTPadIpList_Req, _T("ev_tppGetTPadIpList_Req") );
	MAPPING_HELPER( ev_tppGetTPadIpList_Rsp, _T("ev_tppGetTPadIpList_Rsp") );
	MAPPING_HELPER( ev_tppGetTPadIpList_Nty, _T("ev_tppGetTPadIpList_Nty") );

	//会议信息通知
	MAPPING_HELPER( ev_tppCnConfInfo_Nty, _T("ev_tppCnConfInfo_Nty") );

	//T300升级
	MAPPING_HELPER( ev_tppUpdate_Cmd, _T("ev_tppUpdate_Cmd") );
	MAPPING_HELPER( ev_tppUpdate_Ind, _T("ev_tppUpdate_Ind") );

	//音频接口配置
	MAPPING_HELPER( ev_tppSetAudioPort_Cmd, _T("ev_tppSetAudioPort_Cmd") );
	MAPPING_HELPER( ev_tppSetAudioPort_Ind, _T("ev_tppSetAudioPort_Ind") );
	MAPPING_HELPER( ev_tppAudioRestoreDefaults_Cmd, _T("ev_tppAudioRestoreDefaults_Cmd") );
	MAPPING_HELPER( ev_tppAudioRestoreDefaults_Ind, _T("ev_tppAudioRestoreDefaults_Ind") );

	//音视频选择矩阵方案
	MAPPING_HELPER( ev_tppSelAVMatrixProject_Cmd, _T("ev_tppSelAVMatrixProject_Cmd") );
	MAPPING_HELPER( ev_tppSelAVMatrixProject_Ind, _T("ev_tppSelAVMatrixProject_Ind"), 3 );

	//停止诊断测试
	MAPPING_HELPER( ev_tppStopAVMatrixProject_Req, _T("ev_tppStopAVMatrixProject_Req") );
	MAPPING_HELPER( ev_tppStopAVMatrixProject_Rsp, _T("ev_tppStopAVMatrixProject_Rsp") );

	//丢包信息(后续会有更改或者添加)
	MAPPING_HELPER( ev_tppGetLostPacketInfo_Req, _T("ev_tppGetLostPacketInfo_Req") );
	MAPPING_HELPER( ev_tppGetLostPacketInfo_Rsp, _T("ev_tppGetLostPacketInfo_Rsp"), 3 );

	//上下行网络
	MAPPING_HELPER( ev_tppGetUpDownstreamRate_Nty, _T("ev_tppGetUpDownstreamRate_Nty"), 3 );

	//演示源输入接口及输出接口
	MAPPING_HELPER( ev_tppAVInfo_Nty, _T("ev_tppAVInfo_Nty") );

	//重启CNS
	MAPPING_HELPER( ev_tppReboot_Nty, _T("ev_tppReboot_Nty") );
	
	//台标上传成功通知
	MAPPING_HELPER( ev_tppSetBannerLogo_Cmd, _T("ev_tppSetBannerLogo_Cmd") );
	
	//基本信息界面刷新请求
	MAPPING_HELPER( ev_tppRefresh_Req, _T("ev_tppRefresh_Req") );

	//导入地址簿
	MAPPING_HELPER( ev_tppImportAddr_Req, _T("ev_tppImportAddr_Req") );
	MAPPING_HELPER( ev_tppImportAddr_Rsp, _T("ev_tppImportAddr_Rsp") );

	//音频混音数及均衡机命令
	MAPPING_HELPER( ev_tppSetAudiotMixer_Cmd, _T("ev_tppSetAudiotMixer_Cmd") );
	MAPPING_HELPER( ev_tppSetAudioEqualizer_Cmd, _T("ev_tppSetAudioEqualizer_Cmd") );
	MAPPING_HELPER( ev_tppSetAudiotMixer_Ind, _T("ev_tppSetAudiotMixer_Ind") );
	MAPPING_HELPER( ev_tppSetAudioEqualizer_Ind, _T("ev_tppSetAudioEqualizer_Ind") );
	MAPPING_HELPER( ev_tppMixRestoreDefaults_Cmd, _T("ev_tppMixRestoreDefaults_Cmd") );
	MAPPING_HELPER( ev_tppMixRestoreDefaults_Ind, _T("ev_tppMixRestoreDefaults_Ind") );
	MAPPING_HELPER( ev_tppEqualizerReDefaults_Cmd, _T("ev_tppEqualizerReDefaults_Cmd") );
	MAPPING_HELPER( ev_tppEqualizerReDefaults_Ind, _T("ev_tppEqualizerReDefaults_Ind") );

	//呼叫服务器
	MAPPING_HELPER( ev_tppSetCallServerInfo_Cmd, _T("ev_tppSetCallServerInfo_Cmd") );
	MAPPING_HELPER( ev_tppSetCallServerInfo_Ind, _T("ev_tppSetCallServerInfo_Ind") );
	MAPPING_HELPER( ev_tppSetCallServerInfo_Nty, _T("ev_tppSetCallServerInfo_Nty") );
	
	//升级服务器
	MAPPING_HELPER( ev_tppSetUpgradeServerInfo_Cmd, _T("ev_tppSetUpgradeServerInfo_Cmd") );
	MAPPING_HELPER( ev_tppSetUpgradeServerInfo_Ind, _T("ev_tppSetUpgradeServerInfo_Ind") );
	MAPPING_HELPER( ev_tppSetUpgradeServerInfo_Nty, _T("ev_tppSetUpgradeServerInfo_Nty") );

	//网管
	MAPPING_HELPER( ev_CnAgtAddNMServerCmd, _T("ev_CnAgtAddNMServerCmd") );
	MAPPING_HELPER( ev_CnAgtDelNMServerCmd, _T("ev_CnAgtDelNMServerCmd") );
	MAPPING_HELPER( ev_CnAgtModifyNMServerCmd, _T("ev_CnAgtModifyNMServerCmd") );
	MAPPING_HELPER( ev_tppCnAgtAddNMServer_Ind, _T("ev_tppCnAgtAddNMServer_Ind") );
	MAPPING_HELPER( ev_tppCnAgtDelNMServer_Ind, _T("ev_tppCnAgtDelNMServer_Ind") );
	MAPPING_HELPER( ev_tppCnAgtModifyNMServer_Ind, _T("ev_tppCnAgtModifyNMServer_Ind") );
	MAPPING_HELPER( ev_tppCurAgtNMServerInfo_Nty, _T("ev_tppCurAgtNMServerInfo_Nty") );

	//摄像机
	MAPPING_HELPER( ev_Cn_CameraCfgNty, _T("ev_Cn_CameraCfgNty") );
	MAPPING_HELPER( ev_Cn_CameraSelectCmd, _T("ev_Cn_CameraSelectCmd") );
	MAPPING_HELPER( ev_Cn_CameraSelectInd, _T("ev_Cn_CameraSelectInd") );
	//MAPPING_HELPER( ev_Cn_CameraListCmd, _T("ev_Cn_CameraListCmd") );
	//MAPPING_HELPER( ev_Cn_CameraListInd, _T("ev_Cn_CameraListInd") );
	//MAPPING_HELPER( ev_Cn_SetCameraFileCmd, _T("ev_Cn_SetCameraFileCmd") );//shi
	//MAPPING_HELPER( ev_Cn_SetCameraFileInd, _T("ev_Cn_SetCameraFileInd") );//shi
	MAPPING_HELPER( ev_Cn_CameraFocusCmd, _T("ev_Cn_CameraFocusCmd") );
	MAPPING_HELPER( ev_Cn_CameraFocusInd, _T("ev_Cn_CameraFocusInd") );
	MAPPING_HELPER( ev_Cn_CameraAutoFocusCmd, _T("ev_Cn_CameraAutoFocusCmd") );
	MAPPING_HELPER( ev_Cn_CameraAutoFocusInd, _T("ev_Cn_CameraAutoFocusInd") );
	MAPPING_HELPER( ev_Cn_CameraPresetCmd, _T("ev_Cn_CameraPresetCmd") );
	MAPPING_HELPER( ev_Cn_CameraPresetInd, _T("ev_Cn_CameraPresetInd") );
	MAPPING_HELPER( ev_Cn_CameraPersetSaveCmd, _T("ev_Cn_CameraPersetSaveCmd") );
	MAPPING_HELPER( ev_Cn_CameraPersetSaveInd, _T("ev_Cn_CameraPersetSaveInd") );


	MAPPING_HELPER( ev_Cn_CameraZoomCmd, _T("ev_Cn_CameraZoomCmd") );
	MAPPING_HELPER( ev_Cn_CameraZoomInd, _T("ev_Cn_CameraZoomInd") );
	MAPPING_HELPER( ev_Cn_CameraPersetSaveCmd, _T("ev_Cn_CameraPersetSaveCmd") );
	MAPPING_HELPER( ev_Cn_CameraPersetSaveInd, _T("ev_Cn_CameraPersetSaveInd") );


	MAPPING_HELPER( ev_Cn_CameraResetCmd, _T("ev_Cn_CameraResetCmd") );
	MAPPING_HELPER( ev_Cn_CameraResetInd, _T("ev_Cn_CameraResetInd") );
	MAPPING_HELPER( ev_Cn_CameraOSDShowCmd, _T("ev_Cn_CameraOSDShowCmd") );
	MAPPING_HELPER( ev_Cn_CameraOSDShowInd, _T("ev_Cn_CameraOSDShowInd") );
	MAPPING_HELPER( ev_Cn_CameraAutoExposureCmd, _T("ev_Cn_CameraAutoExposureCmd") );
	MAPPING_HELPER( ev_Cn_CameraAutoExposureInd, _T("ev_Cn_CameraAutoExposureInd") );
	MAPPING_HELPER( ev_Cn_CameraGainCmd, _T("ev_Cn_CameraGainCmd") );
	MAPPING_HELPER( ev_Cn_CameraGainInd, _T("ev_Cn_CameraGainInd") );


	MAPPING_HELPER( ev_Cn_CameraShutSpdCmd, _T("ev_Cn_CameraShutSpdCmd") );
	MAPPING_HELPER( ev_Cn_CameraShutSpdInd, _T("ev_Cn_CameraShutSpdInd") );
	MAPPING_HELPER( ev_Cn_CameraBackLightCmd, _T("ev_Cn_CameraBackLightCmd") );
	MAPPING_HELPER( ev_Cn_CameraBackLightInd, _T("ev_Cn_CameraBackLightInd") );
	MAPPING_HELPER( ev_Cn_CameraApertureCmd, _T("ev_Cn_CameraApertureCmd") );
	MAPPING_HELPER( ev_Cn_CameraApertureInd, _T("ev_Cn_CameraApertureInd") );
	MAPPING_HELPER( ev_Cn_CameraAutoWBCmd, _T("ev_Cn_CameraAutoWBCmd") );
	MAPPING_HELPER( ev_Cn_CameraAutoWBInd, _T("ev_Cn_CameraAutoWBInd") );
	MAPPING_HELPER( ev_Cn_CameraRGainCmd, _T("ev_Cn_CameraRGainCmd") );
	MAPPING_HELPER( ev_Cn_CameraRGainInd, _T("ev_Cn_CameraRGainInd") );
	MAPPING_HELPER( ev_Cn_CameraBGainCmd, _T("ev_Cn_CameraBGainCmd") );
	MAPPING_HELPER( ev_Cn_CameraBGainInd, _T("ev_Cn_CameraBGainInd") );


	MAPPING_HELPER( ev_Cn_CameraPicSceneModeCmd, _T("ev_Cn_CameraPicSceneModeCmd") );
	MAPPING_HELPER( ev_Cn_CameraPicSceneModeInd, _T("ev_Cn_CameraPicSceneModeInd") );

	MAPPING_HELPER( ev_Cn_CameraGammaCmd, _T("ev_Cn_CameraGammaCmd") );
	MAPPING_HELPER( ev_Cn_CameraGammaInd, _T("ev_Cn_CameraGammaInd") );

	MAPPING_HELPER( ev_Cn_CameraPicSceAptCmd, _T("ev_Cn_CameraPicSceAptCmd") );
	MAPPING_HELPER( ev_Cn_CameraPicSceAptInd, _T("ev_Cn_CameraPicSceAptInd") );

	MAPPING_HELPER( ev_Cn_CameraContrastCmd, _T("ev_Cn_CameraContrastCmd") );
	MAPPING_HELPER( ev_Cn_CameraContrastInd, _T("ev_Cn_CameraContrastInd") );

	MAPPING_HELPER( ev_Cn_CameraHueCmd, _T("ev_Cn_CameraHueCmd") );
	MAPPING_HELPER( ev_Cn_CameraHueInd, _T("ev_Cn_CameraHueInd") );
	MAPPING_HELPER( ev_Cn_CameraSaturatCmd, _T("ev_Cn_CameraSaturatCmd") );
	MAPPING_HELPER( ev_Cn_CameraSaturatInd, _T("ev_Cn_CameraSaturatInd") );
	MAPPING_HELPER( ev_Cn_CameraBrightCmd, _T("ev_Cn_CameraBrightCmd") );
	MAPPING_HELPER( ev_Cn_CameraBrightInd, _T("ev_Cn_CameraBrightInd") );
	MAPPING_HELPER( ev_Cn_CameraNRStmCleanCmd, _T("ev_Cn_CameraNRStmCleanCmd") );
	MAPPING_HELPER( ev_Cn_CameraNRStmCleanInd, _T("ev_Cn_CameraNRStmCleanInd") );
	MAPPING_HELPER( ev_Cn_Camera3DNRCmd, _T("ev_Cn_Camera3DNRCmd") );
	MAPPING_HELPER( ev_Cn_Camera3DNRInd, _T("ev_Cn_Camera3DNRInd") );
	MAPPING_HELPER( ev_Cn_SetCameraZoomCmd, _T("ev_Cn_SetCameraZoomCmd") );
	MAPPING_HELPER( ev_Cn_SetCameraZoomInd, _T("ev_Cn_SetCameraZoomInd") );

	MAPPING_HELPER( ev_Cn_LoadCameraFileNty, _T("ev_Cn_LoadCameraFileNty") );

    MAPPING_HELPER( ev_TPadLoginInfo_Nty, _T("ev_TPadLoginInfo_Nty") );
    MAPPING_HELPER( ev_TPadHomeKey_Nty, _T("ev_TPadHomeKey_Nty") );
	MAPPING_HELPER( ev_CnsDisconnect_Nty, _T("ev_CnsDisconnect_Nty") );
	//TPad调出中控 dyy 2015-5-26
	MAPPING_HELPER( ev_OpenCenterCtrl_Nty, _T("ev_OpenCenterCtrl_Nty") );
	//调出设置 B5 dyy 2016-8-29
	MAPPING_HELPER( ev_OpenSysConfig_Nty, _T("ev_OpenSysConfig_Nty") ); 
    MAPPING_HELPER( ev_OpenCnsManage_Nty, _T("ev_OpenCnsManage_Nty") ); 
    //中控待机 dyy 2013-9-23 
    MAPPING_HELPER( ev_Cn_CentreSleepNty, _T("ev_Cn_CentreSleepNty") );

	MAPPING_HELPER( ev_Cn_CameraPowerOnNty, _T("ev_Cn_CameraPowerOnNty") );
	MAPPING_HELPER( ev_CnFtpInfo_Nty, _T("ev_CnFtpInfo_Nty") );
	MAPPING_HELPER( ev_CnTelnetInfo_Nty, _T("ev_CnTelnetInfo_Nty") );
	MAPPING_HELPER( ev_CnSetFtp_Req, _T("ev_CnSetFtp_Req") );
    MAPPING_HELPER( ev_CnSetFtp_Rsp, _T("ev_CnSetFtp_Rsp") );

	//B3版本
	MAPPING_HELPER( ev_CnCfgVideoFormat_Cmd, _T("ev_CnCfgVideoFormat_Cmd") );
	MAPPING_HELPER( ev_CnCfgVideoFormat_Ind, _T("ev_CnCfgVideoFormat_Ind") );
	MAPPING_HELPER( ev_CnCfgDualVideoFormat_Cmd, _T("ev_CnCfgDualVideoFormat_Cmd") );
	MAPPING_HELPER( ev_CnCfgDualVideoFormat_Ind, _T("ev_CnCfgDualVideoFormat_Ind") );
	MAPPING_HELPER( ev_CnCfgCallRate_Cmd, _T("ev_CnCfgCallRate_Cmd") );
	MAPPING_HELPER( ev_CnCfgCallRate_Ind, _T("ev_CnCfgCallRate_Ind") );
	MAPPING_HELPER( ev_CnCfgDualCallRate_Cmd, _T("ev_CnCfgDualCallRate_Cmd") );
	MAPPING_HELPER( ev_CnCfgDualCallRate_Ind, _T("ev_CnCfgDualCallRate_Ind") );
	MAPPING_HELPER( ev_CnVerInfo_Nty, _T("ev_CnVerInfo_Nty") );
	MAPPING_HELPER( ev_CnCfgConfDis_Cmd, _T("ev_CnCfgConfDis_Cmd") );
	MAPPING_HELPER( ev_CnCfgConfDis_Ind, _T("ev_CnCfgConfDis_Ind") );
	MAPPING_HELPER( ev_CnCfgConfPoll_Cmd, _T("ev_CnCfgConfPoll_Cmd") );
	MAPPING_HELPER( ev_CnCfgConfPoll_Ind, _T("ev_CnCfgConfPoll_Ind") );
	MAPPING_HELPER( ev_CnCfgAudioFormat_Cmd, _T("ev_CnCfgAudioFormat_Cmd") );
	MAPPING_HELPER( ev_CnCfgAudioFormat_Ind, _T("ev_CnCfgAudioFormat_Ind") );
	MAPPING_HELPER( ev_CnViewLocalPIP_Cmd, _T("ev_CnViewLocalPIP_Cmd") );
	MAPPING_HELPER( ev_CnViewLocalPIP_Ind, _T("ev_CnViewLocalPIP_Ind") );
	MAPPING_HELPER( ev_CnViewLocalPIP_Nty, _T("ev_CnViewLocalPIP_Nty") );

	//选看电视墙	
	MAPPING_HELPER( ev_CnSelTVSView_Cmd, _T("ev_CnSelTVSView_Cmd") );	
	MAPPING_HELPER( ev_CnSelTVSView_Ind, _T("ev_CnSelTVSView_Ind") );	
	MAPPING_HELPER( ev_CnCancelSelTVSView_Cmd, _T("ev_CnCancelSelTVSView_Cmd") );	
	MAPPING_HELPER( ev_CnCancelSelTVSView_Ind, _T("ev_CnCancelSelTVSView_Ind") );

	//台标横幅
	MAPPING_HELPER( ev_CnIsShowLogo_Cmd, _T("ev_CnIsShowLogo_Cmd") );	
	MAPPING_HELPER( ev_CnIsShowLogo_Ind, _T("ev_CnIsShowLogo_Ind") );	
	MAPPING_HELPER( ev_CnIsShowLogo_Nty, _T("ev_CnIsShowLogo_Nty") );	
	MAPPING_HELPER( ev_CnIsShowBanner_Cmd, _T("ev_CnIsShowBanner_Cmd") );
	MAPPING_HELPER( ev_CnIsShowBanner_Ind, _T("ev_CnIsShowBanner_Ind") );	
	MAPPING_HELPER( ev_CnIsShowBanner_Nty, _T("ev_CnIsShowBanner_Nty") );

	//AES加密
	MAPPING_HELPER( ev_tppSetEncryptInfo_Ind, _T("ev_tppSetEncryptInfo_Ind") );	
	MAPPING_HELPER( ev_tppSetEncryptInfo_Nty, _T("ev_tppSetEncryptInfo_Nty") );

	//全景摄像机
	MAPPING_HELPER( ev_Cns_SetPanCamOpenCmd, _T("ev_Cns_SetPanCamOpenCmd") );	
	MAPPING_HELPER( ev_Cns_SetPanCamOpenInd, _T("ev_Cns_SetPanCamOpenInd") );	
	MAPPING_HELPER( ev_Cns_PanCamAutoFocusCmd, _T("ev_Cns_PanCamAutoFocusCmd") );	
	MAPPING_HELPER( ev_Cns_PanCamPresetDeleteCmd, _T("ev_Cns_PanCamPresetDeleteCmd") );	
	MAPPING_HELPER( ev_Cns_SetPanCamNameCmd, _T("ev_Cns_SetPanCamNameCmd") );
	MAPPING_HELPER( ev_Cns_PanCamPresetMoveCmd, _T("ev_Cns_PanCamPresetMoveCmd") );	
	MAPPING_HELPER( ev_Cns_PanCamFocusCmd, _T("ev_Cns_PanCamFocusCmd") );
	MAPPING_HELPER( ev_Cns_PanCamSavePresetCmd, _T("ev_Cns_PanCamSavePresetCmd") );
	
	MAPPING_HELPER( ev_Cns_PanCamBackLightCmd, _T("ev_Cns_PanCamBackLightCmd") );	
	MAPPING_HELPER( ev_Cns_PanCamZoomCmd, _T("ev_Cns_PanCamZoomCmd") );
	MAPPING_HELPER( ev_Cns_PanCamPTCmd, _T("ev_Cns_PanCamPTCmd") );	
	MAPPING_HELPER( ev_Cns_PanCamSelectCmd, _T("ev_Cns_PanCamSelectCmd") );
	MAPPING_HELPER( ev_CnSetVedioInfo_Cmd, _T("ev_CnSetVedioInfo_Cmd") );

	MAPPING_HELPER( ev_CnSetVedioInfo_Ind, _T("ev_CnSetVedioInfo_Ind") );	
	MAPPING_HELPER( ev_tppSetDataServerInfo_Nty, _T("ev_tppSetDataServerInfo_Nty") );	
	MAPPING_HELPER( ev_Cns_PanCamCfgNty, _T("ev_Cns_PanCamCfgNty") );
	MAPPING_HELPER( ev_tppSetSnapShotInd, _T("ev_tppSetSnapShotInd") );
	MAPPING_HELPER( ev_Cns_PanCamPresetDeleteInd, _T("ev_Cns_PanCamPresetDeleteInd") );	
	MAPPING_HELPER( ev_Cns_PanCamPresetMoveInd, _T("ev_Cns_PanCamPresetMoveInd") );
	MAPPING_HELPER( ev_Cns_SetPanCamNameInd, _T("ev_Cns_SetPanCamNameInd") );	
	MAPPING_HELPER( ev_Cns_PanCamSelectInd, _T("ev_Cns_PanCamSelectInd") );

	//B4 会议轮询
	MAPPING_HELPER( ev_Cn_Conf_SuspendPoll_Req, _T("ev_Cn_Conf_SuspendPoll_Req") );
	MAPPING_HELPER( ev_Cn_Conf_SuspendPoll_Ind, _T("ev_Cn_Conf_SuspendPoll_Ind") );	
	MAPPING_HELPER( ev_Cn_UpdateConfTurnlist_Nty, _T("ev_Cn_UpdateConfTurnlist_Nty") );
	MAPPING_HELPER( ev_Cn_UpdateConfTurnlist_Ind, _T("ev_Cn_UpdateConfTurnlist_Ind") );

    //画面合成
    MAPPING_HELPER( ev_Cn_RervesBrdVmp_Req, _T("ev_Cn_RervesBrdVmp_Req") );
    MAPPING_HELPER( ev_Cn_RervesBrdVmp_Ind, _T("ev_Cn_RervesBrdVmp_Ind") );
    MAPPING_HELPER( ev_Cn_SetBrdVmp_Req, _T("ev_Cn_SetBrdVmp_Req") );
    MAPPING_HELPER( ev_Cn_SetBrdVmp_Ind, _T("ev_Cn_SetBrdVmp_Ind") );
    MAPPING_HELPER( ev_Cn_BrdVmp_Req, _T("ev_Cn_BrdVmp_Req") );
    MAPPING_HELPER( ev_Cn_BrdVmp_Ind, _T("ev_Cn_BrdVmp_Ind") );
    MAPPING_HELPER( ev_Cn_BrdVmp_Ntfy, _T("ev_Cn_BrdVmp_Ntfy") );
    MAPPING_HELPER( ev_Cn_SaveVmpCfg_Cmd, _T("ev_Cn_SaveVmpCfg_Cmd") );
    MAPPING_HELPER( ev_CnSaveVmpCfg_Ind, _T("ev_CnSaveVmpCfg_Ind") );    

    MAPPING_HELPER( ev_tppSetEncryptInfo_Cmd, _T("ev_tppSetEncryptInfo_Cmd") );
    MAPPING_HELPER( ev_tppSetEncryptInfo_Ind, _T("ev_tppSetEncryptInfo_Ind") );
    MAPPING_HELPER( ev_tppSetEncryptInfo_Nty, _T("ev_tppSetEncryptInfo_Nty") );
    //视频源状态通知 2015-6-17 
	MAPPING_HELPER( ev_TppGetVidSrc_Nty, _T("ev_TppGetVidSrc_Nty") );
	MAPPING_HELPER( ev_CnSetVedioInfo_Nty, _T("ev_CnSetVedioInfo_Nty") );
	//B4中控
	MAPPING_HELPER( ev_Cn_centreCurInfoNty, _T("ev_Cn_centreCurInfoNty") );
	MAPPING_HELPER( ev_Cn_centreCurActCmd, _T("ev_Cn_centreCurActCmd") );	
	MAPPING_HELPER( ev_Cn_centreCurActInd, _T("ev_Cn_centreCurActInd") );
	MAPPING_HELPER( ev_Cn_DocSetSnapShotCmd, _T("ev_Cn_DocSetSnapShotCmd") );
	MAPPING_HELPER( ev_Cn_DocSetSnapShotInd, _T("ev_Cn_DocSetSnapShotInd") );
	//B4选看
	MAPPING_HELPER( ev_Cn_SelView_Req, _T("ev_Cn_SelView_Req") );
	MAPPING_HELPER( ev_Cn_SelView_Ind, _T("ev_Cn_SelView_Ind") );
	MAPPING_HELPER( ev_UMS_CNC_ByCNS_SelView_Ntfy, _T("ev_UMS_CNC_ByCNS_SelView_Ntfy") );
	//辅视频源 第二路
	MAPPING_HELPER( ev_CnMinVedioInfo_Nty, _T("ev_CnMinVedioInfo_Nty") );

	//终端会场入会请求
	MAPPING_HELPER( ev_UmsToCncJoinConf_Req, _T("ev_UmsToCncJoinConf_Req") );
	MAPPING_HELPER( ev_UmsToCncJoinConf_Rsp, _T("ev_UmsToCncJoinConf_Rsp") );

	//白板相关
	MAPPING_HELPER( ev_WBConfStateNty, _T("ev_WBConfStateNty") );
	MAPPING_HELPER( ev_tpp_NewConfCall_Req, _T("ev_tpp_NewConfCall_Req") );

	//点名相关
	MAPPING_HELPER( evConfRollCallUpdate_Cmd, _T("evConfRollCallUpdate_Cmd") );
	MAPPING_HELPER( evConfRollCallUpdate_Ind, _T("evConfRollCallUpdate_Ind") );
	MAPPING_HELPER( evConfRollCallNextEp_Cmd, _T("evConfRollCallNextEp_Cmd") );
	MAPPING_HELPER( evConfRollCallNextEp_Ind, _T("evConfRollCallNextEp_Ind") );
	MAPPING_HELPER( evConfRollCallUpdate_Nty, _T("evConfRollCallUpdate_Nty") );
	MAPPING_HELPER( evConfRollCallUpdataList_Cmd, _T("evConfRollCallUpdataList_Cmd") );
	MAPPING_HELPER( evConfRollCallUpdataList_Ind, _T("evConfRollCallUpdataList_Ind") );
	MAPPING_HELPER( evtpRollUpdate_PresentState, _T("evtpRollUpdate_PresentState") );
	MAPPING_HELPER( evtpRoll_PresentState_Nty, _T("evtpRoll_PresentState_Nty") );
	//会议混音相关
	MAPPING_HELPER( evtpConfAudMixMode_Nty, _T("evtpConfAudMixMode_Nty") );
	MAPPING_HELPER( evConfAudMixList_Ind, _T("evConfAudMixList_Ind") );
	MAPPING_HELPER( evConfAudMixList_Cmd, _T("evConfAudMixList_Cmd") );
	MAPPING_HELPER( evConfAudMixMode_Ind, _T("evConfAudMixMode_Ind") );
	MAPPING_HELPER( evConfAudMixMode_Cmd, _T("evConfAudMixMode_Cmd") );

	//当前会场是否在会议混音中
	MAPPING_HELPER( ev_EpInAudMix_Nty, _T("ev_EpInAudMix_Nty") );

	//加密相关
	MAPPING_HELPER( ev_tppSetQtEncryptInfo_Cmd, _T("ev_tppSetQtEncryptInfo_Cmd") );
	MAPPING_HELPER( ev_tppSetQtEncryptInfo_Ind, _T("ev_tppSetQtEncryptInfo_Ind") );

	//远摇
	MAPPING_HELPER( ev_Cn_FeccChangeSrc_Cmd, _T("ev_Cn_FeccChangeSrc_Cmd") );
	MAPPING_HELPER( ev_Cn_FeccCamPT_Cmd, _T("ev_Cn_FeccCamPT_Cmd") );
	MAPPING_HELPER( ev_Cn_FeccCamAutoFocus_Cmd, _T("ev_Cn_FeccCamAutoFocus_Cmd") );
	MAPPING_HELPER( ev_Cn_FeccCamFocus_Cmd, _T("ev_Cn_FeccCamFocus_Cmd") );
	MAPPING_HELPER( ev_Cn_FeccCamZoom_Cmd, _T("ev_Cn_FeccCamZoom_Cmd") );
	MAPPING_HELPER( ev_Cn_FeccCamBackLight_Cmd, _T("ev_Cn_FeccCamBackLight_Cmd") );

	//323终端上报视频源类型消息
	MAPPING_HELPER( ev_H323SendVideoInfo_Nty, _T("ev_H323SendVideoInfo_Nty") );
	MAPPING_HELPER( ev_H323SendVideoFeccInfo_Nty, _T("ev_H323SendVideoFeccInfo_Nty") );
	MAPPING_HELPER( ev_SendFeccVidSrcInfo_Nty, _T("ev_SendFeccVidSrcInfo_Nty") );
    MAPPING_HELPER( ev_SendFeccVidSrcInfo_Ind, _T("ev_SendFeccVidSrcInfo_Ind") );

	//麦克风相关
	MAPPING_HELPER( ev_CnSetDesktopMicInfo_Nty, _T("ev_CnSetDesktopMicInfo_Nty") );
	MAPPING_HELPER( ev_CnSetDesktopMicInfo_Ind, _T("ev_CnSetDesktopMicInfo_Ind") );
	MAPPING_HELPER( ev_CnMicPowerStyle_Nty, _T("ev_CnMicPowerStyle_Nty") );
	MAPPING_HELPER( ev_CnMicPower_Nty, _T("ev_CnMicPower_Nty") );
	MAPPING_HELPER( ev_CnSetMicPower_Cmd, _T("ev_CnSetMicPower_Cmd") );
	MAPPING_HELPER( ev_CnSetMicPower_Rsp, _T("ev_CnSetMicPower_Rsp") );

	//时间同步
	MAPPING_HELPER( ev_CnSysTimeToCnc_Nty, _T("ev_CnSysTimeToCnc_Nty") );
	MAPPING_HELPER( ev_CnNtpIsUsed_Cmd, _T("ev_CnNtpIsUsed_Cmd") );
	MAPPING_HELPER( ev_CnNtpIsUsed_Ind, _T("ev_CnNtpIsUsed_Ind") );	

	//密码周期更新
	MAPPING_HELPER( ev_CnTpadPasswordUpdate_Nty, _T("ev_CnTpadPasswordUpdate_Nty") );
	MAPPING_HELPER( ev_CnTpadPasswordUpdate_Cmd, _T("ev_CnTpadPasswordUpdate_Cmd") );
	MAPPING_HELPER( ev_CnTpadPasswordUpdate_Ind, _T("ev_CnTpadPasswordUpdate_Ind") );

    //超级管理员
    MAPPING_HELPER( ev_tppSuperadmin_Nty, _T("ev_tppSuperadmin_Nty") );
    MAPPING_HELPER( ev_tppSuperadmin_Cmd, _T("ev_tppSuperadmin_Cmd") );
    MAPPING_HELPER( ev_tppSuperadmin_Ind, _T("ev_tppSuperadmin_Ind") );
    MAPPING_HELPER( ev_tppSuperadminClear_Cmd, _T("ev_tppSuperadminClear_Cmd") );

    //升级后第一次登录
    MAPPING_HELPER( ev_Cn_CNCFirstLogin_Nty, _T("ev_Cn_CNCFirstLogin_Nty") );

    //矩阵
    MAPPING_HELPER( ev_CnMatrixConfigInfo_Nty, _T("ev_CnMatrixConfigInfo_Nty") );
    MAPPING_HELPER( ev_CnSetMatrixConfig_Ind, _T("ev_CnSetMatrixConfig_Ind") );
    MAPPING_HELPER( ev_CnMatrixOnlineState_Nty, _T("ev_CnMatrixOnlineState_Nty") );
    MAPPING_HELPER( ev_CnMatrixSceneInfo_Nty, _T("ev_CnMatrixSceneInfo_Nty") );
    MAPPING_HELPER( ev_CnCurMatrixInfo_Nty, _T("ev_CnCurMatrixInfo_Nty") );
    MAPPING_HELPER( ev_Cn_MatrixInOutRelation_Nty, _T("ev_Cn_MatrixInOutRelation_Nty") );
    MAPPING_HELPER( ev_CnChangeMatrixOutInRelation_Cmd, _T("ev_CnChangeMatrixOutInRelation_Cmd") );
    MAPPING_HELPER( ev_CnMatrixOutInRelation_Ind, _T("ev_CnMatrixOutInRelation_Ind") );
    MAPPING_HELPER( ev_CnSaveMatrixScence_Cmd, _T("ev_CnSaveMatrixScence_Cmd") );
    MAPPING_HELPER( ev_CnSaveMatrixScence_Ind, _T("ev_CnSaveMatrixScence_Ind") );
    MAPPING_HELPER( ev_CnRenameMatrixScence_Cmd, _T("ev_CnRenameMatrixScence_Cmd") );
    MAPPING_HELPER( ev_CnRenameMatrixScence_Ind, _T("ev_CnRenameMatrixScence_Ind") );
    MAPPING_HELPER( ev_CnDeleteMatrixScence_Cmd, _T("ev_CnDeleteMatrixScence_Cmd") );
    MAPPING_HELPER( ev_CnDeleteMatrixScence_Ind, _T("ev_CnDeleteMatrixScence_Ind") );
    MAPPING_HELPER( ev_CnUseMatrixScence_Cmd, _T("ev_CnUseMatrixScence_Cmd") );
    MAPPING_HELPER( ev_CnUseMatrixScence_Ind, _T("ev_CnUseMatrixScence_Ind") );

    //串口
    MAPPING_HELPER( ev_Cn_SelectCom_Nty, _T("ev_Cn_SelectCom_Nty") );
    MAPPING_HELPER( ev_Cn_SelectCom_Ind, _T("ev_Cn_SelectCom_Ind") );

    //升降屏
    MAPPING_HELPER( ev_Cn_CentreDFScreenConfig_Nty, _T("ev_Cn_CentreDFScreenConfig_Nty") );
    MAPPING_HELPER( ev_cns_SelectDFScreen_Nty, _T("ev_cns_SelectDFScreen_Nty") );
    MAPPING_HELPER( ev_cns_ScreenType_Nty, _T("ev_cns_ScreenType_Nty") );
    MAPPING_HELPER( ev_cns_SelectDFScreen_Cmd, _T("ev_cns_SelectDFScreen_Cmd") );
    MAPPING_HELPER( ev_Cn_DFScreenCommand_Cmd, _T("ev_Cn_DFScreenCommand_Cmd") );
    MAPPING_HELPER( ev_Cn_DFScreenCommand_Ind, _T("ev_Cn_DFScreenCommand_Ind") );
    MAPPING_HELPER( ev_cns_SelectDFScreen_Ind, _T("ev_cns_SelectDFScreen_Ind") );
    MAPPING_HELPER( ev_Cn_ModifyDFScreenGroup_Ind, _T("ev_Cn_ModifyDFScreenGroup_Ind") );
}

void CCncMsgDriver::SetInsID( u16 wDstInsID )
{
	m_wInsID = wDstInsID;
}
u16  CCncMsgDriver::GetInsID()
{
	return m_wInsID;
}

void CCncMsgDriver::SetAppID( u16 wAppID )
{
	m_wAppID = wAppID;
}

u16 CCncMsgDriver::GetAppID()
{
	return m_wAppID;
}


void CCncMsgDriver::SetDesIID( u32 dwDesID )
{
    m_dwDesIID = dwDesID;
}

u32 CCncMsgDriver::GetDesIID()
{
    return m_dwDesIID;
}



// MainFrameLogic.h: interface for the CMainFrameLogic class.
 /*****************************************************************************
 模块名      : cnsTool主窗口逻辑处理模块
 文件名      : MainFrameLogic.h
 相关文件    : 
 文件实现功能: 主窗口逻辑单元 
 说明       ： 对应mainFrame.xml,其中包含主框架dlg 
 作者        : 俞锦锦
 版本        : V1.0  Copyright(C) 2011-2012 KDC, All rights reserved.
 -----------------------------------------------------------------------------
 修改记录:
 日  期         版本        修改人      修改内容
 2012/11/23    1.0          俞锦锦      创建
******************************************************************************/

#if !defined(AFX_MAINFRAMELOGIC_H__7A9522E4_68BE_4C94_83CB_71ECEEC91F9E__INCLUDED_)
#define AFX_MAINFRAMELOGIC_H__7A9522E4_68BE_4C94_83CB_71ECEEC91F9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "logicBase.h"

class CMainFrameLogic : public CLogicBase, public ISingleTon<CMainFrameLogic>
{
public: 
	virtual ~CMainFrameLogic();  
    CMainFrameLogic();

public:	 	
	/** 功能:  注册回调函数
	*  @param[in] 
	*  @return 
	*  @remarks  在读取xml文件时候,会调用其中注册的函数
	*/
	virtual void RegCBFun();

	/** 功能:  反注册回调函数
	*  @param[in] 
	*  @return 
	*  @remarks  与上面函数相反,注销注册的函数
	*/
	virtual void UnRegFunc( );

	/** 功能:  初始化窗口
	*  @param[in] 
	*  @return 
	*  @remarks  在读取xml文件时候调用,根据里面的内容初始化窗口
	*/	
    virtual bool InitWnd( const IArgs & arg ); 
 
	/** 功能:  清理窗口
	*  @param[in] 
	*  @return 
	*  @remarks  在窗口关闭的时候调用,清空窗口数据
	*/
    virtual void Clear() ;
protected:	
	/** 功能: 注册消息,接收来自lib库中发送来的消息
	*  @param[in] 
	*  @return 
	*  @remarks  
	*/	
	virtual void RegMsg(); 
		
	/** 功能: 反注册消息,与上面函数相反,注销注册的消息
	*  @param[in] 
	*  @return 
	*  @remarks  
	*/	
	virtual void UnRegMsg();

	/** 功能:  断链消息响应
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/
	HRESULT OnDisConnect( WPARAM wparam, LPARAM lparam );

	/** 功能:  登录消息响应
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/
	HRESULT OnConnectRsp( WPARAM wparam, LPARAM lparam );	

	/** 功能:  抢断登录通知
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/
	HRESULT OnCNSGrabLoginNty( WPARAM wparam, LPARAM lparam ); 
	/** 功能:  抢断登录通知
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/
	HRESULT OnUMSGrabLoginNty( WPARAM wparam, LPARAM lparam ); 
	
	/** 功能:  对方在抓包，登陆失败
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/
	HRESULT OnLoginRefuseNty( WPARAM wparam, LPARAM lparam ); 

	/** 功能:  登陆超时提示消息
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/
	HRESULT OnLoginTimeOut( WPARAM wparam, LPARAM lparam );		
	
private: 
	/** 功能:  注销窗口,回到登录界面
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/	
	bool OnBtnLogoff( const IArgs& args );		

	/** 功能:  	窗口最小化
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/	
	bool OnBtnMinmize( const IArgs& args );	

	/** 功能:  	窗口关闭
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/	
	bool OnBtnTerminalCnstool( const IArgs& args );	

	/** 功能:  下载日志
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/		
	bool OnBtnDowloadLog( const IArgs& args );		

	/** 功能:  	重启服务器
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/		
	bool OnBtnResetSrv( const IArgs& args );		
	
	//主功能菜单点击事件
    bool OnClickCfg( const IArgs& args );
    bool OnClickBaSic( const IArgs& args );
	bool OnClickDiag( const IArgs& args );
	bool OnClickCentCtrl( const IArgs& args );
	bool OnClickCamera( const IArgs& args );
	bool OnClickAddrBook( const IArgs& args );
	bool OnClickCnsCfg( const IArgs& args );
	/** 功能:  	初始化功能列表
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/		
	void InitFunMenu();

	/** 功能:  	将IP地址转换为点分式
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/	
    String TransformIp(u32 dwIp);	

	/** 功能:  Table页切换窗口
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/
	void SwitchCfgTabWnd( string strWnd );

	/** 功能:  隐藏所有窗口
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/
	void HideAllWnd();		

	/** 功能:  判断配置界面的数据是否改动
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/
	s32 IsCfgChange();
	//判断是否正在抓包
	bool IsNetCapturing();
	//判断是否正在导出抓包
	bool IsExortingPackage();

private:
    string m_strLstFunMenu;			//功能列表 
    const String m_stcStrCnsIp;		//主界面左上角CNS的IP地址
	const String m_stcStcCprt;
};

#endif // !defined(AFX_MAINFRAMELOGIC_H__7A9522E4_68BE_4C94_83CB_71ECEEC91F9E__INCLUDED_)

// centCtrlLogic.h: interface for the CCentCtrlLogic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CENTCTRLLOGIC_H__777D44E7_A11A_4507_AA4E_F26B05745548__INCLUDED_)
#define AFX_CENTCTRLLOGIC_H__777D44E7_A11A_4507_AA4E_F26B05745548__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "logicBase.h"
//状态信息
class CCentCfgStatusMesDlg : public CLogicBase, public ISingleTon<CCentCfgStatusMesDlg>  
{
public:
	CCentCfgStatusMesDlg();
	virtual ~CCentCfgStatusMesDlg();
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
	/** 功能:  隐藏所有窗口
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/
//	void HideAllWnd();
private:
	/** 功能:  接收服务器端发过来的消息然后更新摄像机状态
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/		
	LRESULT OnCameraInfoNty( WPARAM wParam, LPARAM lParam );

	/** 功能:  将摄像机的开关状态转换为string类型
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/		
	String CamStateToString( EmCamPowerMode& emSwitchState );
	/** 功能:  接收服务器端发过来的消息然后更新文档摄像机状态
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/		
	LRESULT OnDocCamInfoNty( WPARAM wParam, LPARAM lParam );
	/** 功能:  接收服务器端发过来的消息然后更新显示器状态
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/	
	LRESULT OnDisplayInfoNty( WPARAM wParam, LPARAM lParam );
	/** 功能:  将显示器的开关状态转换为string类型
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/	
	String StateToString( EmTvPowerMode& emSwitchState );
	/** 功能:  接收服务器端发过来的消息然后更新空调状态
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/
	LRESULT OnUpdateAirInfoNty( WPARAM wParam, LPARAM lParam );
	/** 功能:  接收服务器端发过来的消息然后更新灯光状态
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/		
	LRESULT OnLightInfoNty( WPARAM wParam, LPARAM lParam );
	/** 功能:  接收服务器端发过来的消息然后更新空调状态
	*  @param[in] 
	*  @return 
	*  @remarks   
	*/		
	LRESULT OnAirInfoNty( WPARAM wParam, LPARAM lParam );
protected:	
	/** 功能: 注册消息,接收来自lib库中发送来的消息
	*  @param[in] 
	*  @return 
	*  @remarks  
	*/	
	void RegMsg();
};

//窗帘
class CCentCfgCurtainDlg  : public CLogicBase, public ISingleTon<CCentCfgCurtainDlg>  
{
public:
	CCentCfgCurtainDlg();
	virtual ~CCentCfgCurtainDlg();
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

	 HRESULT OnUpdateCurNty( WPARAM wparam, LPARAM lparam );
	 /** 功能:  更新窗帘名称
	 *  @param[in] 
	 *  @return 
	 *  @remarks 
	 */
	 HRESULT OnUpdateCurName( WPARAM wparam, LPARAM lparam );
	 /** 功能:  处理配置窗帘开关反馈
	 *  @param[in] 
	 *  @return 
	 *  @remarks 
	 */
	 HRESULT OnCurtainInd( WPARAM wparam, LPARAM lparam );
	 /** 功能:  处理配置窗帘数量反馈
	 *  @param[in] 
	 *  @return 
	 *  @remarks 
	 */
	 HRESULT OnCurtainNumInd( WPARAM wparam, LPARAM lparam );

	 void RegMsg();
private:
     /** 功能:  单击窗帘重命名
	 *  @param[in] IArgs  点击操作
	 *  @return bool
	 *  @remarks 
	 */
	bool OnBtnOptRenameDlg( const IArgs& args );
	/** 功能:  单击列表条目
	*  @param[in] IArgs  点击操作
	*  @return bool
	*  @remarks 
	*/
	bool OnClickCurList( const IArgs & arg );
	/** 功能:  配置窗帘数量
	*  @param[in] IArgs  点击操作
	*  @return bool
	*  @remarks 
	*/
	bool OnCurNumCfg( const IArgs& args );
	/** 功能:  配置窗帘开关
	*  @param[in] IArgs  点击操作
	*  @return bool
	*  @remarks 
	*/
	bool OnBtnSwitchCurtain( const IArgs& args );
	/** 功能:  点击列表背景图片
	*  @param[in] IArgs  点击操作
	*  @return bool
	*  @remarks 
	*/
	bool OnClickCurLstBlack( const IArgs& args );

	void SetDefaultCurName( );
	/** 功能:  获取选中的列表条目索引
	*  @param[in] IArgs  点击操作
	*  @return bool
	*  @remarks 
	*/
	s32 GetSelItemIndex( TCentreCurName &tCurtainName );

private:
	TCentreCurInfo m_tCentreCurInfo;
	TCentreCurInfo m_tTempCenCurInfo;
	s32            m_nSelIndex;
};

//空调新风
class CCentCfgAirCondition  : public CLogicBase, public ISingleTon<CCentCfgAirCondition>  
{
public:
	CCentCfgAirCondition();
	virtual ~CCentCfgAirCondition();
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
	 *  @remarks 
	 */
	 virtual void Clear() ;

	 void RegMsg();

	 bool IsACChange();
	 /** 功能:  
	 *  @param[in] 
	 *  @return 
	 *  @remarks  
	 */
	 LRESULT OnUpdateAirInfoNty( WPARAM wParam, LPARAM lParam );

	 LRESULT OnAirConditionNty( WPARAM wParam, LPARAM lParam );
	 /** 功能:  获取窗口修改配置项数量
	 *  @param[in] 
	 *  @return 
	 *  @remarks  
	 */
	 s32 GetCtrlChangeNum() { return m_vctWndName.size(); }
private:
	 bool OnSwitchAirCondition( const IArgs & arg );

	 bool OnBtnSave( const IArgs & arg );

	 bool OnBtnCancel( const IArgs & arg );
	 /** 功能:  根据是否存在空调，更新空调界面
	 *  @param[in] 
	 *  @return 
	 *  @remarks  
	 */
	 void UpdateAcState();

	 bool OnACNumCfg( const IArgs & arg );
	 /** 功能:  处理界面修改状态
	 *  @param[in] 
	 *  @return 
	 *  @remarks  
	 */
	 void UpdateState();

	 bool SaveMsgBox();
private:
	u8	m_byAcNum;
};

class CCentCfgDuoVideo  : public CLogicBase, public ISingleTon<CCentCfgDuoVideo>  
{
public:
	CCentCfgDuoVideo();
	virtual ~CCentCfgDuoVideo();
public:
	void RegMsg();
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

	 LRESULT OnDuoVideoNty( WPARAM wParam, LPARAM lParam );

	 bool OnBtnDuoVideo( const IArgs & arg );
private:
	BOOL		m_bOpen;
};

class CCentCfgMatrix  : public CLogicBase, public ISingleTon<CCentCfgMatrix>  
{
public:
	CCentCfgMatrix();
	virtual ~CCentCfgMatrix();
public:
	void RegMsg();
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

	 //修改矩阵名称
	 virtual bool OnChangedMatrixName( const IArgs & arg ); 
	 //修改矩阵厂商
	 virtual bool OnChangedMatrixStore( const IArgs & arg ); 
	 //修改矩阵型号
	 virtual bool OnChangedMatrixModel( const IArgs & arg ); 
	 //修改矩阵服务器
	 virtual bool OnChangedMatrixServer( const IArgs & arg ); 
	 //修改矩阵显示
	 virtual bool OnChangedMatrixSwitch( const IArgs & arg );
	 //保存
	 virtual bool OnBtnSave( const IArgs & arg ); 
	 //取消
	 virtual bool OnBtnCancel( const IArgs & arg ); 
	 //矩阵配置信息
	 LRESULT OnMatrixConfigNty( WPARAM wParam, LPARAM lParam );

	 bool IsMatrixChange();
	 bool SaveMsgBox();

	 s32 GetCtrlChangeNum() { return m_vctWndName.size(); }

private:
	//矩阵配置
	TTPMatrixConfig m_tTPMatrixConfig;
	
};

//组数编号
enum EmGroupNum{
	emOneGroup = 1,
	emTweGroup,
	emThreeGroup,
	emFourGroup,
	emFiveGroup,
};
class CCentCfgSrceen  : public CLogicBase, public ISingleTon<CCentCfgSrceen>  
{
public:
	CCentCfgSrceen();
	virtual ~CCentCfgSrceen();
public:
	void RegMsg();
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

    bool IsDFScreenCfgChange();
    bool SaveMsgBox();
private:
    //修改设备类型
    bool OnChangedSrceenType( const IArgs & arg );
    //修改波特率
    bool OnChangedBaudRate( const IArgs & arg );
    //修改数据位
    bool OnChangedDataBits( const IArgs & arg );
    //修改校验位
    bool OnChangedCheckBits( const IArgs & arg );
    //修改停止位
    bool OnChangedStopBits( const IArgs & arg );
    //修改分组数
	bool OnChangedGroupCount( const IArgs & arg );
    //修改组名
    bool OnChangedGroupName1( const IArgs & arg );
    bool OnChangedGroupName2( const IArgs & arg );
    bool OnChangedGroupName3( const IArgs & arg );
    bool OnChangedGroupName4( const IArgs & arg );
    bool OnChangedGroupName5( const IArgs & arg );
    //修改地址码
    bool OnChangedAddrCode1( const IArgs & arg );
    bool OnChangedAddrCode2( const IArgs & arg );
    bool OnChangedAddrCode3( const IArgs & arg );
    bool OnChangedAddrCode4( const IArgs & arg );
    bool OnChangedAddrCode5( const IArgs & arg );
    //保存
    bool OnBtnSave( const IArgs & arg ); 
    //取消
	bool OnBtnCancel( const IArgs & arg );

    //升降屏配置信息
    LRESULT OnCenDFScreenConfigNty( WPARAM wParam, LPARAM lParam );
    LRESULT OnModifyDFScreenSerCfgInd( WPARAM wParam, LPARAM lParam );
    LRESULT OnModifyDFScreenGroupInd( WPARAM wParam, LPARAM lParam );

    bool IsAddrCodeUsed(String strIndex, String strAddrCode);
    bool IsCenDFScreenSerChange( EmComConfigType emDevieceType, TSerialCfg & tSerialCfg );
    bool IsCenDFScreenGrpChange( u32 dwGrpNum, TCenDownOrFlipScreenCfg* ptCenDFScreenCfg );

private:
    map<u16, String> m_mapAddrCode;//当前选中的地址码
    TCenDownOrFlipScreenInfo m_tCenDFScreenInfo;//升降屏配置信息
};

#endif // !defined(AFX_CENTCTRLLOGIC_H__777D44E7_A11A_4507_AA4E_F26B05745548__INCLUDED_)

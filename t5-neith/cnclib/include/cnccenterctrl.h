#include "cnsSession.h"

//用于屏幕的取位
const int adwTagArray[MAX_CENTREDFSCREEN_GROUP_NUM] = {0x01,0x02,0x04,0x08,0x10};

class CCncCenterCtrl : public CCncCenterCtrlIF
{
public:
	CCncCenterCtrl(CCnsSession &cSession);
	virtual ~CCncCenterCtrl();

    /** 
	* 功能  设置系统电源
	* @param [in]  emPower  开关
	* @return 
	* @remarks 
	*/
    virtual u16 SetSysPower( EmSchPower emPower );
    /** 
	* 功能  设置灯光全开
	* @param [in]  emLight  开关
	* @return 
	* @remarks 
	*/
    virtual u16 SetSysAllLight( EmSchLight emLight );
    /** 
	* 功能  设置主灯开关
	* @param [in]  emMidLight  开关
	* @return 
	* @remarks 
	*/
    virtual u16 SetMainLight( EmSchMidLight emMidLight );
    /** 
	* 功能  设置辅灯开关
	* @param [in]  emScrLight  开关
	* @return 
	* @remarks 
	*/
    virtual u16 SetAuxiLight( EmSchScrLight emScrLight );
    /** 
	* 功能  选择电视机
	* @param [in]  byIndex  序号
	* @return 
	* @remarks 
	*/
	virtual u16 SelectTV( u8 byIndex );
    /** 
	* 功能  设置电视机电源
	* @param [in]  byIndex  序号
	* @return 
	* @remarks 
	*/
    virtual u16 SetTVPowerMode( u8 byIndex, EmTvPowerMode emMode );
    /** 
	* 功能  设置电视机视听模式
	* @param [in]  emMode  视听模式
	* @return 
	* @remarks 
	*/
    virtual u16 SetTVAudVisMode( EmTVAudVisMode emMode );
    /** 
	* 功能  设置电视机显示模式
	* @param [in]  emMode  显示模式
	* @return 
	* @remarks 
	*/
    virtual u16 SetTVDisMode( EmTVDisplayMode emMode );
    /** 
	* 功能  设置电视机输入选择
	* @param [in]  emSelect  输入选择
	* @return 
	*/
    virtual u16 SetTVInpSelect( u8 byIndex, EmTVInputSelect emSelect );
    /** 
	* 功能  获取电视机配置
	* @return TCentreTVCfg*  电视机配置数组指针
	*/
    virtual TCentreTVCfg* GetCentreTVCfg(); 

    /** 
	* 功能  切换文档摄像机
    * @param [in]  byIndex  索引
	* @return 
	*/
    virtual u16 SetDCamSelectIndex( u8 byIndex );
    /** 
	* 功能  设置文档摄像机开关
    * @param [in]  emPower  开关
	* @return 
	*/
    virtual u16 SetDCamPower( EmDCamPower emPower );
    /** 
	* 功能  设置文档摄像机自动聚焦模式
	* @param [in]  emMode  模式
	* @return 
	*/
    virtual u16 SetDCamAFMode( EmDCamAFMode emMode );
    /** 
	* 功能  设置文档摄像机白平衡模式
	* @param [in]  emMode  模式
	* @return 
	*/
    virtual u16 SetDCamWBMode( EmDCamWBMode emMode );
    /** 
	* 功能  设置文档摄像机视野调节
	* @param [in]  emZoom  视野
	* @return 
	*/
    virtual u16 SetDCamZoom( EmDCamZoom emZoom, EmCnAction emAction );
    /** 
	* 功能  保存文档摄像机预置位
	* @param [in]  emNum  预置位
	* @return 
	*/
    virtual u16 SetDCamPreset( EmDCamPresetNum emNum );
    /** 
	* 功能  调度文档摄像机预置位
	* @param [in]  emNum  预置位
	* @return 
	*/
    virtual u16 SetDCamRecall( EmDCamRecallNum emNum );
	//文档摄像机快照请求
	virtual u16 SetDocSnapShot();
    /** 
	* 功能  调度空调温度
	* @param [in]  wTemp  温度
	* @return 
	*/
    virtual u16 SetACTemp( u8 byTemp );
    /** 
	* 功能  调度空调温度
	* @param [in]  wTemp  温度
	* @return 
	*/
    u16 SetACMode( EmCentreACMode emMode );
    /** 
	* 功能  调度新风风量
	* @param [in]  byVol  风量
	* @return 
	*/
    virtual u16 SetACAirVol( EmCentreACAirVol emAirVol );
	/** 
	* 功能  调度空调界面电源开关
	* @param [in] empower 电源开关
	* @return 
	*/
 /*   virtual u16 SetAirPower( EmCentreACPower emPower );*/
    /** 
	* 功能  选择空调
	* @param [in]  byIndex  序号
	* @return 
	* @remarks 
	*/
	virtual u16 SelectAir( u8 byIndex );
	/** 
	* 功能  获取空调界面配置
	* @return TCentreACCfg*  空调界面数组指针
	*/
    virtual void GetCentreACCfg( vector<TCentreACCfg> &vecTCentreACCfg )const; 
    /** 
	* 功能  设置空调电源
	* @param [in]  byIndex  序号
	* @return 
	* @remarks 
	*/
    virtual u16 SetACPowerMode( u8 byIndex, EmCentreACPower emMode );
    /** 
	* 功能  设置应用到所有
	* @param [in]  byIndex  序号
	* @return 
	* @remarks 
	*/
	virtual u16 SetApplyToAll( u8 byIndex, BOOL** atbIsCtrl );
	virtual BOOL* GetApplyToAllInfo();

	virtual u16 SetCurtainState( BOOL** bIsCtrl, EmCurAction emCurAction );

	void GetCentreCurName( vector<TCentreCurName> &vecTCentreCurName )const;
	void GetCentreCurInfor( TCentreCurInfo &tCentreCurInfo )const;
	
	void GetCentreSchCfg( TCentreSchCfg &tCentreSchCfg )const;
    void GetDCamStateMap( map<u8, TCentreDCamCfg> &mapDCamCfg ) const;
    void GetDCamState( TCentreDCamCfg &tCentreDCamCfg, u8 byIndex ) const;

	virtual u16 SetDisplayCmd( BOOL bDisplay, const TTPDisplayType& tDisplayType1, const TTPDisplayType& tDisplayType3 );
	virtual const BOOL& GetDisplayState() const;
	virtual const TTPDisplayType& GetDisplayType1() const;
	virtual const TTPDisplayType& GetDisplayType3() const;

    //矩阵
    virtual TTPMatrixSceneInfo* GetMatrixScneInfo(); 
    virtual TTPCurMatrixInfo GetCurMatrixInfo(); 
    virtual BOOL GetMatrixOnlineState(); 
    virtual TTPMatrixConfig GetMatrixConfig(); 

    virtual u16 SetMatrixInOutCmd( u32 dwIn, u32 dwOut );
    virtual u16 SaveMatrixScenceCmd( s32 dwIndex, s8* achName );
    virtual u16 ReNameMatrixScenceCmd( u32 dwIndex, s8* achName );
    virtual u16 DeleteMatrixScenceCmd( u32 dwIndex );
    virtual u16 ApplyMatrixScenceCmd( s32 dwIndex );

    //串口
    virtual EmComType* GetComType();

    //升降屏
    virtual u16 SelectCentreDFScreenCmd( u8 bySrceenControl );
    virtual u16 SetCentreDFScreenCmd(EmCommandType emCommand);

    virtual TCenDownOrFlipScreenInfo GetCenDownOrFlipScreenInfo();
    virtual BOOL* GetCenSelectDFScreen();

protected:
    virtual void OnTimeOut(u16 wEvent); 
    virtual void DispEvent(const CMessage &cMsg);
	void BuildEventsMap();
	//断链消息处理
	void OnLinkBreak(const CMessage& cMsg);


    //控制面板
    void OnSchStateNty(const CMessage& cMsg);
    void OnSchTempNty(const CMessage& cMsg);
    void OnSchPowerInd(const CMessage& cMsg);
    void OnSchPowerNty(const CMessage& cMsg);
    void OnSchLightInd(const CMessage& cMsg);
    void OnSchMainLightInd(const CMessage& cMsg);
    void OnSchAuxiLightInd(const CMessage& cMsg);
    void OnSchLightNty(const CMessage& cMsg);
    //显示器
    void OnTvStateNty(const CMessage& cMsg);
	void OnTvModeNty(const CMessage& cMsg);
    void OnSeleteTvInd(const CMessage& cMsg);
    void OnTvPowerModeInd(const CMessage& cMsg);
    void OnTvAudVisModeInd(const CMessage& cMsg);
    void OnTvDisModeInd(const CMessage& cMsg);
    void OnTvAudInpSelectInd(const CMessage& cMsg);
    //文档摄像机
    void SetDCamSelectIndexNty(const CMessage& cMsg);
    void SetDCamSelectIndexInd(const CMessage& cMsg);
    void OnDCamPowerInd(const CMessage& cMsg);
    void OnDCamAFModeInd(const CMessage& cMsg);
    void OnDCamWBModeInd(const CMessage& cMsg);
    void OnDCamZoomInd(const CMessage& cMsg);
    void OnDCamPresetInd(const CMessage& cMsg);
    void OnDCamRecallInd(const CMessage& cMsg);
    void OnDCamStateNty(const CMessage& cMsg);
    //空调
    void OnACTempInd(const CMessage& cMsg);
    void OnACModeInd(const CMessage& cMsg);
    void OnACAirVolInd(const CMessage& cMsg);
    void OnACStateNty(const CMessage& cMsg);
	void OnAirPowerInd(const CMessage& cMsg);
	void OnSeleteAirInd(const CMessage& cMsg);
	void OnApplyToAllInd(const CMessage& cMsg);
	//窗帘
	void OnCurtainStateNty(const CMessage& cMsg);
	void OnCurtainActInd(const CMessage& cMsg);
	//摄像机
	void OnCamStateNty(const CMessage& cMsg);
    //中控待机
    void OnCentreSleepNty(const CMessage& cMsg);
	//中控电视墙
	void OnDisplayInd(const CMessage& cMsg);
	//文档摄像机快照
	void OnDocSetSnapShotInd( const CMessage& cMsg );
	//辅视频源  如果选了第二个 进行限制
	void OnMinVedioInfo( const CMessage& cMsg );
	//文档摄像机类型通知
	void OnDCamTypeInd(const CMessage& cMsg);
    //显示器界面显示
    void OnDisplayShowNty(const CMessage& cMsg);
    void OnDisplayShowInd(const CMessage& cMsg);

    //矩阵
    void OnMatrixConfigNty(const CMessage& cMsg);
    void OnMatrixOnlineStateNty(const CMessage& cMsg);
    void OnMatrixSceneInfoNty(const CMessage& cMsg);
    void OnCurMatrixInfoNty(const CMessage& cMsg);
    void OnSaveMatrixSceneInd(const CMessage& cMsg);
    void OnReNameMatrixSceneInd(const CMessage& cMsg);
    void OnDeleteMatrixSceneInd(const CMessage& cMsg);
    void OnApplyMatrixSceneInd(const CMessage& cMsg);
    void OnChangeMatrixOutInRelationInd(const CMessage& cMsg);
    void OnMatrixOutInRelationNty(const CMessage& cMsg);

    //串口配置
    void OnSelectComNty( const CMessage& cMsg );
    void OnSelectComInd( const CMessage& cMsg );

    //升降屏
    void OnCentreDFScreenConfigNty(const CMessage& cMsg);
    void OnSelectDFScreenNty(const CMessage& cMsg);
    void OnScreenTypeNty(const CMessage& cMsg);
    void OnCentreModifydDFScreenGroupInd(const CMessage& cMsg);
    void OnCentreDFScreenCommandInd(const CMessage& cMsg);
    void OnSelectDFScreenInd(const CMessage& cMsg);

private:
	CCnsSession				*m_pSession;
    TCentreTVCfg            m_atTVCfg[MAX_CENTRETV_NUM];   //显示器信息 
	#define MAX_AC_NUM      3                         //冗余
	vector<TCentreACCfg>	m_vecTCentreACCfg;		//空调显示器信息
	vector<TCentreCurName>  m_vecTCentreCurName;    //窗帘名称向量

	TCentreCurInfo m_tCentreCurInfo;//空调信息 dyy 2015-6-15

	BOOL m_atbIsCtrl[3];
	BOOL m_bIsCtrl[3];
	BOOL m_bDisplay;
	TTPDisplayType m_tDisplayType1;
	TTPDisplayType m_tDisplayType3;

	TCentreSchCfg m_tCentreSchCfg;//中控开关信息 dyy 2015-6-17
	map<u8, TCentreDCamCfg> m_mapDCamCfg;//文档摄像机信息 dyy 2015-6-18(ylp 2019-3-21 扩展为容器<标号， 数据>)

    //矩阵
    TTPMatrixConfig     m_tTPMatrixConfig;                                  //矩阵配置信息
    TTPMatrixSceneInfo  m_atTPMatrixSceneInfo[TP_MATRIX_SCENENUM_MAX];      //矩阵预案
    TTPCurMatrixInfo    m_tTPCurMatrixInfo;                                 //当前操作矩阵预案
    BOOL                m_bIsMatrixOnline;                                  //矩阵是否在线

    //升降屏
    TCenDownOrFlipScreenInfo m_tCenDownOrFlipScreenInfo;                    //升降屏配置信息
    BOOL m_abSelectDFScreen[MAX_CENTREDFSCREEN_GROUP_NUM];                  //升降屏排数选择状态

    //com2 com3 串口类型
    EmComType m_aemComType[2];
};
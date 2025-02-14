/*****************************************************************************
模块名      : CCameraLogic
文件名      : cameraLogic.h
相关文件    : 
文件实现功能: 摄像调节页面
作者        : 石城
版本        : V1.0  Copyright(C) 2013-2014 KDV, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2013/2/29      1.0         石城			创建
******************************************************************************/
#if !defined(AFX_CAMERALOGIC_H__AE177678_C26D_4119_9634_55EB6A4DB4C3__INCLUDED_)
#define AFX_CAMERALOGIC_H__AE177678_C26D_4119_9634_55EB6A4DB4C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "logicBase.h"

class CCameraLogic : public CLogicBase, public ISingleTon<CCameraLogic>
{
public:
	CCameraLogic();
	virtual ~CCameraLogic();


public:  
	/**	 
	* 功能:  注册该logic中所有的回调函数
	* @return  void
	* @remarks	在此注册的回调函数主要是控件的响应事件，多为成员回调函数,也可注册其他的回调函数
	                    注册时，应注意回调函数对应名子的命名规则：“类名::函数名”
	*/
	virtual void RegCBFun();

	/**	 
	* 功能:  注册要处理的消息
	* @return  void
	* @remarks	 注册后该消息才能发送到本逻辑模块，否则，接收不到该消息
	*/
	virtual void RegMsg(); 

	/**	 
	* 功能:  注销某回调函数
	* @return  void
	* @remarks	 
	*/
	virtual void UnRegFunc( );

	/**	 
	* 功能:  注销某回调函数
	* @return  void
	* @remarks	 
	*/
	virtual void UnRegMsg();

	/**	 
	* 功能:  初始化窗口数据,
	* @return  void
	* @remarks	由于该功能被归属到控件属性并在xml中与控件绑定，
	                    需要在RegCBFun（）中注册到回调函数管理模块 
	*/
    virtual bool InitWnd( const IArgs & arg );

   /**	 
	* 功能:  清空所有状态和数据
	* @return   
	* @remarks	 
	*/
    virtual void Clear();

	/**	 
	* 功能:  恢复界面控件状态    by xhx
	* @return   
	* @remarks	 
	*/
    void ResetNormal();

protected:
	/**	 
	* 功能:  单击ComboboxCamera某项的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnComboboxCameraClick( const IArgs& args );

	/**	 
	* 功能:  单击Camera型号的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnCameraClick( const IArgs& args );

	/**	 
	* 功能:  单击sony光圈的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnComboboxApertureClick( const IArgs& args );

	/**	 
	* 功能:  单击sony降噪开关的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnSwitchNRClick( const IArgs& args );

	/**	 
	* 功能:  选择sony降噪等级的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnComboboxNRClick( const IArgs& args );
	
	/**	 
	* 功能:  选择sony背光调节的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnSwitchsonyBLClick( const IArgs& args );
	
	/**	 
	* 功能:  单击BtnImport的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnImportClick( const IArgs& args );

	/**	 
	* 功能:  单击BtnExport的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnExportClick( const IArgs& args );

	/**	 
	* 功能:  单击BtnResume的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnResumeClick( const IArgs& args );

	/**	 
	* 功能:  单击BtnUpgradeCamera的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnUpgradeCameraClick( const IArgs& args );	

	//zoom+- 按下和抬起事件响应
	bool OnBtnZoomPlusDown( const IArgs& args );
	bool OnBtnZoomPlusUp( const IArgs& args );
	bool OnBtnZoomSubDown( const IArgs& args );
	bool OnBtnZoomSubUp( const IArgs& args );

	/**	 
	* 功能:  单击BtnZoomPlus的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnZoomPlusClick( const IArgs& args );	

	/**	 
	* 功能:  单击BtnZoomSub的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnZoomSubClick( const IArgs& args );

	/**	 
	* 功能: BtnZoomPlusHold的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnZoomPlusHold( const IArgs& args );

	/**	 
	* 功能: BtnZoomPlusHoldEnd的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnZoomPlusHoldEnd( const IArgs& args );

	/**	 
	* 功能: BtnZoomSubHold的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnZoomSubHold( const IArgs& args );

	/**	 
	* 功能: BtnZoomSubHoldEnd的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnZoomSubHoldEnd( const IArgs& args );

	/**	 
	* 功能:  单击BtnVisualFieldSwitch的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnSwitchVisualFieldClick( const IArgs& args );
	
	/**	 
	* 功能:  单击BtnPresetSave的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnPresetSaveClick( const IArgs& args );

	/**	 
	* 功能:  单击BtnManuelFocus的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnManuelFocusClick( const IArgs& args );

	/**	 
	* 功能:  单击BtnAutoFocus的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnAutoFocusClick( const IArgs& args );

	/**	 
	* 功能:  BtnFocusNear的LBtnDown的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnLBtnDownFocusNear( const IArgs& args );

	/**	 
	* 功能:  BtnFocusNear的LBtnUp的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnLBtnUpFocusNear( const IArgs& args );

	/**	 
	* 功能:  BtnFocusFar的LBtnDown的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnLBtnDownFocusFar( const IArgs& args );

	/**	 
	* 功能:  BtnFocusFar的LBtnUp的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnLBtnUpFocusFar( const IArgs& args );

	/**	 
	* 功能:  单击BtnSwitchPersetMove的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnSwitchPersetMove( const IArgs& args );
	

	/**	 
	* 功能:  单击BtnSwitchAutoExposure的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnSwitchAutoExposure( const IArgs& args );

	/**	 
	* 功能:  SliderAperture值改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnSliderApertureChanged( const IArgs& args );

	/**	 
	* 功能:  单击BtnSwitchAutoWB的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnSwitchAutoWB( const IArgs& args );

	/**	 
	* 功能:  SliderRGain值改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnSliderRGainChanged( const IArgs& args );

	/**	 
	* 功能:  SliderBGain值改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnSliderBGainChanged( const IArgs& args );

	/**	 
	* 功能:  BtnRadioLamp的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnRadioLamp( const IArgs& args );

	/**	 
	* 功能:  BtnRadioSun的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnRadioSun( const IArgs& args );

	/**	 
	* 功能:  BtnRadioManuel的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnRadioManuel( const IArgs& args );

	/**	 
	* 功能:  单击BtnGammaOn的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnGammaOnClick( const IArgs& args );

	/**	 
	* 功能:  单击BtnGammaOff的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnGammaOffClick( const IArgs& args );

	/**	 
	* 功能:  单击BtnSmart1的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnSmart1Click( const IArgs& args );

	/**	 
	* 功能:  单击BtnSmart2的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnSmart2Click( const IArgs& args );

	/**	 
	* 功能:  SliderPicSceApt值改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnSliderPicSceAptChanged( const IArgs& args );

	/**	 
	* 功能:  SliderContrast值改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnSliderContrastChanged( const IArgs& args );

	/**	 
	* 功能:  SliderContrast值改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnSliderSaturatChanged( const IArgs& args );

	/**	 
	* 功能:  SliderHue值改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnSliderHueChanged( const IArgs& args );

	/**	 
	* 功能:  SliderBright值改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnSliderBrightChanged( const IArgs& args );

	/**	 
	* 功能: 单击BtnNROff的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnNROffClick( const IArgs& args );

	/**	 
	* 功能: 单击BtnNRLow的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnNRLowClick( const IArgs& args );

	/**	 
	* 功能: 单击BtnNRMid的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnNRMidClick( const IArgs& args );

	/**	 
	* 功能: 单击BtnNRHigh的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnNRHighClick( const IArgs& args );

	/**	 
	* 功能: 单击BtnSwitch3DNR的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnBtnSwitch3DNRClick( const IArgs& args );

	/**	 
	* 功能: 单击ComboboxExposureGain某一项的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnComboboxExposureGainClick( const IArgs& args );

	/**	 
	* 功能: 单击ComboboxShut某一项的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnComboboxShutClick( const IArgs& args );
	
	/**	 
	* 功能: 单击ComboboxBackLight某一项的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnComboboxBackLightClick( const IArgs& args );
	
	/**	 
	* 功能: EdtAperture改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnEdtApertureChange( const IArgs& args );	

	/**	 
	* 功能: EdtAperture失去焦点的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnKillFocusEdtAperture( const IArgs& args );	

	/**	 
	* 功能: EdtRGain改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnEdtRGainChange( const IArgs& args );

	/**	 
	* 功能: EdtRGain失去焦点的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnKillFocusEdtRGain( const IArgs& args );

	/**	 
	* 功能: EdtBGain改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnEdtBGainChange( const IArgs& args );

	/**	 
	* 功能: EdtBGain失去焦点的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnKillFocusEdtBGain( const IArgs& args );

	/**	 
	* 功能: EdtPicSceApt改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnEdtPicSceAptChange( const IArgs& args );

	/**	 
	* 功能: EdtPicSceApt失去焦点的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnKillFocusEdtPicSceApt( const IArgs& args );

	/**	 
	* 功能: EdtContrast改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnEdtContrastChange( const IArgs& args );

	/**	 
	* 功能: EdtContrast失去焦点的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnKillFocusEdtContrast( const IArgs& args );

	/**	 
	* 功能: EdtSaturat改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnEdtSaturatChange( const IArgs& args );

	/**	 
	* 功能: EdtSaturat失去焦点的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnKillFocusEdtSaturat( const IArgs& args );

	/**	 
	* 功能: EdtHue改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnEdtHueChange( const IArgs& args );

	/**	 
	* 功能: EdtHue失去焦点的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnKillFocusEdtHue( const IArgs& args );
	
	/**	 
	* 功能: EdtBright改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnEdtBrightChange( const IArgs& args );

	/**	 
	* 功能: EdtBright失去焦点的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnKillFocusEdtBright( const IArgs& args );

	/**	 
	* 功能: EdtZoom改变的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnEdtZoomChange( const IArgs& args );

	/**	 
	* 功能: EdtZoom失去焦点的响应函数
	* @return   bool
	* @remarks	 
	*/
	bool OnEdtZoomKillFocus( const IArgs& args );
	
	

protected:
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_CFG_NTY的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraCfgNty( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_SELECT_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraSelectInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_ZOOM_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraZoomInd( WPARAM wparam, LPARAM lparam );

	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_OSDSHOW_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraOSDShowInd( WPARAM wparam, LPARAM lparam );

	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_AUTO_FOCUS_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraAutoFocusInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_FOCUS_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraFocusInd( WPARAM wparam, LPARAM lparam );

	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_PERSET_MOVE_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraPersetMoveInd( WPARAM wparam, LPARAM lparam );

	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_AUTO_EXPOSURE_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraAutoExposureInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_GAIN_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraGainInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_SHUT_SPD_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraShutSpdInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_BACK_LIGHT_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraBackLightInd( WPARAM wparam, LPARAM lparam );

	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_APERTURE_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraApertureInd( WPARAM wparam, LPARAM lparam );

	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_AUTO_WB_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraAutoWBInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_RGAIN_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraRGainInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_BGAIN_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraBGainInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_PIC_SCENE_MODE_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraPicSceneModeInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_GAMMA_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraGammaInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_PIC_SCE_APT_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraPicSceAptInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_CONTRAST_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraContrastInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_SATURAT_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraSaturatInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_HUE_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraHueInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_BRIGHT_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraBrightInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_NR_STM_CLEAN_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraNRStmCleanInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_3DNR_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCamera3DNRInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_RESET_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraResetInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_PERSET_SAVE_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraPersetSaveInd( WPARAM wparam, LPARAM lparam );

	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_SET_CAMERA_ZOOM_IND的函数
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnSetCameraZoomInd( WPARAM wparam, LPARAM lparam );
	
	/**	 
	* 功能:  响应服务器发来的UI_CNSTOOL_CAMERA_POWER_ON_NTY的函数    by xhx
	* @return   LRESULT
	* @remarks	 
	*/
	HRESULT OnCameraPowerOnNty( WPARAM wparam, LPARAM lparam );

private:
	void GetBackLightValue( EmBackLightType &emBackLightType, u8 &byMulti );
	void GetShutSpdValue( EmShutSpd &emShutSpd );
	void GetGainValue( EmGain &emGain );
	void SetExpGainValue( EmGain emGain );
	void SetShutSpdValue( EmShutSpd emShutSpd );
	void SetBackLightValue( EmBackLightType emBackLightType, u8 byMulti );
	void SetAutoWB( EmWBMode emWBMode );
	void SetAutoExp( EmExpMode emExpMode );
	void SetCameraCfg( TCnCameraCfg tCnCameraCfg );
	void SetAptValue( u8 byApt );
	void SetPresetValue( BOOL bUsed );
	void SetOSDShowValue( EmOSDShow emOSDShow );
	void SetCamName( CString strCaption );
	void SetRGainValue( u16 wRGain );
	void SetBGainValue( u16 wBGain );
	void SetPicSceModeValue( EmPicSceMode emPicSceMode );
	void SetGammaValue( EmGamma emGamma );
	void SetPicSceAptValue( u8 byPicSceApt );
	void SetContrastValue( u16 wContrast );
	void SetSaturatValue( u16 wSaturat );
	void SetHueValue( u16 wHue );
	void SetBrightValue( u16 wBright );
	void SetNRStmCleanValue( EmStmClean emStmClean );
	void Set3DNRValue( Em3DNR em3DNR );
	void GetSonyIrisValue( EmSonyIris& emSonyIris );
	void GetStmClenValue( u8& bylevel );

	float StringToFloatTwo( CString str );
	CString OnlyFloat( CString str );

	void SetZoomValue( float fZoom );
	void SetZoomCmd( CString str );
	void SetApertureCmd( CString str );
	void SetRGainCmd( CString str );
	void SetBGainCmd( CString str );
	void SetPicSceAptCmd( CString str );
	void SetContrastCmd( CString str );
	void SetSaturatCmd( CString str );
	void SetHueCmd( CString str );
	void SetBrightCmd( CString str );

private:
	//该逻辑单元对应的所有控件名
	const String m_strComboboxCamera;
	const String m_strBtnSwitchVisualField;
	const String m_strBtnSwitchPersetMove;
	const String m_strComboboxExposureGain;
	const String m_strComboboxShut;
	const String m_strComboboxBackLight;
	const String m_strSliderAperture;
	const String m_strEdtAperture;
	const String m_strSliderRGain;
	const String m_strEdtRGain;
	const String m_strSliderBGain;
	const String m_strEdtBGain;
	const String m_strSliderPicSceApt;
	const String m_strEdtPicSceApt;
	const String m_strSliderContrast;
	const String m_strEdtContrast;
	const String m_strSliderSaturat;
	const String m_strEdtSaturat;
	const String m_strSliderHue;
	const String m_strEdtHue;
	const String m_strSliderBright;
	const String m_strEdtBright;
	const String m_strEdtZoom;
	const String m_strBtnSwitch3DNR;
	const String m_strBtnSwitchAutoExposure;
	const String m_strBtnSwitchAutoWB;
	const String m_strBtnZoomPlus;
	const String m_strBtnZoomSub;
	const String m_strStaticCamCategory;
	bool         m_bIsCameraPowerOn;   //摄像机是否正在上电   by xhx
	bool         m_bLoginByOther;      //tptools是否被抢登    by xhx
	EmCamType	 m_emCamType;		   //当前摄像机选择型号
	u8			 m_byCameraIndx;	   //当前摄像机编号
public:
	bool		 m_bZoomPlus;
	bool		 m_bZoomSub;
};

#define CAMERALOGICRPTR    CCameraLogic::GetSingletonPtr()               //Cameralogic指针

#endif // !defined(AFX_CAMERALOGIC_H__AE177678_C26D_4119_9634_55EB6A4DB4C3__INCLUDED_)

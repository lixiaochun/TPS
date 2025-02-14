#pragma once


class CSystem : public Singleton<CSystem>
{
public:
/*
	xml标签定义
*/
	static const String strMainWindow;	// 主界面
	static const String strSkin;		// 界面路径
	static const String strDefaultSkin; // 默认皮肤路径

public:
	CSystem(void);
	CSystem(String strFile);
	~CSystem(void);

/** 系统启动 
 *  @param[in] 主xml文件 
 *  @node 
 *  @return 是否成功
 */
	bool Launch();

protected:
/** 初始化系统 
 *  @param[in] 
 *  @node 主要是读取xml配置信息
 *  @return 是否成功
 */
	bool IniSystem();

protected:
/*
	主文件路径
*/
	String m_strFile;
};



extern const String g_stcStrMainFrameDlg; 
extern const String g_stcStrLoginDlg;
extern const String g_strMainMenuDlg;
extern const String g_strMessageBoxDlg;
extern const String g_strFilemanagerDlg;
extern const String g_strFilePreviewDlg;
extern const String g_strToolWnd;
extern const String g_strWbManager;
extern const String g_strVideoDlg;
extern const String g_strAboutDlg;
extern const String g_strModalDlg;
extern const String g_strNetCfgDlg;
extern const String g_strUpdateDlg;
extern const String g_strUdiscUpdateDlg;
extern const String g_strReconnectBox;
#ifndef __UILIST_H__
#define __UILIST_H__

#pragma once

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class CListHeaderUI;

#define UILIST_MAX_COLUMNS 32

typedef struct tagTListInfoUI
{
    int nColumns;
    RECT rcColumn[UILIST_MAX_COLUMNS];
    int nFont;
    UINT uTextStyle;
    RECT rcTextPadding;
    DWORD dwTextColor;
    DWORD dwBkColor;
    CStdString sBkImage;
    bool bAlternateBk;
    DWORD dwSelectedTextColor;
    DWORD dwSelectedBkColor;
    CStdString sSelectedImage;
    DWORD dwHotTextColor;
    DWORD dwHotBkColor;
    CStdString sHotImage;
    DWORD dwDisabledTextColor;
    DWORD dwDisabledBkColor;
    CStdString sDisabledImage;
    DWORD dwLineColor;
    bool bShowHtml;
    bool bMultiExpandable;
} TListInfoUI;


/////////////////////////////////////////////////////////////////////////////////////
//

class IListCallbackUI
{
public:
    virtual LPCTSTR GetItemText(CControlUI* pList, int iItem, int iSubItem) = 0;
};

class IListOwnerUI
{
public:
    virtual TListInfoUI* GetListInfo() = 0;
    virtual int GetCurSel() const = 0;
    virtual bool SelectItem(int iIndex) = 0;
    virtual void DoEvent(TEventUI& event) = 0;
};

class IListUI : public IListOwnerUI
{
public:
    virtual CListHeaderUI* GetHeader() const = 0;
    virtual CContainerUI* GetList() const = 0;
    virtual IListCallbackUI* GetTextCallback() const = 0;
    virtual void SetTextCallback(IListCallbackUI* pCallback) = 0;
    virtual bool ExpandItem(int iIndex, bool bExpand = true) = 0;
    virtual int GetExpandedItem() const = 0;
};

class IListItemUI
{
public:
    virtual int GetIndex() const = 0;
    virtual void SetIndex(int iIndex) = 0;
    virtual IListOwnerUI* GetOwner() = 0;
    virtual void SetOwner(CControlUI* pOwner) = 0;
    virtual bool IsSelected() const = 0;
    virtual bool Select(bool bSelect = true) = 0;
    virtual bool IsExpanded() const = 0;
    virtual bool Expand(bool bExpand = true) = 0;
    virtual void DrawItemText(HDC hDC, const RECT& rcItem) = 0;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CListBodyUI;
class CListHeaderUI;

class UILIB_API CListUI : public CVerticalLayoutUI, public IListUI
{
public:
    CListUI();

    LPCTSTR GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    bool GetScrollSelect();
    void SetScrollSelect(bool bScrollSelect);
    int GetCurSel() const;
    bool SelectItem(int iIndex);

    CListHeaderUI* GetHeader() const;  
    CContainerUI* GetList() const;
    TListInfoUI* GetListInfo();

    CControlUI* GetItemAt(int iIndex) const;
    int GetItemIndex(CControlUI* pControl) const;
    bool SetItemIndex(CControlUI* pControl, int iIndex);
    int GetCount() const;
    bool Add(CControlUI* pControl);
    bool AddAt(CControlUI* pControl, int iIndex);
    bool Remove(CControlUI* pControl);
    bool RemoveAt(int iIndex);
    void RemoveAll();

    void EnsureVisible(int iIndex);
    void Scroll(int dx, int dy);

    int GetChildPadding() const;
    void SetChildPadding(int iPadding);

    void SetItemFont(int index);
    void SetItemTextStyle(UINT uStyle);
    void SetItemTextPadding(RECT rc);
    void SetItemTextColor(DWORD dwTextColor);
    void SetItemBkColor(DWORD dwBkColor);
    void SetItemBkImage(LPCTSTR pStrImage);
    void SetAlternateBk(bool bAlternateBk);
    void SetSelectedItemTextColor(DWORD dwTextColor);
    void SetSelectedItemBkColor(DWORD dwBkColor);
    void SetSelectedItemImage(LPCTSTR pStrImage); 
    void SetHotItemTextColor(DWORD dwTextColor);
    void SetHotItemBkColor(DWORD dwBkColor);
    void SetHotItemImage(LPCTSTR pStrImage);
    void SetDisabledItemTextColor(DWORD dwTextColor);
    void SetDisabledItemBkColor(DWORD dwBkColor);
    void SetDisabledItemImage(LPCTSTR pStrImage);
    void SetItemLineColor(DWORD dwLineColor);
    bool IsItemShowHtml();
    void SetItemShowHtml(bool bShowHtml = true);
	RECT GetItemTextPadding() const;
	DWORD GetItemTextColor() const;
	DWORD GetItemBkColor() const;
	LPCTSTR GetItemBkImage() const;
    bool IsAlternateBk() const;
	DWORD GetSelectedItemTextColor() const;
	DWORD GetSelectedItemBkColor() const;
	LPCTSTR GetSelectedItemImage() const;
	DWORD GetHotItemTextColor() const;
	DWORD GetHotItemBkColor() const;
	LPCTSTR GetHotItemImage() const;
	DWORD GetDisabledItemTextColor() const;
	DWORD GetDisabledItemBkColor() const;
	LPCTSTR GetDisabledItemImage() const;
	DWORD GetItemLineColor() const;

    void SetMultiExpanding(bool bMultiExpandable); 
    int GetExpandedItem() const;
    bool ExpandItem(int iIndex, bool bExpand = true);

    void SetPos(RECT rc);
    void DoEvent(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    IListCallbackUI* GetTextCallback() const;
    void SetTextCallback(IListCallbackUI* pCallback);

    SIZE GetScrollPos() const;
    SIZE GetScrollRange() const;
    void SetScrollPos(SIZE szPos);

    void LineUp();
    void LineDown();
    void PageUp();
    void PageDown();
    void HomeUp();
    void EndDown();
    void LineLeft();
    void LineRight();
    void PageLeft();
    void PageRight();
    void HomeLeft();
    void EndRight();
    void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
    virtual CScrollBarUI* GetVerticalScrollBar() const;
    virtual CScrollBarUI* GetHorizontalScrollBar() const;

public:
	void SetDragWndIng(bool bDragWndIng){ m_bDragWndIng = bDragWndIng;};

public:
    bool m_bScrollSelect;
    int m_iCurSel;
    int m_iExpandedItem;
    IListCallbackUI* m_pCallback;
    CListBodyUI* m_pList;
    CListHeaderUI* m_pHeader;
    TListInfoUI m_ListInfo;

	//remark 滑动条滚动及拖拽
	POINT m_ptLButtonDown;      //鼠标按下位置
	POINT m_ptLButtonUp;	    //鼠标弹起位置
	HWND m_hwndItemDrag;        //拖拽条目显示窗口

	bool m_bDragWndIng;			//当前显示dragwnd窗口   此时禁止滚动列表
};

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CListBodyUI : public CVerticalLayoutUI
{
public:
    CListBodyUI(CListUI* pOwner);

	LPCTSTR GetClass() const;

    void SetScrollPos(SIZE szPos);
    void SetPos(RECT rc);
    void DoEvent(TEventUI& event);

protected:
    CListUI* m_pOwner;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CListHeaderUI : public CHorizontalLayoutUI
{
public:
    CListHeaderUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    SIZE EstimateSize(SIZE szAvailable);
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CListHeaderItemUI : public CControlUI
{
public:
    CListHeaderItemUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    void SetEnabled(bool bEnable = true);

	bool IsDragable() const;
    void SetDragable(bool bDragable);
	DWORD GetSepWidth() const;
    void SetSepWidth(int iWidth);
	DWORD GetTextStyle() const;
    void SetTextStyle(UINT uStyle);
	DWORD GetTextColor() const;
    void SetTextColor(DWORD dwTextColor);
	void SetTextPadding(RECT rc);
	RECT GetTextPadding() const;
    void SetFont(int index);
    bool IsShowHtml();
    void SetShowHtml(bool bShowHtml = true);
    LPCTSTR GetNormalImage() const;
    void SetNormalImage(LPCTSTR pStrImage);
    LPCTSTR GetHotImage() const;
    void SetHotImage(LPCTSTR pStrImage);
    LPCTSTR GetPushedImage() const;
    void SetPushedImage(LPCTSTR pStrImage);
    LPCTSTR GetFocusedImage() const;
    void SetFocusedImage(LPCTSTR pStrImage);
    LPCTSTR GetSepImage() const;
    void SetSepImage(LPCTSTR pStrImage);

    void DoEvent(TEventUI& event);
    SIZE EstimateSize(SIZE szAvailable);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    RECT GetThumbRect() const;

    void PaintText(HDC hDC);
    void PaintStatusImage(HDC hDC);

protected:
    POINT ptLastMouse;
    bool m_bDragable;
    UINT m_uButtonState;
    int m_iSepWidth;
    DWORD m_dwTextColor;
    int m_iFont;
    UINT m_uTextStyle;
    bool m_bShowHtml;
	RECT m_rcTextPadding;
    CStdString m_sNormalImage;
    CStdString m_sHotImage;
    CStdString m_sPushedImage;
    CStdString m_sFocusedImage;
    CStdString m_sSepImage;
    CStdString m_sSepImageModify;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CListElementUI : public CControlUI, public IListItemUI
{
public:
    CListElementUI();

    LPCTSTR GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetEnabled(bool bEnable = true);

    int GetIndex() const;
    void SetIndex(int iIndex);

    IListOwnerUI* GetOwner();
    void SetOwner(CControlUI* pOwner);
    void SetVisible(bool bVisible = true);

    bool IsSelected() const;
    bool Select(bool bSelect = true);
    bool IsExpanded() const;
    bool Expand(bool bExpand = true);

    void Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
    bool Activate();

    void DoEvent(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void DrawItemBk(HDC hDC, const RECT& rcItem);

protected:
    int m_iIndex;
    bool m_bSelected;
    UINT m_uButtonState;
    IListOwnerUI* m_pOwner;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CListLabelElementUI : public CListElementUI
{
public:
    CListLabelElementUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void DoEvent(TEventUI& event);
    SIZE EstimateSize(SIZE szAvailable);
    void DoPaint(HDC hDC, const RECT& rcPaint);

    void DrawItemText(HDC hDC, const RECT& rcItem);
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CListTextElementUI : public CListLabelElementUI
{
public:
    CListTextElementUI();
    ~CListTextElementUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    LPCTSTR GetText(int iIndex) const;
    void SetText(int iIndex, LPCTSTR pstrText);

    void SetOwner(CControlUI* pOwner);
    CStdString* GetLinkContent(int iIndex);

    void DoEvent(TEventUI& event);
    SIZE EstimateSize(SIZE szAvailable);

    void DrawItemText(HDC hDC, const RECT& rcItem);

protected:
    enum { MAX_LINK = 8 };
    int m_nLinks;
    RECT m_rcLinks[MAX_LINK];
    CStdString m_sLinks[MAX_LINK];
    int m_nHoverLink;
    IListUI* m_pOwner;
    CStdPtrArray m_aTexts;
};

/////////////////////////////////////////////////////////////////////////////////////
//
const static LPCTSTR pItemSelectLeft = _T("item_select_left.png");
const static LPCTSTR pItemSelectMiddle = _T("item_select_middle.png");
const static LPCTSTR pItemSelectRight = _T("item_select_right.png");

class UILIB_API CListContainerElementUI : public CContainerUI, public IListItemUI
{
public:
    CListContainerElementUI();

    LPCTSTR GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    int GetIndex() const;
    void SetIndex(int iIndex);

    IListOwnerUI* GetOwner();
    void SetOwner(CControlUI* pOwner);
    void SetVisible(bool bVisible = true);
    void SetEnabled(bool bEnable = true);

    bool IsSelected() const;
    bool Select(bool bSelect = true);
    bool IsExpanded() const;
    bool Expand(bool bExpand = true);

    void Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
    bool Activate();

    void DoEvent(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    void DoPaint(HDC hDC, const RECT& rcPaint);

    void DrawItemText(HDC hDC, const RECT& rcItem);    
    void DrawItemBk(HDC hDC, const RECT& rcItem);
	//////////////////////////////////////////////////////////////////////////
	void DrawMosaicItemBk(HDC hDC, const RECT& rcItem);   //拼接条目 by xhx
	void DrawMosaicSelectItemBk(HDC hDC, const RECT& rcItem); //by xhx

	void SetMosaicItemImage(LPCTSTR pStrImage);
	LPCTSTR GetMosaicItemImage();
	void SetSelBkPos(int nPos){ m_nSelBkPos = nPos; };
	int GetSetBkPos(){ return m_nSelBkPos; };

	void PreDrawMosaicSelectItemBk();
	//////////////////////////////////////////////////////////////////////////

	tString GetItemName(){return m_strItemName;};
	void SetItemName(const tString& strItemName){ m_strItemName = strItemName; };

protected:
	tString m_strItemName;

    int m_iIndex;
    bool m_bSelected;
    UINT m_uButtonState;
    IListOwnerUI* m_pOwner;

	//////////////////////////////////////////////////////////////////////////
	int m_nSelBkPos;

	tString m_strMosaicItemImg;
	tString m_strItemSelectLeft;
	tString m_strItemSelectMiddle;
	tString m_strItemSelectRight;
};

} // namespace DuiLib

#endif // __UILIST_H__

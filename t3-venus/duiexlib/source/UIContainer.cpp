#include "StdAfx.h"

namespace DuiLib {

/////////////////////////////////////////////////////////////////////////////////////
//
//
CContainerUI::CContainerUI() : 
m_iChildPadding(0),
m_bAutoDestroy(true),
m_bDelayedDestroy(true),
m_bMouseChildEnabled(true),
m_pVerticalScrollBar(NULL),
m_pHorizontalScrollBar(NULL),
m_bScrollProcess(false)
{
    ::ZeroMemory(&m_rcInset, sizeof(m_rcInset));
}

CContainerUI::~CContainerUI()
{
    m_bDelayedDestroy = false;
    RemoveAll();
    if( m_pVerticalScrollBar ) delete m_pVerticalScrollBar;
    if( m_pHorizontalScrollBar ) delete m_pHorizontalScrollBar;
}

LPCTSTR CContainerUI::GetClass() const
{
    return _T("ContainerUI");
}

LPVOID CContainerUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("IContainer")) == 0 ) return static_cast<IContainerUI*>(this);
    else if( _tcscmp(pstrName, _T("Container")) == 0 ) return static_cast<CContainerUI*>(this);
    return CControlUI::GetInterface(pstrName);
}

CControlUI* CContainerUI::GetItemAt(int iIndex) const
{
    if( iIndex < 0 || iIndex >= m_items.GetSize() ) return NULL;
    return static_cast<CControlUI*>(m_items[iIndex]);
}

int CContainerUI::GetItemIndex(CControlUI* pControl) const
{
    for( int it = 0; it < m_items.GetSize(); it++ ) {
        if( static_cast<CControlUI*>(m_items[it]) == pControl ) {
            return it;
        }
    }

    return -1;
}

bool CContainerUI::SetItemIndex(CControlUI* pControl, int iIndex)
{
    for( int it = 0; it < m_items.GetSize(); it++ ) {
        if( static_cast<CControlUI*>(m_items[it]) == pControl ) {
            NeedUpdate();            
            m_items.Remove(it);
            return m_items.InsertAt(iIndex, pControl);
        }
    }

    return false;
}

int CContainerUI::GetCount() const
{
    return m_items.GetSize();
}

bool CContainerUI::Add(CControlUI* pControl)
{
    if( pControl == NULL) return false;

    if( m_pManager != NULL ) m_pManager->InitControls(pControl, this);
    if( IsVisible() ) NeedUpdate();
    else pControl->SetInternVisible(false);
    return m_items.Add(pControl);   
}

bool CContainerUI::AddAt(CControlUI* pControl, int iIndex)
{
    if( pControl == NULL) return false;

    if( m_pManager != NULL ) m_pManager->InitControls(pControl, this);
    if( IsVisible() ) NeedUpdate();
    else pControl->SetInternVisible(false);
    return m_items.InsertAt(iIndex, pControl);
}

bool CContainerUI::Remove(CControlUI* pControl)
{
    if( pControl == NULL) return false;

    for( int it = 0; it < m_items.GetSize(); it++ ) {
        if( static_cast<CControlUI*>(m_items[it]) == pControl ) {
            NeedUpdate();
            if( m_bAutoDestroy ) {
                if( m_bDelayedDestroy && m_pManager ) m_pManager->AddDelayedCleanup(pControl);             
                else delete pControl;
            }
            return m_items.Remove(it);
        }
    }
    return false;
}

bool CContainerUI::RemoveAt(int iIndex)
{
    CControlUI* pControl = GetItemAt(iIndex);
    if (pControl != NULL) {
        return CContainerUI::Remove(pControl);
    }

    return false;
}

void CContainerUI::RemoveAll()
{
    for( int it = 0; m_bAutoDestroy && it < m_items.GetSize(); it++ ) {
        if( m_bDelayedDestroy && m_pManager ) m_pManager->AddDelayedCleanup(static_cast<CControlUI*>(m_items[it]));             
        else delete static_cast<CControlUI*>(m_items[it]);
    }
    m_items.Empty();
    NeedUpdate();
}

bool CContainerUI::IsAutoDestroy() const
{
    return m_bAutoDestroy;
}

void CContainerUI::SetAutoDestroy(bool bAuto)
{
    m_bAutoDestroy = bAuto;
}

bool CContainerUI::IsDelayedDestroy() const
{
    return m_bDelayedDestroy;
}

void CContainerUI::SetDelayedDestroy(bool bDelayed)
{
    m_bDelayedDestroy = bDelayed;
}

RECT CContainerUI::GetInset() const
{
    return m_rcInset;
}

void CContainerUI::SetInset(RECT rcInset)
{
    m_rcInset = rcInset;
    NeedUpdate();
}

int CContainerUI::GetChildPadding() const
{
    return m_iChildPadding;
}

void CContainerUI::SetChildPadding(int iPadding)
{
    m_iChildPadding = iPadding;
    NeedUpdate();
}

bool CContainerUI::IsMouseChildEnabled() const
{
    return m_bMouseChildEnabled;
}

void CContainerUI::SetMouseChildEnabled(bool bEnable)
{
    m_bMouseChildEnabled = bEnable;
}

void CContainerUI::SetVisible(bool bVisible)
{
    if( m_bVisible == bVisible ) return;
    CControlUI::SetVisible(bVisible);
    for( int it = 0; it < m_items.GetSize(); it++ ) {
        static_cast<CControlUI*>(m_items[it])->SetInternVisible(IsVisible());
    }
}

void CContainerUI::SetInternVisible(bool bVisible)
{
    CControlUI::SetInternVisible(bVisible);
    if( m_items.IsEmpty() ) return;
    for( int it = 0; it < m_items.GetSize(); it++ ) {
        static_cast<CControlUI*>(m_items[it])->SetInternVisible(IsVisible());
    }
}

void CContainerUI::SetMouseEnabled(bool bEnabled)
{
    if( m_pVerticalScrollBar != NULL ) m_pVerticalScrollBar->SetMouseEnabled(bEnabled);
    if( m_pHorizontalScrollBar != NULL ) m_pHorizontalScrollBar->SetMouseEnabled(bEnabled);
    CControlUI::SetMouseEnabled(bEnabled);
}

void CContainerUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->DoEvent(event);
        else CControlUI::DoEvent(event);
        return;
    }

    if( event.Type == UIEVENT_SETFOCUS ) 
    {
        m_bFocused = true;
        return;
    }
    if( event.Type == UIEVENT_KILLFOCUS ) 
    {
        m_bFocused = false;
        return;
    }
    if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() && m_pVerticalScrollBar->IsEnabled() )
    {
        if( event.Type == UIEVENT_KEYDOWN ) 
        {
            switch( event.chKey ) {
            case VK_DOWN:
             LineDown();
             return;
            case VK_UP:
             LineUp();
             return;
            case VK_NEXT:
             PageDown();
             return;
            case VK_PRIOR:
             PageUp();
             return;
            case VK_HOME:
             HomeUp();
             return;
            case VK_END:
             EndDown();
             return;
            }
        }
        else if( event.Type == UIEVENT_SCROLLWHEEL )
        {
            switch( LOWORD(event.wParam) ) {
            case SB_LINEUP:
              LineUp();
              return;
            case SB_LINEDOWN:
              LineDown();
              return;
            }
        }
    }
    else if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() && m_pHorizontalScrollBar->IsEnabled() ) {
        if( event.Type == UIEVENT_KEYDOWN ) 
        {
            switch( event.chKey ) {
            case VK_DOWN:
                LineRight();
                return;
            case VK_UP:
                LineLeft();
                return;
            case VK_NEXT:
                PageRight();
                return;
            case VK_PRIOR:
                PageLeft();
                return;
            case VK_HOME:
                HomeLeft();
                return;
            case VK_END:
                EndRight();
                return;
            }
        }
        else if( event.Type == UIEVENT_SCROLLWHEEL )
        {
            switch( LOWORD(event.wParam) ) {
            case SB_LINEUP:
                LineLeft();
                return;
            case SB_LINEDOWN:
                LineRight();
                return;
            }
        }
    }

	//触摸手势   2013.06.27 by ldy
	if( event.Type == UIEVENT_GESTURENOTIFY ) 
	{
		int test1 = 10;
		return;
	}
	if( event.Type == UIEVENT_GESTURE ) 
	{
		int test = 10;
		return;
	}

    CControlUI::DoEvent(event);
}

SIZE CContainerUI::GetScrollPos() const
{
    SIZE sz = {0, 0};
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) sz.cy = m_pVerticalScrollBar->GetScrollPos();
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) sz.cx = m_pHorizontalScrollBar->GetScrollPos();
    return sz;
}

SIZE CContainerUI::GetScrollRange() const
{
    SIZE sz = {0, 0};
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) sz.cy = m_pVerticalScrollBar->GetScrollRange();
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) sz.cx = m_pHorizontalScrollBar->GetScrollRange();
    return sz;
}

void CContainerUI::SetScrollPos(SIZE szPos)
{
    int cx = 0;
    int cy = 0;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
        int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
        m_pVerticalScrollBar->SetScrollPos(szPos.cy);
        cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
    }

    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
        int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
        m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
        cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
    }

    if( cx == 0 && cy == 0 ) return;

    RECT rcPos;
    for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
        CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) continue;

        rcPos = pControl->GetPos();
        rcPos.left -= cx;
        rcPos.right -= cx;
        rcPos.top -= cy;
        rcPos.bottom -= cy;
        pControl->SetPos(rcPos);
    }

    Invalidate();
}

void CContainerUI::LineUp(int nCyLine)
{
    int cyLine = nCyLine;
    if( m_pManager ) cyLine = m_pManager->GetDefaultFontInfo()->tm.tmHeight + cyLine;

    SIZE sz = GetScrollPos();
    sz.cy -= cyLine;
    SetScrollPos(sz);
}

void CContainerUI::LineDown(int nCyLine)
{
    int cyLine = nCyLine;
    if( m_pManager ) cyLine = m_pManager->GetDefaultFontInfo()->tm.tmHeight + cyLine;

    SIZE sz = GetScrollPos();
    sz.cy += cyLine;
    SetScrollPos(sz);
}

void CContainerUI::PageUp()
{
    SIZE sz = GetScrollPos();
    int iOffset = m_rcItem.bottom - m_rcItem.top - m_rcInset.top - m_rcInset.bottom;
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) iOffset -= m_pHorizontalScrollBar->GetFixedHeight();
    sz.cy -= iOffset;
    SetScrollPos(sz);
}

void CContainerUI::PageDown()
{
	SIZE sz = GetScrollPos();
	int iOffset = m_rcItem.bottom - m_rcItem.top - m_rcInset.top - m_rcInset.bottom;
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) iOffset -= m_pHorizontalScrollBar->GetFixedHeight();
	sz.cy += iOffset;
	SetScrollPos(sz);
}

void CContainerUI::HomeUp()
{
    SIZE sz = GetScrollPos();
    sz.cy = 0;
    SetScrollPos(sz);
}

void CContainerUI::EndDown()
{
    SIZE sz = GetScrollPos();
    sz.cy = GetScrollRange().cy;
    SetScrollPos(sz);
}

void CContainerUI::LineLeft(int nCyLine)
{
    SIZE sz = GetScrollPos();
    sz.cx -= nCyLine;
    SetScrollPos(sz);
}

void CContainerUI::LineRight(int nCyLine)
{
    SIZE sz = GetScrollPos();
    sz.cx += nCyLine;
    SetScrollPos(sz);
}

void CContainerUI::PageLeft()
{
    SIZE sz = GetScrollPos();
    int iOffset = m_rcItem.right - m_rcItem.left - m_rcInset.left - m_rcInset.right;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) iOffset -= m_pVerticalScrollBar->GetFixedWidth();
    sz.cx -= iOffset;
    SetScrollPos(sz);
}

void CContainerUI::PageRight()
{
    SIZE sz = GetScrollPos();
    int iOffset = m_rcItem.right - m_rcItem.left - m_rcInset.left - m_rcInset.right;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) iOffset -= m_pVerticalScrollBar->GetFixedWidth();
    sz.cx += iOffset;
    SetScrollPos(sz);
}

void CContainerUI::HomeLeft()
{
    SIZE sz = GetScrollPos();
    sz.cx = 0;
    SetScrollPos(sz);
}

void CContainerUI::EndRight()
{
    SIZE sz = GetScrollPos();
    sz.cx = GetScrollRange().cx;
    SetScrollPos(sz);
}

void CContainerUI::EnableScrollBar(bool bEnableVertical, bool bEnableHorizontal)
{
    if( bEnableVertical && !m_pVerticalScrollBar ) {
        m_pVerticalScrollBar = new CScrollBarUI;

		if ( this->GetMosaicBk() )
		{
			//m_pVerticalScrollBar->SetMosaicBk(true);
			m_pVerticalScrollBar->SetMosaicSliderImage(GetBkImage());
			m_pVerticalScrollBar->PreDrawMosaicScroll();
		}

        m_pVerticalScrollBar->SetOwner(this);
        m_pVerticalScrollBar->SetManager(m_pManager, NULL, false);
        if ( m_pManager ) {
            LPCTSTR pDefaultAttributes = m_pManager->GetDefaultAttributeList(_T("VScrollBar"));
            if( pDefaultAttributes ) {
                m_pVerticalScrollBar->ApplyAttributeList(pDefaultAttributes);
            }
        }
    }
    else if( !bEnableVertical && m_pVerticalScrollBar ) {
        delete m_pVerticalScrollBar;
        m_pVerticalScrollBar = NULL;
    }

    if( bEnableHorizontal && !m_pHorizontalScrollBar ) {
        m_pHorizontalScrollBar = new CScrollBarUI;
        m_pHorizontalScrollBar->SetHorizontal(true);
        m_pHorizontalScrollBar->SetOwner(this);
        m_pHorizontalScrollBar->SetManager(m_pManager, NULL, false);
        if ( m_pManager ) {
            LPCTSTR pDefaultAttributes = m_pManager->GetDefaultAttributeList(_T("HScrollBar"));
            if( pDefaultAttributes ) {
                m_pHorizontalScrollBar->ApplyAttributeList(pDefaultAttributes);
            }
        }
    }
    else if( !bEnableHorizontal && m_pHorizontalScrollBar ) {
        delete m_pHorizontalScrollBar;
        m_pHorizontalScrollBar = NULL;
    }

    NeedUpdate();
}

CScrollBarUI* CContainerUI::GetVerticalScrollBar() const
{
    return m_pVerticalScrollBar;
}

CScrollBarUI* CContainerUI::GetHorizontalScrollBar() const
{
    return m_pHorizontalScrollBar;
}

int CContainerUI::FindSelectable(int iIndex, bool bForward /*= true*/) const
{
    // NOTE: This is actually a helper-function for the list/combo/ect controls
    //       that allow them to find the next enabled/available selectable item
    if( GetCount() == 0 ) return -1;
    iIndex = CLAMP(iIndex, 0, GetCount() - 1);
    if( bForward ) {
        for( int i = iIndex; i < GetCount(); i++ ) {
            if( GetItemAt(i)->GetInterface(_T("ListItem")) != NULL 
                && GetItemAt(i)->IsVisible()
                && GetItemAt(i)->IsEnabled() ) return i;
        }
        return -1;
    }
    else {
        for( int i = iIndex; i >= 0; --i ) {
            if( GetItemAt(i)->GetInterface(_T("ListItem")) != NULL 
                && GetItemAt(i)->IsVisible()
                && GetItemAt(i)->IsEnabled() ) return i;
        }
        return FindSelectable(0, true);
    }
}

void CContainerUI::SetPos(RECT rc)
{
    CControlUI::SetPos(rc);
    if( m_items.IsEmpty() ) return;
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.top;
    rc.right -= m_rcInset.right;
    rc.bottom -= m_rcInset.bottom;

    for( int it = 0; it < m_items.GetSize(); it++ ) {
        CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) {
            SetFloatPos(it);
        }
        else {
            pControl->SetPos(rc); // 所有非float子控件放大到整个客户区
        }
    }
}

void CContainerUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("inset")) == 0 ) {
        RECT rcInset = { 0 };
        LPTSTR pstr = NULL;
        rcInset.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        rcInset.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
        rcInset.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
        rcInset.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
        SetInset(rcInset);
    }
    else if( _tcscmp(pstrName, _T("mousechild")) == 0 ) SetMouseChildEnabled(_tcscmp(pstrValue, _T("true")) == 0);
    else if( _tcscmp(pstrName, _T("vscrollbar")) == 0 ) {
        EnableScrollBar(_tcscmp(pstrValue, _T("true")) == 0, GetHorizontalScrollBar() != NULL);
    }
    else if( _tcscmp(pstrName, _T("vscrollbarstyle")) == 0 ) {
        EnableScrollBar(true, GetHorizontalScrollBar() != NULL);
        if( GetVerticalScrollBar() ) GetVerticalScrollBar()->ApplyAttributeList(pstrValue);
    }
    else if( _tcscmp(pstrName, _T("hscrollbar")) == 0 ) {
        EnableScrollBar(GetVerticalScrollBar() != NULL, _tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("hscrollbarstyle")) == 0 ) {
        EnableScrollBar(GetVerticalScrollBar() != NULL, true);
        if( GetHorizontalScrollBar() ) GetHorizontalScrollBar()->ApplyAttributeList(pstrValue);
    }
    else if( _tcscmp(pstrName, _T("childpadding")) == 0 ) SetChildPadding(_ttoi(pstrValue));
    else {CControlUI::SetAttribute(pstrName, pstrValue);}
}

void CContainerUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
{
    for( int it = 0; it < m_items.GetSize(); it++ ) {
        static_cast<CControlUI*>(m_items[it])->SetManager(pManager, this, bInit);
    }
    
    if( m_pVerticalScrollBar != NULL ) m_pVerticalScrollBar->SetManager(pManager, this, bInit);
    if( m_pHorizontalScrollBar != NULL ) m_pHorizontalScrollBar->SetManager(pManager, this, bInit);
    CControlUI::SetManager(pManager, pParent, bInit);
}

CScrollBarUI* CContainerUI::GetVerticalScrollBar()
{
	return m_pVerticalScrollBar;
}
CScrollBarUI* CContainerUI::GetHorizontalScrollBar()
{
	return m_pHorizontalScrollBar;
}

CControlUI* CContainerUI::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags)
{
    // Check if this guy is valid
    if( (uFlags & UIFIND_VISIBLE) != 0 && !IsVisible() ) return NULL;
    if( (uFlags & UIFIND_ENABLED) != 0 && !IsEnabled() ) return NULL;
    if( (uFlags & UIFIND_HITTEST) != 0 ) {
        if( !::PtInRect(&m_rcItem, *(static_cast<LPPOINT>(pData))) ) return NULL;
        if( !m_bMouseChildEnabled ) {
            CControlUI* pResult = NULL;
            if( m_pVerticalScrollBar != NULL ) pResult = m_pVerticalScrollBar->FindControl(Proc, pData, uFlags);
            if( pResult == NULL && m_pHorizontalScrollBar != NULL ) pResult = m_pHorizontalScrollBar->FindControl(Proc, pData, uFlags);
            if( pResult == NULL ) pResult = CControlUI::FindControl(Proc, pData, uFlags);
            return pResult;
        }
    }

    CControlUI* pResult = NULL;
    if( m_pVerticalScrollBar != NULL ) pResult = m_pVerticalScrollBar->FindControl(Proc, pData, uFlags);
    if( pResult == NULL && m_pHorizontalScrollBar != NULL ) pResult = m_pHorizontalScrollBar->FindControl(Proc, pData, uFlags);
    if( pResult != NULL ) return pResult;

    if( (uFlags & UIFIND_ME_FIRST) != 0 ) {
        CControlUI* pControl = CControlUI::FindControl(Proc, pData, uFlags);
        if( pControl != NULL ) return pControl;
    }
    RECT rc = m_rcItem;
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.top;
    rc.right -= m_rcInset.right;
    rc.bottom -= m_rcInset.bottom;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
    if( (uFlags & UIFIND_TOP_FIRST) != 0 ) {
        for( int it = m_items.GetSize() - 1; it >= 0; it-- ) {
			CControlUI* pFindControl = NULL;
			pFindControl = static_cast<CControlUI*>(m_items[it])->FindControl(Proc, pData, uFlags);
			if ( NULL != pFindControl )
			{
				CControlUI* pControl = static_cast<CControlUI*>(pFindControl);
				if( pControl != NULL ) {
					if( (uFlags & UIFIND_HITTEST) != 0 && !pControl->IsFloat() && !::PtInRect(&rc, *(static_cast<LPPOINT>(pData))) )
						continue;
					else 
						return pControl;
				}  
			}
        }
    }
    else {
        for( int it = 0; it < m_items.GetSize(); it++ ) {
            CControlUI* pControl = static_cast<CControlUI*>(m_items[it])->FindControl(Proc, pData, uFlags);
            if( pControl != NULL ) {
                if( (uFlags & UIFIND_HITTEST) != 0 && !pControl->IsFloat() && !::PtInRect(&rc, *(static_cast<LPPOINT>(pData))) )
                    continue;
                else 
                    return pControl;
            } 
        }
    }

    if( pResult == NULL ) pResult = CControlUI::FindControl(Proc, pData, uFlags);
    return pResult;
}

void CContainerUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
    RECT rcTemp = { 0 };
    if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return;

    CRenderClip clip;
    CRenderClip::GenerateClip(hDC, rcTemp, clip);
	CControlUI::DoPaint(hDC, rcPaint);

    if( m_items.GetSize() > 0 ) {
        RECT rc = m_rcItem;
        rc.left += m_rcInset.left;
        rc.top += m_rcInset.top;
        rc.right -= m_rcInset.right;
        rc.bottom -= m_rcInset.bottom;
        if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
        if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

        if( !::IntersectRect(&rcTemp, &rcPaint, &rc) ) {
            for( int it = 0; it < m_items.GetSize(); it++ ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
                if( !pControl->IsVisible() ) continue;
                if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
                if( pControl ->IsFloat() ) {
                    if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
                    pControl->DoPaint(hDC, rcPaint);
                }
            }
        }
        else {
            CRenderClip childClip;
            CRenderClip::GenerateClip(hDC, rcTemp, childClip);
            for( int it = 0; it < m_items.GetSize(); it++ ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
                if( !pControl->IsVisible() ) continue;
                if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
                if( pControl ->IsFloat() ) {
                    if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
                    CRenderClip::UseOldClipBegin(hDC, childClip);
                    pControl->DoPaint(hDC, rcPaint);
                    CRenderClip::UseOldClipEnd(hDC, childClip);
                }
                else {
                    if( !::IntersectRect(&rcTemp, &rc, &pControl->GetPos()) ) continue;
                    pControl->DoPaint(hDC, rcPaint);
                }
            }
        }
    }

    if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() ) {
        if( ::IntersectRect(&rcTemp, &rcPaint, &m_pVerticalScrollBar->GetPos()) ) {
            m_pVerticalScrollBar->DoPaint(hDC, rcPaint);
        }
    }

    if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() ) {
        if( ::IntersectRect(&rcTemp, &rcPaint, &m_pHorizontalScrollBar->GetPos()) ) {
            m_pHorizontalScrollBar->DoPaint(hDC, rcPaint);
        }
    }
}

void CContainerUI::SetFloatPos(int iIndex)
{
    // 因为CControlUI::SetPos对float的操作影响，这里不能对float组件添加滚动条的影响
    if( iIndex < 0 || iIndex >= m_items.GetSize() ) return;

    CControlUI* pControl = static_cast<CControlUI*>(m_items[iIndex]);

    if( !pControl->IsVisible() ) return;
    if( !pControl->IsFloat() ) return;

    SIZE szXY = pControl->GetFixedXY();
    SIZE sz = {pControl->GetFixedWidth(), pControl->GetFixedHeight()};
    RECT rcCtrl = { 0 };
    if( szXY.cx >= 0 ) {
        rcCtrl.left = m_rcItem.left + szXY.cx;
        rcCtrl.right = m_rcItem.left + szXY.cx + sz.cx;
    }
    else {
        rcCtrl.left = m_rcItem.right + szXY.cx - sz.cx;
        rcCtrl.right = m_rcItem.right + szXY.cx;
    }
    if( szXY.cy >= 0 ) {
        rcCtrl.top = m_rcItem.top + szXY.cy;
        rcCtrl.bottom = m_rcItem.top + szXY.cy + sz.cy;
    }
    else {
        rcCtrl.top = m_rcItem.bottom + szXY.cy - sz.cy;
        rcCtrl.bottom = m_rcItem.bottom + szXY.cy;
    }
	if( pControl->IsRelativePos() )
	{
		TRelativePosUI tRelativePos = pControl->GetRelativePos();
		SIZE szParent = {m_rcItem.right-m_rcItem.left,m_rcItem.bottom-m_rcItem.top};
		if(tRelativePos.szParent.cx != 0)
		{
			int nIncrementX = szParent.cx-tRelativePos.szParent.cx;
			int nIncrementY = szParent.cy-tRelativePos.szParent.cy;
			rcCtrl.left += (nIncrementX*tRelativePos.nMoveXPercent/100);
			rcCtrl.top += (nIncrementY*tRelativePos.nMoveYPercent/100);
			rcCtrl.right = rcCtrl.left+sz.cx+(nIncrementX*tRelativePos.nZoomXPercent/100);
			rcCtrl.bottom = rcCtrl.top+sz.cy+(nIncrementY*tRelativePos.nZoomYPercent/100);
		}
		pControl->SetRelativeParentSize(szParent);
	}
    pControl->SetPos(rcCtrl);
}

void CContainerUI::ProcessScrollBar(RECT rc, int cxRequired, int cyRequired)
{
    if( m_pHorizontalScrollBar != NULL /*&& m_pHorizontalScrollBar->IsVisible()*/ ) {
        RECT rcScrollBarPos = { rc.left, rc.bottom, rc.right, rc.bottom + m_pHorizontalScrollBar->GetFixedHeight()};

		//ldy 2013.06.19  修正CContainerUI水平滚动条显示问题
		int nWidth =  cxRequired - (rc.right - rc.left);
		m_pHorizontalScrollBar->SetScrollRange(::abs(nWidth));

        m_pHorizontalScrollBar->SetPos(rcScrollBarPos);


		//ldy 2013.06.27  控制水平滚动条显示和隐藏
		if( cxRequired > rc.right - rc.left && !m_pHorizontalScrollBar->IsVisible() ) 
		{
			m_pHorizontalScrollBar->SetVisible(true);
			this->/*m_pParent->*/SetScrollVisible(true);
			m_pHorizontalScrollBar->SetScrollRange(cxRequired - (rc.right - rc.left));
			m_pHorizontalScrollBar->SetScrollPos(0);
		}
		// No scrollbar required
		if( !m_pHorizontalScrollBar->IsVisible() ) return;

		// Scroll not needed anymore?
		int cxScroll = cxRequired - (rc.right - rc.left);
		if( cxScroll <= 0 ) 
		{
			m_pHorizontalScrollBar->SetVisible(false);
			this->/*m_pParent->*/SetScrollVisible(false);
			m_pHorizontalScrollBar->SetScrollPos(0);
			m_pHorizontalScrollBar->SetScrollRange(0);
		}
    }

    if( m_pVerticalScrollBar == NULL ) return;

    if( cyRequired > rc.bottom - rc.top && !m_pVerticalScrollBar->IsVisible() ) {
        m_pVerticalScrollBar->SetVisible(true);
		this->/*m_pParent->*/SetScrollVisible(true);
        m_pVerticalScrollBar->SetScrollRange(cyRequired - (rc.bottom - rc.top));
        m_pVerticalScrollBar->SetScrollPos(0);
        m_bScrollProcess = true;
        SetPos(m_rcItem);
        m_bScrollProcess = false;
        return;
    }
    // No scrollbar required
    if( !m_pVerticalScrollBar->IsVisible() ) return;

    // Scroll not needed anymore?
    int cyScroll = cyRequired - (rc.bottom - rc.top);
    if( cyScroll <= 0 && !m_bScrollProcess) {
        m_pVerticalScrollBar->SetVisible(false);
		this->/*m_pParent->*/SetScrollVisible(false);
        m_pVerticalScrollBar->SetScrollPos(0);
        m_pVerticalScrollBar->SetScrollRange(0);
        SetPos(m_rcItem);
    }
    else
    {
        RECT rcScrollBarPos = { rc.right, rc.top, rc.right + m_pVerticalScrollBar->GetFixedWidth(), rc.bottom };
        m_pVerticalScrollBar->SetPos(rcScrollBarPos);

        if( m_pVerticalScrollBar->GetScrollRange() != cyScroll ) {
            int iScrollPos = m_pVerticalScrollBar->GetScrollPos();
			m_pVerticalScrollBar->SetScrollRange(::abs(cyScroll));
            if( m_pVerticalScrollBar->GetScrollRange() == 0 ) {
                m_pVerticalScrollBar->SetVisible(false);
				this->/*m_pParent->*/SetScrollVisible(false);
                m_pVerticalScrollBar->SetScrollPos(0);
            }
            if( iScrollPos > m_pVerticalScrollBar->GetScrollPos() ) {
                SetPos(m_rcItem);
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

CVerticalLayoutUI::CVerticalLayoutUI() : m_iSepHeight(0), m_uButtonState(0), m_bImmMode(false)
{
    ptLastMouse.x = ptLastMouse.y = 0;
    ::ZeroMemory(&m_rcNewPos, sizeof(m_rcNewPos));
}

LPCTSTR CVerticalLayoutUI::GetClass() const
{
    return _T("VerticalLayoutUI");
}

LPVOID CVerticalLayoutUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("VerticalLayout")) == 0 ) return static_cast<CVerticalLayoutUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CVerticalLayoutUI::GetControlFlags() const
{
    if( IsEnabled() && m_iSepHeight != 0 ) return UIFLAG_SETCURSOR;
    else return 0;
}

void CVerticalLayoutUI::SetPos(RECT rc)
{
    CControlUI::SetPos(rc);
    rc = m_rcItem;

    // Adjust for inset
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.top;
    rc.right -= m_rcInset.right;
    rc.bottom -= m_rcInset.bottom;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

    if( m_items.GetSize() == 0) {
        ProcessScrollBar(rc, 0, 0);
        return;
    }

    // Determine the minimum size
    SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
        szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();

    int nAdjustables = 0;
    int cyFixed = 0;
    int nEstimateNum = 0;
    for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) {
        CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) continue;
        SIZE sz = pControl->EstimateSize(szAvailable);
        if( sz.cy == 0 ) {
            nAdjustables++;
        }
        else {
            if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
            if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
        }
        cyFixed += sz.cy + pControl->GetPadding().top + pControl->GetPadding().bottom;
        nEstimateNum++;
    }
    cyFixed += (nEstimateNum - 1) * m_iChildPadding;

    // Place elements
    int cyNeeded = 0;
    int cyExpand = 0;
    if( nAdjustables > 0 ) cyExpand = MAX(0, (szAvailable.cy - cyFixed) / nAdjustables);
    // Position the elements
    SIZE szRemaining = szAvailable;
    int iPosY = rc.top;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
        iPosY -= m_pVerticalScrollBar->GetScrollPos();
    }
    int iPosX = rc.left;
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
        iPosX -= m_pHorizontalScrollBar->GetScrollPos();
    }
    int iAdjustable = 0;
    int cyFixedRemaining = cyFixed;
    for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
        CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) {
            SetFloatPos(it2);
            continue;
        }

        RECT rcPadding = pControl->GetPadding();
        szRemaining.cy -= rcPadding.top;
        SIZE sz = pControl->EstimateSize(szRemaining);
        if( sz.cy == 0 ) {
            iAdjustable++;
            sz.cy = cyExpand;
            // Distribute remaining to last element (usually round-off left-overs)
            if( iAdjustable == nAdjustables ) {
                sz.cy = MAX(0, szRemaining.cy - rcPadding.bottom - cyFixedRemaining);
            } 
            if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
            if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
        }
        else {
            if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
            if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
            cyFixedRemaining -= sz.cy;
        }

        sz.cx = pControl->GetFixedWidth();
        if( sz.cx == 0 ) sz.cx = szAvailable.cx - rcPadding.left - rcPadding.right;
        if( sz.cx < 0 ) sz.cx = 0;
        if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
        if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();

        RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + rcPadding.left + sz.cx, iPosY + sz.cy + rcPadding.top + rcPadding.bottom };
        pControl->SetPos(rcCtrl);

        iPosY += sz.cy + m_iChildPadding + rcPadding.top + rcPadding.bottom;
        cyNeeded += sz.cy + rcPadding.top + rcPadding.bottom;
        szRemaining.cy -= sz.cy + m_iChildPadding + rcPadding.bottom;
    }
    cyNeeded += (nEstimateNum - 1) * m_iChildPadding;

    // Process the scrollbar
    ProcessScrollBar(rc, 0, cyNeeded);
}

void CVerticalLayoutUI::DoPostPaint(HDC hDC, const RECT& rcPaint)
{
    if( (m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode ) {
        RECT rcSeparator = GetThumbRect(true);
        CRenderEngine::DrawColor(hDC, rcSeparator, 0xAA000000);
    }
}

void CVerticalLayoutUI::SetSepHeight(int iHeight)
{
    m_iSepHeight = iHeight;
}

int CVerticalLayoutUI::GetSepHeight() const
{
    return m_iSepHeight;
}

void CVerticalLayoutUI::SetSepImmMode(bool bImmediately)
{
    if( m_bImmMode == bImmediately ) return;
    if( (m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode && m_pManager != NULL ) {
        m_pManager->RemovePostPaint(this);
    }

    m_bImmMode = bImmediately;
}

bool CVerticalLayoutUI::IsSepImmMode() const
{
    return m_bImmMode;
}

void CVerticalLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("sepheight")) == 0 ) SetSepHeight(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("sepimm")) == 0 ) SetSepImmMode(_tcscmp(pstrValue, _T("true")) == 0);
    else CContainerUI::SetAttribute(pstrName, pstrValue);
}

void CVerticalLayoutUI::DoEvent(TEventUI& event)
{
    if( m_iSepHeight != 0 ) {
        if( event.Type == UIEVENT_BUTTONDOWN && IsEnabled() )
        {
            RECT rcSeparator = GetThumbRect(false);
            if( ::PtInRect(&rcSeparator, event.ptMouse) ) {
                m_uButtonState |= UISTATE_CAPTURED;
                ptLastMouse = event.ptMouse;
                m_rcNewPos = m_rcItem;
                if( !m_bImmMode && m_pManager ) m_pManager->AddPostPaint(this);
                return;
            }
        }
        if( event.Type == UIEVENT_BUTTONUP )
        {
            if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
                m_uButtonState &= ~UISTATE_CAPTURED;
                m_rcItem = m_rcNewPos;
                if( !m_bImmMode && m_pManager ) m_pManager->RemovePostPaint(this);
                NeedParentUpdate();
                return;
            }
        }
        if( event.Type == UIEVENT_MOUSEMOVE )
        {
            if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
                LONG cy = event.ptMouse.y - ptLastMouse.y;
                ptLastMouse = event.ptMouse;
                RECT rc = m_rcNewPos;
                if( m_iSepHeight >= 0 ) {
                    if( cy > 0 && event.ptMouse.y < m_rcNewPos.bottom + m_iSepHeight ) return;
                    if( cy < 0 && event.ptMouse.y > m_rcNewPos.bottom ) return;
                    rc.bottom += cy;
                    if( rc.bottom - rc.top <= GetMinHeight() ) {
                        if( m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight() ) return;
                        rc.bottom = rc.top + GetMinHeight();
                    }
                    if( rc.bottom - rc.top >= GetMaxHeight() ) {
                        if( m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight() ) return;
                        rc.bottom = rc.top + GetMaxHeight();
                    }
                }
                else {
                    if( cy > 0 && event.ptMouse.y < m_rcNewPos.top ) return;
                    if( cy < 0 && event.ptMouse.y > m_rcNewPos.top + m_iSepHeight ) return;
                    rc.top += cy;
                    if( rc.bottom - rc.top <= GetMinHeight() ) {
                        if( m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight() ) return;
                        rc.top = rc.bottom - GetMinHeight();
                    }
                    if( rc.bottom - rc.top >= GetMaxHeight() ) {
                        if( m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight() ) return;
                        rc.top = rc.bottom - GetMaxHeight();
                    }
                }

                CRect rcInvalidate = GetThumbRect(true);
                m_rcNewPos = rc;
                m_cxyFixed.cy = m_rcNewPos.bottom - m_rcNewPos.top;

                if( m_bImmMode ) {
                    m_rcItem = m_rcNewPos;
                    NeedParentUpdate();
                }
                else {
                    rcInvalidate.Join(GetThumbRect(true));
                    rcInvalidate.Join(GetThumbRect(false));
                    if( m_pManager ) m_pManager->Invalidate(rcInvalidate);
                }
                return;
            }
        }
        if( event.Type == UIEVENT_SETCURSOR )
        {
            RECT rcSeparator = GetThumbRect(false);
            if( IsEnabled() && ::PtInRect(&rcSeparator, event.ptMouse) ) {
                ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
                return;
            }
        }
    }
    CContainerUI::DoEvent(event);
}

RECT CVerticalLayoutUI::GetThumbRect(bool bUseNew) const
{
    if( (m_uButtonState & UISTATE_CAPTURED) != 0 && bUseNew) {
        if( m_iSepHeight >= 0 ) 
            return CRect(m_rcNewPos.left, MAX(m_rcNewPos.bottom - m_iSepHeight, m_rcNewPos.top), 
            m_rcNewPos.right, m_rcNewPos.bottom);
        else 
            return CRect(m_rcNewPos.left, m_rcNewPos.top, m_rcNewPos.right, 
            MIN(m_rcNewPos.top - m_iSepHeight, m_rcNewPos.bottom));
    }
    else {
        if( m_iSepHeight >= 0 ) 
            return CRect(m_rcItem.left, MAX(m_rcItem.bottom - m_iSepHeight, m_rcItem.top), m_rcItem.right, 
            m_rcItem.bottom);
        else 
            return CRect(m_rcItem.left, m_rcItem.top, m_rcItem.right, 
            MIN(m_rcItem.top - m_iSepHeight, m_rcItem.bottom));

    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

CHorizontalLayoutUI::CHorizontalLayoutUI() : m_iSepWidth(0), m_uButtonState(0), m_bImmMode(false)
{
    ptLastMouse.x = ptLastMouse.y = 0;
    ::ZeroMemory(&m_rcNewPos, sizeof(m_rcNewPos));
}

LPCTSTR CHorizontalLayoutUI::GetClass() const
{
    return _T("HorizontalLayoutUI");
}

LPVOID CHorizontalLayoutUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("HorizontalLayout")) == 0 ) return static_cast<CHorizontalLayoutUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CHorizontalLayoutUI::GetControlFlags() const
{
    if( IsEnabled() && m_iSepWidth != 0 ) return UIFLAG_SETCURSOR;
    else return 0;
}

void CHorizontalLayoutUI::SetPos(RECT rc)
{
    CControlUI::SetPos(rc);
    rc = m_rcItem;

    // Adjust for inset
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.top;
    rc.right -= m_rcInset.right;
    rc.bottom -= m_rcInset.bottom;

    if( m_items.GetSize() == 0) {
        ProcessScrollBar(rc, 0, 0);
        return;
    }

    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

    // Determine the width of elements that are sizeable
    SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
        szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();

    int nAdjustables = 0;
    int cxFixed = 0;
    int nEstimateNum = 0;
    for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) {
        CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) continue;
        SIZE sz = pControl->EstimateSize(szAvailable);
        if( sz.cx == 0 ) {
            nAdjustables++;
        }
        else {
            if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
            if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
        }
        cxFixed += sz.cx +  pControl->GetPadding().left + pControl->GetPadding().right;
        nEstimateNum++;
    }
    cxFixed += (nEstimateNum - 1) * m_iChildPadding;

    int cxExpand = 0;
    if( nAdjustables > 0 ) cxExpand = MAX(0, (szAvailable.cx - cxFixed) / nAdjustables);
    // Position the elements
    SIZE szRemaining = szAvailable;
    int iPosX = rc.left;
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
        iPosX -= m_pHorizontalScrollBar->GetScrollPos();
    }
    int iAdjustable = 0;
    int cxFixedRemaining = cxFixed;
    for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
        CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) {
            SetFloatPos(it2);
            continue;
        }
        RECT rcPadding = pControl->GetPadding();
        szRemaining.cx -= rcPadding.left;
        SIZE sz = pControl->EstimateSize(szRemaining);
        if( sz.cx == 0 ) {
            iAdjustable++;
            sz.cx = cxExpand;
            // Distribute remaining to last element (usually round-off left-overs)
            if( iAdjustable == nAdjustables ) {
                sz.cx = MAX(0, szRemaining.cx - rcPadding.right - cxFixedRemaining);
            }
            if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
            if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
        }
        else {
            if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
            if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();

            cxFixedRemaining -= sz.cx;
        }

        sz.cy = pControl->GetFixedHeight();
        if( sz.cy == 0 ) sz.cy = rc.bottom - rc.top - rcPadding.top - rcPadding.bottom;
        if( sz.cy < 0 ) sz.cy = 0;
        if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
        if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();

        RECT rcCtrl = { iPosX + rcPadding.left, rc.top + rcPadding.top, iPosX + sz.cx + rcPadding.left + rcPadding.right, rc.top + rcPadding.top + sz.cy};
        pControl->SetPos(rcCtrl);
        iPosX += sz.cx + m_iChildPadding + rcPadding.left + rcPadding.right;
        szRemaining.cx -= sz.cx + m_iChildPadding + rcPadding.right;
    }

    // Process the scrollbar
    ProcessScrollBar(rc, 0, 0);
}

void CHorizontalLayoutUI::DoPostPaint(HDC hDC, const RECT& rcPaint)
{
    if( (m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode ) {
        RECT rcSeparator = GetThumbRect(true);
        CRenderEngine::DrawColor(hDC, rcSeparator, 0xAA000000);
    }
}

void CHorizontalLayoutUI::SetSepWidth(int iWidth)
{
    m_iSepWidth = iWidth;
}

int CHorizontalLayoutUI::GetSepWidth() const
{
    return m_iSepWidth;
}

void CHorizontalLayoutUI::SetSepImmMode(bool bImmediately)
{
    if( m_bImmMode == bImmediately ) return;
    if( (m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode && m_pManager != NULL ) {
        m_pManager->RemovePostPaint(this);
    }

    m_bImmMode = bImmediately;
}

bool CHorizontalLayoutUI::IsSepImmMode() const
{
    return m_bImmMode;
}

void CHorizontalLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("sepwidth")) == 0 ) SetSepWidth(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("sepimm")) == 0 ) SetSepImmMode(_tcscmp(pstrValue, _T("true")) == 0);
    else CContainerUI::SetAttribute(pstrName, pstrValue);
}

void CHorizontalLayoutUI::DoEvent(TEventUI& event)
{
    if( m_iSepWidth != 0 ) {
        if( event.Type == UIEVENT_BUTTONDOWN && IsEnabled() )
        {
            RECT rcSeparator = GetThumbRect(false);
            if( ::PtInRect(&rcSeparator, event.ptMouse) ) {
                m_uButtonState |= UISTATE_CAPTURED;
                ptLastMouse = event.ptMouse;
                m_rcNewPos = m_rcItem;
                if( !m_bImmMode && m_pManager ) m_pManager->AddPostPaint(this);
                return;
            }
        }
        if( event.Type == UIEVENT_BUTTONUP )
        {
            if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
                m_uButtonState &= ~UISTATE_CAPTURED;
                m_rcItem = m_rcNewPos;
                if( !m_bImmMode && m_pManager ) m_pManager->RemovePostPaint(this);
                NeedParentUpdate();
                return;
            }
        }
        if( event.Type == UIEVENT_MOUSEMOVE )
        {
            if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
                LONG cx = event.ptMouse.x - ptLastMouse.x;
                ptLastMouse = event.ptMouse;
                RECT rc = m_rcNewPos;
                if( m_iSepWidth >= 0 ) {
                    if( cx > 0 && event.ptMouse.x < m_rcNewPos.right - m_iSepWidth ) return;
                    if( cx < 0 && event.ptMouse.x > m_rcNewPos.right ) return;
                    rc.right += cx;
                    if( rc.right - rc.left <= GetMinWidth() ) {
                        if( m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth() ) return;
                        rc.right = rc.left + GetMinWidth();
                    }
                    if( rc.right - rc.left >= GetMaxWidth() ) {
                        if( m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth() ) return;
                        rc.right = rc.left + GetMaxWidth();
                    }
                }
                else {
                    if( cx > 0 && event.ptMouse.x < m_rcNewPos.left ) return;
                    if( cx < 0 && event.ptMouse.x > m_rcNewPos.left - m_iSepWidth ) return;
                    rc.left += cx;
                    if( rc.right - rc.left <= GetMinWidth() ) {
                        if( m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth() ) return;
                        rc.left = rc.right - GetMinWidth();
                    }
                    if( rc.right - rc.left >= GetMaxWidth() ) {
                        if( m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth() ) return;
                        rc.left = rc.right - GetMaxWidth();
                    }
                }

                CRect rcInvalidate = GetThumbRect(true);
                m_rcNewPos = rc;
                m_cxyFixed.cx = m_rcNewPos.right - m_rcNewPos.left;

                if( m_bImmMode ) {
                    m_rcItem = m_rcNewPos;
                    NeedParentUpdate();
                }
                else {
                    rcInvalidate.Join(GetThumbRect(true));
                    rcInvalidate.Join(GetThumbRect(false));
                    if( m_pManager ) m_pManager->Invalidate(rcInvalidate);
                }
                return;
            }
        }
        if( event.Type == UIEVENT_SETCURSOR )
        {
            RECT rcSeparator = GetThumbRect(false);
            if( IsEnabled() && ::PtInRect(&rcSeparator, event.ptMouse) ) {
                ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
                return;
            }
        }
    }
    CContainerUI::DoEvent(event);
}

RECT CHorizontalLayoutUI::GetThumbRect(bool bUseNew) const
{
    if( (m_uButtonState & UISTATE_CAPTURED) != 0 && bUseNew) {
        if( m_iSepWidth >= 0 ) return CRect(m_rcNewPos.right - m_iSepWidth, m_rcNewPos.top, m_rcNewPos.right, m_rcNewPos.bottom);
        else return CRect(m_rcNewPos.left, m_rcNewPos.top, m_rcNewPos.left - m_iSepWidth, m_rcNewPos.bottom);
    }
    else {
        if( m_iSepWidth >= 0 ) return CRect(m_rcItem.right - m_iSepWidth, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
        else return CRect(m_rcItem.left, m_rcItem.top, m_rcItem.left - m_iSepWidth, m_rcItem.bottom);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

CScrollContainerUI::CScrollContainerUI() : m_nColumns(1)
{
	m_szImgXY.cx = 0;
	m_szImgXY.cy = 0;
}

LPCTSTR CScrollContainerUI::GetClass() const
{
    return _T("CScrollContainerUI");
}

LPVOID CScrollContainerUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("CScrollContainerUI")) == 0 ) 
	{	
		return static_cast<CScrollContainerUI*>(this);
	}

	return CContainerUI::GetInterface(pstrName);
}

int CScrollContainerUI::GetColumns() const
{
    return m_nColumns;
}

void CScrollContainerUI::SetColumns(int nCols)
{
}

void CScrollContainerUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CContainerUI::SetAttribute(pstrName, pstrValue);
}

void CScrollContainerUI::SetPos(RECT rc)
{
    CControlUI::SetPos(rc);

	rc = m_rcItem;

    // 校验inset
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.top;
    rc.right -= m_rcInset.right;
    rc.bottom -= m_rcInset.bottom;

    if( m_items.GetSize() == 0) 
	{
        ProcessScrollBar(rc, 0, 0);
        return;
    }

    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

    // 设置elements位置
    int cyNeeded = 0;
    int cxWidth = (rc.right - rc.left) / m_nColumns;
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
	{
        cxWidth = (rc.right - rc.left + m_pHorizontalScrollBar->GetScrollRange() ) / m_nColumns;
	}

    int cyHeight = 0;
    int iCount = 0;
    POINT ptTile = { rc.left, rc.top };
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
	{
        ptTile.y -= m_pVerticalScrollBar->GetScrollPos();
    }

    int iPosX = rc.left;
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
	{
        iPosX -= m_pHorizontalScrollBar->GetScrollPos();
        ptTile.x = iPosX;
    }

	for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) 
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) 
		{
            SetFloatPos(it1);
            continue;
        }

        // Determine 大小
        RECT rcTile = { ptTile.x, ptTile.y, ptTile.x + cxWidth, ptTile.y };
        if( (iCount % m_nColumns) == 0 )
        {
            int iIndex = iCount;
            for( int it2 = it1; it2 < m_items.GetSize(); it2++ ) 
			{
                CControlUI* pLineControl = static_cast<CControlUI*>(m_items[it2]);
                if( !pLineControl->IsVisible() ) continue;
                if( pLineControl->IsFloat() ) continue;

                RECT rcPadding = pLineControl->GetPadding();
                SIZE szAvailable = { rcTile.right - rcTile.left - rcPadding.left - rcPadding.right, 9999 };
                if( iIndex == iCount || (iIndex + 1) % m_nColumns == 0 ) 
				{
                    szAvailable.cx -= m_iChildPadding / 2;
                }
                else 
				{
                    szAvailable.cx -= m_iChildPadding;
                }

                if( szAvailable.cx < pControl->GetMinWidth() ) szAvailable.cx = pControl->GetMinWidth();
                if( szAvailable.cx > pControl->GetMaxWidth() ) szAvailable.cx = pControl->GetMaxWidth();

                SIZE szTile = pLineControl->EstimateSize(szAvailable);
                if( szTile.cx < pControl->GetMinWidth() ) szTile.cx = pControl->GetMinWidth();
                if( szTile.cx > pControl->GetMaxWidth() ) szTile.cx = pControl->GetMaxWidth();
                if( szTile.cy < pControl->GetMinHeight() ) szTile.cy = pControl->GetMinHeight();
                if( szTile.cy > pControl->GetMaxHeight() ) szTile.cy = pControl->GetMaxHeight();

                cyHeight = MAX(cyHeight, szTile.cy + rcPadding.top + rcPadding.bottom);
                if( (++iIndex % m_nColumns) == 0) break;
            }
        }

        RECT rcPadding = pControl->GetPadding();

        rcTile.left += rcPadding.left + m_iChildPadding / 2;
        rcTile.right -= rcPadding.right + m_iChildPadding / 2;
        if( (iCount % m_nColumns) == 0 ) 
		{
            rcTile.left -= m_iChildPadding / 2;
        }

        if( ( (iCount + 1) % m_nColumns) == 0 ) 
		{
            rcTile.right += m_iChildPadding / 2;
        }

        rcTile.top = ptTile.y + rcPadding.top;
        rcTile.bottom = ptTile.y + cyHeight;

        RECT rcPos;
		rcPos.left = 0;
		rcPos.top = ptTile.y;
		rcPos.right = m_szImgXY.cx/*1920*//*cxWidth*/;
		rcPos.bottom = cyHeight + ptTile.y;

        pControl->SetPos(rcPos);

        if( (++iCount % m_nColumns) == 0 ) 
		{
            ptTile.x = iPosX;
            ptTile.y += cyHeight + m_iChildPadding;
			cyHeight = 0;
		}
		else 
		{
			ptTile.x += cxWidth;
		}

		cyNeeded = rcTile.bottom - rc.top;
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) cyNeeded += m_pVerticalScrollBar->GetScrollPos();
	}

    // 更新滚动条位置
    ProcessScrollBar(rc, m_szImgXY.cx/*1920*//*cxWidth*/, cyNeeded);
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

CDialogLayoutUI::CDialogLayoutUI() : m_aModes(sizeof(STRETCHMODE))
{
}

LPCTSTR CDialogLayoutUI::GetClass() const
{
    return _T("DialogLayoutUI");
}

LPVOID CDialogLayoutUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("DialogLayout")) == 0 ) return static_cast<CDialogLayoutUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

void CDialogLayoutUI::SetStretchMode(CControlUI* pControl, UINT uMode)
{
    STRETCHMODE mode;
    mode.pControl = pControl;
    mode.uMode = uMode;
    RECT rcPos = { pControl->GetFixedXY().cx, pControl->GetFixedXY().cy, pControl->GetFixedXY().cx + pControl->GetFixedWidth(), 
        pControl->GetFixedXY().cy + pControl->GetFixedHeight() };
    mode.rcItem = rcPos;
    m_aModes.Add(&mode);
}

SIZE CDialogLayoutUI::EstimateSize(SIZE szAvailable)
{
    RECT rcDialog = RecalcArea();
    SIZE rc = { rcDialog.right - rcDialog.left, rcDialog.bottom - rcDialog.top };
    if( m_pVerticalScrollBar != NULL ) {
        if( rc.cx > szAvailable.cx ) rc.cx = szAvailable.cx;
        if( rc.cy > szAvailable.cy ) rc.cy = szAvailable.cy;
    }
    return rc;
}

void CDialogLayoutUI::SetPos(RECT rc)
{
    CControlUI::SetPos(rc);
    rc = m_rcItem;

    if( m_items.GetSize() == 0) return;

    RECT rcDialog = RecalcArea();

    // Adjust for inset
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.top;
    rc.right -= m_rcInset.right;
    rc.bottom -= m_rcInset.bottom;

    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
        rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
        RECT rcScrollBarPos = { rc.left, rc.bottom, rc.right, rc.bottom + m_pHorizontalScrollBar->GetFixedHeight()};
        m_pHorizontalScrollBar->SetPos(rcScrollBarPos);
    }

    if( m_pVerticalScrollBar != NULL ) {
        bool bScrollBar = m_pVerticalScrollBar->IsVisible();
        if( bScrollBar ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
        ProcessScrollBar(rc, 0, rcDialog.bottom - rcDialog.top);
        if( bScrollBar != m_pVerticalScrollBar->IsVisible() ) {
            if( bScrollBar ) rc.right += m_pVerticalScrollBar->GetFixedWidth();
            else rc.right -= m_pVerticalScrollBar->GetFixedWidth();
        }
    }

    // Determine how "scaled" the dialog is compared to the original size
    int cxDiff = (rc.right - rc.left) - (rcDialog.right - rcDialog.left);
    int cyDiff = (rc.bottom - rc.top) - (rcDialog.bottom - rcDialog.top);
    if( cxDiff < 0 ) cxDiff = 0;
    if( cyDiff < 0 ) cyDiff = 0;
    // Stretch each control
    // Controls are coupled in "groups", which determine a scaling factor.
    // A "line" indicator is used to apply the same scaling to a new group of controls.
    int nCount, cxStretch, cyStretch, cxMove, cyMove;
    for( int i = 0; i < m_aModes.GetSize(); i++ ) {
        STRETCHMODE* pItem = static_cast<STRETCHMODE*>(m_aModes[i]);
        if( !pItem->pControl->IsVisible() ) continue;
        if( pItem->pControl->IsFloat() ) {
            SetFloatPos(i);
            continue;
        }

        if( i == 0 || (pItem->uMode & UISTRETCH_NEWGROUP) != 0 ) {
            nCount = 0;
            for( int j = i + 1; j < m_aModes.GetSize(); j++ ) {
                STRETCHMODE* pNext = static_cast<STRETCHMODE*>(m_aModes[j]);
                if( (pNext->uMode & (UISTRETCH_NEWGROUP | UISTRETCH_NEWLINE)) != 0 ) break;
                if( (pNext->uMode & (UISTRETCH_SIZE_X | UISTRETCH_SIZE_Y)) != 0 ) nCount++;
            }
            if( nCount == 0 ) nCount = 1;
            cxStretch = cxDiff / nCount;
            cyStretch = cyDiff / nCount;
            cxMove = 0;
            cyMove = 0;
        }
        if( (pItem->uMode & UISTRETCH_NEWLINE) != 0 ) {
            cxMove = 0;
            cyMove = 0;
        }
        RECT rcPos = pItem->rcItem;
        int iScrollPosY = 0;
        if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) iScrollPosY = m_pVerticalScrollBar->GetScrollPos();
        int iScrollPosX = 0;
        if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) iScrollPosX = m_pHorizontalScrollBar->GetScrollPos();

        ::OffsetRect(&rcPos, rc.left - iScrollPosX, rc.top - iScrollPosY);
        
        if( (pItem->uMode & UISTRETCH_MOVE_X) != 0 ) ::OffsetRect(&rcPos, cxMove, 0);
        if( (pItem->uMode & UISTRETCH_MOVE_Y) != 0 ) ::OffsetRect(&rcPos, 0, cyMove);
        if( (pItem->uMode & UISTRETCH_SIZE_X) != 0 ) rcPos.right += cxStretch;
        if( (pItem->uMode & UISTRETCH_SIZE_Y) != 0 ) rcPos.bottom += cyStretch;
        if( (pItem->uMode & (UISTRETCH_SIZE_X | UISTRETCH_SIZE_Y)) != 0 ) {
            cxMove += cxStretch;
            cyMove += cyStretch;
        }
        if( rcPos.right - rcPos.left > pItem->pControl->GetMaxWidth() ) rcPos.right = rc.left + pItem->pControl->GetMaxWidth();
        if( rcPos.right - rcPos.left < pItem->pControl->GetMinWidth() ) rcPos.right = rc.left + pItem->pControl->GetMinWidth();
        if( rcPos.bottom - rcPos.top > pItem->pControl->GetMaxHeight() ) rcPos.bottom = rc.top + pItem->pControl->GetMaxHeight();
        if( rcPos.bottom - rcPos.top < pItem->pControl->GetMinHeight() ) rcPos.bottom = rc.top + pItem->pControl->GetMinHeight();

        pItem->pControl->SetPos(rcPos);
    }
}

RECT CDialogLayoutUI::RecalcArea()
{
    CStdValArray aTempModes( sizeof(STRETCHMODE), m_items.GetSize() );

    for( int it = 0; it < m_items.GetSize(); it++ ) {      
        CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);

        bool bFound = false;
        int i = 0;
        for( ; !bFound && i < m_aModes.GetSize(); i++ ) {
            if( static_cast<STRETCHMODE*>(m_aModes[i])->pControl == pControl ) {
                bFound = true;
                break;
            }
        }

        if( bFound ) {
            RECT rcPos = { pControl->GetFixedXY().cx, pControl->GetFixedXY().cy, pControl->GetFixedXY().cx + pControl->GetFixedWidth(), 
                pControl->GetFixedXY().cy + pControl->GetFixedHeight() };
            static_cast<STRETCHMODE*>(m_aModes[i])->rcItem = rcPos;
            aTempModes.Add(m_aModes[i]);
        }
        else {
            STRETCHMODE mode;
            mode.pControl = pControl;
            mode.uMode = UISTRETCH_NEWGROUP;
            RECT rcPos = { pControl->GetFixedXY().cx, pControl->GetFixedXY().cy, pControl->GetFixedXY().cx + pControl->GetFixedWidth(), 
                pControl->GetFixedXY().cy + pControl->GetFixedHeight() };
            mode.rcItem = rcPos;
            aTempModes.Add(&mode);
        }
    }

    LPBYTE bTempBuffer[sizeof(CStdValArray)];
    ::CopyMemory( (LPVOID)&bTempBuffer, (LPCVOID)&m_aModes, sizeof(CStdValArray) );
    ::CopyMemory( (LPVOID)&m_aModes, (LPCVOID)&aTempModes, sizeof(CStdValArray) );
    ::CopyMemory( (LPVOID)&aTempModes, (LPCVOID)&bTempBuffer, sizeof(CStdValArray) );

    // Figure out the actual size of the dialog so we can add proper scrollbars later
    CRect rcDialog(9999, 9999, 0,0);
    for( int i = 0; i < m_aModes.GetSize(); i++ ) {
        STRETCHMODE* pItem = static_cast<STRETCHMODE*>(m_aModes[i]);
        if( !pItem->pControl->IsVisible() ) continue;
        if( pItem->pControl->IsFloat() ) continue;
        const RECT& rcPos = static_cast<STRETCHMODE*>(m_aModes[i])->rcItem;
        rcDialog.Join(rcPos);
    }
    for( int j = 0; j < m_aModes.GetSize(); j++ ) {
        RECT& rcPos = static_cast<STRETCHMODE*>(m_aModes[j])->rcItem;
        ::OffsetRect(&rcPos, -rcDialog.left, -rcDialog.top);
    }

    return rcDialog;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

CTabLayoutUI::CTabLayoutUI() : m_iCurSel(-1)
{
}

LPCTSTR CTabLayoutUI::GetClass() const
{
    return _T("TabLayoutUI");
}

LPVOID CTabLayoutUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("TabLayout")) == 0 ) return static_cast<CTabLayoutUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

bool CTabLayoutUI::Add(CControlUI* pControl)
{
    bool ret = CContainerUI::Add(pControl);
    if( !ret ) return ret;

    if(m_iCurSel == -1 && pControl->IsVisible())
    {
        m_iCurSel = GetItemIndex(pControl);
    }
    else
    {
        pControl->SetVisible(false);
    }

    return ret;
}

bool CTabLayoutUI::AddAt(CControlUI* pControl, int iIndex)
{
    bool ret = CContainerUI::AddAt(pControl, iIndex);
    if( !ret ) return ret;

    if(m_iCurSel == -1 && pControl->IsVisible())
    {
        m_iCurSel = GetItemIndex(pControl);
    }
    else if( m_iCurSel != -1 && iIndex <= m_iCurSel )
    {
        m_iCurSel += 1;
    }
    else
    {
        pControl->SetVisible(false);
    }

    return ret;
}

bool CTabLayoutUI::Remove(CControlUI* pControl)
{
    if( pControl == NULL) return false;
    
    int index = GetItemIndex(pControl);
    bool ret = CContainerUI::Remove(pControl);
    if( !ret ) return false;

    if( m_iCurSel == index)
    {
        if( GetCount() > 0 ) GetItemAt(0)->SetVisible(true);
        NeedParentUpdate();
    }
    else if( m_iCurSel > index )
    {
        m_iCurSel -= 1;
    }
    
    return ret;
}

void CTabLayoutUI::RemoveAll()
{
    m_iCurSel = -1;
    CContainerUI::RemoveAll();
    NeedParentUpdate();
}

int CTabLayoutUI::GetCurSel() const
{
    return m_iCurSel;
}

bool CTabLayoutUI::SelectItem(int iIndex)
{
    if( iIndex < 0 || iIndex >= m_items.GetSize() ) return false;
    if( iIndex == m_iCurSel ) return true;

    int iOldSel = m_iCurSel;
    m_iCurSel = iIndex;
    for( int it = 0; it < m_items.GetSize(); it++ )
    {
        if( it == iIndex ) {
            GetItemAt(it)->SetVisible(true);
            GetItemAt(it)->SetFocus();
        }
        else GetItemAt(it)->SetVisible(false);
    }
    NeedParentUpdate();

    if( m_pManager != NULL ) {
        m_pManager->SetNextTabControl();
        m_pManager->SendNotify(this, DUI_MSGTYPE_TABSELECT, m_iCurSel, iOldSel);
    }
    return true;
}

void CTabLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("selectedid")) == 0 ) SelectItem(_ttoi(pstrValue));
    return CContainerUI::SetAttribute(pstrName, pstrValue);
}

void CTabLayoutUI::SetPos(RECT rc)
{
    CControlUI::SetPos(rc);
    rc = m_rcItem;

    // Adjust for inset
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.top;
    rc.right -= m_rcInset.right;
    rc.bottom -= m_rcInset.bottom;

    for( int it = 0; it < m_items.GetSize(); it++ ) {
        CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) {
            SetFloatPos(it);
            continue;
        }

        if( it != m_iCurSel ) continue;

        RECT rcPadding = pControl->GetPadding();
        rc.left += rcPadding.left;
        rc.top += rcPadding.top;
        rc.right -= rcPadding.right;
        rc.bottom -= rcPadding.bottom;

        SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
        
        SIZE sz = pControl->EstimateSize(szAvailable);
        if( sz.cx == 0 ) {
            sz.cx = MAX(0, szAvailable.cx);
        }
        if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
        if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();

        if(sz.cy == 0) {
            sz.cy = MAX(0, szAvailable.cy);
        }
        if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
        if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();

        RECT rcCtrl = { rc.left, rc.top, rc.left + sz.cx, rc.top + sz.cy};
        pControl->SetPos(rcCtrl);
    }
}

} // namespace DuiLib

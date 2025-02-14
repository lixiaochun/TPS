#include "StdAfx.h"
#include "logicManager.h"
namespace DuiLib {

CDialogBuilder::CDialogBuilder() : m_pCallback(NULL), m_pstrtype(NULL)
{

}

CControlUI* CDialogBuilder::Create(STRINGorID xml, LPCTSTR type, IDialogBuilderCallback* pCallback, 
                                   CPaintManagerUI* pManager, CControlUI* pParent)
{ 
    if( HIWORD(xml.m_lpstr) != NULL ) {
        if( *(xml.m_lpstr) == _T('<') ) {
            if( !m_xml.Load(xml.m_lpstr) ) return NULL;
        }
        else {
            if( !m_xml.LoadFromFile(xml.m_lpstr) ) return NULL;
        }
    }
    else {
        HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), xml.m_lpstr, type);
        if( hResource == NULL ) return NULL;
        HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
        if( hGlobal == NULL ) {
            FreeResource(hResource);
            return NULL;
        }

        m_pCallback = pCallback;
        if( !m_xml.LoadFromMem((BYTE*)::LockResource(hGlobal), ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource) )) return NULL;
        ::FreeResource(hResource);
        m_pstrtype = type;
    }

    return Create(pCallback, pManager);
}

CControlUI* CDialogBuilder::Create(IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent)
{
    m_pCallback = pCallback;
    CMarkupNode root = m_xml.GetRoot();
    if( !root.IsValid() ) return NULL;

    if( pManager ) {
        LPCTSTR pstrClass = NULL;
        int nAttributes = 0;
        LPCTSTR pstrName = NULL;
        LPCTSTR pstrValue = NULL;
        LPTSTR pstr = NULL;
        for( CMarkupNode node = root.GetChild() ; node.IsValid(); node = node.GetSibling() ) {
            pstrClass = node.GetName();
            if( _tcscmp(pstrClass, _T("Image")) == 0 ) {
                nAttributes = node.GetAttributeCount();
                LPCTSTR pImageName = NULL;
                LPCTSTR pImageResType = NULL;
                DWORD mask = 0;
                for( int i = 0; i < nAttributes; i++ ) {
                    pstrName = node.GetAttributeName(i);
                    pstrValue = node.GetAttributeValue(i);
                    if( _tcscmp(pstrName, _T("name")) == 0 ) {
                        pImageName = pstrValue;
                    }
                    else if( _tcscmp(pstrName, _T("restype")) == 0 ) {
                        pImageResType = pstrValue;
                    }
                    else if( _tcscmp(pstrName, _T("mask")) == 0 ) {
                        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
                        mask = _tcstoul(pstrValue, &pstr, 16);
                    }
                }
                if( pImageName ) pManager->AddImage(pImageName, pImageResType, mask);
            }
            else if( _tcscmp(pstrClass, _T("Font")) == 0 ) {
                nAttributes = node.GetAttributeCount();
                LPCTSTR pFontName = NULL;
                int size = 12;
                bool bold = false;
                bool underline = false;
                bool italic = false;
                bool defaultfont = false;
                for( int i = 0; i < nAttributes; i++ ) {
                    pstrName = node.GetAttributeName(i);
                    pstrValue = node.GetAttributeValue(i);
                    if( _tcscmp(pstrName, _T("name")) == 0 ) {
                        pFontName = pstrValue;
                    }
                    else if( _tcscmp(pstrName, _T("size")) == 0 ) {
                        size = _tcstol(pstrValue, &pstr, 10);
                    }
                    else if( _tcscmp(pstrName, _T("bold")) == 0 ) {
                        bold = (_tcscmp(pstrValue, _T("true")) == 0);
                    }
                    else if( _tcscmp(pstrName, _T("underline")) == 0 ) {
                        underline = (_tcscmp(pstrValue, _T("true")) == 0);
                    }
                    else if( _tcscmp(pstrName, _T("italic")) == 0 ) {
                        italic = (_tcscmp(pstrValue, _T("true")) == 0);
                    }
                    else if( _tcscmp(pstrName, _T("default")) == 0 ) {
                        defaultfont = (_tcscmp(pstrValue, _T("true")) == 0);
                    }
                }
                if( pFontName ) {
                    pManager->AddFont(pFontName, size, bold, underline, italic);
                    if( defaultfont ) pManager->SetDefaultFont(pFontName, size, bold, underline, italic);
                }
            }
            else if( _tcscmp(pstrClass, _T("Default")) == 0 ) {
                nAttributes = node.GetAttributeCount();
                LPCTSTR pControlName = NULL;
                LPCTSTR pControlValue = NULL;
                for( int i = 0; i < nAttributes; i++ ) {
                    pstrName = node.GetAttributeName(i);
                    pstrValue = node.GetAttributeValue(i);
                    if( _tcscmp(pstrName, _T("name")) == 0 ) {
                        pControlName = pstrValue;
                    }
                    else if( _tcscmp(pstrName, _T("value")) == 0 ) {
                        pControlValue = pstrValue;
                    }
                }
                if( pControlName ) {
                    pManager->AddDefaultAttributeList(pControlName, pControlValue);
                }
            }
			 
        }

        pstrClass = root.GetName();
        if( _tcscmp(pstrClass, _T("Window")) == 0 ) {
            if( pManager->GetPaintWindow() ) {
                int nAttributes = root.GetAttributeCount();
                for( int i = 0; i < nAttributes; i++ ) {
                    pstrName = root.GetAttributeName(i);
                    pstrValue = root.GetAttributeValue(i);
                    if( _tcscmp(pstrName, _T("size")) == 0 ) {
                        LPTSTR pstr = NULL;
                        int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
                        int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
                        pManager->SetInitSize(cx, cy);
                    } 
                    else if( _tcscmp(pstrName, _T("sizebox")) == 0 ) {
                        RECT rcSizeBox = { 0 };
                        LPTSTR pstr = NULL;
                        rcSizeBox.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
                        rcSizeBox.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
                        rcSizeBox.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
                        rcSizeBox.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
                        pManager->SetSizeBox(rcSizeBox);
                    }
                    else if( _tcscmp(pstrName, _T("caption")) == 0 ) {
                        RECT rcCaption = { 0 };
                        LPTSTR pstr = NULL;
                        rcCaption.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
                        rcCaption.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
                        rcCaption.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
                        rcCaption.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
                        pManager->SetCaptionRect(rcCaption);
                    }
                    else if( _tcscmp(pstrName, _T("roundcorner")) == 0 ) {
                        LPTSTR pstr = NULL;
                        int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
                        int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
                        pManager->SetRoundCorner(cx, cy);
                    } 
                    else if( _tcscmp(pstrName, _T("mininfo")) == 0 ) {
                        LPTSTR pstr = NULL;
                        int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
                        int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
                        pManager->SetMinInfo(cx, cy);
                    }
                    else if( _tcscmp(pstrName, _T("maxinfo")) == 0 ) {
                        LPTSTR pstr = NULL;
                        int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
                        int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
                        pManager->SetMaxInfo(cx, cy);
                    }
                    else if( _tcscmp(pstrName, _T("showdirty")) == 0 ) {
                        pManager->SetShowUpdateRect(_tcscmp(pstrValue, _T("true")) == 0);
                    } 
                    else if( _tcscmp(pstrName, _T("alpha")) == 0 ) {
                        pManager->SetTransparent(_ttoi(pstrValue));
                    } 
                    else if( _tcscmp(pstrName, _T("bktrans")) == 0 ) {
                        pManager->SetBackgroundTransparent(_tcscmp(pstrValue, _T("true")) == 0);
                    } 
                    else if( _tcscmp(pstrName, _T("disabledfontcolor")) == 0 ) {
                        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
                        LPTSTR pstr = NULL;
                        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
                        pManager->SetDefaultDisabledColor(clrColor);
                    } 
                    else if( _tcscmp(pstrName, _T("defaultfontcolor")) == 0 ) {
                        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
                        LPTSTR pstr = NULL;
                        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
                        pManager->SetDefaultFontColor(clrColor);
                    }
                    else if( _tcscmp(pstrName, _T("linkfontcolor")) == 0 ) {
                        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
                        LPTSTR pstr = NULL;
                        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
                        pManager->SetDefaultLinkFontColor(clrColor);
                    } 
                    else if( _tcscmp(pstrName, _T("linkhoverfontcolor")) == 0 ) {
                        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
                        LPTSTR pstr = NULL;
                        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
                        pManager->SetDefaultLinkHoverFontColor(clrColor);
                    } 
                    else if( _tcscmp(pstrName, _T("selectedcolor")) == 0 ) {
                        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
                        LPTSTR pstr = NULL;
                        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
                        pManager->SetDefaultSelectedBkColor(clrColor);
                    } 
					else if( _tcscmp(pstrName, _T("logic")) == 0 ) //2013.4.19  增加windows对应的逻辑处理模块 yjj
					{
						 
						INotifyUI *pNotify = CLogicManager::FindLogicByName( pstrValue );
						pManager->AddNotifier( pNotify );
					}
                }
            }
        }
    }
    return _Parse(&root, NULL, pManager);
}

CMarkup* CDialogBuilder::GetMarkup()
{
    return &m_xml;
}

void CDialogBuilder::GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const
{
    return m_xml.GetLastErrorMessage(pstrMessage, cchMax);
}

void CDialogBuilder::GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const
{
    return m_xml.GetLastErrorLocation(pstrSource, cchMax);
}

CControlUI* CDialogBuilder::_Parse(CMarkupNode* pRoot, CControlUI* pParent, CPaintManagerUI* pManager)
{
    CDialogLayoutUI* pDialogLayout = NULL;
    IContainerUI* pContainer = NULL;
    CControlUI* pReturn = NULL;
	for( CMarkupNode node = pRoot->GetChild() ; node.IsValid(); node = node.GetSibling() ) {
		LPCTSTR pstrClass = node.GetName();
        if( _tcscmp(pstrClass, _T("Image")) == 0 || _tcscmp(pstrClass, _T("Font")) == 0 \
            || _tcscmp(pstrClass, _T("Default")) == 0 ) continue;

        CControlUI* pControl = NULL;
        if( _tcscmp(pstrClass, _T("Include")) == 0 ) {
            if( !node.HasAttributes() ) continue;
            int count = 1;
            LPTSTR pstr = NULL;
            TCHAR szValue[500] = { 0 };
            SIZE_T cchLen = lengthof(szValue) - 1;
            if ( node.GetAttributeValue(_T("count"), szValue, cchLen) )
                count = _tcstol(szValue, &pstr, 10);
            cchLen = lengthof(szValue) - 1;
            if ( !node.GetAttributeValue(_T("source"), szValue, cchLen) ) continue;
            for ( int i = 0; i < count; i++ ) {
                CDialogBuilder builder;
                if( m_pstrtype != NULL ) { // 使用资源dll，从资源中读取
                    WORD id = (WORD)_tcstol(szValue, &pstr, 10); 
                    pControl = builder.Create((UINT)id, m_pstrtype, m_pCallback, pManager, pParent);
                }
                else {
                    pControl = builder.Create((LPCTSTR)szValue, (UINT)0, m_pCallback, pManager, pParent);
                }
            }
            continue;
        }
        else {
            SIZE_T cchLen = _tcslen(pstrClass);
            switch( cchLen ) {
            case 4:
                if( _tcscmp(pstrClass, _T("Edit")) == 0 )                   pControl = new CEditUI;
                else if( _tcscmp(pstrClass, _T("List")) == 0 )              pControl = new CListUI;
                else if( _tcscmp(pstrClass, _T("Text")) == 0 )              pControl = new CTextUI;
				else if( _tcscmp(pstrClass, _T("Case")) == 0 )              pManager->AddCase( &node );//xml中添加“方案”配置,在此创建 2013.4.23 yjj
                break;
            case 5:
                if( _tcscmp(pstrClass, _T("Combo")) == 0 )                  pControl = new CComboUI;
                else if( _tcscmp(pstrClass, _T("Label")) == 0 )             pControl = new CLabelUI;
                break;
            case 6:
                if( _tcscmp(pstrClass, _T("Button")) == 0 )                 pControl = new CButtonUI;
                else if( _tcscmp(pstrClass, _T("Option")) == 0 )            pControl = new COptionUI;
                else if( _tcscmp(pstrClass, _T("Slider")) == 0 )            pControl = new CSliderUI;
                break;
            case 7:
                if( _tcscmp(pstrClass, _T("Control")) == 0 )                pControl = new CControlUI;
                else if( _tcscmp(pstrClass, _T("ActiveX")) == 0 )           pControl = new CActiveXUI;
                break;
            case 8:
                if( _tcscmp(pstrClass, _T("Progress")) == 0 )               pControl = new CProgressUI;
                else if(  _tcscmp(pstrClass, _T("RichEdit")) == 0 )         pControl = new CRichEditUI;
				break;
			case 9:
                if( _tcscmp(pstrClass, _T("Container")) == 0 )              pControl = new CContainerUI;
                else if( _tcscmp(pstrClass, _T("TabLayout")) == 0 )         pControl = new CTabLayoutUI;
                else if( _tcscmp(pstrClass, _T("ScrollBar")) == 0 )         pControl = new CScrollBarUI; 
                break;
            case 10:
                if( _tcscmp(pstrClass, _T("ListHeader")) == 0 )             pControl = new CListHeaderUI;
                else if( _tcscmp(pstrClass, _T("CTileLayoutUITest")) == 0 )        pControl = new CScrollContainerUI;
				else if( _tcscmp(pstrClass, _T("ListIconUI")) == 0 )        pControl = new CListIconUI; //增加CListIconUI类 
                break;
			case 11:
				if( _tcscmp(pstrClass, _T("ChildWindow")) == 0 )
				{
					bool bRet = CreateChildControl( &node, pParent, pManager );
					continue;
				}
				break;
            case 12:
                if( _tcscmp(pstrClass, _T("DialogLayout")) == 0 )           pControl = new CDialogLayoutUI;
				else if( _tcscmp(pstrClass, _T("ScollImageUI")) == 0 )      pControl = new CScollImageUI;
				else if ( _tcsicmp(pstrClass, _T("CommonListUI") ) == 0 )	pControl = new CCommonListUI;
                break;
			case 13:
				if( _tcscmp(pstrClass, _T("WindowChildUI")) == 0 )         pControl = new CWindowChildUI; //增加子对话框类
            case 14:
                if( _tcscmp(pstrClass, _T("VerticalLayout")) == 0 )         pControl = new CVerticalLayoutUI;
                else if( _tcscmp(pstrClass, _T("ListHeaderItem")) == 0 )    pControl = new CListHeaderItemUI;
                break;
            case 15:
                if( _tcscmp(pstrClass, _T("ListTextElement")) == 0 )        pControl = new CListTextElementUI;
                break;
            case 16:
                if( _tcscmp(pstrClass, _T("HorizontalLayout")) == 0 )       pControl = new CHorizontalLayoutUI;
                else if( _tcscmp(pstrClass, _T("ListLabelElement")) == 0 )  pControl = new CListLabelElementUI;
                break;
            case 20:
                if( _tcscmp(pstrClass, _T("ListContainerElement")) == 0 )   pControl = new CListContainerElementUI;
                break;
			case 23:
				if( _tcscmp(pstrClass, _T("CommonScrollContainerUI")) == 0 )  pControl = new CommonScrollContainerUI;
				break;
            }
            // User-supplied control factory
            if( pControl == NULL && m_pCallback != NULL ) {
                pControl = m_pCallback->CreateControl(pstrClass);
            }
        }

        //ASSERT(pControl);当节点类型是Case时pControl为NULL， 2013.5.2 yjj 
        if( pControl == NULL ) continue;

        // Add children
        if( node.HasChildren() ) {
            _Parse(&node, pControl, pManager);
        }
        // Attach to parent
        // 因为某些属性和父窗口相关，比如selected，必须先Add到父窗口
        if( pParent != NULL ) {
            if( pContainer == NULL ) pContainer = static_cast<IContainerUI*>(pParent->GetInterface(_T("IContainer")));
            ASSERT(pContainer);
            if( pContainer == NULL ) return NULL;
            if( !pContainer->Add(pControl) ) {
                delete pControl;
                continue;
            }
        }
        // Init default attributes
        if( pManager ) {
            pControl->SetManager(pManager, NULL, false);
            LPCTSTR pDefaultAttributes = pManager->GetDefaultAttributeList(pstrClass);
            if( pDefaultAttributes ) {
                pControl->ApplyAttributeList(pDefaultAttributes);
            }
        }
        // Process attributes
        if( node.HasAttributes() ) {
            TCHAR szValue[500] = { 0 };
            SIZE_T cchLen = lengthof(szValue) - 1;
            // Set ordinary attributes
            int nAttributes = node.GetAttributeCount();
            for( int i = 0; i < nAttributes; i++ ) {
                pControl->SetAttribute(node.GetAttributeName(i), node.GetAttributeValue(i));
            }

            // Very custom attributes
            if ( node.GetAttributeValue(_T("stretch"), szValue, cchLen) ) {
                if( pParent == NULL ) continue;

                if( pDialogLayout == NULL ) pDialogLayout = static_cast<CDialogLayoutUI*>(pParent->GetInterface(_T("DialogLayout")));
                ASSERT(pDialogLayout);
                if( pDialogLayout == NULL ) continue;

                UINT uMode = 0;
                if( _tcsstr(szValue, _T("move_x")) != NULL ) uMode |= UISTRETCH_MOVE_X;
                if( _tcsstr(szValue, _T("move_y")) != NULL ) uMode |= UISTRETCH_MOVE_Y;
                if( _tcsstr(szValue, _T("move_xy")) != NULL ) uMode |= UISTRETCH_MOVE_X | UISTRETCH_MOVE_Y;
                if( _tcsstr(szValue, _T("size_x")) != NULL ) uMode |= UISTRETCH_SIZE_X;
                if( _tcsstr(szValue, _T("size_y")) != NULL ) uMode |= UISTRETCH_SIZE_Y;
                if( _tcsstr(szValue, _T("size_xy")) != NULL ) uMode |= UISTRETCH_SIZE_X | UISTRETCH_SIZE_Y;
                if( _tcsstr(szValue, _T("group")) != NULL ) uMode |= UISTRETCH_NEWGROUP;
                if( _tcsstr(szValue, _T("line")) != NULL ) uMode |= UISTRETCH_NEWLINE;
                pDialogLayout->SetStretchMode(pControl, uMode);

            }
        }
        if( pManager ) {
            pControl->SetManager(NULL, NULL, false);
        }
        // Return first item
        if( pReturn == NULL ) pReturn = pControl;
    }
    return pReturn;
}


bool CDialogBuilder::CreateChildControl(CMarkupNode* pNode, CControlUI* pParent /* = NULL */, CPaintManagerUI* pManager /* = NULL */)
{
	TCHAR szValue[500] = { 0 };
	SIZE_T cchLen = lengthof(szValue) - 1;
	if ( pNode->GetAttributeValue(_T("file"), szValue, cchLen) )
	{
		CDialogBuilder builder;

		CControlUI* pControl = builder.Create( (LPCTSTR)szValue, 0, 0, pManager, pParent );

		if( pParent != NULL ) {
			IContainerUI* pContainer = static_cast<IContainerUI*>(pParent->GetInterface(_T("IContainer")));
			ASSERT(pContainer);
			if( pContainer == NULL ) return false;
			if( !pContainer->Add(pControl) ) {
				delete pControl;
			}
		}

		return true;
	}
	return false;
}

/*

bool CDialogBuilder::DoDefCase( LPCTSTR pstrXml, IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent )
{
	if( !m_xml.LoadFromFile( pstrXml ) )
	{
	    TRACE("\n[CDialogBuilder::DoDefCase] load file false\n");
		return false;
	}

	m_pCallback = pCallback;

	CMarkupNode root = m_xml.GetRoot();
	if( !root.IsValid() ) return NULL;

	//设置dlg的属性
	if( pManager ) {
		LPCTSTR pstrClass = NULL;
		int nAttributes = 0;
		LPCTSTR pstrName = NULL;
		LPCTSTR pstrValue = NULL;
		LPTSTR pstr = NULL; 
		pstrClass = root.GetName();
		if( _tcscmp(pstrClass, _T("Window")) == 0 ) {
			if( pManager->GetPaintWindow() ) {
				int nAttributes = root.GetAttributeCount();
				for( int i = 0; i < nAttributes; i++ ) {
					pstrName = root.GetAttributeName(i);
					pstrValue = root.GetAttributeValue(i);
					if( _tcscmp(pstrName, _T("size")) == 0 ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pManager->SetInitSize(cx, cy);
					} 
					else if( _tcscmp(pstrName, _T("sizebox")) == 0 ) {
						RECT rcSizeBox = { 0 };
						LPTSTR pstr = NULL;
						rcSizeBox.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
						rcSizeBox.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
						rcSizeBox.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
						rcSizeBox.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
						pManager->SetSizeBox(rcSizeBox);
					}
					else if( _tcscmp(pstrName, _T("caption")) == 0 ) {
						RECT rcCaption = { 0 };
						LPTSTR pstr = NULL;
						rcCaption.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
						rcCaption.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
						rcCaption.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
						rcCaption.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
						pManager->SetCaptionRect(rcCaption);
					}
					else if( _tcscmp(pstrName, _T("roundcorner")) == 0 ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pManager->SetRoundCorner(cx, cy);
					} 
					else if( _tcscmp(pstrName, _T("mininfo")) == 0 ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pManager->SetMinInfo(cx, cy);
					}
					else if( _tcscmp(pstrName, _T("maxinfo")) == 0 ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pManager->SetMaxInfo(cx, cy);
					}
					else if( _tcscmp(pstrName, _T("showdirty")) == 0 ) {
						pManager->SetShowUpdateRect(_tcscmp(pstrValue, _T("true")) == 0);
					} 
					else if( _tcscmp(pstrName, _T("alpha")) == 0 ) {
						pManager->SetTransparent(_ttoi(pstrValue));
					} 
					else if( _tcscmp(pstrName, _T("bktrans")) == 0 ) {
						pManager->SetBackgroundTransparent(_tcscmp(pstrValue, _T("true")) == 0);
					} 
					else if( _tcscmp(pstrName, _T("disabledfontcolor")) == 0 ) {
						if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
						LPTSTR pstr = NULL;
						DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
						pManager->SetDefaultDisabledColor(clrColor);
					} 
					else if( _tcscmp(pstrName, _T("defaultfontcolor")) == 0 ) {
						if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
						LPTSTR pstr = NULL;
						DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
						pManager->SetDefaultFontColor(clrColor);
					}
					else if( _tcscmp(pstrName, _T("linkfontcolor")) == 0 ) {
						if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
						LPTSTR pstr = NULL;
						DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
						pManager->SetDefaultLinkFontColor(clrColor);
					} 
					else if( _tcscmp(pstrName, _T("linkhoverfontcolor")) == 0 ) {
						if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
						LPTSTR pstr = NULL;
						DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
						pManager->SetDefaultLinkHoverFontColor(clrColor);
					} 
					else if( _tcscmp(pstrName, _T("selectedcolor")) == 0 ) {
						if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
						LPTSTR pstr = NULL;
						DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
						pManager->SetDefaultSelectedBkColor(clrColor);
					} 
					 
				}
			}
		}
	}


	//设置其他控件的属性 



	for( CMarkupNode node = root.GetChild() ; node.IsValid(); node = node.GetSibling() ) {
		LPCTSTR pstrClass = node.GetName();
		if( _tcscmp(pstrClass, _T("Image")) == 0 || _tcscmp(pstrClass, _T("Font")) == 0 \
			|| _tcscmp(pstrClass, _T("Default")) == 0 ) continue;

		
		CControlUI* pControl = NULL; 

		SIZE_T cchLen = _tcslen(pstrClass);
		bool bCtrl = false;
		switch( cchLen ) { 
		case 4:
			if( _tcscmp(pstrClass, _T("Edit")) == 0  ||  _tcscmp(pstrClass, _T("List")) == 0 
				|| _tcscmp(pstrClass, _T("Text")) == 0 )
			{
				bCtrl = true;   
				break; 
			}   
		case 5:
			if( _tcscmp(pstrClass, _T("Combo")) == 0 || _tcscmp(pstrClass, _T("Label")) == 0 )            
			{
				bCtrl = true;   
				break; 
			}  
		case 6:
			if( _tcscmp(pstrClass, _T("Button")) == 0 || _tcscmp(pstrClass, _T("Option")) == 0 
				|| _tcscmp(pstrClass, _T("Slider")) == 0 )           
			{
				bCtrl = true;   
				break; 
			}  
		case 7:
			if( _tcscmp(pstrClass, _T("Control")) == 0  || _tcscmp(pstrClass, _T("ActiveX")) == 0 )          
			{
				bCtrl = true;   
				break; 
			}
		case 8:
			if( _tcscmp(pstrClass, _T("Progress")) == 0 || _tcscmp(pstrClass, _T("RichEdit")) == 0 )        
			{
				bCtrl = true;   
				break; 
			}
		case 9:
			if( _tcscmp(pstrClass, _T("Container")) == 0 || _tcscmp(pstrClass, _T("TabLayout")) == 0
				|| _tcscmp(pstrClass, _T("ScrollBar")) == 0 )        
			{
				bCtrl = true;   
				break; 
			}
		case 10:
			if( _tcscmp(pstrClass, _T("ListHeader")) == 0 || _tcscmp(pstrClass, _T("TileLayout")) == 0 )       
			{
				bCtrl = true;   
				break; 
			}
		case 12:
			if( _tcscmp(pstrClass, _T("DialogLayout")) == 0 )          
			{
				bCtrl = true;   
				break; 
			}
		case 14:
			if( _tcscmp(pstrClass, _T("VerticalLayout")) == 0 || _tcscmp(pstrClass, _T("ListHeaderItem")) == 0 )   
			{
				bCtrl = true;   
				break; 
			}
		case 15:
			if( _tcscmp(pstrClass, _T("ListTextElement")) == 0 )       
			{
				bCtrl = true;   
				break; 
			}
		case 16:
			if( _tcscmp(pstrClass, _T("HorizontalLayout")) == 0 || _tcscmp(pstrClass, _T("ListLabelElement")) == 0 ) 
			{
				bCtrl = true;   
				break; 
			}
		case 20:
			if( _tcscmp(pstrClass, _T("ListContainerElement")) == 0 )  
			{
				bCtrl = true;   
				break; 
			}
		}

		if( bCtrl )
			pManager->FindControl( )
	}









	if( pManager ) {
		pControl->SetManager(pManager, NULL, false);
		LPCTSTR pDefaultAttributes = pManager->GetDefaultAttributeList(pstrClass);
		if( pDefaultAttributes ) {
			pControl->ApplyAttributeList(pDefaultAttributes);
		}
	}
	// Process attributes
	if( node.HasAttributes() ) {
		TCHAR szValue[500] = { 0 };
		SIZE_T cchLen = lengthof(szValue) - 1;
		// Set ordinary attributes
		int nAttributes = node.GetAttributeCount();
		for( int i = 0; i < nAttributes; i++ ) {
			pControl->SetAttribute(node.GetAttributeName(i), node.GetAttributeValue(i));
		}

		// Very custom attributes
		if ( node.GetAttributeValue(_T("stretch"), szValue, cchLen) ) {
			if( pParent == NULL ) continue;

			if( pDialogLayout == NULL ) pDialogLayout = static_cast<CDialogLayoutUI*>(pParent->GetInterface(_T("DialogLayout")));
			ASSERT(pDialogLayout);
			if( pDialogLayout == NULL ) continue;

			UINT uMode = 0;
			if( _tcsstr(szValue, _T("move_x")) != NULL ) uMode |= UISTRETCH_MOVE_X;
			if( _tcsstr(szValue, _T("move_y")) != NULL ) uMode |= UISTRETCH_MOVE_Y;
			if( _tcsstr(szValue, _T("move_xy")) != NULL ) uMode |= UISTRETCH_MOVE_X | UISTRETCH_MOVE_Y;
			if( _tcsstr(szValue, _T("size_x")) != NULL ) uMode |= UISTRETCH_SIZE_X;
			if( _tcsstr(szValue, _T("size_y")) != NULL ) uMode |= UISTRETCH_SIZE_Y;
			if( _tcsstr(szValue, _T("size_xy")) != NULL ) uMode |= UISTRETCH_SIZE_X | UISTRETCH_SIZE_Y;
			if( _tcsstr(szValue, _T("group")) != NULL ) uMode |= UISTRETCH_NEWGROUP;
			if( _tcsstr(szValue, _T("line")) != NULL ) uMode |= UISTRETCH_NEWLINE;
			pDialogLayout->SetStretchMode(pControl, uMode);

		}
	}
	if( pManager ) {
		pControl->SetManager(NULL, NULL, false);
	}
	// Return first item
	if( pReturn == NULL ) pReturn = pControl;


}*/



} // namespace DuiLib

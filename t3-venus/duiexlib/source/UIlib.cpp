//
//
// DirectUI - UI Library
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2006-2007 Bjarke Viksoe.
//
// This code may be used in compiled form in any way you desire. These
// source files may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//
//


#include "stdafx.h"
#include "UIlib.h"

ULONG_PTR m_Token;
//Gdiplus::GdiplusShutdown(m_Token);

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  dwReason, LPVOID /*lpReserved*/)
{
   switch( dwReason ) {
   case DLL_PROCESS_ATTACH:
	   {
		   Gdiplus::GdiplusStartupInput input; 
		   Gdiplus::GdiplusStartup(& m_Token, & input, NULL); 
	   }
	   break;
   case DLL_THREAD_ATTACH:
   case DLL_THREAD_DETACH:
   case DLL_PROCESS_DETACH:
       ::DisableThreadLibraryCalls((HMODULE)hModule);
       break;
    }
    return TRUE;
}


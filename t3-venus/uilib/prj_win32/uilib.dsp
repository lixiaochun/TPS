# Microsoft Developer Studio Project File - Name="UILib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=UILib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "uilib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "uilib.mak" CFG="UILib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UILib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "UILib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "UILib"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UILib - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\include" /I "..\..\..\10-common\include\nms1" /I "..\..\common\include\platform sdk\INCLUDE" /I "..\..\common\include" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "WIN32" /D "NDEBUG" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\10-common\lib\release\win32\uilib.lib"

!ELSEIF  "$(CFG)" == "UILib - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\..\..\10-common\include\nms1" /I "..\..\common\include\platform sdk\INCLUDE" /I "..\..\common\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\10-common\lib\debug\win32\uilib.lib"

!ENDIF 

# Begin Target

# Name "UILib - Win32 Release"
# Name "UILib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\source\BMPDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\source\CheckButton.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ColorSelCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ColourPopup.cpp
# End Source File
# Begin Source File

SOURCE=..\source\FlatButton.cpp
# End Source File
# Begin Source File

SOURCE=..\source\FlatDDButton.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinCheckList.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinDateTime.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinDDListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinIPCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinPage.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinScroll.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinSliderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinSpinCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SkinTab.cpp
# End Source File
# Begin Source File

SOURCE=..\source\StaticEx.cpp
# End Source File
# Begin Source File

SOURCE=..\source\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\source\TextButton30.cpp
# End Source File
# Begin Source File

SOURCE=..\source\UITools.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Include\StdAfx.h
# End Source File
# Begin Source File

SOURCE="..\..\..\10-common\include\nms1\UILibHead.h"
# End Source File
# End Group
# Begin Source File

SOURCE=..\Compile.bat
# End Source File
# Begin Source File

SOURCE=.\Compile.bat
# End Source File
# Begin Source File

SOURCE=..\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\UILib.mak
# End Source File
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="tpTools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=tpTools - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tpTools.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tpTools.mak" CFG="tpTools - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tpTools - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "tpTools - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "tpTools"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tpTools - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /ZI /Od /I "../include" /I "../source" /I "./" /I "..\..\common\include" /I "..\..\..\10-common\include\nms1" /I "..\..\..\10-common\include\system1" /I "..\..\..\10-common\include\protocol" /I "..\..\..\10-common\include\platform" /I "..\..\common\include\platform sdk\include" /I "..\..\..\10-common\include\tp" /I "..\..\uiexlib\include\\" /I "..\..\UIFactory\include\Tinyxml" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 umstooldll.lib WS2_32.LIB /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"nafxcw" /nodefaultlib:"nafxcwd" /nodefaultlib:"libcd" /nodefaultlib:"libc" /nodefaultlib:"libcmt" /nodefaultlib:"msvcrtd" /nodefaultlib:"Imm32.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"strmiids.lib" /libpath:".\Release" /libpath:"..\..\..\10-common\lib\release\win32" /libpath:"..\..\common\include\platform sdk\LIB" /mapinfo:lines
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "tpTools - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../include" /I "../source" /I "./" /I "..\..\common\include" /I "..\..\..\10-common\include\nms1" /I "..\..\..\10-common\include\system1" /I "..\..\..\10-common\include\protocol" /I "..\..\..\10-common\include\platform" /I "..\..\common\include\platform sdk\include" /I "..\..\..\10-common\include\tp" /I "..\..\uiexlib\include\\" /I "..\..\UIFactory\include\Tinyxml" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 umstooldll.lib WS2_32.LIB /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:".\Debug" /libpath:"..\..\..\10-common\lib\debug\win32" /libpath:"..\..\common\include\platform sdk\LIB"

!ENDIF 

# Begin Target

# Name "tpTools - Win32 Release"
# Name "tpTools - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\source\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\source\tpTools.cpp
# End Source File
# Begin Source File

SOURCE=.\tpTools.rc
# End Source File
# Begin Source File

SOURCE=..\source\tpToolsDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\include\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\include\tpTools.h
# End Source File
# Begin Source File

SOURCE=..\include\tpToolsDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\tpTools.ico
# End Source File
# Begin Source File

SOURCE=.\res\tpTools.rc2
# End Source File
# End Group
# Begin Group "tinyxml"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\UIFactory\include\Tinyxml\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\UIFactory\include\Tinyxml\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=..\..\UIFactory\include\Tinyxml\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=..\..\UIFactory\include\Tinyxml\tinyxmlparser.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

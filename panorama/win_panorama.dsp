# Microsoft Developer Studio Project File - Name="panorama" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT O. Senhadji El Rhazi ENPC 2002, Paris France **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=panorama - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "win_panorama.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "win_panorama.mak" CFG="panorama - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "panorama - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "panorama - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "panorama - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "c:/winlib/include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib win1.lib win2.lib win3.lib win4.lib /nologo /subsystem:console /machine:I386 /libpath:"c:/winlib/lib/vc6/release"

!ELSEIF  "$(CFG)" == "panorama - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:/winlib/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib win1.lib win2.lib win3.lib win4.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /pdbtype:sept /libpath:"C:/winlib/lib/vc6/debug"

!ENDIF 

# Begin Target

# Name "panorama - Win32 Release"
# Name "panorama - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Common\camera.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\cimage.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\dimage.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\fft.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\image.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\inverse.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\mat.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\pic.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\rec.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\vect.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Common\camera.h
# End Source File
# Begin Source File

SOURCE=..\Common\cimage.h
# End Source File
# Begin Source File

SOURCE=..\Common\complex.h
# End Source File
# Begin Source File

SOURCE=..\Common\dimage.h
# End Source File
# Begin Source File

SOURCE=..\Common\fft.h
# End Source File
# Begin Source File

SOURCE=..\Common\image.h
# End Source File
# Begin Source File

SOURCE=..\Common\inverse.h
# End Source File
# Begin Source File

SOURCE=..\Common\lmeds.h
# End Source File
# Begin Source File

SOURCE=..\Common\mat.h
# End Source File
# Begin Source File

SOURCE=..\Common\miniP.h
# End Source File
# Begin Source File

SOURCE=..\Common\pic.h
# End Source File
# Begin Source File

SOURCE=..\Common\rec.h
# End Source File
# Begin Source File

SOURCE=..\Common\vect.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

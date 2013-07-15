# Microsoft Developer Studio Project File - Name="vc5" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=vc5 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vc5.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vc5.mak" CFG="vc5 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vc5 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "vc5 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vc5 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".." /I "..\..\include" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "HAVE_STDLIB_H" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 fu.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "vc5 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I ".." /I "..\..\include" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "HAVE_STDLIB_H" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "vc5 - Win32 Release"
# Name "vc5 - Win32 Debug"
# Begin Source File

SOURCE=..\..\libgroff\assert.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\change_lf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\cmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\cset.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\device.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\errarg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\error.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\fatal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\filename.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\font.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\fontfile.cpp
# End Source File
# Begin Source File

SOURCE="..\..\..\grep-2.3\src\getopt.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\grep-2.3\src\getopt1.c"
# End Source File
# Begin Source File

SOURCE=..\groff.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\iftoa.c
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\illegal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\itoa.c
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\lf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\lineno.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\macropath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\nametoindex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\new.cpp
# End Source File
# Begin Source File

SOURCE=..\pipeline_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\prime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\progname.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\ptable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\searchpath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\string.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\strsave.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\tmpfile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\libgroff\version.cpp
# End Source File
# End Target
# End Project

# Microsoft Developer Studio Generated NMAKE File, Based on VC6.dsp
!IF "$(CFG)" == ""
CFG=VC6 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to VC6 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "VC6 - Win32 Release" && "$(CFG)" != "VC6 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VC6.mak" CFG="VC6 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VC6 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "VC6 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VC6 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\VC6.exe"


CLEAN :
	-@erase "$(INTDIR)\bits.obj"
	-@erase "$(INTDIR)\crypt.obj"
	-@erase "$(INTDIR)\deflate.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\gzip.obj"
	-@erase "$(INTDIR)\inflate.obj"
	-@erase "$(INTDIR)\lzw.obj"
	-@erase "$(INTDIR)\trees.obj"
	-@erase "$(INTDIR)\unlzh.obj"
	-@erase "$(INTDIR)\unlzw.obj"
	-@erase "$(INTDIR)\unpack.obj"
	-@erase "$(INTDIR)\unzip.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\zip.obj"
	-@erase "$(OUTDIR)\VC6.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\VC6.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\VC6.pdb" /machine:I386 /out:"$(OUTDIR)\VC6.exe" 
LINK32_OBJS= \
	"$(INTDIR)\zip.obj" \
	"$(INTDIR)\crypt.obj" \
	"$(INTDIR)\deflate.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\gzip.obj" \
	"$(INTDIR)\inflate.obj" \
	"$(INTDIR)\lzw.obj" \
	"$(INTDIR)\trees.obj" \
	"$(INTDIR)\unlzh.obj" \
	"$(INTDIR)\unlzw.obj" \
	"$(INTDIR)\unpack.obj" \
	"$(INTDIR)\unzip.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\bits.obj"

"$(OUTDIR)\VC6.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\gunzip.exe" "$(OUTDIR)\VC6.bsc"


CLEAN :
	-@erase "$(INTDIR)\bits.obj"
	-@erase "$(INTDIR)\bits.sbr"
	-@erase "$(INTDIR)\crypt.obj"
	-@erase "$(INTDIR)\crypt.sbr"
	-@erase "$(INTDIR)\deflate.obj"
	-@erase "$(INTDIR)\deflate.sbr"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\getopt.sbr"
	-@erase "$(INTDIR)\gzip.obj"
	-@erase "$(INTDIR)\gzip.sbr"
	-@erase "$(INTDIR)\inflate.obj"
	-@erase "$(INTDIR)\inflate.sbr"
	-@erase "$(INTDIR)\lzw.obj"
	-@erase "$(INTDIR)\lzw.sbr"
	-@erase "$(INTDIR)\trees.obj"
	-@erase "$(INTDIR)\trees.sbr"
	-@erase "$(INTDIR)\unlzh.obj"
	-@erase "$(INTDIR)\unlzh.sbr"
	-@erase "$(INTDIR)\unlzw.obj"
	-@erase "$(INTDIR)\unlzw.sbr"
	-@erase "$(INTDIR)\unpack.obj"
	-@erase "$(INTDIR)\unpack.sbr"
	-@erase "$(INTDIR)\unzip.obj"
	-@erase "$(INTDIR)\unzip.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\zip.obj"
	-@erase "$(INTDIR)\zip.sbr"
	-@erase "$(OUTDIR)\gunzip.exe"
	-@erase "$(OUTDIR)\gunzip.ilk"
	-@erase "$(OUTDIR)\gunzip.pdb"
	-@erase "$(OUTDIR)\VC6.bsc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\VC6.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ  /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\VC6.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\zip.sbr" \
	"$(INTDIR)\crypt.sbr" \
	"$(INTDIR)\deflate.sbr" \
	"$(INTDIR)\getopt.sbr" \
	"$(INTDIR)\gzip.sbr" \
	"$(INTDIR)\inflate.sbr" \
	"$(INTDIR)\lzw.sbr" \
	"$(INTDIR)\trees.sbr" \
	"$(INTDIR)\unlzh.sbr" \
	"$(INTDIR)\unlzw.sbr" \
	"$(INTDIR)\unpack.sbr" \
	"$(INTDIR)\unzip.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\bits.sbr"

"$(OUTDIR)\VC6.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\gunzip.pdb" /debug /machine:I386 /out:"$(OUTDIR)\gunzip.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\zip.obj" \
	"$(INTDIR)\crypt.obj" \
	"$(INTDIR)\deflate.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\gzip.obj" \
	"$(INTDIR)\inflate.obj" \
	"$(INTDIR)\lzw.obj" \
	"$(INTDIR)\trees.obj" \
	"$(INTDIR)\unlzh.obj" \
	"$(INTDIR)\unlzw.obj" \
	"$(INTDIR)\unpack.obj" \
	"$(INTDIR)\unzip.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\bits.obj"

"$(OUTDIR)\gunzip.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("VC6.dep")
!INCLUDE "VC6.dep"
!ELSE 
!MESSAGE Warning: cannot find "VC6.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "VC6 - Win32 Release" || "$(CFG)" == "VC6 - Win32 Debug"
SOURCE=..\bits.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\bits.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\bits.obj"	"$(INTDIR)\bits.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\crypt.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\crypt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\crypt.obj"	"$(INTDIR)\crypt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\deflate.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\deflate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\deflate.obj"	"$(INTDIR)\deflate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\getopt.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\getopt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\getopt.obj"	"$(INTDIR)\getopt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\gzip.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\gzip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\gzip.obj"	"$(INTDIR)\gzip.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\inflate.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\inflate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\inflate.obj"	"$(INTDIR)\inflate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\lzw.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\lzw.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\lzw.obj"	"$(INTDIR)\lzw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\trees.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\trees.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\trees.obj"	"$(INTDIR)\trees.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\unlzh.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\unlzh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\unlzh.obj"	"$(INTDIR)\unlzh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\unlzw.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\unlzw.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\unlzw.obj"	"$(INTDIR)\unlzw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\unpack.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\unpack.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\unpack.obj"	"$(INTDIR)\unpack.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\unzip.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\unzip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\unzip.obj"	"$(INTDIR)\unzip.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\util.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\zip.c

!IF  "$(CFG)" == "VC6 - Win32 Release"


"$(INTDIR)\zip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "VC6 - Win32 Debug"


"$(INTDIR)\zip.obj"	"$(INTDIR)\zip.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 


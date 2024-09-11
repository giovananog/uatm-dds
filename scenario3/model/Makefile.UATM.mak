# Microsoft Developer Studio Generated NMAKE File
!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified. Defaulting to Win32 Debug.
!ENDIF

!IF "$(CFG)" == "Win32 Debug" || "$(CFG)" == "Win32 Release" || "$(CFG)" == "Win32 Static Debug" || "$(CFG)" == "Win32 Static Release"
!ELSE
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Makefile.UATM.mak" CFG="Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE
!ERROR An invalid configuration was specified.
!ENDIF

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

!IF "$(DEPGEN)" == ""
!IF EXISTS("$(MPC_ROOT)/depgen.pl")
DEPGEN=perl $(MPC_ROOT)/depgen.pl -i -t nmake
!ELSEIF EXISTS("$(DEPGEN_ROOT)/depgen.pl")
DEPGEN=perl $(DEPGEN_ROOT)/depgen.pl -i -t nmake
!ELSEIF EXISTS("$(ACE_ROOT)/bin/depgen.pl")
DEPGEN=perl $(ACE_ROOT)/bin/depgen.pl -i -t nmake
!ENDIF
!ENDIF

GENERATED_DIRTY = "UATMTypeSupportC.h" "UATMTypeSupportS.h" "UATMTypeSupportC.inl" "UATMTypeSupportS.cpp" "UATMTypeSupportC.cpp" "UATMC.h" "UATMS.h" "UATMC.inl" "UATMS.cpp" "UATMC.cpp" "UATMTypeSupportImpl.h" "UATMTypeSupport.idl" "UATMTypeSupportImpl.cpp"

!IF  "$(CFG)" == "Win32 Debug"

OUTDIR=.
INTDIR=Debug\UATM\X64

ALL : "$(INTDIR)" "$(OUTDIR)" __prebuild__ DEPENDCHECK $(GENERATED_DIRTY) ".\UATMd.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I"$(ACE_ROOT)" -I"$(TAO_ROOT)" -I"$(DDS_ROOT)" -I"C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" -I"$(DDS_ROOT)\tools\modeling\codegen" -D_DEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DOPENDDS_RAPIDJSON -DUATM_BUILD_DLL -f "Makefile.UATM.dep" "UATMC.cpp" "UATMS.cpp" "UATMTraits.cpp" "UATMTypeSupportC.cpp" "UATMTypeSupportImpl.cpp" "UATMTypeSupportS.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\UATMd.pdb"
	-@del /f/q ".\UATMd.dll"
	-@del /f/q "$(OUTDIR)\UATMd.lib"
	-@del /f/q "$(OUTDIR)\UATMd.exp"
	-@del /f/q "$(OUTDIR)\UATMd.ilk"
	-@del /f/q "UATMTypeSupportC.h"
	-@del /f/q "UATMTypeSupportS.h"
	-@del /f/q "UATMTypeSupportC.inl"
	-@del /f/q "UATMTypeSupportS.cpp"
	-@del /f/q "UATMTypeSupportC.cpp"
	-@del /f/q "UATMC.h"
	-@del /f/q "UATMS.h"
	-@del /f/q "UATMC.inl"
	-@del /f/q "UATMS.cpp"
	-@del /f/q "UATMC.cpp"
	-@del /f/q "UATMTypeSupportImpl.h"
	-@del /f/q "UATMTypeSupport.idl"
	-@del /f/q "UATMTypeSupportImpl.cpp"

"$(INTDIR)" :
	if not exist "Debug\$(NULL)" mkdir "Debug"
	if not exist "Debug\UATM\$(NULL)" mkdir "Debug\UATM"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3   /EHsc /Zi /MDd /GR /Gy /wd4355 /wd4503 /wd4355 /wd4250 /wd4290 /Fd"$(INTDIR)/" /I "$(ACE_ROOT)" /I "$(TAO_ROOT)" /I "$(DDS_ROOT)" /I "C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" /I "$(DDS_ROOT)\tools\modeling\codegen" /D _DEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D OPENDDS_RAPIDJSON /D UATM_BUILD_DLL /D MPC_LIB_MODIFIER=\"d\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib TAOd.lib TAO_AnyTypeCoded.lib TAO_PortableServerd.lib TAO_Valuetyped.lib TAO_CodecFactoryd.lib TAO_PId.lib TAO_BiDirGIOPd.lib OpenDDS_Dcpsd.lib iphlpapi.lib /libpath:"." /libpath:"$(ACE_ROOT)\lib" /libpath:"$(DDS_ROOT)\lib" /nologo /subsystem:windows /dll /debug /pdb:".\UATMd.pdb" /machine:X64 /out:".\UATMd.dll" /implib:"$(OUTDIR)\UATMd.lib"
LINK32_OBJS= \
	"$(INTDIR)\UATMC.obj" \
	"$(INTDIR)\UATMS.obj" \
	"$(INTDIR)\UATMTraits.obj" \
	"$(INTDIR)\UATMTypeSupportC.obj" \
	"$(INTDIR)\UATMTypeSupportImpl.obj" \
	"$(INTDIR)\UATMTypeSupportS.obj"

".\UATMd.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist ".\UATMd.dll.manifest" mt.exe -manifest ".\UATMd.dll.manifest" -outputresource:$@;2

__prebuild__:
	@perl $(DDS_ROOT)/tools/scripts/expfile.pl UATM

!ELSEIF  "$(CFG)" == "Win32 Release"

OUTDIR=.
INTDIR=Release\UATM\X64

ALL : "$(INTDIR)" "$(OUTDIR)" __prebuild__ DEPENDCHECK $(GENERATED_DIRTY) ".\UATM.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I"$(ACE_ROOT)" -I"$(TAO_ROOT)" -I"$(DDS_ROOT)" -I"C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" -I"$(DDS_ROOT)\tools\modeling\codegen" -DNDEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DOPENDDS_RAPIDJSON -DUATM_BUILD_DLL -f "Makefile.UATM.dep" "UATMC.cpp" "UATMS.cpp" "UATMTraits.cpp" "UATMTypeSupportC.cpp" "UATMTypeSupportImpl.cpp" "UATMTypeSupportS.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q ".\UATM.dll"
	-@del /f/q "$(OUTDIR)\UATM.lib"
	-@del /f/q "$(OUTDIR)\UATM.exp"
	-@del /f/q "$(OUTDIR)\UATM.ilk"
	-@del /f/q "UATMTypeSupportC.h"
	-@del /f/q "UATMTypeSupportS.h"
	-@del /f/q "UATMTypeSupportC.inl"
	-@del /f/q "UATMTypeSupportS.cpp"
	-@del /f/q "UATMTypeSupportC.cpp"
	-@del /f/q "UATMC.h"
	-@del /f/q "UATMS.h"
	-@del /f/q "UATMC.inl"
	-@del /f/q "UATMS.cpp"
	-@del /f/q "UATMC.cpp"
	-@del /f/q "UATMTypeSupportImpl.h"
	-@del /f/q "UATMTypeSupport.idl"
	-@del /f/q "UATMTypeSupportImpl.cpp"

"$(INTDIR)" :
	if not exist "Release\$(NULL)" mkdir "Release"
	if not exist "Release\UATM\$(NULL)" mkdir "Release\UATM"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4503 /wd4355 /wd4250 /wd4290 /I "$(ACE_ROOT)" /I "$(TAO_ROOT)" /I "$(DDS_ROOT)" /I "C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" /I "$(DDS_ROOT)\tools\modeling\codegen" /D NDEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D OPENDDS_RAPIDJSON /D UATM_BUILD_DLL  /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib TAO.lib TAO_AnyTypeCode.lib TAO_PortableServer.lib TAO_Valuetype.lib TAO_CodecFactory.lib TAO_PI.lib TAO_BiDirGIOP.lib OpenDDS_Dcps.lib iphlpapi.lib /libpath:"." /libpath:"$(ACE_ROOT)\lib" /libpath:"$(DDS_ROOT)\lib" /nologo /subsystem:windows /dll  /machine:X64 /out:".\UATM.dll" /implib:"$(OUTDIR)\UATM.lib"
LINK32_OBJS= \
	"$(INTDIR)\UATMC.obj" \
	"$(INTDIR)\UATMS.obj" \
	"$(INTDIR)\UATMTraits.obj" \
	"$(INTDIR)\UATMTypeSupportC.obj" \
	"$(INTDIR)\UATMTypeSupportImpl.obj" \
	"$(INTDIR)\UATMTypeSupportS.obj"

".\UATM.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist ".\UATM.dll.manifest" mt.exe -manifest ".\UATM.dll.manifest" -outputresource:$@;2

__prebuild__:
	@perl $(DDS_ROOT)/tools/scripts/expfile.pl UATM

!ELSEIF  "$(CFG)" == "Win32 Static Debug"

OUTDIR=.
INTDIR=Static_Debug\UATM\X64

ALL : "$(INTDIR)" "$(OUTDIR)" __prebuild__ DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\UATMsd.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I"$(ACE_ROOT)" -I"$(TAO_ROOT)" -I"$(DDS_ROOT)" -I"C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" -I"$(DDS_ROOT)\tools\modeling\codegen" -D_DEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DOPENDDS_RAPIDJSON -DACE_AS_STATIC_LIBS -DTAO_AS_STATIC_LIBS -f "Makefile.UATM.dep" "UATMC.cpp" "UATMS.cpp" "UATMTraits.cpp" "UATMTypeSupportC.cpp" "UATMTypeSupportImpl.cpp" "UATMTypeSupportS.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\UATMsd.lib"
	-@del /f/q "$(OUTDIR)\UATMsd.exp"
	-@del /f/q "$(OUTDIR)\UATMsd.ilk"
	-@del /f/q ".\UATMsd.pdb"
	-@del /f/q "UATMTypeSupportC.h"
	-@del /f/q "UATMTypeSupportS.h"
	-@del /f/q "UATMTypeSupportC.inl"
	-@del /f/q "UATMTypeSupportS.cpp"
	-@del /f/q "UATMTypeSupportC.cpp"
	-@del /f/q "UATMC.h"
	-@del /f/q "UATMS.h"
	-@del /f/q "UATMC.inl"
	-@del /f/q "UATMS.cpp"
	-@del /f/q "UATMC.cpp"
	-@del /f/q "UATMTypeSupportImpl.h"
	-@del /f/q "UATMTypeSupport.idl"
	-@del /f/q "UATMTypeSupportImpl.cpp"

"$(INTDIR)" :
	if not exist "Static_Debug\$(NULL)" mkdir "Static_Debug"
	if not exist "Static_Debug\UATM\$(NULL)" mkdir "Static_Debug\UATM"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3   /EHsc /Zi /GR /Gy /MDd /wd4355 /wd4503 /wd4355 /wd4250 /wd4290 /Fd".\UATMsd.pdb" /I "$(ACE_ROOT)" /I "$(TAO_ROOT)" /I "$(DDS_ROOT)" /I "C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" /I "$(DDS_ROOT)\tools\modeling\codegen" /D _DEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D OPENDDS_RAPIDJSON /D ACE_AS_STATIC_LIBS /D TAO_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"sd\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:X64 /out:".\UATMsd.lib"
LINK32_OBJS= \
	"$(INTDIR)\UATMC.obj" \
	"$(INTDIR)\UATMS.obj" \
	"$(INTDIR)\UATMTraits.obj" \
	"$(INTDIR)\UATMTypeSupportC.obj" \
	"$(INTDIR)\UATMTypeSupportImpl.obj" \
	"$(INTDIR)\UATMTypeSupportS.obj"

"$(OUTDIR)\UATMsd.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\UATMsd.lib.manifest" mt.exe -manifest "$(OUTDIR)\UATMsd.lib.manifest" -outputresource:$@;2

__prebuild__:
	@perl $(DDS_ROOT)/tools/scripts/expfile.pl UATM

!ELSEIF  "$(CFG)" == "Win32 Static Release"

OUTDIR=.
INTDIR=Static_Release\UATM\X64

ALL : "$(INTDIR)" "$(OUTDIR)" __prebuild__ DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\UATMs.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I"$(ACE_ROOT)" -I"$(TAO_ROOT)" -I"$(DDS_ROOT)" -I"C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" -I"$(DDS_ROOT)\tools\modeling\codegen" -DNDEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DOPENDDS_RAPIDJSON -DACE_AS_STATIC_LIBS -DTAO_AS_STATIC_LIBS -f "Makefile.UATM.dep" "UATMC.cpp" "UATMS.cpp" "UATMTraits.cpp" "UATMTypeSupportC.cpp" "UATMTypeSupportImpl.cpp" "UATMTypeSupportS.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\UATMs.lib"
	-@del /f/q "$(OUTDIR)\UATMs.exp"
	-@del /f/q "$(OUTDIR)\UATMs.ilk"
	-@del /f/q "UATMTypeSupportC.h"
	-@del /f/q "UATMTypeSupportS.h"
	-@del /f/q "UATMTypeSupportC.inl"
	-@del /f/q "UATMTypeSupportS.cpp"
	-@del /f/q "UATMTypeSupportC.cpp"
	-@del /f/q "UATMC.h"
	-@del /f/q "UATMS.h"
	-@del /f/q "UATMC.inl"
	-@del /f/q "UATMS.cpp"
	-@del /f/q "UATMC.cpp"
	-@del /f/q "UATMTypeSupportImpl.h"
	-@del /f/q "UATMTypeSupport.idl"
	-@del /f/q "UATMTypeSupportImpl.cpp"

"$(INTDIR)" :
	if not exist "Static_Release\$(NULL)" mkdir "Static_Release"
	if not exist "Static_Release\UATM\$(NULL)" mkdir "Static_Release\UATM"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4503 /wd4355 /wd4250 /wd4290 /I "$(ACE_ROOT)" /I "$(TAO_ROOT)" /I "$(DDS_ROOT)" /I "C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" /I "$(DDS_ROOT)\tools\modeling\codegen" /D NDEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D OPENDDS_RAPIDJSON /D ACE_AS_STATIC_LIBS /D TAO_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"s\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:X64 /out:".\UATMs.lib"
LINK32_OBJS= \
	"$(INTDIR)\UATMC.obj" \
	"$(INTDIR)\UATMS.obj" \
	"$(INTDIR)\UATMTraits.obj" \
	"$(INTDIR)\UATMTypeSupportC.obj" \
	"$(INTDIR)\UATMTypeSupportImpl.obj" \
	"$(INTDIR)\UATMTypeSupportS.obj"

"$(OUTDIR)\UATMs.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\UATMs.lib.manifest" mt.exe -manifest "$(OUTDIR)\UATMs.lib.manifest" -outputresource:$@;2

__prebuild__:
	@perl $(DDS_ROOT)/tools/scripts/expfile.pl UATM

!ENDIF

CLEAN :
	-@del /f/s/q "$(INTDIR)"

"$(OUTDIR)" ::
	if not exist "$(OUTDIR)\$(NULL)" mkdir "$(OUTDIR)"

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
!IF EXISTS("Makefile.UATM.dep")
!INCLUDE "Makefile.UATM.dep"
!ENDIF
!ENDIF

!IF "$(CFG)" == "Win32 Debug" || "$(CFG)" == "Win32 Release" || "$(CFG)" == "Win32 Static Debug" || "$(CFG)" == "Win32 Static Release" 
SOURCE="UATMC.cpp"

"$(INTDIR)\UATMC.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UATMC.obj" $(SOURCE)

SOURCE="UATMS.cpp"

"$(INTDIR)\UATMS.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UATMS.obj" $(SOURCE)

SOURCE="UATMTraits.cpp"

"$(INTDIR)\UATMTraits.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UATMTraits.obj" $(SOURCE)

SOURCE="UATMTypeSupportC.cpp"

"$(INTDIR)\UATMTypeSupportC.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UATMTypeSupportC.obj" $(SOURCE)

SOURCE="UATMTypeSupportImpl.cpp"

"$(INTDIR)\UATMTypeSupportImpl.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UATMTypeSupportImpl.obj" $(SOURCE)

SOURCE="UATMTypeSupportS.cpp"

"$(INTDIR)\UATMTypeSupportS.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UATMTypeSupportS.obj" $(SOURCE)

!IF  "$(CFG)" == "Win32 Debug"
SOURCE="UATMTypeSupport.idl"

InputPath=UATMTypeSupport.idl

"UATMTypeSupportC.h" "UATMTypeSupportS.h" "UATMTypeSupportC.inl" "UATMTypeSupportS.cpp" "UATMTypeSupportC.cpp" : $(SOURCE)  "$(ACE_ROOT)\bin\tao_idl.exe" "$(ACE_ROOT)\lib\TAO_IDL_BEd.dll" "$(ACE_ROOT)\lib\TAO_IDL_FEd.dll"
	<<tempfile-Win32-Debug-idl_files-UATMTypeSupport_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(ACE_ROOT)\bin\tao_idl -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I $(TAO_ROOT) --idl-version 4 --unknown-annotations ignore -as -Sa -St -I$(DDS_ROOT) -Wb,export_macro=UATM_Export -Wb,export_include=UATM_export.h "$(InputPath)"
<<

SOURCE="UATM.idl"

InputPath=UATM.idl

"UATMC.h" "UATMS.h" "UATMC.inl" "UATMS.cpp" "UATMC.cpp" : $(SOURCE)  "$(ACE_ROOT)\bin\tao_idl.exe" "$(ACE_ROOT)\lib\TAO_IDL_BEd.dll" "$(ACE_ROOT)\lib\TAO_IDL_FEd.dll"
	<<tempfile-Win32-Debug-idl_files-UATM_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(ACE_ROOT)\bin\tao_idl -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I $(TAO_ROOT) --idl-version 4 --unknown-annotations ignore -as -Sa -St -I$(DDS_ROOT) -Wb,export_macro=UATM_Export -Wb,export_include=UATM_export.h "$(InputPath)"
<<

SOURCE="UATM.idl"

InputPath=UATM.idl

"UATMTypeSupportImpl.h" "UATMTypeSupport.idl" "UATMTypeSupportImpl.cpp" : $(SOURCE)  "$(DDS_ROOT)\bin\opendds_idl.exe" "$(DDS_ROOT)\dds\idl\IDLTemplate.txt"
	<<tempfile-Win32-Debug-typesupport_files-UATM_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(DDS_ROOT)\bin\opendds_idl -Sa -St -Wb,export_macro=UATM_Export "$(InputPath)"
<<

!ELSEIF  "$(CFG)" == "Win32 Release"
SOURCE="UATMTypeSupport.idl"

InputPath=UATMTypeSupport.idl

"UATMTypeSupportC.h" "UATMTypeSupportS.h" "UATMTypeSupportC.inl" "UATMTypeSupportS.cpp" "UATMTypeSupportC.cpp" : $(SOURCE)  "$(ACE_ROOT)\bin\tao_idl.exe" "$(ACE_ROOT)\lib\TAO_IDL_BE.dll" "$(ACE_ROOT)\lib\TAO_IDL_FE.dll"
	<<tempfile-Win32-Release-idl_files-UATMTypeSupport_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(ACE_ROOT)\bin\tao_idl -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I $(TAO_ROOT) --idl-version 4 --unknown-annotations ignore -as -Sa -St -I$(DDS_ROOT) -Wb,export_macro=UATM_Export -Wb,export_include=UATM_export.h "$(InputPath)"
<<

SOURCE="UATM.idl"

InputPath=UATM.idl

"UATMC.h" "UATMS.h" "UATMC.inl" "UATMS.cpp" "UATMC.cpp" : $(SOURCE)  "$(ACE_ROOT)\bin\tao_idl.exe" "$(ACE_ROOT)\lib\TAO_IDL_BE.dll" "$(ACE_ROOT)\lib\TAO_IDL_FE.dll"
	<<tempfile-Win32-Release-idl_files-UATM_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(ACE_ROOT)\bin\tao_idl -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I $(TAO_ROOT) --idl-version 4 --unknown-annotations ignore -as -Sa -St -I$(DDS_ROOT) -Wb,export_macro=UATM_Export -Wb,export_include=UATM_export.h "$(InputPath)"
<<

SOURCE="UATM.idl"

InputPath=UATM.idl

"UATMTypeSupportImpl.h" "UATMTypeSupport.idl" "UATMTypeSupportImpl.cpp" : $(SOURCE)  "$(DDS_ROOT)\bin\opendds_idl.exe" "$(DDS_ROOT)\dds\idl\IDLTemplate.txt"
	<<tempfile-Win32-Release-typesupport_files-UATM_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(DDS_ROOT)\bin\opendds_idl -Sa -St -Wb,export_macro=UATM_Export "$(InputPath)"
<<

!ELSEIF  "$(CFG)" == "Win32 Static Debug"
SOURCE="UATMTypeSupport.idl"

InputPath=UATMTypeSupport.idl

"UATMTypeSupportC.h" "UATMTypeSupportS.h" "UATMTypeSupportC.inl" "UATMTypeSupportS.cpp" "UATMTypeSupportC.cpp" : $(SOURCE)  "$(ACE_ROOT)\bin\tao_idl.exe"
	<<tempfile-Win32-Static_Debug-idl_files-UATMTypeSupport_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(ACE_ROOT)\bin\tao_idl -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I $(TAO_ROOT) --idl-version 4 --unknown-annotations ignore -as -Sa -St -I$(DDS_ROOT) -Wb,export_macro=UATM_Export -Wb,export_include=UATM_export.h "$(InputPath)"
<<

SOURCE="UATM.idl"

InputPath=UATM.idl

"UATMC.h" "UATMS.h" "UATMC.inl" "UATMS.cpp" "UATMC.cpp" : $(SOURCE)  "$(ACE_ROOT)\bin\tao_idl.exe"
	<<tempfile-Win32-Static_Debug-idl_files-UATM_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(ACE_ROOT)\bin\tao_idl -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I $(TAO_ROOT) --idl-version 4 --unknown-annotations ignore -as -Sa -St -I$(DDS_ROOT) -Wb,export_macro=UATM_Export -Wb,export_include=UATM_export.h "$(InputPath)"
<<

SOURCE="UATM.idl"

InputPath=UATM.idl

"UATMTypeSupportImpl.h" "UATMTypeSupport.idl" "UATMTypeSupportImpl.cpp" : $(SOURCE)  "$(DDS_ROOT)\bin\opendds_idl.exe" "$(DDS_ROOT)\dds\idl\IDLTemplate.txt"
	<<tempfile-Win32-Static_Debug-typesupport_files-UATM_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(DDS_ROOT)\bin\opendds_idl -Sa -St -Wb,export_macro=UATM_Export "$(InputPath)"
<<

!ELSEIF  "$(CFG)" == "Win32 Static Release"
SOURCE="UATMTypeSupport.idl"

InputPath=UATMTypeSupport.idl

"UATMTypeSupportC.h" "UATMTypeSupportS.h" "UATMTypeSupportC.inl" "UATMTypeSupportS.cpp" "UATMTypeSupportC.cpp" : $(SOURCE)  "$(ACE_ROOT)\bin\tao_idl.exe"
	<<tempfile-Win32-Static_Release-idl_files-UATMTypeSupport_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(ACE_ROOT)\bin\tao_idl -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I $(TAO_ROOT) --idl-version 4 --unknown-annotations ignore -as -Sa -St -I$(DDS_ROOT) -Wb,export_macro=UATM_Export -Wb,export_include=UATM_export.h "$(InputPath)"
<<

SOURCE="UATM.idl"

InputPath=UATM.idl

"UATMC.h" "UATMS.h" "UATMC.inl" "UATMS.cpp" "UATMC.cpp" : $(SOURCE)  "$(ACE_ROOT)\bin\tao_idl.exe"
	<<tempfile-Win32-Static_Release-idl_files-UATM_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(ACE_ROOT)\bin\tao_idl -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I $(TAO_ROOT) --idl-version 4 --unknown-annotations ignore -as -Sa -St -I$(DDS_ROOT) -Wb,export_macro=UATM_Export -Wb,export_include=UATM_export.h "$(InputPath)"
<<

SOURCE="UATM.idl"

InputPath=UATM.idl

"UATMTypeSupportImpl.h" "UATMTypeSupport.idl" "UATMTypeSupportImpl.cpp" : $(SOURCE)  "$(DDS_ROOT)\bin\opendds_idl.exe" "$(DDS_ROOT)\dds\idl\IDLTemplate.txt"
	<<tempfile-Win32-Static_Release-typesupport_files-UATM_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(DDS_ROOT)\bin\opendds_idl -Sa -St -Wb,export_macro=UATM_Export "$(InputPath)"
<<

!ENDIF


!ENDIF

GENERATED : "$(INTDIR)" "$(OUTDIR)" $(GENERATED_DIRTY)
	-@rem

DEPENDCHECK :
!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Makefile.UATM.dep")
	@echo Using "Makefile.UATM.dep"
!ELSE
	@echo Warning: cannot find "Makefile.UATM.dep"
!ENDIF
!ENDIF


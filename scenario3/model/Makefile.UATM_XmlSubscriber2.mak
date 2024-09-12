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
!MESSAGE NMAKE /f "Makefile.UATM_XmlSubscriber2.mak" CFG="Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Win32 Static Release" (based on "Win32 (x86) Console Application")
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

GENERATED_DIRTY = "UATMS.h" "UATMC.h" "UATMC.cpp" "UATMS.cpp" "UATMC.inl"

!IF  "$(CFG)" == "Win32 Debug"

OUTDIR=.
INSTALLDIR=.
INTDIR=Debug\UATM_XmlSubscriber2\x64

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(INSTALLDIR)\subscriber2.exe"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I"$(ACE_ROOT)" -I"$(TAO_ROOT)" -I"$(DDS_ROOT)" -I"C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" -I"$(DDS_ROOT)\tools\modeling\codegen" -D_DEBUG -DWIN32 -D_CONSOLE -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DOPENDDS_RAPIDJSON -f "Makefile.UATM_XmlSubscriber2.dep" "..\skyportOperatorDP\skyportOperatorSub.cpp" "UATMC.cpp" "UATMS.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(INSTALLDIR)\subscriber2.pdb"
	-@del /f/q "$(INSTALLDIR)\subscriber2.exe"
	-@del /f/q "$(INSTALLDIR)\subscriber2.ilk"
	-@del /f/q "UATMS.h"
	-@del /f/q "UATMC.h"
	-@del /f/q "UATMC.cpp"
	-@del /f/q "UATMS.cpp"
	-@del /f/q "UATMC.inl"

"$(INTDIR)" :
	if not exist "Debug\$(NULL)" mkdir "Debug"
	if not exist "Debug\UATM_XmlSubscriber2\$(NULL)" mkdir "Debug\UATM_XmlSubscriber2"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /Gm /EHsc /Zi /MDd /GR /Gy /wd4355 /wd4503 /wd4355 /wd4250 /wd4290 /Fd"$(INTDIR)/" /I "$(ACE_ROOT)" /I "$(TAO_ROOT)" /I "$(DDS_ROOT)" /I "C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" /I "$(DDS_ROOT)\tools\modeling\codegen" /D _DEBUG /D WIN32 /D _CONSOLE /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D OPENDDS_RAPIDJSON /D MPC_LIB_MODIFIER=\"d\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib TAOd.lib TAO_AnyTypeCoded.lib TAO_PortableServerd.lib TAO_Valuetyped.lib TAO_CodecFactoryd.lib TAO_PId.lib TAO_BiDirGIOPd.lib OpenDDS_Dcpsd.lib OpenDDS_Tcpd.lib OpenDDS_InfoRepoDiscoveryd.lib OpenDDS_Udpd.lib OpenDDS_Multicastd.lib OpenDDS_Rtpsd.lib OpenDDS_Rtps_Udpd.lib OpenDDS_Shmemd.lib OpenDDS_Modeld.lib UATMd.lib iphlpapi.lib /libpath:"." /libpath:"$(ACE_ROOT)\lib" /libpath:"$(DDS_ROOT)\lib" /libpath:"model" /nologo /subsystem:console /debug /pdb:"$(INSTALLDIR)\subscriber2.pdb" /machine:x64 /out:"$(INSTALLDIR)\subscriber2.exe"
LINK32_OBJS= \
	"$(INTDIR)\dotdot\skyportOperatorDP\skyportOperatorSub.obj" \
	"$(INTDIR)\UATMC.obj" \
	"$(INTDIR)\UATMS.obj"

"$(INSTALLDIR)\subscriber2.exe" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(INSTALLDIR)\subscriber2.exe.manifest" mt.exe -manifest "$(INSTALLDIR)\subscriber2.exe.manifest" -outputresource:$@;1

!ELSEIF  "$(CFG)" == "Win32 Release"

OUTDIR=Release
INSTALLDIR=Release
INTDIR=Release\UATM_XmlSubscriber2\x64

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(INSTALLDIR)\subscriber2.exe"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I"$(ACE_ROOT)" -I"$(TAO_ROOT)" -I"$(DDS_ROOT)" -I"C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" -I"$(DDS_ROOT)\tools\modeling\codegen" -DNDEBUG -DWIN32 -D_CONSOLE -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DOPENDDS_RAPIDJSON -f "Makefile.UATM_XmlSubscriber2.dep" "..\skyportOperatorDP\skyportOperatorSub.cpp" "UATMC.cpp" "UATMS.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(INSTALLDIR)\subscriber2.exe"
	-@del /f/q "$(INSTALLDIR)\subscriber2.ilk"
	-@del /f/q "UATMS.h"
	-@del /f/q "UATMC.h"
	-@del /f/q "UATMC.cpp"
	-@del /f/q "UATMS.cpp"
	-@del /f/q "UATMC.inl"

"$(INTDIR)" :
	if not exist "Release\$(NULL)" mkdir "Release"
	if not exist "Release\UATM_XmlSubscriber2\$(NULL)" mkdir "Release\UATM_XmlSubscriber2"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4503 /wd4355 /wd4250 /wd4290 /I "$(ACE_ROOT)" /I "$(TAO_ROOT)" /I "$(DDS_ROOT)" /I "C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" /I "$(DDS_ROOT)\tools\modeling\codegen" /D NDEBUG /D WIN32 /D _CONSOLE /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D OPENDDS_RAPIDJSON  /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib TAO.lib TAO_AnyTypeCode.lib TAO_PortableServer.lib TAO_Valuetype.lib TAO_CodecFactory.lib TAO_PI.lib TAO_BiDirGIOP.lib OpenDDS_Dcps.lib OpenDDS_Tcp.lib OpenDDS_InfoRepoDiscovery.lib OpenDDS_Udp.lib OpenDDS_Multicast.lib OpenDDS_Rtps.lib OpenDDS_Rtps_Udp.lib OpenDDS_Shmem.lib OpenDDS_Model.lib UATM.lib iphlpapi.lib /libpath:"." /libpath:"$(ACE_ROOT)\lib" /libpath:"$(DDS_ROOT)\lib" /libpath:"model" /nologo /subsystem:console  /machine:x64 /out:"$(INSTALLDIR)\subscriber2.exe"
LINK32_OBJS= \
	"$(INTDIR)\dotdot\skyportOperatorDP\skyportOperatorSub.obj" \
	"$(INTDIR)\UATMC.obj" \
	"$(INTDIR)\UATMS.obj"

"$(INSTALLDIR)\subscriber2.exe" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(INSTALLDIR)\subscriber2.exe.manifest" mt.exe -manifest "$(INSTALLDIR)\subscriber2.exe.manifest" -outputresource:$@;1

!ELSEIF  "$(CFG)" == "Win32 Static Debug"

OUTDIR=Static_Debug
INSTALLDIR=Static_Debug
INTDIR=Static_Debug\UATM_XmlSubscriber2\x64

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(INSTALLDIR)\subscriber2.exe"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I"$(ACE_ROOT)" -I"$(TAO_ROOT)" -I"$(DDS_ROOT)" -I"C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" -I"$(DDS_ROOT)\tools\modeling\codegen" -D_DEBUG -DWIN32 -D_CONSOLE -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DOPENDDS_RAPIDJSON -DACE_AS_STATIC_LIBS -DTAO_AS_STATIC_LIBS -f "Makefile.UATM_XmlSubscriber2.dep" "..\skyportOperatorDP\skyportOperatorSub.cpp" "UATMC.cpp" "UATMS.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(INSTALLDIR)\subscriber2.pdb"
	-@del /f/q "$(INSTALLDIR)\subscriber2.exe"
	-@del /f/q "$(INSTALLDIR)\subscriber2.ilk"
	-@del /f/q "UATMS.h"
	-@del /f/q "UATMC.h"
	-@del /f/q "UATMC.cpp"
	-@del /f/q "UATMS.cpp"
	-@del /f/q "UATMC.inl"

"$(INTDIR)" :
	if not exist "Static_Debug\$(NULL)" mkdir "Static_Debug"
	if not exist "Static_Debug\UATM_XmlSubscriber2\$(NULL)" mkdir "Static_Debug\UATM_XmlSubscriber2"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /Gm /EHsc /Zi /MDd /GR /Gy /wd4355 /wd4503 /wd4355 /wd4250 /wd4290 /Fd"$(INTDIR)/" /I "$(ACE_ROOT)" /I "$(TAO_ROOT)" /I "$(DDS_ROOT)" /I "C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" /I "$(DDS_ROOT)\tools\modeling\codegen" /D _DEBUG /D WIN32 /D _CONSOLE /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D OPENDDS_RAPIDJSON /D ACE_AS_STATIC_LIBS /D TAO_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"sd\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACEsd.lib TAOsd.lib TAO_AnyTypeCodesd.lib TAO_PortableServersd.lib TAO_Valuetypesd.lib TAO_CodecFactorysd.lib TAO_PIsd.lib TAO_BiDirGIOPsd.lib OpenDDS_Dcpssd.lib OpenDDS_Tcpsd.lib OpenDDS_InfoRepoDiscoverysd.lib OpenDDS_Udpsd.lib OpenDDS_Multicastsd.lib OpenDDS_Rtpssd.lib OpenDDS_Rtps_Udpsd.lib OpenDDS_Shmemsd.lib OpenDDS_Modelsd.lib UATMsd.lib iphlpapi.lib /libpath:"." /libpath:"$(ACE_ROOT)\lib" /libpath:"$(DDS_ROOT)\lib" /libpath:"model" /nologo /subsystem:console /debug /pdb:"$(INSTALLDIR)\subscriber2.pdb" /machine:x64 /out:"$(INSTALLDIR)\subscriber2.exe"
LINK32_OBJS= \
	"$(INTDIR)\dotdot\skyportOperatorDP\skyportOperatorSub.obj" \
	"$(INTDIR)\UATMC.obj" \
	"$(INTDIR)\UATMS.obj"

"$(INSTALLDIR)\subscriber2.exe" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(INSTALLDIR)\subscriber2.exe.manifest" mt.exe -manifest "$(INSTALLDIR)\subscriber2.exe.manifest" -outputresource:$@;1

!ELSEIF  "$(CFG)" == "Win32 Static Release"

OUTDIR=Static_Release
INSTALLDIR=Static_Release
INTDIR=Static_Release\UATM_XmlSubscriber2\x64

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(INSTALLDIR)\subscriber2.exe"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I"$(ACE_ROOT)" -I"$(TAO_ROOT)" -I"$(DDS_ROOT)" -I"C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" -I"$(DDS_ROOT)\tools\modeling\codegen" -DNDEBUG -DWIN32 -D_CONSOLE -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DOPENDDS_RAPIDJSON -DACE_AS_STATIC_LIBS -DTAO_AS_STATIC_LIBS -f "Makefile.UATM_XmlSubscriber2.dep" "..\skyportOperatorDP\skyportOperatorSub.cpp" "UATMC.cpp" "UATMS.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(INSTALLDIR)\subscriber2.exe"
	-@del /f/q "$(INSTALLDIR)\subscriber2.ilk"
	-@del /f/q "UATMS.h"
	-@del /f/q "UATMC.h"
	-@del /f/q "UATMC.cpp"
	-@del /f/q "UATMS.cpp"
	-@del /f/q "UATMC.inl"

"$(INTDIR)" :
	if not exist "Static_Release\$(NULL)" mkdir "Static_Release"
	if not exist "Static_Release\UATM_XmlSubscriber2\$(NULL)" mkdir "Static_Release\UATM_XmlSubscriber2"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4503 /wd4355 /wd4250 /wd4290 /I "$(ACE_ROOT)" /I "$(TAO_ROOT)" /I "$(DDS_ROOT)" /I "C:\Users\ongio_1lak36v\Downloads\OpenDDS-3.29.1\tools\rapidjson\include" /I "$(DDS_ROOT)\tools\modeling\codegen" /D NDEBUG /D WIN32 /D _CONSOLE /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D OPENDDS_RAPIDJSON /D ACE_AS_STATIC_LIBS /D TAO_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"s\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACEs.lib TAOs.lib TAO_AnyTypeCodes.lib TAO_PortableServers.lib TAO_Valuetypes.lib TAO_CodecFactorys.lib TAO_PIs.lib TAO_BiDirGIOPs.lib OpenDDS_Dcpss.lib OpenDDS_Tcps.lib OpenDDS_InfoRepoDiscoverys.lib OpenDDS_Udps.lib OpenDDS_Multicasts.lib OpenDDS_Rtpss.lib OpenDDS_Rtps_Udps.lib OpenDDS_Shmems.lib OpenDDS_Models.lib UATMs.lib iphlpapi.lib /libpath:"." /libpath:"$(ACE_ROOT)\lib" /libpath:"$(DDS_ROOT)\lib" /libpath:"model" /nologo /subsystem:console  /machine:x64 /out:"$(INSTALLDIR)\subscriber2.exe"
LINK32_OBJS= \
	"$(INTDIR)\dotdot\skyportOperatorDP\skyportOperatorSub.obj" \
	"$(INTDIR)\UATMC.obj" \
	"$(INTDIR)\UATMS.obj"

"$(INSTALLDIR)\subscriber2.exe" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(INSTALLDIR)\subscriber2.exe.manifest" mt.exe -manifest "$(INSTALLDIR)\subscriber2.exe.manifest" -outputresource:$@;1

!ENDIF

CLEAN :
	-@del /f/s/q "$(INTDIR)"


"$(INSTALLDIR)" ::
	if not exist "$(INSTALLDIR)\$(NULL)" mkdir "$(INSTALLDIR)"

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
!IF EXISTS("Makefile.UATM_XmlSubscriber2.dep")
!INCLUDE "Makefile.UATM_XmlSubscriber2.dep"
!ENDIF
!ENDIF

!IF "$(CFG)" == "Win32 Debug" || "$(CFG)" == "Win32 Release" || "$(CFG)" == "Win32 Static Debug" || "$(CFG)" == "Win32 Static Release" 
SOURCE="..\skyportOperatorDP\skyportOperatorSub.cpp"

"$(INTDIR)\dotdot\skyportOperatorDP\skyportOperatorSub.obj" : $(SOURCE)
	@if not exist "$(INTDIR)\dotdot\skyportOperatorDP\$(NULL)" mkdir "$(INTDIR)\dotdot\skyportOperatorDP\"
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\dotdot\skyportOperatorDP\skyportOperatorSub.obj" $(SOURCE)

SOURCE="UATMC.cpp"

"$(INTDIR)\UATMC.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UATMC.obj" $(SOURCE)

SOURCE="UATMS.cpp"

"$(INTDIR)\UATMS.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\UATMS.obj" $(SOURCE)

!IF  "$(CFG)" == "Win32 Debug"
SOURCE="UATM.idl"

InputPath=UATM.idl

"UATMS.h" "UATMC.h" "UATMC.cpp" "UATMS.cpp" "UATMC.inl" : $(SOURCE)  "$(ACE_ROOT)\bin\tao_idl.exe" "$(ACE_ROOT)\lib\TAO_IDL_BEd.dll" "$(ACE_ROOT)\lib\TAO_IDL_FEd.dll"
	<<tempfile-Win32-Debug-idl_files-UATM_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(ACE_ROOT)\bin\tao_idl -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I $(TAO_ROOT) --idl-version 4 --unknown-annotations ignore -as -Sa -St -I$(DDS_ROOT) "$(InputPath)"
<<

!ELSEIF  "$(CFG)" == "Win32 Release"
SOURCE="UATM.idl"

InputPath=UATM.idl

"UATMS.h" "UATMC.h" "UATMC.cpp" "UATMS.cpp" "UATMC.inl" : $(SOURCE)  "$(ACE_ROOT)\bin\tao_idl.exe" "$(ACE_ROOT)\lib\TAO_IDL_BE.dll" "$(ACE_ROOT)\lib\TAO_IDL_FE.dll"
	<<tempfile-Win32-Release-idl_files-UATM_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(ACE_ROOT)\bin\tao_idl -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I $(TAO_ROOT) --idl-version 4 --unknown-annotations ignore -as -Sa -St -I$(DDS_ROOT) "$(InputPath)"
<<

!ELSEIF  "$(CFG)" == "Win32 Static Debug"
SOURCE="UATM.idl"

InputPath=UATM.idl

"UATMS.h" "UATMC.h" "UATMC.cpp" "UATMS.cpp" "UATMC.inl" : $(SOURCE)  "$(ACE_ROOT)\bin\tao_idl.exe"
	<<tempfile-Win32-Static_Debug-idl_files-UATM_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(ACE_ROOT)\bin\tao_idl -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I $(TAO_ROOT) --idl-version 4 --unknown-annotations ignore -as -Sa -St -I$(DDS_ROOT) "$(InputPath)"
<<

!ELSEIF  "$(CFG)" == "Win32 Static Release"
SOURCE="UATM.idl"

InputPath=UATM.idl

"UATMS.h" "UATMC.h" "UATMC.cpp" "UATMS.cpp" "UATMC.inl" : $(SOURCE)  "$(ACE_ROOT)\bin\tao_idl.exe"
	<<tempfile-Win32-Static_Release-idl_files-UATM_idl.bat
	@echo off
	PATH=%PATH%;$(ACE_ROOT)\lib
	$(ACE_ROOT)\bin\tao_idl -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I $(TAO_ROOT) --idl-version 4 --unknown-annotations ignore -as -Sa -St -I$(DDS_ROOT) "$(InputPath)"
<<

!ENDIF


!ENDIF

GENERATED : "$(INTDIR)" "$(OUTDIR)" $(GENERATED_DIRTY)
	-@rem

DEPENDCHECK :
!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Makefile.UATM_XmlSubscriber2.dep")
	@echo Using "Makefile.UATM_XmlSubscriber2.dep"
!ELSE
	@echo Warning: cannot find "Makefile.UATM_XmlSubscriber2.dep"
!ENDIF
!ENDIF


# Created by IBM WorkFrame/2 MakeMake at 14:15:12 on 06/09/94
#
# The actions included in this makefile are:
#   BIND::Resource Bind
#   COMPILE::C Set ++ Compile
#   COMPILE::Resource Compile
#   LINK::C Set ++ Link

.all: \
  .\peterpam.dll

.SUFFIXES:

.SUFFIXES: .C .RC

.RC.res:
      @echo WF::COMPILE::Resource Compile
      rc.exe -r %s %|fF.RES

.C.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Gm /Gd /Ge- /Ms /C %s

.\peterpam.dll: \
    .\PETERPAM.obj \
    .\PPCNR.obj \
    .\PPCNR.res \
    {$(LIB)}wkfbpm21.lib \
    {$(LIB)}peterpam.def \
    peterpam.MAK
      @echo WF::LINK::C Set ++ Link
      icc.exe @<<
 /B" /st:0x10000 /bat"
 /Fepeterpam.dll 
 wkfbpm21.lib 
 peterpam.def
 .\PETERPAM.obj
 .\PPCNR.obj
<<
      @echo WF::BIND::Resource Bind
      rc.exe .\PPCNR.res peterpam.dll


!include peterpam.Dep
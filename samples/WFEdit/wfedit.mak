# Created by IBM WorkFrame/2 MakeMake at 13:25:09 on 06/08/94
#
# The actions included in this makefile are:
#   BIND::Resource Bind
#   COMPILE::C Set ++ Compile
#   COMPILE::Resource Compile
#   LINK::C Set ++ Link

.all: \
  .\wfedit.EXE

.SUFFIXES:

.SUFFIXES: .C .RC

.RC.res:
      @echo WF::COMPILE::Resource Compile
      rc.exe -r %s %|fF.RES

.C.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Sp1 /Ss /C %s

.\wfedit.EXE: \
    .\wfedit.obj \
    .\wfedit.res \
    wfedit.mak
      @echo WF::LINK::C Set ++ Link
      icc.exe @<<
 /B" /st:8096 /pmtype:pm"
 /Fewfedit.EXE 
 .\wfedit.obj
<<
      @echo WF::BIND::Resource Bind
      rc.exe .\wfedit.res wfedit.EXE


!include wfedit.Dep
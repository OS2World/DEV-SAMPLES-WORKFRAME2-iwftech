# Created by IBM WorkFrame/2 MakeMake at 11:51:27 on 07/28/94
#
# The actions included in this makefile are:
#   BIND::Resource Bind
#   COMPILE::C Set ++ Compile
#   COMPILE::Resource Compile
#   LINK::C Set ++ Link

.all: \
  .\wfpeek.exe

.SUFFIXES:

.SUFFIXES: .C .rc

.rc.res:
      @echo WF::COMPILE::Resource Compile
      rc.exe -r %s %|fF.RES

.C.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Wgen /Ti /Gm /Gd /G4 /C %s

.\wfpeek.exe: \
    .\wfpeek.obj \
    .\wfpeek.res \
    {$(LIB)}wkf21.lib \
    wfpeek.MAK
      @echo WF::LINK::C Set ++ Link
      icc.exe @<<
 /B" /de /st:8096 /pmtype:pm /nologo /bat /w"
 /Fewfpeek.exe 
   wkf21.lib 
 .\wfpeek.obj
<<
      @echo WF::BIND::Resource Bind
      rc.exe .\wfpeek.res wfpeek.exe

.\wfpeek.res: \
    wfpeek.rc \
    {$(INCLUDE)}WFPEEK.H \
    wfpeek.MAK

.\wfpeek.obj: \
    wfpeek.C \
    {.;$(INCLUDE);}wfpeek.h \
    {.;$(INCLUDE);}wkf.h \
    wfpeek.MAK


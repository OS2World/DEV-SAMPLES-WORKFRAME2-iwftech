# Created by IBM WorkFrame/2 MakeMake at 12:26:26 on 07/21/94
#
# The actions included in this makefile are:
#   BIND::Resource Bind
#   COMPILE::C Set ++ Compile
#   COMPILE::Resource Compile
#   LINK::C Set ++ Link

.all: \
  .\wkfinfo.EXE \
  .\wkfinfo.map

.SUFFIXES:

.SUFFIXES: .c .RC

.RC.res:
      @echo WF::COMPILE::Resource Compile
      rc.exe -r %s %|fF.RES

.c.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Wgen /Gm /Ms /C %s

.\wkfinfo.map \
  .\wkfinfo.EXE: \
    .\wkfinfo.obj \
    .\WKFINFO.res \
    {$(LIB)}wkf21.lib \
    {$(LIB)}wkfinfo.def \
    wkfinfo.MAK
      @echo WF::LINK::C Set ++ Link
      icc.exe @<<
 /B" /st:0x10000 /nologo /bat /w"
 /Fewkfinfo.EXE 
 /Fm"wkfinfo.map" 
 wkf21.lib 
 wkfinfo.def
 .\wkfinfo.obj
<<
      @echo WF::BIND::Resource Bind
      rc.exe .\WKFINFO.res wkfinfo.EXE

.\WKFINFO.res: \
    WKFINFO.RC \
    {$(INCLUDE)}wkfinfo.h \
    wkfinfo.MAK

.\wkfinfo.obj: \
    wkfinfo.c \
    {.;$(INCLUDE);}wkfinfo.h \
    {.;$(INCLUDE);}wkf.h \
    wkfinfo.MAK


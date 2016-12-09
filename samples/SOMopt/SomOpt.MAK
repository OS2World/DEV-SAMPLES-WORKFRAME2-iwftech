# Created by IBM WorkFrame/2 MakeMake at 13:43:40 on 06/03/94
#
# The actions included in this makefile are:
#   BIND::Resource Bind
#   COMPILE::C Set ++ Compile
#   COMPILE::Resource Compile
#   LINK::Link

.all: \
  .\SomOpt.dll

.SUFFIXES:

.SUFFIXES: .c .RC

.RC.res:
      @echo WF::COMPILE::Resource Compile
      rc.exe -r %s %|fF.RES

.c.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Tl- /Q /Tx /Fi /Si /Ti /W2 /Gm /Ge- /C %s

.\SomOpt.dll: \
    .\somparse.obj \
    .\somopt.obj \
    .\somtgt.obj \
    .\somopt.res \
    {$(LIB)}somopt.def \
    SomOpt.MAK
      @echo WF::LINK::Link
      link386.exe @<<
         /DE /NOI /NOLOGO +
        .\somparse.obj +
        .\somopt.obj +
        .\somtgt.obj
        SomOpt.dll
        
        
        somopt;
<<
      @echo WF::BIND::Resource Bind
      rc.exe .\somopt.res SomOpt.dll


!include SomOpt.Dep
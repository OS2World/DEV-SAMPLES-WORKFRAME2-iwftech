# Created by IBM WorkFrame/2 MakeMake at 18:12:47 on 07/18/94
#
# The actions included in this makefile are:
#   COMPILE::C Set ++ Compile
#   LINK::C Set ++ Link

.all: \
  .\install.EXE

.SUFFIXES:

.SUFFIXES: .c

.c.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Ti /Gm /C %s

.\install.EXE: \
    .\profile.obj \
    .\project.obj \
    .\install.obj \
    {$(LIB)}wkf21.lib \
    install.MAK
      @echo WF::LINK::C Set ++ Link
      icc.exe @<<
 /B" /de"
 /Feinstall.EXE 
   wkf21.lib 
 .\profile.obj
 .\project.obj
 .\install.obj
<<

.\profile.obj: \
    profile.c \
    {$(INCLUDE);}wkf.h \
    {$(INCLUDE);}profile.h \
    install.MAK

.\install.obj: \
    install.c \
    {$(INCLUDE);}wkf.h \
    {$(INCLUDE);}project.h \
    {$(INCLUDE);}install.h \
    {$(INCLUDE);}profile.h \
    install.MAK

.\project.obj: \
    project.c \
    {$(INCLUDE);}wkf.h \
    {$(INCLUDE);}project.h \
    install.MAK


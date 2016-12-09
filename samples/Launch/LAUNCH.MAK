# Created by IBM WorkFrame/2 MakeMake at 14:02:31 on 07/28/94
#
# The actions included in this makefile are:
#   COMPILE::C Set ++ Compile
#   LINK::C Set ++ Link

.all: \
  .\launch.exe

.SUFFIXES:

.SUFFIXES: .c

.c.obj:
      @echo WF::COMPILE::C Set ++ Compile
      icc.exe /Wgen /Ti /Gm /Gd /G4 /C %s

.\launch.exe: \
    .\launch.obj \
    .\wklaunch.obj \
    {$(LIB)}wkf21.lib \
    launch.MAK
      @echo WF::LINK::C Set ++ Link
      icc.exe @<<
 /Felaunch.exe 
   wkf21.lib 
 .\launch.obj
 .\wklaunch.obj
<<

.\launch.obj: \
    launch.c \
    launch.MAK

.\wklaunch.obj: \
    wklaunch.c \
    launch.MAK


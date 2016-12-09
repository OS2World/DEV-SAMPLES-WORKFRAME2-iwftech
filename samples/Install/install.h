/*+--------------------------------------------------------------------------+*/
/*|Install WF/2 enabled product with samples                                 |*/
/*|--------------------------------------------------------------------------|*/
/*|                                                                          |*/
/*| PROGRAM NAME: INSTALL                                                    |*/
/*| -------------                                                            |*/
/*|                                                                          |*/
/*| COPYRIGHT:                                                               |*/
/*| ----------                                                               |*/
/*| Copyright (C) International Business Machines Corp., 1991,1992,1993, 1994|*/
/*|                                                                          |*/
/*| DISCLAIMER OF WARRANTIES:                                                |*/
/*| -------------------------                                                |*/
/*| The following [enclosed] code is sample code created by IBM              |*/
/*| Corporation.  This sample code is not part of any standard IBM product   |*/
/*| and is provided to you solely for the purpose of assisting you in the    |*/
/*| development of your applications.  The code is provided "AS IS",         |*/
/*| without warranty of any kind.  IBM shall not be liable for any damages   |*/
/*| arising out of your use of the sample code, even if they have been       |*/
/*| advised of the possibility of such damages.                              |*/
/*|                                                                          |*/
/*| REVISION LEVEL: 2.1                                                      |*/
/*| -------------------                                                      |*/
/*|                                                                          |*/
/*|  This program illustrates installing a WF/2 V 2.1 enabled product.       |*/
/*|  A product folder is created in which a sample base project and an       |*/
/*|  actions profile is created.                                             |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/
/*|                                                                          |*/
/*|  ICC /Kb /Gm install.c project.c profile.c querywf.c                     |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/
#ifndef __INSTALL_H__
   #define __INSTALL_H__

   #define INCL_WF
   #define INCL_WKFINS
   #include <wkf.h>

   #include "project.h"
   #include "profile.h"

   #define ID_MAINFOLDER      "<SAMPLE_MAIN>"

   #define ID_PROFILE         "<SAMPLE_PROFILE>"
   #define ID_PROJECT         "<SAMPLE_PROJECT>"

   #define TOOL_CLASS         "SAMPLE"
   #define TOOL_NAME          "Sample Compiler"
   #define TOOL_SCOPE         WKF_SCOPE_FILE
   #define TOOL_PROGRAM       "sample.exe"
   #define TOOL_SOURCEMASK    "*.c\n*.cpp"
   #define TOOL_TARGETMASK    "*.obj\n*.exe"
   #define TOOL_SUPPORTDLL    "DDE3DEF2"
   #define TOOL_SUPPORTENTRY  "DEFAULT"
   #define TOOL_TYPE          "WKF_ACTIONTYPE_CMD"
   #define TOOL_ASCOPE        "WKF_ACTIONSCOPE_FILE"
   #define TOOL_MENUTYPE      "WKF_MENUSCOPE_SHORT"
   #define TOOL_RUNMODE       "WKF_RUNMODE_MONITOR"

   #define TOOL_OPTIONS       "-sample options"

   #define OPTIONS_SIGNATURE  "TMF"

   typedef struct _OPTIONS
           {
           ULONG cb;
           CHAR  szSignature[sizeof(OPTIONS_SIGNATURE)];
           int   fPrompt:1;
           int   fEditDDE:1;
           PSZ   pszCmdLine;
           PSZ   pszErrTemplate;
           CHAR  Reserved[8];
           CHAR  szData[1];
           } OPTIONS, *POPTIONS;

   #define TOOL2_CLASS         "EDIT"
   #define TOOL2_NAME          "Sample Editor"
   #define TOOL2_SCOPE         WKF_SCOPE_FILE
   #define TOOL2_PROGRAM       "SEDIT"
   #define TOOL2_SOURCEMASK    "*"
   #define TOOL2_TARGETMASK    ""
   #define TOOL2_SUPPORTDLL    "DDE3DEF2"
   #define TOOL2_SUPPORTENTRY  "EDIT"
   #define TOOL2_TYPE          "WKF_ACTIONTYPE_CMD"
   #define TOOL2_ASCOPE        "WKF_ACTIONSCOPE_FILE"
   #define TOOL2_MENUTYPE      "WKF_MENUSCOPE_SHORT"
   #define TOOL2_RUNMODE       "WKF_RUNMODE_DEFAULT"

   #endif

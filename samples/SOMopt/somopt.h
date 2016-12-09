
/*+--------------------------------------------------------------------------+*/
/*| SOMOPT - WorkFrame/2-enabled SOM actions support (Options DLL)           |*/
/*|--------------------------------------------------------------------------|*/
/*|                                                                          |*/
/*| PROGRAM NAME: SOMOPT                                                     |*/
/*| -------------                                                            |*/
/*|                                                                          |*/
/*| COPYRIGHT:                                                               |*/
/*| ----------                                                               |*/
/*| Copyright (C) International Business Machines Corp., 1991,1992,1993,1994 |*/
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
/*| This program demonstrates providing WorkFrame/2 actions support (i.e.    |*/
/*| Options DLL) for the SOM compiler.                                       |*/
/*+--------------------------------------------------------------------------+*/

#pragma title("IBM WorkFrame/2 Ver 2.1")

#ifndef _DEFLSOM_H_
   #define _DEFLSOM_H_


   #define SOM_OPTIONS_SIGNATURE        "SOM"

   #define BASIC_BOX_STYLE       ( MB_MOVEABLE | MB_HELP | MB_APPLMODAL )

   #define DEFAULT_CMD_LINE      " %f"

   #define SZ_OUTPUTDIR          " -d"
   #define SZ_VERIFYRELEASE      " -r"
   #define SZ_DISPLAYWARNINGS    " -w"
   #define SZ_COMMENTSTYLE       " -acstyle="

   #define COMMENT_S             's'
   #define SZ_COMMENT_S          "s"

   #define COMMENT_C             'c'
   #define SZ_COMMENT_C          "c"

   #define COMMENT_CPP           '+'
   #define SZ_COMMENT_CPP        "+"

   #define SZ_EMITFILE           " -s"
   #define SZ_EMITFILEDEF        ";def"
   #define SZ_EMITFILESC         ";sc"
   #define SZ_EMITFILEPSC        ";psc"
   #define SZ_EMITFILECS2        ";cs2"
   #define SZ_EMITFILEC          ";c"
   #define SZ_EMITFILEH          ";h"
   #define SZ_EMITFILEIH         ";ih"
   #define SZ_EMITFILEPH         ";ph"
   #define SZ_EMITFILECPP        ";xc"
   #define SZ_EMITFILEXH         ";xh"
   #define SZ_EMITFILEXIH        ";xih"
   #define SZ_EMITFILEXPH        ";xph"


   typedef struct _SOM_OPTIONS
   {
      ULONG cb;
      CHAR  szSignature[sizeof(SOM_OPTIONS_SIGNATURE)];
      int   bVerifyRelease:1;
      int   bDisplayWarnings:1;
      int   bEmitFileDEF:1;
      int   bEmitFileSC:1;
      int   bEmitFilePSC:1;
      int   bEmitFileCS2:1;
      int   bEmitFileC:1;
      int   bEmitFileH:1;
      int   bEmitFileIH:1;
      int   bEmitFilePH:1;
      int   bEmitFileCPP:1;
      int   bEmitFileXH:1;
      int   bEmitFileXIH:1;
      int   bEmitFileXPH:1;
      CHAR  cCommentStyle;
      CHAR  szOutputDir[ CCHMAXPATH ];
   } SOM_OPTIONS;


   typedef struct _SOM_DLGINFO
   {
      SOM_OPTIONS *pOptions;
      SOM_OPTIONS *pDeflOptions;
   } SOM_DLGINFO;


   WKF_QUERY_TARGET    WKFQUERYCOMPILETARGET;
   WKF_QUERY_OPTIONS   WKFQUERYCOMPILEOPTIONS;
   WKF_PARSEERROR      WKFCOMPILEPARSEERROR;


   extern void SetDefaultOptions( SOM_OPTIONS *pDeflOptions );

   extern MRESULT APIENTRY SomDlgProc( HWND hwndDlg,
                                       ULONG msg,
                                       MPARAM mp1,
                                       MPARAM mp2 );




#endif

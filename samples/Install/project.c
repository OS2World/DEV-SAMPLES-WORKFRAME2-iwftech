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
/*|  This program illustrates creating a base project and setting options    |*/
/*|  for an action defined in the base project.                              |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/ /*|                                                                          |*/
/*|                                                                          |*/
/*|   icc /Kb /Gm install.c project.c profile.c querywf.c                    |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/
#define INCL_WIN
#include <os2.h>

#include <stdlib.h>
#include <stdio.h>

#define INCL_WF
#include <wkf.h>

#include "project.h"

/* ------------------------------------------------------------------------- */
/* - macro to add a data element only if requested and update pointer      - */
/* ------------------------------------------------------------------------- */
#define ADD_STRING( pszCurrent, value, template ) \
        {                                         \
        if ( NULL!=value )                        \
           pszCurrent += sprintf( pszCurrent, template"=%s;", value ); \
        }

/* ------------------------------------------------------------------------- */
/* - create a base project - NULL should be passed for any parameters      - */
/* -                         not required                                  - */
/* ------------------------------------------------------------------------- */
HOBJECT _Optlink projectCreate( PSZ pszTitle,
                                PSZ pszLocation,
                                PSZ pszObjectID,
                                PSZ pszDir,
                                PSZ pszMask,
                                PSZ pszTargetName,
                                PSZ pszTargetPath,
                                PSZ pszTargetParm,
                                PSZ pszTargetPrompt,
                                PSZ pszTargetType,
                                PSZ pszMakefile,
                                PSZ pszMonitor,
                                PSZ pszFileMenu,
                                PSZ pszProjectMenu,
                                PSZ pszOpenAction,
                                PSZ pszExecAction,
                                PSZ pszPAM,
                                PSZ pszProfile,
                                PSZ pszPathname_Addr )
   {
   char   szSetupStr[4096];
   char * pcCurrent;

   pcCurrent = szSetupStr;

   ADD_STRING( pcCurrent, pszObjectID,      "OBJECTID" );
   ADD_STRING( pcCurrent, pszDir,           WKF_PJIOKEY_DIR           );
   ADD_STRING( pcCurrent, pszMask,          WKF_PJIOKEY_MASK          );
   ADD_STRING( pcCurrent, pszTargetName,    WKF_PJIOKEY_TARGETNAME    );
   ADD_STRING( pcCurrent, pszTargetPath,    WKF_PJIOKEY_TARGETPATH    );
   ADD_STRING( pcCurrent, pszTargetParm,    WKF_PJIOKEY_TARGETPARM    );
   ADD_STRING( pcCurrent, pszTargetPrompt,  WKF_PJIOKEY_TARGETPROMPT  );
   ADD_STRING( pcCurrent, pszTargetType,    WKF_PJIOKEY_TARGETTYPE    );
   ADD_STRING( pcCurrent, pszMakefile,      WKF_PJIOKEY_MAKEFILE      );
   ADD_STRING( pcCurrent, pszMonitor,       WKF_PJIOKEY_MONITOR       );
   ADD_STRING( pcCurrent, pszFileMenu,      WKF_PJIOKEY_FILEMENU      );
   ADD_STRING( pcCurrent, pszProjectMenu,   WKF_PJIOKEY_PROJECTMENU   );
   ADD_STRING( pcCurrent, pszOpenAction,    WKF_PJIOKEY_OPENACTION    );
   ADD_STRING( pcCurrent, pszExecAction,    WKF_PJIOKEY_EXECACTION    );
   ADD_STRING( pcCurrent, pszPAM,           WKF_PJIOKEY_PAM           );
   ADD_STRING( pcCurrent, pszProfile,       WKF_PJIOKEY_PROFILE       );
   if ( pszPathname_Addr!=NULL )
      sprintf( pcCurrent,
               WKF_PJIOKEY_PATHNAME_ADDR"=%u;",
               pszPathname_Addr );

   return( WinCreateObject ( WKF_BPROJECTCLASS,
                             pszTitle,
                             szSetupStr,
                             pszLocation,
                             CO_REPLACEIFEXISTS ) );
   }

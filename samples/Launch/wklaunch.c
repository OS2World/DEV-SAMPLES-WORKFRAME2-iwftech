/*+--------------------------------------------------------------------------+*/
/*|WF/2 Launching actions sample                                             |*/
/*|--------------------------------------------------------------------------|*/
/*|                                                                          |*/
/*| PROGRAM NAME: LAUNCH                                                     |*/
/*| -------------                                                            |*/
/*|                                                                          |*/
/*| COPYRIGHT:                                                               |*/
/*| ----------                                                               |*/
/*| Copyright (C) International Business Machines Corp., 1991,1992,1993,1994.|*/
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
/*|  This program illustrates registering a non-PM program with WF/2 and     |*/
/*|  launching context sensitive actions. Also, this program does not        |*/
/*|  statically link to WF/2, and can be used where WF/2 support is optional.|*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/
#define INCL_ERRORS
#define INCL_DOS
#define INCL_WIN
#include <os2.h>

#define INCL_WKFMSG
#include <wkf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

BOOL _System WkfLaunchAction( PSZ   pszProject,
                              PSZ   pszAction,
                              PSZ * ppszFiles )
   {
   HMODULE          hMod;
   LHANDLE          hRouter;

   PWKF_EXECUTEINFO pExecuteInfo;
   ULONG            ulSizeofBuffer;
   ULONG            i;
   PSZ              pszCurr;
   ULONG            ulNameLen;
   BOOL             bReturn = FALSE;


      /* ---------------------------------------------------------------- */
      /* register ourselves with the router                               */
      /* ---------------------------------------------------------------- */
      hRouter = WkfInitialize ( WKF_CONNECT_PIPE,
                                NULLHANDLE, /* we don't care about 2 way comm */
                                WKF_TYPE_FILETOOL,
                                "FileTool",
                                0,
                                WKF_OPT_NONE );
      if ( hRouter == NULLHANDLE )
         return( FALSE );

      ulSizeofBuffer = sizeof( WKF_EXECUTEINFO );
      i = 0;
      while( ppszFiles[i] ) i++;

      ulSizeofBuffer = sizeof( WKF_EXECUTEINFO ) + i*CCHMAXPATH;

      /* ---------------------------------------------------------------- */
      /* build up the execute editor message                              */
      /* ---------------------------------------------------------------- */
      pExecuteInfo            = calloc( 1, ulSizeofBuffer );
      if ( pExecuteInfo==NULL )
         return( FALSE );

      pExecuteInfo->cb        = ulSizeofBuffer;
      pExecuteInfo->hActionId = NULLHANDLE;
      /* pExecuteInfo->flOptions = WKF_MTR_MINIMIZE | WKF_MTR_AUTOCLOSE; */
      pExecuteInfo->flOptions = 0;
      pExecuteInfo->ulRunMode = 0;
      strcpy( pExecuteInfo->szProjectFile, pszProject );
      strcpy( pExecuteInfo->szActionName, "" );
      strcpy( pExecuteInfo->szActionClass, pszAction );
      pExecuteInfo->cFiles = i;
      i = 0;
      pszCurr = pExecuteInfo->szFiles;
      /* ---------------------------------------------------------------- */
      /* build up the files list                                          */
      /* ---------------------------------------------------------------- */
      while( ppszFiles[i] )
         {
         ulNameLen = strlen( ppszFiles[i] );
         memcpy( pszCurr, ppszFiles[i], ulNameLen );
         pszCurr[ulNameLen] = WKF_LIST_DELIM;
         pszCurr += ulNameLen + 1;
         i++;
         }

      /* ---------------------------------------------------------------- */
      /* send out the request message                                     */
      /* ---------------------------------------------------------------- */
      bReturn =
        WkfNotify (hRouter, NULLHANDLE, WKFM_EXECUTEACTION, pExecuteInfo);

      /* ---------------------------------------------------------------- */
      /* deregister with the wormhole                                     */
      /* ---------------------------------------------------------------- */
      WkfTerminate (hRouter, NULLHANDLE);

      free( pExecuteInfo );

      return( bReturn );
   }

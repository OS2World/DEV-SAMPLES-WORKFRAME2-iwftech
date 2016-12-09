/*+--------------------------------------------------------------------------+*/
/*|WF/2 Message spy sample                                                   |*/
/*|--------------------------------------------------------------------------|*/
/*|                                                                          |*/
/*| PROGRAM NAME: WFPEEK                                                     |*/
/*| -------------                                                            |*/
/*|                                                                          |*/
/*| COPYRIGHT:                                                               |*/
/*| ----------                                                               |*/
/*|  Copyright (C) International Business Machines Corp., 1991,1992,1993.    |*/
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
/*|  This program illustrates registering a PM program with WF/2 and         |*/
/*|  receiving messages from WF/2.                                           |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/
#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_WIN

#include <os2.h>

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "wfpeek.h"

#define INCL_WKFMSG
#include <wkf.h>

static char szTitle[255] = "";

MRESULT EXPENTRY DlgProc( HWND   hwndDlg,
                          ULONG  msg,
                          MPARAM mp1,
                          MPARAM mp2 );

static void _Optlink OutList( HWND hwndDlg, PSZ pszTemplate, ... );

static void _Optlink AddTitle( HWND hwndDlg, PSZ pszTitle );

typedef struct _WFROUTER
  {
    LHANDLE           hRouter;
  } WFROUTER;


int main(int argc, char * argv[])
   {
   HAB               hAB;
   HMQ               hMQ;
   HWND              hwndDlg;
   ULONG             ulLength;
   HMODULE           hMod;
   WFROUTER          wf;
   char              szModError[CCHMAXPATH];


   hAB = WinInitialize( 0 );
   if ( hAB == NULLHANDLE )
      {
      puts( "Can't initialize anchor block" );
      DosExit( EXIT_PROCESS, 255 );
      }

   hMQ = WinCreateMsgQueue( hAB, 0 );

   ulLength = WinLoadString( hAB,
                             NULLHANDLE,
                             IDS_WFPEEK_TITLE,
                             sizeof( szTitle ),
                             szTitle );
   szTitle[ulLength] = '\0';

   hwndDlg = WinLoadDlg( HWND_DESKTOP,
                         HWND_DESKTOP,
                         DlgProc,
                         0,
                         IDD_WFPEEK_MAIN,
                         (PVOID)&wf );

   if ( hwndDlg != NULLHANDLE )
      WinProcessDlg( hwndDlg );

   WinDestroyMsgQueue( hMQ );
   WinTerminate( hAB );
   return( 0 );
   }

/*----------------------------------------------------------------------------*/
/*- main dialog procedure                                                    -*/
/*----------------------------------------------------------------------------*/
MRESULT EXPENTRY DlgProc( HWND   hwndDlg,
                          ULONG  msg,
                          MPARAM mp1,
                          MPARAM mp2 )
   {
   WFROUTER * pwf;
   BOOL fSuccess;

   switch( msg )
      {
      case WKFM_CLIENTRENAMED:
      case WKFM_CLIENTMOVED:
           {
           PWKF_RENAMEINFO pMsgData;
           PSZ             pszNewName;
           PSZ             pszOldName;

           pwf = (WFROUTER *)WinQueryWindowULong( hwndDlg, QWL_USER );

           /* -------------------------------------------------------------- */
           /* make sure we can access the message data before mucking about  */
           /* -------------------------------------------------------------- */
           if ( WkfAccessMsgData (mp2) )
              {
              pMsgData   = (PWKF_RENAMEINFO)mp2;
              pszOldName = pMsgData->szParm + strlen( pMsgData->szParm) + 1;
              pszNewName = pszOldName + strlen( pszOldName ) + 1;

              if ( msg==WKFM_CLIENTRENAMED )
                 OutList( hwndDlg,
                          "Client (%s) renamed to (%s)",
                          pszOldName,
                          pszNewName );
              else
                 OutList( hwndDlg,
                          "Client (%s) moved to (%s)",
                          pszOldName,
                          pszNewName );
              }

           return( (MRESULT)TRUE );
           }

      case WKFM_CLIENTDELETED:
           {
           PWKF_DELETEINFO pMsgData;

           pwf = (WFROUTER *)WinQueryWindowULong( hwndDlg, QWL_USER );

           /* -------------------------------------------------------------- */
           /* make sure we can access the message data before mucking about  */
           /* -------------------------------------------------------------- */
           if ( WkfAccessMsgData( mp2 ) )
              {
              pMsgData = (PWKF_RENAMEINFO)mp2;

              OutList( hwndDlg, "Client (%s) deleted", pMsgData->szParm );
              }

           return( (MRESULT)TRUE );
           }

      case WKFM_CLIENTCHANGED:
           {
           PWKF_CHANGEINFO pMsgData;

           pwf = (WFROUTER *)WinQueryWindowULong( hwndDlg, QWL_USER );

           /* -------------------------------------------------------------- */
           /* make sure we can access the message data before mucking about  */
           /* -------------------------------------------------------------- */
           if ( WkfAccessMsgData( mp2 ) )
              {
              pMsgData = (PWKF_CHANGEINFO)mp2;

              OutList( hwndDlg,
                       "Client (%s) changed:0x%08x",
                       pMsgData->szParm,
                       pMsgData->flChanged );
              }

           return( (MRESULT)TRUE );
           }

      case WKFM_INITIALIZE:
           {
           PWKF_INITIALIZEINFO pMsgData;

           pwf = (WFROUTER *)WinQueryWindowULong( hwndDlg, QWL_USER );

           /* -------------------------------------------------------------- */
           /* make sure we can access the message data before mucking about  */
           /* -------------------------------------------------------------- */
           if ( WkfAccessMsgData( mp2 ) )
              {
              pMsgData = (PWKF_INITIALIZEINFO)mp2;

              OutList( hwndDlg,
                       "Client (%s) active",
                       pMsgData->szClientTitle );
              }

           return( (MRESULT)TRUE );
           }

      case WKFM_APPSTARTNOTIFY:
      case WKFM_APPTERMNOTIFY:
      case WKFM_ACTIONSTART:
      case WKFM_ACTIONEND:
      case WKFM_FILECHANGED:
      case WKFM_FILERENAMED:
      case WKFM_FILEDELETED:
      case WKFM_EXECUTEACTION:
      case WKFM_EXECUTECOMMAND:
      case WKFM_TERMINATE:
           /* -------------------------------------------------------------- */
           /* currently nothing is done with these messages                  */
           /* -------------------------------------------------------------- */
           break;

      case WM_COMMAND:
           switch( LOUSHORT( mp1 ))
              {
              case DID_CANCEL:
                   return( MRFROMLONG( FALSE ));
              }
           break;

      case WM_SYSCOMMAND:


           switch( LOUSHORT( mp1 ))
              {
              case SC_CLOSE:
                   pwf = (WFROUTER *)WinQueryWindowULong( hwndDlg, QWL_USER );
                   /* ------------------------------------------------------ */
                   /* de-register with WF/2                                  */
                   /* ------------------------------------------------------ */
                   if ( pwf->hRouter != NULLHANDLE )
                      fSuccess = WkfTerminate( pwf->hRouter, hwndDlg );
                   WinDismissDlg( hwndDlg, 0 );
                   return( MRFROMLONG( TRUE ));
              }
           break;

      case WM_INITDLG:
           AddTitle( hwndDlg, szTitle );

           WinSetWindowULong( hwndDlg, QWL_USER, LONGFROMMP(mp2) );
           pwf = (WFROUTER *)PVOIDFROMMP(mp2);

           /* -------------------------------------------------------------- */
           /* register with WF/2                                             */
           /* -------------------------------------------------------------- */
           pwf->hRouter = WkfInitialize( WKF_CONNECT_PM,
                                         hwndDlg,
                                         WKF_TYPE_UNKNOWN,
                                         szTitle,
                                         WKF_TYPE_PROJECT   |
                                         WKF_TYPE_FOLDER    |
                                         WKF_TYPE_PROFILE   |
                                         WKF_TYPE_MAKEMAKE  |
                                         WKF_TYPE_LIBRARY   |
                                         WKF_TYPE_MONITOR   |
                                         WKF_TYPE_FILETOOL  |
                                         WKF_TYPE_PROJTOOL,
                                         WKF_OPT_NONE );
           return( MRFROMLONG( FALSE ));
      }

   return( WinDefDlgProc( hwndDlg, msg, mp1, mp2 ) );
   }


/*----------------------------------------------------------------------------*/
/*- write a line out to the top of the listbox                               -*/
/*----------------------------------------------------------------------------*/
static void _Optlink OutList( HWND hwndDlg, PSZ pszTemplate, ... )
   {
   char     szBuffer[2048];
   DATETIME dtStamp;
   va_list  va;

   DosGetDateTime( &dtStamp );
   sprintf( szBuffer,
            "%02d/%02d/%04d %02d:%02d> ",
            dtStamp.month, dtStamp.day, dtStamp.year,
            dtStamp.hours, dtStamp.minutes );


   va_start( va, pszTemplate );
   vsprintf( szBuffer+strlen(szBuffer), pszTemplate, va );
   va_end( va );


   WinInsertLboxItem( WinWindowFromID(hwndDlg, IDC_WFPEEK_LISTBOX),
                      0,
                      szBuffer );

   return;
   }


/*----------------------------------------------------------------------------*/
/*- add this program to the "Windows's List"                                 -*/
/*----------------------------------------------------------------------------*/
static void _Optlink AddTitle( HWND hwndDlg, PSZ pszTitle )
   {
   SWCNTRL scEntry;
   PID     idProcess;
   TID     idThread;


   WinQueryWindowProcess( hwndDlg, &idProcess, &idThread );

   scEntry.hwnd          = hwndDlg,
   scEntry.hwndIcon      = (HWND) 0;
   scEntry.hprog         = (HPROGRAM) 0;
   scEntry.idProcess     = idProcess;
   scEntry.idSession     = 0;
   scEntry.uchVisibility = SWL_VISIBLE;
   scEntry.fbJump        = SWL_JUMPABLE;

   strncpy( scEntry.szSwtitle, pszTitle, sizeof(scEntry.szSwtitle) - 1 );

   WinAddSwitchEntry( &scEntry );

   return;
   }

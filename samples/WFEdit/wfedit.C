/*+--------------------------------------------------------------------------+*/
/*|WF/2 Editor DDE sample                                                    |*/
/*|--------------------------------------------------------------------------|*/
/*|                                                                          |*/
/*| PROGRAM NAME: WFEDIT                                                     |*/
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
/*|  This program illustrates the editor side of the DDE and D&D protocols   |*/
/*|  used by WorkFrame/2 version 1.x and 2.1.                                |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/
#define INCL_DOSFILEMGR
#define INCL_WINDDE
#define INCL_WINFRAMEMGR
#define INCL_WINHELP
#define INCL_WINLISTBOXES
#define INCL_WINMENUS
#define INCL_WINPOINTERS
#define INCL_WINSTDDRAG
#define INCL_WINSYS
#define INCL_ERRORS
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wkfedit.h"   /* DDE signatures       */
#include "wfedit.h"    /* resource definitions */


/* ----------------------------------------------------------------- */
/* Function prototypes                                               */
/* ----------------------------------------------------------------- */
FNWP       fnwpClientWndProc;

MRESULT    mrCreate       ( HWND hwnd );
MRESULT    mrDragOver     ( HWND hwnd, PDRAGINFO pDraginfo );
MRESULT    mrDrop         ( HWND hwnd, PDRAGINFO pDraginfo );
MRESULT    mrDropHelp     ( HWND hwnd, PDRAGINFO pDraginfo );
MRESULT    mrPaint        ( HWND hwnd, CHAR *szErrorMsg );
MRESULT    mrSize         ( HWND hwnd, MPARAM mp2 );

VOID       ddeExecute     ( HWND hwnd, HWND hwndMsg, PDDESTRUCT pDDE );
VOID       ddeInitiate    ( HWND hwnd, HWND hwndMsg, PDDEINIT pDDEi );
PDDESTRUCT pddeMakeRequest( USHORT usFormat,
                            PSZ    pszItemName,
                            PVOID  pvData,
                            USHORT usDataSize );
BOOL       ddeSendGoto    ( HWND hwnd );
VOID       ddeTerminate   ( HWND hwnd, HWND hwndMsg );


/* ----------------------------------------------------------------- */
/* Globals                                                           */
/* ----------------------------------------------------------------- */
CHAR szClientClass[] = "WorkFrame/2 Sample Editor";

HAB  hAB;                              /* anchor block handle        */
HMQ  hMQ;                              /* message queue handle       */
HWND hwndFrame;                        /* frame window handle        */
HWND hwndClient;                       /* client window handle       */
HWND hwndMenu;                         /* menu window handle         */
HWND hwndHelp;                         /* help instance handle       */

BOOL fConnected;                       /* DDE connection status      */
HWND hwndDDE;                          /* DDE client window handle   */
CHAR szTopicName[CCHMAXPATH];          /* DDE topic name buffer      */

CHAR szSourceFile[CCHMAXPATH];         /* qualified source name      */
CHAR szLibraryFile[CCHMAXPATH];        /* library name buffer        */

CHAR szErrorLine[300];                 /* error text buffer          */

PULONG pulErrorLines;                  /* array of error lines       */
PULONG pulErrorOffsets;                /* array of error offsets     */
PULONG pulCookies;                     /* array of error cookies     */

ULONG ulErrorCount=0L;                 /* error count                */
ULONG ulErrorIndex=0L;                 /* error index                */
ULONG ulErrorLimit=0L;                 /* error limit                */

ULONG ulCurrentError;                  /* current error line         */
ULONG ulCurrentOffset;                 /* current error offset       */
ULONG ulCurrentCookie;                 /* current magic cookie       */
ULONG ulCurrentResID;                  /* current resource ID        */


/* ----------------------------------------------------------------- */
/* Trace definitions                                                 */
/* ----------------------------------------------------------------- */

HWND hwndTrace;                        /* listbox window handle      */
CHAR szTrace[CCHMAXPATH+128];          /* Trace data buffer          */

#define TRACE(hwnd,psz) \
{SHORT s = (SHORT)WinSendMsg(hwnd,LM_INSERTITEM,MPFROMSHORT(LIT_END), \
   MPFROMP(psz)); WinSendMsg(hwnd,LM_SETTOPINDEX,MPFROMSHORT(s),NULL);}

#define FRAMEFLAGS \
        FCF_TITLEBAR | FCF_SYSMENU | FCF_MENU                  \
        | FCF_SIZEBORDER | FCF_MINMAX | FCF_TASKLIST


/* ----------------------------------------------------------------- */
/* main()                                                            */
/* ----------------------------------------------------------------- */

int main( int argc, char** argv )
   {
   ULONG flFrameFlags =  FRAMEFLAGS;
   QMSG  qMsg;


   /* Fetch filename if specified */
   if( argc != 2 )
      *szSourceFile = (char) NULL;
   else
      strcpy( szSourceFile, argv[1] );

   strcpy( szTopicName, szSourceFile);

   /* --------------------------------------------------------------- */
   /* Initialize with Presentation Manager                            */
   /* --------------------------------------------------------------- */
   hAB = WinInitialize(0);
   hMQ = WinCreateMsgQueue(hAB, 0);
   WinRegisterClass( hAB, szClientClass, fnwpClientWndProc, 0L, 0 );

   hwndFrame = WinCreateStdWindow( HWND_DESKTOP,
                                   WS_VISIBLE,
                                   &flFrameFlags,
                                   szClientClass,
                                   NULL,
                                   0L,
                                   (HMODULE)NULL,
                                   ID_SERVER,
                                   &hwndClient );

   hwndMenu = WinWindowFromID( hwndFrame, FID_MENU );
   hwndHelp = NULLHANDLE;

   WinSetWindowPos( hwndFrame,
                    HWND_TOP, 200, 200,
                    LONGFROMMR( WinQuerySysValue( HWND_DESKTOP, SV_CXSCREEN ) )
                          / 2,
                    LONGFROMMR( WinQuerySysValue( HWND_DESKTOP, SV_CYSCREEN ) )
                          / 2,
                    SWP_SIZE | SWP_MOVE);

   WinSendMsg( hwndFrame,
               WM_SETICON,
               MPFROMLONG(WinQuerySysPointer( HWND_DESKTOP,
                                              SPTR_APPICON,
                                              FALSE)),
               NULL );

   while( WinGetMsg( hAB, &qMsg, NULLHANDLE, 0, 0 ))
      WinDispatchMsg( hAB, &qMsg );

   WinDestroyWindow( hwndFrame );
   WinDestroyMsgQueue( hMQ );
   WinTerminate( hAB );
   return( 0 );
   }


/* ----------------------------------------------------------------- */
/* fnwpClientWndProc()                                               */
/* ----------------------------------------------------------------- */

MRESULT fnwpClientWndProc ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
   {
   switch( msg )
      {
      case WM_CREATE:
           return( mrCreate( hwnd ));

      case WM_SIZE:
           return( mrSize( hwnd, mp2 ));

      case WM_PAINT:
           return( mrPaint( hwnd, szErrorLine ));

      case WM_ERASEBACKGROUND:
           return( (MRESULT)TRUE  );

      case WM_INITMENU:
           if ( SHORT1FROMMP( mp1 ) == IDM_ERROR )
              {
              WinEnableMenuItem( hwndMenu,
                                 IDM_NEXT,
                                 ( ulErrorIndex < ulErrorLimit ));
              WinEnableMenuItem( hwndMenu,
                                 IDM_PREVIOUS,
                                 ( ulErrorIndex > 0L ));
              }
           break;

      case WM_COMMAND:
           switch( LOUSHORT( mp1 ))
              {
              case IDM_NEXT:
                   ++ulErrorIndex;
                   ddeSendGoto( hwnd );
                   return( NULL );

              case IDM_PREVIOUS:
                   --ulErrorIndex;
                   ddeSendGoto( hwnd );
                   return( NULL );

              case IDM_DESCRIBE:
                   if ( WinSendMsg( hwndHelp,
                                    HM_DISPLAY_HELP,
                                    MPFROMLONG( MAKELONG( ulCurrentResID, NULL )),
                                    MPFROMSHORT( HM_RESOURCEID )))
                      WinAlarm( HWND_DESKTOP, WA_ERROR );
                   return( NULL );
              }
           break;

      case WM_CLOSE:
           if ( fConnected )
               WinDdePostMsg( hwndDDE, hwnd, WM_DDE_TERMINATE, NULL, TRUE );
           WinPostMsg( hwnd, WM_QUIT, 0L, 0L );
           return( NULL );

      case DM_DRAGOVER:
           return( mrDragOver( hwnd, (PDRAGINFO) PVOIDFROMMP( mp1 )));

      case DM_DRAGLEAVE:
           return( NULL );

      case DM_DROPHELP:
           return( mrDropHelp( hwnd, (PDRAGINFO) PVOIDFROMMP( mp1 )));

      case DM_DROP:
           return( mrDrop( hwnd, (PDRAGINFO) PVOIDFROMMP( mp1 )));

      case WM_DDE_INITIATE:
           ddeInitiate( hwnd, HWNDFROMMP( mp1 ), PVOIDFROMMP( mp2 ));
           break;

      case WM_DDE_EXECUTE:
           ddeExecute( hwnd, HWNDFROMMP( mp1 ), PVOIDFROMMP( mp2 ));
           return( NULL );

      case WM_DDE_TERMINATE:
           ddeTerminate( hwnd, HWNDFROMMP( mp1 ));
           break;

      case WM_USER:
           if ( *szSourceFile
                && WinDdeInitiate( hwnd, CV_DDEAPPNAME, szTopicName, NULL ))
              WinSetWindowText( hwndFrame, szSourceFile );
           else
              {
              WinSetWindowText( hwndFrame, szClientClass );
              WinEnableMenuItem( hwndMenu, IDM_ERROR, FALSE );
              }
           return( NULL );
      }
   return( WinDefWindowProc( hwnd, msg, mp1, mp2 ));
   }


/* ----------------------------------------------------------------- */
/* mrCreate()                                                        */
/* ----------------------------------------------------------------- */

MRESULT mrCreate( HWND hwnd )
   {
   RECTL rclClient;
   LONG  cxClient, cyClient;


   WinQueryWindowRect( hwnd, &rclClient );
   cyClient  = (rclClient.yTop - rclClient.yBottom) / 2;
   cxClient  = (rclClient.xRight - rclClient.xLeft) - 1;
   hwndTrace = WinCreateWindow( hwnd,
                                WC_LISTBOX,
                                NULL,
                                WS_VISIBLE | LS_HORZSCROLL,
                                0,
                                0,
                                cxClient,
                                cyClient,
                                hwnd,
                                HWND_TOP,
                                ID_TRACE,
                                NULL,
                                NULL );

   *szErrorLine = '\0';
   fConnected   = FALSE;
   WinPostMsg( hwnd, WM_USER, NULL, NULL );
   return( NULL );
   }


/* ----------------------------------------------------------------- */
/* mrDragOver()                                                      */
/* ----------------------------------------------------------------- */

MRESULT mrDragOver( HWND hwnd, PDRAGINFO pDraginfo )
   {
   PDRAGITEM pDragitem;
   USHORT    usCount;
   USHORT    usIndex;
   USHORT    usIndicator;

   DrgAccessDraginfo( pDraginfo );
   usCount     = DrgQueryDragitemCount( pDraginfo );
   usIndicator = DOR_DROP;

   for( usIndex = 0; usIndex < usCount; ++usIndex )
      {
      pDragitem = DrgQueryDragitemPtr( pDraginfo, usIndex );
      if ( !DrgVerifyRMF( pDragitem, "DRM_DDE", "DDE3ERRORS" ))
         {
         usIndicator = DOR_NODROP;
         break;
         }
      }

   DrgFreeDraginfo( pDraginfo );

   return( MRFROM2SHORT( usIndicator, DO_COPY ));
   }


/* ----------------------------------------------------------------- */
/* mrDrop()                                                          */
/* ----------------------------------------------------------------- */

MRESULT mrDrop( HWND hwnd, PDRAGINFO pDraginfo )
   {
   USHORT usCount;
   USHORT usItem;
   PDRAGITEM pDragitem;


   DrgAccessDraginfo( pDraginfo );
   usCount = DrgQueryDragitemCount( pDraginfo );

   for( usItem = 0; usItem < usCount; usItem++ )
      {
      pDragitem = DrgQueryDragitemPtr( pDraginfo, usItem );
      DrgQueryStrName( pDragitem->hstrSourceName,
                       sizeof( szTopicName ),
                       szTopicName );
      DrgQueryStrName( pDragitem->hstrContainerName,
                       sizeof( szSourceFile ),
                       szSourceFile );

      sprintf( szTrace, "DROP: %s", szTopicName );
      TRACE( hwndTrace, szTrace );

      sprintf( szTrace, "FROM: %s", szSourceFile );
      TRACE( hwndTrace, szTrace );

      if( strchr( szTopicName, '\\' ) || strchr( szTopicName, ':' ))
         strcpy( szSourceFile, szTopicName );
      else
         strcat( szSourceFile, szTopicName );

      DrgSendTransferMsg( pDragitem->hwndItem,
                          DM_ENDCONVERSATION,
                          MPFROMLONG( pDragitem->ulItemID ),
                          MPFROMSHORT( DMFL_TARGETSUCCESSFUL ));
      }

   if ( fConnected )
      {
      WinDdePostMsg( hwndDDE, hwnd, WM_DDE_TERMINATE, NULL, TRUE );
      if ( hwndHelp )
         {
         WinDestroyHelpInstance( hwndHelp );
         hwndHelp = NULLHANDLE;
         }

      hwndDDE = NULLHANDLE;
      fConnected = FALSE;
      }

   WinPostMsg( hwnd, WM_USER, MPFROMHWND(pDraginfo->hwndSource), NULL );

   DrgDeleteDraginfoStrHandles( pDraginfo );
   DrgFreeDraginfo( pDraginfo );
   return( NULL );
   }


/* ----------------------------------------------------------------- */
/* mrDropHelp()                                                      */
/* ----------------------------------------------------------------- */

MRESULT mrDropHelp( HWND hwnd, PDRAGINFO pDraginfo )
   {
   DrgAccessDraginfo( pDraginfo );
   WinAlarm( HWND_DESKTOP, WA_NOTE );

   if ( hwnd != pDraginfo->hwndSource )
      DrgDeleteDraginfoStrHandles( pDraginfo );
   DrgFreeDraginfo( pDraginfo );
   return( NULL );
   }


/* ----------------------------------------------------------------- */
/* mrPaint()                                                         */
/* ----------------------------------------------------------------- */

MRESULT mrPaint( HWND hwnd, CHAR *szErrorMsg )
   {
   HPS    hPS;
   RECTL  rclPaint;
   LONG   lDivisor;
   CHAR   szPaintStr[91];
   CHAR   szWorkStr[16];
   USHORT i;
   ULONG  j;
   PULONG pulWorklines;
   PULONG pulWorkoff;
   PULONG pulWorkcookie;


   hPS = WinBeginPaint( hwnd, NULLHANDLE, NULL );
   GpiErase( hPS );
   WinQueryWindowRect( hwnd, &rclPaint );
   rclPaint.yBottom = rclPaint.yTop / 2 + 1;

   if( !*szSourceFile )
      WinDrawText( hPS,
                   -1,
                   "No file selected",
                   &rclPaint,
         CLR_NEUTRAL, CLR_BACKGROUND, DT_CENTER | DT_VCENTER | DT_ERASERECT );

   else if ( ulErrorCount == 0 )
      WinDrawText( hPS,
                   -1,
                   "No errors",
                   &rclPaint,
         CLR_NEUTRAL, CLR_BACKGROUND, DT_CENTER | DT_VCENTER | DT_ERASERECT );

   else
      {
      lDivisor = ( rclPaint.yTop - rclPaint.yBottom )
                 / ( 6 + ( ulErrorCount / 5 ));
      rclPaint.yBottom = rclPaint.yTop - lDivisor;
      WinDrawText( hPS,
                   -1,
                   "Error Lines:(line,offset,magic)",
                   &rclPaint,
         CLR_NEUTRAL, CLR_BACKGROUND, DT_CENTER | DT_VCENTER | DT_ERASERECT );

      j = 0;
      pulWorklines  = pulErrorLines;
      pulWorkoff    = pulErrorOffsets;
      pulWorkcookie = pulCookies;

      while( j  < ulErrorCount )
         {
         rclPaint.yTop = rclPaint.yBottom;
         rclPaint.yBottom = rclPaint.yTop - lDivisor;
         *szPaintStr = '\0';
         i = 5;
         while(( i > 0 ) && ( j < ulErrorCount ))
            {
            sprintf( szWorkStr,
                     "(%4ld,%3ld,%4ld)",
                     *pulWorklines, *pulWorkoff, *pulWorkcookie );
            ++pulWorklines;
            ++pulWorkoff  ;
            ++pulWorkcookie ;
            strcat( szPaintStr, szWorkStr );
            strcat( szPaintStr, "  " );
            --i;
            ++j;
            }

         WinDrawText( hPS,
                      -1,
                      szPaintStr,
                      &rclPaint,
            CLR_NEUTRAL, CLR_BACKGROUND, DT_CENTER | DT_VCENTER | DT_ERASERECT );
         }

      if ( ulCurrentError > 0 )
         {
         sprintf( szPaintStr,
                  "Current Error - %ld,%ld,%ld",
                  ulCurrentError,

         ulCurrentOffset, ulCurrentCookie );
         rclPaint.yTop = rclPaint.yBottom;
         rclPaint.yBottom = rclPaint.yTop - lDivisor;
         WinDrawText( hPS,
                      -1,
                      szPaintStr,
                      &rclPaint,
            CLR_NEUTRAL, CLR_BACKGROUND, DT_CENTER | DT_VCENTER | DT_ERASERECT );

         rclPaint.yTop    = rclPaint.yBottom;
         rclPaint.yBottom = rclPaint.yTop - lDivisor;
         WinDrawText( hPS,
                      -1,
                      szErrorMsg,
                      &rclPaint,
            CLR_NEUTRAL, CLR_BACKGROUND, DT_CENTER | DT_VCENTER | DT_ERASERECT );

         sprintf( szPaintStr, "Library - %s (%ld)", szLibraryFile, ulCurrentResID );
         rclPaint.yTop = rclPaint.yBottom;
         rclPaint.yBottom = rclPaint.yTop - lDivisor;
         WinDrawText( hPS, -1, szPaintStr, &rclPaint,
            CLR_NEUTRAL, CLR_BACKGROUND, DT_CENTER | DT_VCENTER | DT_ERASERECT );
      }
   }
   WinEndPaint( hPS );

   WinInvalidateRect( hwndTrace, NULL, FALSE );
   return( NULL );
}



/* ----------------------------------------------------------------- */
/* mrSize()                                                          */
/* ----------------------------------------------------------------- */

MRESULT mrSize( HWND hwnd, MPARAM mp2 )
   {
   LONG  cxClient, cyClient;


   if ( hwndTrace )
      {
      cyClient = SHORT2FROMMP( mp2 ) / 2;
      cxClient = SHORT1FROMMP( mp2 ) - 1;
      WinSetWindowPos( hwndTrace,
                       HWND_TOP,
                       0,
                       0,
                       cxClient,
                        cyClient,
                       SWP_MOVE | SWP_SIZE | SWP_SHOW );
      }

   WinInvalidateRect( hwnd, NULL, FALSE );
   return( NULL );
   }


/* ----------------------------------------------------------------- */
/* ddeExecute()                                                      */
/* ----------------------------------------------------------------- */

VOID ddeExecute( HWND hwnd, HWND hwndMsg, PDDESTRUCT pDDE )
   {
   PULONG pulWorkLines;
   PULONG pulLineWork;
   PULONG pulOffWork;
   PULONG pulCookieWork;
   ULONG ulIndex;
   ULONG ulNewCookie;
   PVOID pvWork;
   HELPINIT hlpInit;


   if ( pDDE && ( hwndMsg == hwndDDE ))
      {
      /* Convert pDDE offset into a pointer to the topic field */
      /* using PM macro DDES_PSZITEMNAME */
      sprintf( szTrace, "DDE_EXECUTE: %s", DDES_PSZITEMNAME( pDDE ));
      TRACE( hwndTrace, szTrace );

      /* Check for "Initialize" message topic */
      if ( !strcmp( DDES_PSZITEMNAME( pDDE ), CV_DDEINITIAL ))
         {
         /* Convert offset into a pointer to the message array using */
         /* PM macro DDES_PABDATA */
         pulWorkLines = (PULONG) DDES_PABDATA( pDDE );
         ulErrorCount = *pulWorkLines;
         ulErrorIndex = 0L;
         ulErrorLimit = ulErrorCount - 1L;

         pulErrorLines = malloc( (size_t) ulErrorCount * 4 );
         pulErrorOffsets = malloc( (size_t) ulErrorCount * 4 );
         pulCookies = malloc( (size_t) ulErrorCount * 4 );

         pulLineWork = pulErrorLines;
         pulOffWork = pulErrorOffsets;
         pulCookieWork = pulCookies;
         ++pulWorkLines;

         /* Store error line information in own variables */
         for( ulIndex=0; ulIndex < ulErrorCount; ulIndex++ )
            {
            *pulLineWork++ = *pulWorkLines;
            *pulOffWork++ = *(pulWorkLines+1);
                        /* ignore length in pulWorkLines+2) */
            *pulCookieWork++ = *(pulWorkLines+3);
            pulWorkLines += 4;
            }
         /* Store help file name */
         strcpy( szLibraryFile, (PSZ)(pulWorkLines+1));

         sprintf( szTrace, "LIBRARY: [%s]", szLibraryFile );
         TRACE( hwndTrace, szTrace );

         /* Associate help instance */
         memset( &hlpInit, 0, sizeof( hlpInit ));
         hlpInit.pszHelpLibraryName = szLibraryFile;
         if( hwndHelp = WinCreateHelpInstance( hAB, &hlpInit ))
            WinAssociateHelpInstance( hwndHelp, hwndFrame );

         /* Enable Error menu items */ 
         WinEnableMenuItem( hwndMenu, IDM_ERROR, ( ulErrorCount > 0L ));
         WinEnableMenuItem( hwndMenu, IDM_NEXT, ( ulErrorLimit > 0L ));
         WinEnableMenuItem( hwndMenu, IDM_PREVIOUS, FALSE );
         WinInvalidateRect( hwnd, NULL, FALSE );
         }
      /* Check for "Goto" message topic */
      else if( !strcmp( DDES_PSZITEMNAME( pDDE ), CV_DDEGOTO ))
         {
         pvWork = (PVOID) DDES_PABDATA( pDDE );
         ulNewCookie = *((PULONG) pvWork+3 );

         for( ulIndex = 0; ulIndex < ulErrorCount; ulIndex++ )
            if ( ulNewCookie == pulCookies[ulIndex] )
               {
               if ( ulCurrentCookie != ulNewCookie )
                  {
                  sprintf( szTrace, "Adjust error index (%ld)", ulIndex );
                  TRACE( hwndTrace, szTrace );
                  ulErrorIndex = ulIndex;

                  ulCurrentError = *(PULONG) pvWork;
                  ulCurrentOffset = *((PULONG) pvWork+1 );
                  ulCurrentResID = *((PULONG) pvWork+2 );
                  ulCurrentCookie = ulNewCookie;
                  pvWork = (PSZ) pvWork + 20;
                  strcpy( szErrorLine, pvWork );
                  WinInvalidateRect( hwnd, NULL, FALSE );
                  }
               break;
               }
         }
      WinDdePostMsg( hwndDDE, hwnd, WM_DDE_ACK, pDDE, TRUE );
      }
   }


/* ----------------------------------------------------------------- */
/* ddeInitiate()                                                     */
/* ----------------------------------------------------------------- */

VOID ddeInitiate( HWND hwnd, HWND hwndMsg, PDDEINIT pDDEi )
   {
   sprintf( szTrace, "DDE_INITIATE: FROM=%p TO=%p", hwndMsg, hwnd );
   TRACE( hwndTrace, szTrace );
   sprintf( szTrace, "APPNAME: %s", pDDEi->pszAppName ) ;
   TRACE( hwndTrace, szTrace );
   sprintf( szTrace, "TOPIC: %s", pDDEi->pszTopic ) ;
   TRACE( hwndTrace, szTrace );

   if ( !fConnected && ( hwndMsg != hwnd )
         && !strcmp( CV_DDEAPPNAME, pDDEi->pszAppName )
         && !strcmp( szTopicName, pDDEi->pszTopic ))
      {
      fConnected = TRUE;
      hwndDDE = hwndMsg;
      WinDdeRespond( hwndDDE, hwnd, CV_DDEAPPNAME, szTopicName, NULL );
      }
   DosFreeMem( pDDEi );   /* REQUIRED msj v4n3 may 89   */
   }


/* ----------------------------------------------------------------- */
/* pddeMakeRequest()                                                 */
/*                                                                   */
/* Entry:   Format, item name, and fsStatus are valid.               */
/* Exit:    Valid data segment for WM_DDE_DATA message is built.     */
/* Return:  pDDEstruct if successful, otherwise NULL.                */
/*                                                                   */
/* ----------------------------------------------------------------- */

PDDESTRUCT pddeMakeRequest( USHORT usFormat, PSZ pszItemName,
   PVOID pvData, USHORT usDataSize )
   {
   PDDESTRUCT pDDE;
   PVOID      pvMemory;
   USHORT     usSegSize;
   USHORT     usTotalSize;

   /* Allocate a givable memory segment. */

   usSegSize = (USHORT) strlen( pszItemName ) + 1;
   usTotalSize = sizeof( DDESTRUCT ) + usDataSize + usSegSize;

   if ( DosAllocSharedMem( &pvMemory, NULL, usTotalSize, fALLOCSHR ))
      {
      WinAlarm( HWND_DESKTOP, WA_ERROR );
      WinAlarm( HWND_DESKTOP, WA_WARNING );
      WinAlarm( HWND_DESKTOP, WA_NOTE );
      return( NULL );
      }

   /* Fill in the new DDE structure. */

   pDDE = (PDDESTRUCT) pvMemory;

   memset( (PVOID) pDDE, 0, (size_t) usSegSize );
   pDDE->usFormat = usFormat;
   pDDE->offszItemName = (USHORT) sizeof( DDESTRUCT );
   memcpy( DDES_PSZITEMNAME( pDDE ), pszItemName, usSegSize );
   pDDE->offabData = (USHORT) sizeof( DDESTRUCT ) + usSegSize;
   memcpy( DDES_PABDATA( pDDE ), pvData, usDataSize );
   pDDE->fsStatus = 0;
   pDDE->cbData = (ULONG) usTotalSize;

   return( pDDE );
   }


/* ----------------------------------------------------------------- */
/* ddeSendGoto() - Send a "SendGoto" message to the WorkFrame/2      */
/*                 program                                           */
/* ----------------------------------------------------------------- */
BOOL ddeSendGoto( HWND hwnd )
   {
   PDDESTRUCT pDDE;


   pDDE = pddeMakeRequest( DDEFMT_TEXT, CV_SENDGOTO,
      &pulCookies[ ulErrorIndex ], sizeof( PULONG ));
   if ( !pDDE )
      {
      WinAlarm( HWND_DESKTOP, WA_ERROR );
      return( FALSE );
      }

   WinDdePostMsg( hwndDDE, hwnd, WM_DDE_DATA, pDDE, TRUE );
   return( TRUE );
   }


/* ----------------------------------------------------------------- */
/* ddeTerminate()                                                    */
/* ----------------------------------------------------------------- */

VOID ddeTerminate( HWND hwnd, HWND hwndMsg )
   {
   TRACE( hwndTrace, "DDE_TERMINATE" );

   if ( hwndMsg == hwndDDE )
      {
      WinDdePostMsg( hwndDDE, hwnd, WM_DDE_TERMINATE, NULL, TRUE );
      if( hwndHelp )
         WinDestroyHelpInstance( hwndHelp );
      hwndDDE = NULLHANDLE;
      fConnected = FALSE;
      *szSourceFile = '\0';
      WinPostMsg( hwnd, WM_USER, NULL, NULL );

      }
   }

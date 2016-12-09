/****************************************************************************/
/*                                                                          */
/* PROGRAM NAME: WKFInfo                                                    */
/*                                                                          */
/* COPYRIGHT:                                                               */ 
/* ----------                                                               */ 
/* Copyright (C) International Business Machines Corp., 1991,1992,1993,1994 */
/*                                                                          */ 
/* DISCLAIMER OF WARRANTIES:                                                */ 
/* -------------------------                                                */ 
/* The following [enclosed] code is sample code created by IBM              */ 
/* Corporation.  This sample code is not part of any standard IBM product   */ 
/* and is provided to you solely for the purpose of assisting you in the    */ 
/* development of your applications.  The code is provided "AS IS",         */ 
/* without warranty of any kind.  IBM shall not be liable for any damages   */ 
/* arising out of your use of the sample code, even if they have been       */ 
/* advised of the possibility of such damages.                              */ 
/*                                                                          */ 
/* REVISION LEVEL: 2.1                                                      */ 
/* -------------------                                                      */ 
/*                                                                          */
/* This program illustrates using the project and actions profile APIs to   */
/* obtain information about a project and its actions.                      */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

#pragma strings(readonly)

#define INCL_DOSERRORS
#define INCL_DOS
#define INCL_WIN
#include <os2.h>

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wkfinfo.h"
#include "wkf.h"
                            

/*                                                                                  
** Local function declarations                                                      
*/                                                                                  
static MRESULT OpenFile( HWND hwndDlg );
static MRESULT SaveFile( HWND hwndDlg );                                            
static MRESULT WMCommand( HWND hwndDlg, USHORT usCommand, USHORT usType );          
static MRESULT WMInitDlg( HWND hwndDlg );                                           
static MRESULT WMInitMenu( HWND hwndDlg, SHORT sMenuID, HWND hwndMenu );            
static MRESULT _System DlgProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 );  
static VOID CheckSpaces( HWND hwndMLE, PSZ pszText );                               
static VOID InitMLE( HWND hwndMLE );                                                
static VOID Output( HWND hwndMLE, PSZ pszFormat, ... );                             
static VOID OutputActionScope( HWND hwndMLE, ULONG ulActionScope );                 
static VOID OutputActionType( HWND hwndMLE, ULONG ulActionType );                   
static VOID OutputList( HWND hwndMLE, PSZ pszIndent, PSZ pszList );                 
static VOID OutputMenuScope( HWND hwndMLE, ULONG ulMenuScope );                     
static VOID OutputMsgScope( HWND hwndMLE, ULONG ulMsgScope );                       
static VOID OutputProfileInfo( HWND hwndMLE, WKF_ACTIONS *pwkfActionsList );        
static VOID OutputProjectInfo( HWND hwndMLE, WKFBASEPROJECT *pbpProject );          
static VOID OutputRunMode( HWND hwndMLE, ULONG ulRunmode );                         
static VOID ProcessProject( HWND hwndMLE, PSZ pszProjectFilename );                 
static VOID WriteFile( HWND hwndDlg, PSZ pszFilename );                             



/****************************************************************************/
/* MAIN                                                                     */
/*                                                                          */
/****************************************************************************/

int main(int argc, char *argv[])
{
   HMQ  hmq;
   HAB  hab;
   HWND hwndDlg;

   /*
   ** Connect to Presentation Manager
   */
   hab = WinInitialize( 0 );
   if ( hab == NULLHANDLE )
   {
      WinAlarm( HWND_DESKTOP, WA_ERROR );
      return( ERROR_GEN_FAILURE );
   }

   hmq = WinCreateMsgQueue( hab, 0 );
   if( hmq == NULLHANDLE )
   {
      WinAlarm( HWND_DESKTOP, WA_ERROR );
      WinTerminate( hab );
      return( ERROR_GEN_FAILURE );
   }

   /*
   ** Create MAIN window.
   */

   hwndDlg = WinLoadDlg( HWND_DESKTOP,
                         HWND_DESKTOP,
                         DlgProc,
                         NULLHANDLE,     /* Load the resources from the exe */
                         IDD_DLG,
                         NULL );
   if( hwndDlg == NULLHANDLE )
   {
      WinGetLastError( hab );
      WinAlarm( HWND_DESKTOP, WA_ERROR );
      WinDestroyMsgQueue( hmq );
      WinTerminate( hab );
      return( ERROR_GEN_FAILURE );
   }

   /*
   ** Fetch and dispatch PM messages.
   */
   WinProcessDlg( hwndDlg );
   WinDestroyWindow( hwndDlg );

   WinDestroyMsgQueue( hmq );
   WinTerminate( hab );
   return( NO_ERROR );
}




/****************************************************************************/
/* DlgProc()                                                                */
/*                                                                          */
/****************************************************************************/

static MRESULT _System DlgProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
   switch (msg)
   {
      case WM_INITDLG:
         return( WMInitDlg( hwndDlg ) );

      case WM_INITMENU:
         return( WMInitMenu( hwndDlg, 
                             SHORT1FROMMP( mp1 ),
                             (HWND) mp2 ) );

      case WM_COMMAND:
         return( WMCommand( hwndDlg,
                            SHORT1FROMMP( mp1 ),
                            SHORT1FROMMP( mp2 ) ) );

   } /* endswitch */


   return( WinDefDlgProc( hwndDlg, msg, mp1, mp2 ));
}



/****************************************************************************/
/* WMInitDlg()                                                              */
/*                                                                          */
/****************************************************************************/

static MRESULT WMInitDlg( HWND hwndDlg )
{
   HWND   hwndMenu;

   /*
   ** There is no selected project yet
   */
   WinSetWindowPtr( hwndDlg, QWL_USER, PVOIDFROMMP( NULL ));

   /*
   ** Create an action bar
   */
   hwndMenu = WinLoadMenu( hwndDlg,
                           NULLHANDLE,
                           IDM_DLG_ACTIONBAR );

   WinSetParent( hwndMenu,
                 hwndDlg,
                 FALSE );

   /*
   ** Now update the frame
   */
   WinDefDlgProc( hwndDlg,
                  WM_UPDATEFRAME,
                  MPFROMLONG( FCF_MENU ),
                  MPVOID );


   return( MRFROMLONG( FALSE ));
}



/****************************************************************************/
/* WMInitMenu()                                                             */
/*                                                                          */
/****************************************************************************/

static MRESULT WMInitMenu( HWND hwndDlg, SHORT sMenuID, HWND hwndMenu )
{

   switch (sMenuID)
   {
      case IDM_FILE:
      {
         PSZ pszProjectFilename;

         /*
         ** Only enable the save menuitem if a project has been selected
         */
         pszProjectFilename = WinQueryWindowPtr( hwndDlg, QWL_USER );
      
         if( pszProjectFilename )
            WinEnableMenuItem( hwndMenu, IDM_FILE_SAVE, TRUE );
         else
            WinEnableMenuItem( hwndMenu, IDM_FILE_SAVE, FALSE );
      }
      break;
 
   } /* endswitch */

   return( MRFROMLONG( FALSE ) );
}




/****************************************************************************/
/* WMCommand()                                                              */
/*                                                                          */
/****************************************************************************/

static MRESULT WMCommand( HWND hwndDlg, USHORT usCommand, USHORT usType )
{
   switch (usType)
   {
      case CMDSRC_MENU:
      {
         switch (usCommand)
         {
            case IDM_FILE_OPEN:
               return( OpenFile( hwndDlg ) );
 
            case IDM_FILE_SAVE:
               return( SaveFile( hwndDlg ) );

         } /* endswitch */
      }
      break;
 
   } /* endswitch */

   return( MRFROMLONG( FALSE ) );
}



/****************************************************************************/
/* OpenFile() - Display Open File dialog to let user select a project       */
/*              to open                                                     */
/****************************************************************************/

static MRESULT OpenFile( HWND hwndDlg )
{
   FILEDLG FileDlg;
   PSZ     pszProjectFilename;

   pszProjectFilename = WinQueryWindowPtr( hwndDlg, QWL_USER );

   /*
   ** Display the open file dialog
   */
   memset( &FileDlg, 0, sizeof( FileDlg ) );
   FileDlg.cbSize   =  sizeof( FileDlg );
   FileDlg.fl       =  FDS_OPEN_DIALOG;
   FileDlg.pszIType = "WorkFrame/2 Project";

   if( pszProjectFilename )
      strcpy( FileDlg.szFullFile, pszProjectFilename );

   WinFileDlg( HWND_DESKTOP,
               hwndDlg,
              &FileDlg );

   if( FileDlg.lReturn == DID_OK )
   {
      /*
      ** Clean up the MLE
      */
      InitMLE( WinWindowFromID( hwndDlg, IDD_MLE ) );

      /*
      ** A project was selected -- process it
      */
      ProcessProject( WinWindowFromID( hwndDlg, IDD_MLE ), 
                      FileDlg.szFullFile );

      /*
      ** Free the old, and save the new project filename
      */
      free( pszProjectFilename );

      pszProjectFilename = strdup( FileDlg.szFullFile );
      WinSetWindowPtr( hwndDlg, QWL_USER, pszProjectFilename );
   }

   return( MRFROMLONG( FALSE ) );
}



/****************************************************************************/
/* SaveFile() - Save information in a text file                             */
/*                                                                          */
/****************************************************************************/
static MRESULT SaveFile( HWND hwndDlg )
{
   FILEDLG FileDlg;
   CHAR    szName[CCHMAXPATH];
   PSZ     pszProjectFilename;

   pszProjectFilename = WinQueryWindowPtr( hwndDlg, QWL_USER );

   DosEditName( 1,   /* Use OS/2 1.2 semantics */
                pszProjectFilename,
                "*.TXT",
                szName,
                sizeof( szName ) );

   /*
   ** Display the open file dialog
   */
   memset( &FileDlg, 0, sizeof( FileDlg ) );
   FileDlg.cbSize   =  sizeof( FileDlg );
   FileDlg.fl       =  FDS_SAVEAS_DIALOG;

   if( pszProjectFilename )
      strcpy( FileDlg.szFullFile, szName );

   WinFileDlg( HWND_DESKTOP,
               hwndDlg,
              &FileDlg );

   if( FileDlg.lReturn == DID_OK )
      WriteFile( hwndDlg, FileDlg.szFullFile );

   return( MRFROMLONG( FALSE ) );
}




/****************************************************************************/
/* WriteFile() - Get data from MLE and write to the file                    */
/*                                                                          */
/****************************************************************************/
static VOID WriteFile( HWND hwndDlg, PSZ pszFilename )
{
   LONG  lLen;
   IPT   ipt=0;
   HWND  hwndMLE;
   PSZ   pszData;
   BOOL  fOK = FALSE;

   hwndMLE = WinWindowFromID( hwndDlg, IDD_MLE );

   lLen = (LONG)WinSendMsg( hwndMLE,
                            MLM_QUERYTEXTLENGTH,
                            MPFROMP(NULL),
                            MPFROMP(NULL) );

   pszData = malloc( lLen );
   if( pszData )
   {
      WinSendMsg( hwndMLE,            
                  MLM_SETIMPORTEXPORT,
                  MPFROMP( pszData ),
                  MPFROMLONG( lLen ) );

      lLen = (LONG)WinSendMsg( hwndMLE,
                               MLM_EXPORT,      
                               MPFROMP( &ipt ),
                               MPFROMP( &lLen ) );

      /*
      ** If we actually exported the data from the MLE ok, open the file
      */
      if( lLen )
      {
         FILE *fOutput;
         
         fOutput = fopen( pszFilename, "w" );

         if( fOutput )
         {
            lLen = fwrite( pszData,
                           lLen,
                           1,           /* count */
                           fOutput );
      
            if (lLen == 1)    /* 1 block of data written */
               fOK = TRUE;
      
            fclose( fOutput );
         }
      }

      free( pszData );
   }


   if( fOK )
      WinAlarm( HWND_DESKTOP, WA_NOTE );
   else
      WinAlarm( HWND_DESKTOP, WA_ERROR );
}



/****************************************************************************/
/* ProcessProject() - Do actual work of reading project information         */
/*                                                                          */
/****************************************************************************/
static VOID ProcessProject( HWND hwndMLE, PSZ pszProjectFilename )
{
   WKFBASEPROJECT bpProject;          /* Base project structure */
   WKF_ACTIONS   *pwkfActionsList;    /* Actions list structure */
   APIRET         rc;
   LHANDLE        hProject;           /* Project handle */

   /*
   ** Start querying the project information, and dumping 
   ** it to the listbox
   */

   Output( hwndMLE, "\"%s\"", pszProjectFilename );
   CheckSpaces( hwndMLE, pszProjectFilename );

   /* Open project file for reading */

   hProject = WkfOpenProjectFile( NULLHANDLE,
                                  pszProjectFilename,
                                  WKF_ACCESS_READ);

   /* Read project data into bpProject buffer */

   if (hProject != NULLHANDLE)    /* Project opened successfully */
      rc = WkfReadProject( pszProjectFilename,
                           &bpProject );
   else              
      return;         

      
   /* Write data to MLE */

   Output( hwndMLE, "WkfReadProject() = %d\n", rc );
   if( rc )       
      return;     /* Error reading project */

   /*
   ** Disable the MLE while inputing the data
   ** (to speed it up)
   */
   WinShowWindow( hwndMLE, FALSE );


   OutputProjectInfo( hwndMLE, &bpProject );
   Output( hwndMLE, "\n" );

   /* Finished with project file, so close it */
   WkfCloseProjectFile ( hProject );


   /*
   ** Read the actions profile, and output that information
   */
   rc = WkfQueryActionList( bpProject.szProfile, 
                           &pwkfActionsList );

   Output( hwndMLE, "WkfQueryActionList() = %d\n", rc );

   if( rc == NO_ERROR )
   {
      OutputProfileInfo( hwndMLE, pwkfActionsList );

      /* Always call this after calling WkfQueryActionList or WkfQueryActions */
      WkfFreeActions( pwkfActionsList );
   }


   WinShowWindow( hwndMLE, TRUE );

}




/****************************************************************************/
/* OutputProjectInfo()                                                      */
/*                                                                          */
/****************************************************************************/
static VOID OutputProjectInfo( HWND hwndMLE, WKFBASEPROJECT *pbpProject )
{

   /*
   ** Output the project information
   */
   Output( hwndMLE, "    Access method module = \"%s\"", pbpProject->szPAM );
   CheckSpaces( hwndMLE, pbpProject->szPAM );

   Output( hwndMLE, "    Project directory list:\n" );
   OutputList( hwndMLE, "        ", pbpProject->szDirectory );

   Output( hwndMLE, "    File mask list:\n" );
   OutputList( hwndMLE, "        ", pbpProject->szMask );

   Output( hwndMLE, "    Target program file name = \"%s\"", pbpProject->szTargetName );
   CheckSpaces( hwndMLE, pbpProject->szTargetName );

   Output( hwndMLE, "    Target program parameters = \"%s\"", pbpProject->szTargetParm );
   CheckSpaces( hwndMLE, pbpProject->szTargetParm );

   Output( hwndMLE, "    Target prompt required = %s\n", (pbpProject->fTargetPrompt ? "Yes" : "No" ) );
                                                                                                                      
   Output( hwndMLE, "    Target program type = " );
   OutputRunMode( hwndMLE, pbpProject->ulTargetType );

   Output( hwndMLE, "    Monitor option flags:\n" );
   Output( hwndMLE, "        Multiple concurrent: %s\n", ( (pbpProject->flMonitorFlags & WKF_MTR_MULTIPLE)
                                                           ? "Yes" : "No" ) ); 
   Output( hwndMLE, "        Close on completion: %s\n", ( (pbpProject->flMonitorFlags & WKF_MTR_AUTOCLOSE)
                                                           ? "Yes" : "No" ) ); 
   Output( hwndMLE, "        Start minimized: %s\n", ( (pbpProject->flMonitorFlags & WKF_MTR_MINIMIZE)
                                                       ? "Yes" : "No" ) ); 

   Output( hwndMLE, "    Target path name = \"%s\"", pbpProject->szTargetPath );
   CheckSpaces( hwndMLE, pbpProject->szTargetPath );

   Output( hwndMLE, "    Make file name = \"%s\"", pbpProject->szMakeFile );
   CheckSpaces( hwndMLE, pbpProject->szMakeFile );

   Output( hwndMLE, "    Action profile file name = \"%s\"", pbpProject->szProfile );
   CheckSpaces( hwndMLE, pbpProject->szProfile );

   Output( hwndMLE, "    File-scope menu = %s\n", (pbpProject->fFileDetailMenu ? "Yes" : "No" ) );

   Output( hwndMLE, "    Project-scope menu = %s\n", (pbpProject->fProjDetailMenu ? "Yes" : "No" ) );

   Output( hwndMLE, "    Default open action class = \"%s\"", pbpProject->szOpenAction );
   CheckSpaces( hwndMLE, pbpProject->szOpenAction );

   Output( hwndMLE, "    Default exec action class = \"%s\"", pbpProject->szExecAction );
   CheckSpaces( hwndMLE, pbpProject->szExecAction );

   return;
}





/****************************************************************************/
/* OutputProfileInfo()                                                      */
/*                                                                          */
/****************************************************************************/
static VOID OutputProfileInfo( HWND hwndMLE, WKF_ACTIONS *pwkfActionsList )
{                                                                                           
   ULONG i;

   Output( hwndMLE, "%d Actions defined\n", pwkfActionsList->ulCount );

   for (i=0; i<pwkfActionsList->ulCount; i++ )
   {
      /*
      ** Output the project information
      */
      Output( hwndMLE, "    Action name = \"%s\"", pwkfActionsList->apActions[i]->pszActionName );
      CheckSpaces( hwndMLE, pwkfActionsList->apActions[i]->pszActionName );
   
      Output( hwndMLE, "    Action class = \"%s\"", pwkfActionsList->apActions[i]->pszActionClass );
      CheckSpaces( hwndMLE, pwkfActionsList->apActions[i]->pszActionClass );
   
      Output( hwndMLE, "        Command = \"%s\"", pwkfActionsList->apActions[i]->pszCommand );
      CheckSpaces( hwndMLE, pwkfActionsList->apActions[i]->pszCommand );
   
      Output( hwndMLE, "        Source masks:\n" );
      OutputList( hwndMLE, "            ", pwkfActionsList->apActions[i]->pszSrcMask );
   
      Output( hwndMLE, "        Target masks:\n" );
      OutputList( hwndMLE, "            ", pwkfActionsList->apActions[i]->pszTgtMask );
   
      Output( hwndMLE, "        DLL name = \"%s\"", pwkfActionsList->apActions[i]->pszDllName );
      CheckSpaces( hwndMLE, pwkfActionsList->apActions[i]->pszDllName );
   
      Output( hwndMLE, "        DLL entry point = \"%s\"", pwkfActionsList->apActions[i]->pszDllEntryName );
      CheckSpaces( hwndMLE, pwkfActionsList->apActions[i]->pszDllEntryName );
   
      Output( hwndMLE, "        Action type = " );
      OutputActionType( hwndMLE, pwkfActionsList->apActions[i]->ucActionType );

      Output( hwndMLE, "        Action scope = " );
      OutputActionScope( hwndMLE, pwkfActionsList->apActions[i]->ucActionScope );

      Output( hwndMLE, "        Menu scope = " );
      OutputMenuScope( hwndMLE, pwkfActionsList->apActions[i]->ucMenuScope );

      Output( hwndMLE, "        Run mode = " );
      OutputRunMode( hwndMLE, pwkfActionsList->apActions[i]->ucRunMode );

      /*
      ** Extra fields if this action is a message
      */
      if( pwkfActionsList->apActions[i]->ucActionType & WKF_ACTIONMSG_ENABLED )
      {
         Output( hwndMLE, "        Message enabled\n" );

         Output( hwndMLE, "            Execution host = \"%s\"", pwkfActionsList->apActions[i]->pszExecHost );
         CheckSpaces( hwndMLE, pwkfActionsList->apActions[i]->pszExecHost );
   
         Output( hwndMLE, "            Message action = \"%s\"", pwkfActionsList->apActions[i]->pszMsgAction );
         CheckSpaces( hwndMLE, pwkfActionsList->apActions[i]->pszMsgAction );
   
         Output( hwndMLE, "            Message data = \"%s\"", pwkfActionsList->apActions[i]->pszMsgData );
         CheckSpaces( hwndMLE, pwkfActionsList->apActions[i]->pszMsgData );
   
         Output( hwndMLE, "            Message scope = " );
         OutputMsgScope( hwndMLE, pwkfActionsList->apActions[i]->ucMsgScope );
      }

   } /* endfor */
}




/****************************************************************************/
/* OutputRunMode()                                                          */
/*                                                                          */
/****************************************************************************/
static VOID OutputRunMode( HWND hwndMLE, ULONG ulRunmode )
{
   switch( ulRunmode )
   {
      case WKF_RUNMODE_FULLSCREEN:
         Output( hwndMLE, "Fullscreen\n" );
         break;

      case WKF_RUNMODE_WINDOW:
         Output( hwndMLE, "Window\n" );
         break;

      case WKF_RUNMODE_MONITOR:
         Output( hwndMLE, "Monitor\n" );
         break;

      case WKF_RUNMODE_MESSAGE:
         Output( hwndMLE, "Message\n" );
         break;

      case WKF_RUNMODE_DEFAULT:
         Output( hwndMLE, "Default\n" );
         break;

      default:
         Output( hwndMLE, "Invalid (0x%x)\n", ulRunmode );

   } /* endswitch */

   return;
}




/****************************************************************************/
/* OutputActionType()                                                       */
/*                                                                          */
/****************************************************************************/
static VOID OutputActionType( HWND hwndMLE, ULONG ulActionType )
{
   /*
   ** Ignore the message enablement bit
   */
   switch( ulActionType & ~WKF_ACTIONMSG_ENABLED )
   {
      case WKF_ACTIONTYPE_CMD:
         Output( hwndMLE, "Command\n" );
         break;

      case WKF_ACTIONTYPE_MSG:
         Output( hwndMLE, "Message\n" );
         break;

      default:
         Output( hwndMLE, "Invalid (0x%x)", ulActionType );
   } /* endswitch */

   return;
}  




/****************************************************************************/
/* OutputActionScope()                                                      */
/*                                                                          */
/****************************************************************************/
static VOID OutputActionScope( HWND hwndMLE, ULONG ulActionScope )
{
   switch( ulActionScope )
   {
      case WKF_ACTIONSCOPE_PROJECT:
         Output( hwndMLE, "Project\n" );
         break;

      case WKF_ACTIONSCOPE_FILE:
         Output( hwndMLE, "File\n" );
         break;

      case WKF_ACTIONSCOPE_BOTH:
         Output( hwndMLE, "Both\n" );
         break;

      case WKF_ACTIONSCOPE_NONE:
         Output( hwndMLE, "None\n" );
         break;

      default:
         Output( hwndMLE, "Invalid (0x%x)", ulActionScope );
   } /* endswitch */

   return;
}  





/****************************************************************************/
/* OutputMenuScope()                                                        */
/*                                                                          */
/****************************************************************************/
static VOID OutputMenuScope( HWND hwndMLE, ULONG ulMenuScope )
{
   switch( ulMenuScope )
   {
      case WKF_MENUSCOPE_DETAIL:
         Output( hwndMLE, "Detailed\n" );
         break;

      case WKF_MENUSCOPE_SHORT:
         Output( hwndMLE, "Short\n" );
         break;

      default:
         Output( hwndMLE, "Invalid (0x%x)", ulMenuScope );
   } /* endswitch */

   return;
}  





/****************************************************************************/
/* OutputMsgScope()                                                         */
/*                                                                          */
/****************************************************************************/
static VOID OutputMsgScope( HWND hwndMLE, ULONG ulMsgScope )
{
   switch( ulMsgScope )
   {
      case WKF_MSGSCOPE_HOST:
         Output( hwndMLE, "Host\n" );
         break;

      case WKF_MSGSCOPE_DIR:
         Output( hwndMLE, "Dir\n" );
         break;

      case WKF_MSGSCOPE_NET:
         Output( hwndMLE, "Net\n" );
         break;

      case WKF_MSGSCOPE_OPERAND:
         Output( hwndMLE, "Operand\n" );
         break;

      default:
         Output( hwndMLE, "Invalid (0x%x)", ulMsgScope );
   } /* endswitch */

   return;
}  




/****************************************************************************/
/* InitMLE()                                                                */
/*                                                                          */
/****************************************************************************/
static VOID InitMLE( HWND hwndMLE )
{
   LONG lMLESize;

   lMLESize = (LONG)WinSendMsg( hwndMLE,
                                MLM_QUERYTEXTLENGTH,
                                MPFROMP(NULL),
                                MPFROMP(NULL) );

   WinSendMsg( hwndMLE,     
               MLM_DELETE,  
               MPFROMLONG(0),
               MPFROMLONG( lMLESize ) );

   WinSendMsg( hwndMLE,           
               MLM_FORMAT,        
               MPFROMLONG( MLFIE_NOTRANS ),
               MPFROMP(NULL) );

   return;
}




/****************************************************************************/
/* Output()                                                                 */
/*                                                                          */
/****************************************************************************/
static VOID Output( HWND hwndMLE, PSZ pszFormat, ... )
{
   va_list args;
   CHAR    szBuffer[ MAX_LINE_LENGTH ];
   ULONG   ulLen;
   IPT     ipt;

   va_start( args, pszFormat );
   vsprintf( szBuffer, pszFormat, args );
   va_end( args );

   ulLen = strlen( szBuffer );

   /*
   ** Setup a buffer to import text to the MLE
   */
   WinSendMsg( hwndMLE,            
               MLM_SETIMPORTEXPORT,
               MPFROMP( szBuffer ),
               MPFROMLONG( ulLen ) );

   /*
   ** Make sure we insert the text at the end of the MLE
   */
   ipt = (IPT)WinSendMsg( hwndMLE,
                          MLM_QUERYTEXTLENGTH,
                          MPFROMP(NULL),
                          MPFROMP(NULL) );

   WinSendMsg( hwndMLE,         
               MLM_IMPORT,      
               MPFROMP( &ipt ),
               MPFROMLONG( ulLen ) );

   return;
}



/****************************************************************************/
/* OutputList()                                                             */
/*                                                                          */
/****************************************************************************/
static VOID OutputList( HWND hwndMLE, PSZ pszIndent, PSZ pszList )
{
   PSZ pszNext;

   if( pszList[0] )
   {
      do
      {
         pszNext = strchr( pszList, WKF_LIST_DELIM );
         if( pszNext )
            pszNext[0] = '\0';

         Output( hwndMLE, "%s\"%s\"", pszIndent, pszList );
         CheckSpaces( hwndMLE, pszList );

         if( pszNext )
         {
            pszList = &(pszNext[1]);
            pszNext[0] = WKF_LIST_DELIM;
         }

      } while ( pszNext ); /* enddo */
   }
   else
   {
      Output( hwndMLE, "%s<none>\n", pszIndent );
   }

   return;
}



/****************************************************************************/
/* CheckSpaces()                                                            */
/*                                                                          */
/****************************************************************************/
static VOID CheckSpaces( HWND hwndMLE, PSZ pszText )
{
   ULONG ulLen;

   if( pszText[0] == ' ' )
      Output( hwndMLE, " (Leading blank)" );

   ulLen = max( 0, strlen( pszText )-1 );
   if( pszText[ ulLen ] == ' ' )
      Output( hwndMLE, " (Trailing blank)" );

   Output( hwndMLE, "\n" );
}

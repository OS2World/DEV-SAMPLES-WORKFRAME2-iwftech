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

#pragma title("IBM WorkFrame/2 Version 2.1")
#pragma strings(readonly)


#define INCL_PM
#define INCL_WIN
#define INCL_DOS
#define INCL_DOSERRORS
#include <os2.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define  INCL_WKFOPT
#include <wkf.h>

#include "somopt.h"
#include "somopt.rh"





static VOID OptionsInit( HWND     hwndDlg,
                         SOM_DLGINFO *pDlgInfo );

static VOID OptionsRefresh( HWND     hwndDlg,
                            SOM_OPTIONS *pOptions );

static VOID OptionsSave( HWND     hwndDlg,
                         SOM_OPTIONS *pOptions );



ULONG EXPENTRY WKFQUERYCOMPILEOPTIONS( HWND         hwndWkf,
                                       HMODULE      hMod,
                                       PSZ          pszProjectName,
                                       PSZ          pszTitle,
                                       WKF_ACTION  *pwkfAction, 
                                       ULONG        ulUseDefault,
                                       ULONG       *pulOptionsLen,
                                       VOID        *pvOptions,
                                       WKF_REALLOC *pWkfRealloc,
                                       BOOL         bFileScope )
{
   APIRET       rc=NO_ERROR;
   SOM_DLGINFO  DlgInfo;
   SOM_OPTIONS  DeflOptions;
   ULONG        ulRetCode = WKF_NOERROR;                


   SetDefaultOptions( &DeflOptions );


   /*
   ** Now Setup our "global" dialog structure
   */
   DlgInfo.pOptions         = (SOM_OPTIONS*)pvOptions;
   DlgInfo.pDeflOptions     = &DeflOptions;


   /*
   ** OK, the structures are all setup, now: do we need to initialize to the default?
   */
   if ( *pulOptionsLen < sizeof(SOM_OPTIONS) ||
        strncmp( DlgInfo.pOptions->szSignature,
                 SOM_OPTIONS_SIGNATURE,
                 sizeof(SOM_OPTIONS_SIGNATURE) ) ||
        ulUseDefault == 1 )
   {
      /*
      ** Do we need to increase the size of the options block?
      */
      if ( *pulOptionsLen < sizeof(SOM_OPTIONS) )
      {
         DlgInfo.pOptions = (*pWkfRealloc)( DlgInfo.pOptions, 
                                            sizeof(SOM_OPTIONS) );
   
         if (!DlgInfo.pOptions)
            return( ERROR_NOT_ENOUGH_MEMORY );
      }

      /*
      ** Copy the default options
      */
      memcpy( DlgInfo.pOptions, 
             &DeflOptions, 
              sizeof( DeflOptions ) );
   }


   /*
   ** All done, display the dialog
   */
   rc = WinDlgBox( HWND_DESKTOP,
                   hwndWkf, 
                  &SomDlgProc,
                   hMod,
                   ID_SOM_SETTINGS, 
                  &DlgInfo );

   if (rc)
   {
      CHAR Text[100];

      sprintf( Text, "Error %d", rc );

      WinMessageBox( HWND_DESKTOP,
                     hwndWkf,
                     Text, 
                     pszTitle,
                     0,                                     
                     MB_CANCEL | MB_ERROR | MB_MOVEABLE ); 

      return( WKF_ERROR );  
   }
  
   return( ulRetCode );     
}




extern void SetDefaultOptions( SOM_OPTIONS *pDeflOptions )
{
   memset( pDeflOptions, 0, sizeof( *pDeflOptions ) );

   pDeflOptions->cb               = sizeof( *pDeflOptions );
   pDeflOptions->bDisplayWarnings = TRUE;

   pDeflOptions->cCommentStyle    = COMMENT_S;

   strcpy( pDeflOptions->szOutputDir, "" );
   strcpy( pDeflOptions->szSignature, SOM_OPTIONS_SIGNATURE );
}





/****************************************************************************/
/* SomDlgProc ()                                                            */
/*                                                                          */
/****************************************************************************/

extern MRESULT APIENTRY SomDlgProc( HWND hwndDlg,
                                    ULONG msg,
                                    MPARAM mp1,
                                    MPARAM mp2 )
{
   APIRET rc;
   SOM_DLGINFO *pDlgInfo;

   switch( msg )
   {
      case WM_INITDLG:
      {
         pDlgInfo = PVOIDFROMMP( mp2 );

         WinSetWindowPtr( hwndDlg, QWL_USER, pDlgInfo );

         OptionsInit( hwndDlg, pDlgInfo );

         return( MRFROMLONG( FALSE ));       
      }

      case WM_SYSCOMMAND:
      {
         switch( LOUSHORT( mp1 ))
         {
            case SC_CLOSE:
            {
               WinPostMsg( hwndDlg, 
                           WM_COMMAND, 
                           MPFROMSHORT( ID_CANCEL ), 
                           MPFROMP( NULL ) );
            }
            break;
         }
      }
      break;

      case WM_COMMAND:
      {
         switch( LOUSHORT( mp1 ))
         {
            case ID_UNDO:
            {
               pDlgInfo = WinQueryWindowPtr( hwndDlg, QWL_USER );
               OptionsRefresh( hwndDlg, pDlgInfo->pOptions );
            }
            break;

            case ID_DEFAULT:
            {
               pDlgInfo = WinQueryWindowPtr( hwndDlg, QWL_USER );
               OptionsRefresh( hwndDlg, pDlgInfo->pDeflOptions );
            }
            break;

            case ID_OK:
            {
               pDlgInfo = WinQueryWindowPtr( hwndDlg, QWL_USER );
               OptionsSave( hwndDlg, pDlgInfo->pOptions );

               WinDismissDlg( hwndDlg, NO_ERROR );
            }
            break;

            case ID_CANCEL:
            {
               SOM_OPTIONS SomTemp;
               ULONG Answer;
               ULONG ulLen;

               pDlgInfo = WinQueryWindowPtr( hwndDlg, QWL_USER );

               /*
               ** Copy the current options to the temp options.
               ** The OptionsSave call will overwrite all the
               ** dialog information, we just need to set the 
               ** other fields (such as the signature).
               */
               memcpy( &SomTemp,
                        pDlgInfo->pOptions,
                        sizeof( SomTemp ) );

               OptionsSave( hwndDlg, &SomTemp );


               /*
               ** Now compare the options, to see if any changes have
               ** been made
               */
               if( memcmp( &SomTemp, 
                            pDlgInfo->pOptions, 
                            sizeof( SomTemp ) ) )
               {
                  Answer = WinMessageBox( HWND_DESKTOP,
                                          hwndDlg,
                                          "Changes have been made.  Discard?",
                                          "Som Options",
                                          0,
                                          MB_YESNO | MB_WARNING | MB_MOVEABLE );

                  if( Answer != MBID_YES ) 
                     break;
               }

               WinDismissDlg( hwndDlg, NO_ERROR );
            }
            break;
         }
      }
      break;

      default:
         return( WinDefDlgProc( hwndDlg, msg, mp1, mp2 ));
   }

   return( MRFROMLONG( FALSE ));
}



/****************************************************************************/
/* OptionsInit()                                                            */
/*                                                                          */
/****************************************************************************/
static VOID OptionsInit( HWND     hwndDlg,
                         SOM_DLGINFO *pDlgInfo )
{
   WinSendMsg( WinWindowFromID( hwndDlg, ID_DIRECTORY ),
               EM_SETTEXTLIMIT,
               MPFROMSHORT( CCHMAXPATH ),
               MPFROMP( NULL ) );

   /*
   ** Now perform an undo, to cause the 
   ** various controls to display the values
   */
   WinPostMsg( hwndDlg,
               WM_COMMAND,
               MPFROMSHORT( ID_UNDO ),
               MPFROMP( NULL ) );
}


/****************************************************************************/
/* OptionsRefresh()                                                         */
/*                                                                          */
/****************************************************************************/

static VOID OptionsRefresh( HWND          hwndDlg,
                            SOM_OPTIONS  *pOptions )
{
   CHAR  szNumber[10];


   /*
   ** Set the dialog settings to those passed in
   */
   WinSetWindowText( WinWindowFromID( hwndDlg, ID_DIRECTORY ),
                     pOptions->szOutputDir );

   WinCheckButton( hwndDlg, ID_RELEASE,  pOptions->bVerifyRelease  );
   WinCheckButton( hwndDlg, ID_WARNING,  pOptions->bDisplayWarnings);

   WinCheckButton( hwndDlg, ID_EMIT_DEF, pOptions->bEmitFileDEF    );
   WinCheckButton( hwndDlg, ID_EMIT_SC,  pOptions->bEmitFileSC     );
   WinCheckButton( hwndDlg, ID_EMIT_PSC, pOptions->bEmitFilePSC    );
   WinCheckButton( hwndDlg, ID_EMIT_CS2, pOptions->bEmitFileCS2    );
   WinCheckButton( hwndDlg, ID_EMIT_C,   pOptions->bEmitFileC      );
   WinCheckButton( hwndDlg, ID_EMIT_H,   pOptions->bEmitFileH      );
   WinCheckButton( hwndDlg, ID_EMIT_IH,  pOptions->bEmitFileIH     );
   WinCheckButton( hwndDlg, ID_EMIT_PH,  pOptions->bEmitFilePH     );
   WinCheckButton( hwndDlg, ID_EMIT_CPP, pOptions->bEmitFileCPP    );
   WinCheckButton( hwndDlg, ID_EMIT_XH,  pOptions->bEmitFileXH     );
   WinCheckButton( hwndDlg, ID_EMIT_XIH, pOptions->bEmitFileXIH    );
   WinCheckButton( hwndDlg, ID_EMIT_XPH, pOptions->bEmitFileXPH    );

   if( pOptions->cCommentStyle == COMMENT_S )
      WinCheckButton( hwndDlg, ID_COMMENT_S, TRUE );
   else if( pOptions->cCommentStyle == COMMENT_C )
      WinCheckButton( hwndDlg, ID_COMMENT_C, TRUE );
   else if( pOptions->cCommentStyle == COMMENT_CPP )
      WinCheckButton( hwndDlg, ID_COMMENT_CPP, TRUE );
}





/****************************************************************************/
/* OptionsSave()                                                            */
/*                                                                          */
/****************************************************************************/

static VOID OptionsSave( HWND         hwndDlg,
                         SOM_OPTIONS *pOptions )
{
   /*
   ** Som file name
   */
   WinQueryWindowText( WinWindowFromID( hwndDlg, ID_DIRECTORY ),
                       sizeof( pOptions->szOutputDir ) ,
                       pOptions->szOutputDir );


   /*
   ** Check boxes
   */
   pOptions->bVerifyRelease   = WinQueryButtonCheckstate( hwndDlg, ID_RELEASE  );
   pOptions->bDisplayWarnings = WinQueryButtonCheckstate( hwndDlg, ID_WARNING  );

   pOptions->bEmitFileDEF     = WinQueryButtonCheckstate( hwndDlg, ID_EMIT_DEF );
   pOptions->bEmitFileSC      = WinQueryButtonCheckstate( hwndDlg, ID_EMIT_SC  );
   pOptions->bEmitFilePSC     = WinQueryButtonCheckstate( hwndDlg, ID_EMIT_PSC );
   pOptions->bEmitFileCS2     = WinQueryButtonCheckstate( hwndDlg, ID_EMIT_CS2 );
   pOptions->bEmitFileC       = WinQueryButtonCheckstate( hwndDlg, ID_EMIT_C   );
   pOptions->bEmitFileH       = WinQueryButtonCheckstate( hwndDlg, ID_EMIT_H   );
   pOptions->bEmitFileIH      = WinQueryButtonCheckstate( hwndDlg, ID_EMIT_IH  );
   pOptions->bEmitFilePH      = WinQueryButtonCheckstate( hwndDlg, ID_EMIT_PH  );
   pOptions->bEmitFileCPP     = WinQueryButtonCheckstate( hwndDlg, ID_EMIT_CPP );
   pOptions->bEmitFileXH      = WinQueryButtonCheckstate( hwndDlg, ID_EMIT_XH  );
   pOptions->bEmitFileXIH     = WinQueryButtonCheckstate( hwndDlg, ID_EMIT_XIH );
   pOptions->bEmitFileXPH     = WinQueryButtonCheckstate( hwndDlg, ID_EMIT_XPH );

   if( WinQueryButtonCheckstate( hwndDlg, ID_COMMENT_S ) )
      pOptions->cCommentStyle = COMMENT_S;
   else if( WinQueryButtonCheckstate( hwndDlg, ID_COMMENT_C ) )
      pOptions->cCommentStyle = COMMENT_C;
   else if( WinQueryButtonCheckstate( hwndDlg, ID_COMMENT_CPP ) )
      pOptions->cCommentStyle = COMMENT_CPP;
}






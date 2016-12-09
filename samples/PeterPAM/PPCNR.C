/****************************************************************************/ 
/*                                                                          */ 
/* PROGRAM NAME: PeterPAM                                                   */ 
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
/* This program illustrates the chaining of PAMs.                           */ 
/*                                                                          */ 
/*                                                                          */ 
/****************************************************************************/ 
/*                                                                          */ 
/* NOTES:                                                                   */ 
/*                                                                          */ 
/****************************************************************************/ 

#pragma strings(readonly)

#define INCL_WIN
#define INCL_DOS                        /* Need DOS support           */
#define INCL_DOSERRORS                  /* Get error codes            */
#include <os2.h>

/***********************************************************************/
/*                                                                     */
/*                  Include the C library header files                 */
/*                                                                     */
/***********************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

/* #include "wkfpam.h" */
#include "peterpam.h"
#include "ppcnr.h"
#include "wkf.h"




typedef struct _USERRECORD
{
   MINIRECORDCORE MiniRecordCore;       /* Must be the first element */
   HPOINTER       hptrIcon;
   PSZ            pszExt;
   PSZ            pszIconFilename;
} USERRECORD;


#define CNR_TITLE_ICON      "Icon"
#define CNR_TITLE_EXT       "Extension"
#define CNR_TITLE_FILENAME  "Icon Filename"


/*
** Function prototypes
*/
extern MRESULT EXPENTRY DlgProc( HWND hwndDlg,
                                 ULONG msg,
                                 MPARAM mp1,
                                 MPARAM mp2 );


static BOOL InsertRecords( HWND  hwndDlg,
                           ULONG ulNumEntries, 
                           EXTFILEICON *aExtFileIcon );

static BOOL InitContainer( HWND hwndDlg );

static BOOL WriteINI( PPHAM *pHAM, BOOL fDeleteLastRecord );



extern APIRET APIENTRY Settings( PPHAM  *pHAM,
                                 PSZ     pszProjectFile,
                                 HWND    hwndParent,
                                 HWND    hwndOwner,
                                 ULONG   cbMessage,
                                 PSZ     pszMessage )
{
   HMODULE hmod;

   DosQueryModuleHandle( PETERPAM_DLLNAME,
                        &hmod );

   WinDlgBox( hwndParent, 
              hwndOwner,  
             &DlgProc,
              hmod,
              IDD_PETERPAM,
              pHAM );

   return( NO_ERROR );
}


extern MRESULT EXPENTRY DlgProc( HWND hwndDlg,
                                 ULONG msg,
                                 MPARAM mp1,
                                 MPARAM mp2 )
{
   switch (msg)
   {
      case WM_INITDLG:
      {
         PPHAM   *pHAM;

         pHAM = (PPHAM*)mp2;
         WinSetWindowPtr( hwndDlg, QWL_USER, PVOIDFROMMP( pHAM ));

         /*
         ** Insert the chained PAM DLL name into the entry field
         */
         WinSendMsg( WinWindowFromID( hwndDlg, IDD_CHAINED ),
                     EM_SETTEXTLIMIT,
                     MPFROMSHORT( CCHMAXPATH ),
                     MPFROMP( NULL ) );

         WinSetWindowText( WinWindowFromID( hwndDlg, IDD_CHAINED ), 
                           pHAM->BPAM.szDLLName );


         /*
         ** Setup the container
         */
         InitContainer( hwndDlg );

         InsertRecords( hwndDlg, pHAM->ulNumEntries, pHAM->aExtFileIcon );


         /*
         ** Start ADD initially disabled, and delete dependent on any records existing
         */
         WinEnableWindow( WinWindowFromID( hwndDlg, 
                                           IDD_ADD ),
                          FALSE );

         if( pHAM->ulNumEntries )
            /*
            ** There are entries -- Enable DELETE
            */
            WinEnableWindow( WinWindowFromID( hwndDlg, 
                                              IDD_DELETE ),
                             TRUE );
         else
            /*
            ** There are no entries -- disable DELETE
            */
            WinEnableWindow( WinWindowFromID( hwndDlg, 
                                              IDD_DELETE ),
                             FALSE );


         return( MRFROMLONG( FALSE ));
      }



      case WM_CONTROL:
      {
         switch( SHORT1FROMMP( mp1 ))
         {
            case IDD_FILENAME:
            {
               if( SHORT2FROMMP( mp1 ) == EN_CHANGE )
               {
                  /*
                  ** Enable/Disable the add button, depending if the
                  ** filename field has text in it
                  */
                  if (WinQueryWindowTextLength( WinWindowFromID( hwndDlg, 
                                                                 IDD_FILENAME ) ) )
                     /*
                     ** There is text -- enable ADD
                     */
                     WinEnableWindow( WinWindowFromID( hwndDlg, 
                                                       IDD_ADD ),
                                      TRUE );
                  else
                     /*
                     ** There is no text -- disable ADD
                     */
                     WinEnableWindow( WinWindowFromID( hwndDlg, 
                                                       IDD_ADD ),
                                      FALSE );
               }
            }
            break;
 
            case IDD_CHAINED:
            {
               if( SHORT2FROMMP( mp1 ) == EN_KILLFOCUS )
               {
                  BPAM   NewPAM;
                  PPHAM *pHAM;
                  HINI   hINI;

                  pHAM = (PPHAM*)WinQueryWindowPtr( hwndDlg, QWL_USER );

                  /*
                  ** Validate the Chained PAM information
                  ** (Don't load ourselves as a chained PAM)
                  */
                  WinQueryWindowText( (HWND)mp2,
                                      sizeof( NewPAM.szDLLName ),
                                      NewPAM.szDLLName );

                  if( !stricmp( NewPAM.szDLLName, PETERPAM_DLLNAME ) ||
                      LoadChainedPAM( &NewPAM ) )
                  {
                     /*
                     ** Couldn't load the DLL - reset to BPAM
                     */
                     WinSetWindowText( (HWND)mp2, DEFAULT_DLLNAME );
                     strcpy( NewPAM.szDLLName, DEFAULT_DLLNAME );
                     WinAlarm( HWND_DESKTOP, WA_ERROR );

                     if ( NewPAM.hmod )
                        DosFreeModule( NewPAM.hmod );
                  }
                  else
                  {
                     DosFreeModule( pHAM->BPAM.hmod );
                     memcpy( &pHAM->BPAM, &NewPAM, sizeof( BPAM ) );

                     WinAlarm( HWND_DESKTOP, WA_NOTE );
                  }

                  /*
                  ** Update the INI file with the new DLL name
                  */
                  hINI = PrfOpenProfile( WinQueryAnchorBlock( HWND_DESKTOP ),
                                         pHAM->szProjectFilename );
            
                  if( hINI )
                  {
                     PrfWriteProfileData( hINI,
                                          PRF_APPNAME,
                                          PRF_KEYNAME_CHAINED,
                                          NewPAM.szDLLName,
                                          strlen( NewPAM.szDLLName ) +1 );   /* +1 == null */
            
                     PrfCloseProfile( hINI );    /* All other data is saved in the USER ini file */
                  }
                  else
                     WinAlarm( HWND_DESKTOP, WA_ERROR );

               }
            }
            break;
 
         } /* endswitch */
      }
      break;
 



      case WM_COMMAND:
      {
         switch( SHORT1FROMMP( mp1 ))
         {
            case IDD_ADD:
            {
               PPHAM   *pHAM;
               ULONG    ulNew;
               CHAR     Buffer[ CCHMAXPATH ];

               pHAM = WinQueryWindowPtr( hwndDlg, QWL_USER );

               ulNew = pHAM->ulNumEntries++;

               pHAM->aExtFileIcon = realloc( pHAM->aExtFileIcon, pHAM->ulNumEntries * sizeof( EXTFILEICON ) );

               WinQueryWindowText( WinWindowFromID( hwndDlg, IDD_EXT ),
                                   sizeof( Buffer ),
                                   Buffer );

               pHAM->aExtFileIcon[ulNew].pszExt = strdup( Buffer );

               WinQueryWindowText( WinWindowFromID( hwndDlg, IDD_FILENAME ),
                                   sizeof( Buffer ),
                                   Buffer );

               pHAM->aExtFileIcon[ulNew].pszIconFilename = strdup( Buffer );

               /*
               ** Load the new icon
               */
               pHAM->aExtFileIcon[ulNew].hptrIcon = WinLoadFileIcon( pHAM->aExtFileIcon[ulNew].pszIconFilename, 
                                                                     FALSE );

               /*
               ** Now update the INI file
               */
               if( !WriteINI( pHAM,
                              FALSE ) )   /* Don't delete any records */
                  WinAlarm( HWND_DESKTOP, WA_ERROR );


               InsertRecords( hwndDlg, 1, &pHAM->aExtFileIcon[ulNew] );


               /*
               ** Finally enable the delete button
               */
               WinEnableWindow( WinWindowFromID( hwndDlg, 
                                                 IDD_DELETE ),
                                TRUE );

            }
            break;
 


            case IDD_DELETE:
            {
               CHAR        szMsg[ 500 ];    /* Enough for a confirmation message */
               HWND        hwndCnr;
               PPHAM      *pHAM;
               ULONG       ulDelete;
               ULONG       i;
               USERRECORD *pUserRecord;

               pHAM = WinQueryWindowPtr( hwndDlg, QWL_USER );
               hwndCnr = WinWindowFromID( hwndDlg, IDD_CONTAINER );

               /*
               ** Find which record has been selected
               */
               pUserRecord = (USERRECORD*)WinSendMsg( hwndCnr,
                                                      CM_QUERYRECORDEMPHASIS,
                                                      MPFROMP( CMA_FIRST ),
                                                      MPFROMSHORT( CRA_CURSORED ) );

               if( pUserRecord == NULL )
                  return( FALSE );
               

               sprintf( szMsg,
                        "Do you want to delete the association between *.%s and %s?",
                        pUserRecord->pszExt,
                        pUserRecord->pszIconFilename );

               if ( WinMessageBox( HWND_DESKTOP,
                                   hwndDlg,
                                   szMsg,
                                   "WorkFrame/2 - PeterPAM",
                                   0,
                                   MB_YESNO | 
                                      MB_QUERY | 
                                      MB_APPLMODAL | 
                                      MB_MOVEABLE
                                 ) != MBID_YES )
                  return( FALSE );
               

               for (i = 0; i < pHAM->ulNumEntries; i++)
               {
                  if (!stricmp( pUserRecord->pszExt, 
                                pHAM->aExtFileIcon[i].pszExt ) &&
                      !stricmp( pUserRecord->pszIconFilename,
                                pHAM->aExtFileIcon[i].pszIconFilename ) )
                     /*
                     ** Found the record to delete
                     */
                  {
                     ulDelete = i;
                     break;
                  }
               } /* endfor */



               if( i == pHAM->ulNumEntries )
               {
                  /*
                  ** Didn't find a match for some reason
                  */
                  WinAlarm( HWND_DESKTOP, WA_ERROR );
                  return( FALSE );
               }



               /*
               ** Delete the record from the container
               */
               WinSendMsg( hwndCnr,
                           CM_REMOVERECORD,
                           MPFROMP( &pUserRecord ),
                           MPFROM2SHORT( 1,           /* Delete 1 record */ 
                                         CMA_FREE | CMA_INVALIDATE ) 
                         );


               /*
               ** Free the data
               */
               free( pHAM->aExtFileIcon[ ulDelete ].pszExt );
               free( pHAM->aExtFileIcon[ ulDelete ].pszIconFilename );


               /*
               ** Move the remaining entries down in the array
               */
               for (i = ulDelete+1; i < pHAM->ulNumEntries; i++)
               {
                  pHAM->aExtFileIcon[ i-1 ] = pHAM->aExtFileIcon[ i ];
               } /* endfor */

               /*
               ** ...and finally, shrink the array itself
               */
               pHAM->ulNumEntries--;

               pHAM->aExtFileIcon = realloc( pHAM->aExtFileIcon, 
                                             pHAM->ulNumEntries * sizeof( EXTFILEICON ) );

               /*
               ** Now update the INI file
               */
               if( !WriteINI( pHAM,
                              TRUE ) )  /* Delete last record */
                  WinAlarm( HWND_DESKTOP, WA_ERROR );


               /*
               ** optionally disable the delete button
               */
               if( !pHAM->ulNumEntries )
                  WinEnableWindow( WinWindowFromID( hwndDlg, 
                                                    IDD_DELETE ),
                                   FALSE );

            }
            break;
 
         } /* endswitch */ 

         return( FALSE );
      }

   } /* endswitch */

   return( WinDefDlgProc( hwndDlg, msg, mp1, mp2 ));
}




static BOOL InsertRecords( HWND hwndDlg, ULONG ulNumEntries, EXTFILEICON *aExtFileIcon )
{
   HWND              hwndCnr;
   RECORDINSERT      RecordInsert;
   ULONG             i;
   USERRECORD       *pFirstUserRecord;
   USERRECORD       *pUserRecord;


   hwndCnr = WinWindowFromID( hwndDlg, IDD_CONTAINER );


   /*
   ** Start to fill in the data
   */
   pFirstUserRecord =
      pUserRecord = WinSendMsg( hwndCnr,
                                CM_ALLOCRECORD,
                                MPFROMLONG( sizeof( USERRECORD ) - sizeof( MINIRECORDCORE ) ),
                                MPFROMLONG( ulNumEntries ) );


   for (i=0; i < ulNumEntries; i++)
   {

      /*
      ** These are required elements
      */
      pUserRecord->MiniRecordCore.cb           = sizeof( MINIRECORDCORE );
      pUserRecord->MiniRecordCore.flRecordAttr = CRA_RECORDREADONLY;

      /*
      ** Set the detail for each record
      */
      pUserRecord->hptrIcon         = aExtFileIcon[ i ].hptrIcon;
      pUserRecord->pszExt           = aExtFileIcon[ i ].pszExt;
      pUserRecord->pszIconFilename  = aExtFileIcon[ i ].pszIconFilename;

      pUserRecord = (USERRECORD*)pUserRecord->MiniRecordCore.preccNextRecord;

   } /* endfor */


   RecordInsert.cb                = sizeof( RECORDINSERT );
   RecordInsert.pRecordParent     = NULL;
   RecordInsert.pRecordOrder      = (RECORDCORE*)CMA_END;
   RecordInsert.zOrder            = CMA_TOP;
   RecordInsert.cRecordsInsert    = ulNumEntries;
   RecordInsert.fInvalidateRecord = TRUE;

   /*
   ** Insert the records
   */
   WinSendMsg( hwndCnr,
               CM_INSERTRECORD,
               MPFROMP( pFirstUserRecord ),
               MPFROMP( &RecordInsert ) );


   return( TRUE );
}



static BOOL InitContainer( HWND hwndDlg )
{
   CNRINFO           CnrInfo;
   FIELDINFO        *pFieldInfo;
   FIELDINFO        *pFirstFieldInfo;
   FIELDINFOINSERT   FieldInfoInsert;
   HWND              hwndCnr;


   hwndCnr = WinWindowFromID( hwndDlg, IDD_CONTAINER );
   /*
   ** Setup the details view -- tell the
   ** container what data to display, and where to get it
   */

   FieldInfoInsert.cb = (sizeof(FIELDINFOINSERT));
   FieldInfoInsert.pFieldInfoOrder = (FIELDINFO*)CMA_FIRST;
   FieldInfoInsert.cFieldInfoInsert = 3;     /* 3 columns */
   FieldInfoInsert.fInvalidateFieldInfo = TRUE;

   pFirstFieldInfo =
      pFieldInfo = WinSendMsg( hwndCnr,
                               CM_ALLOCDETAILFIELDINFO,
                               MPFROMLONG( 3 ),  /* Number of columns */
                               MPFROMP( NULL ) );

   /*
   ** Now, indicate how to display each column
   */

   /*
   ** Column 1 - Extension
   */
   pFieldInfo->cb          = sizeof( FIELDINFO );
   pFieldInfo->flData      = CFA_STRING |
                                CFA_SEPARATOR |
                                CFA_FIREADONLY;
   pFieldInfo->flTitle     = CFA_FITITLEREADONLY;
   pFieldInfo->pTitleData  = CNR_TITLE_EXT;
   pFieldInfo->offStruct   = offsetof( USERRECORD, pszExt );
   
   /*
   ** Column 2 - Icon
   */
   pFieldInfo = pFieldInfo->pNextFieldInfo;

   pFieldInfo->cb          = sizeof( FIELDINFO );
   pFieldInfo->flData      = CFA_BITMAPORICON |
                                CFA_SEPARATOR;
   pFieldInfo->flTitle     = CFA_FITITLEREADONLY;
   pFieldInfo->pTitleData  = CNR_TITLE_ICON;
   pFieldInfo->offStruct   = offsetof( USERRECORD, hptrIcon );



   /*
   ** Column 3 - icon filename
   */
   pFieldInfo = pFieldInfo->pNextFieldInfo;

   pFieldInfo->cb          = sizeof( FIELDINFO );
   pFieldInfo->flData      = CFA_STRING |
                                CFA_FIREADONLY;
   pFieldInfo->flTitle     = CFA_FITITLEREADONLY;
   pFieldInfo->pTitleData  = CNR_TITLE_FILENAME;
   pFieldInfo->offStruct   = offsetof( USERRECORD, pszIconFilename );

   /*
   ** ...and send the data to the container
   */
   WinSendMsg( hwndCnr,
               CM_INSERTDETAILFIELDINFO,
               MPFROMP( pFirstFieldInfo ),
               MPFROMP( &FieldInfoInsert ) );

   /*
   ** Finally, make sure the container is setup in detail view
   */
   CnrInfo.flWindowAttr = CV_DETAIL |
                             CA_CONTAINERTITLE |
                             CA_TITLESEPARATOR |
                             CA_DETAILSVIEWTITLES;

   WinSendMsg( hwndCnr,
               CM_SETCNRINFO,
               MPFROMP( &CnrInfo ),
               MPFROMLONG( CMA_FLWINDOWATTR ) );


   return( TRUE );

}






static BOOL WriteINI( PPHAM *pHAM, BOOL fDeleteLastRecord )
{
   ULONG i;
   CHAR  Buffer[ CCHMAXPATH ];
   BOOL  fOK;

   PrfWriteProfileData( HINI_USERPROFILE,
                        PRF_APPNAME,
                        PRF_KEYNAME_NUM,
                       &pHAM->ulNumEntries,
                        sizeof( pHAM->ulNumEntries ) );


   for (i=0; i < pHAM->ulNumEntries; i++)
   {
      sprintf( Buffer, PRF_KEYNAME_EXT, i );

      fOK = PrfWriteProfileData( HINI_USERPROFILE,
                                 PRF_APPNAME,
                                 Buffer,
                                 pHAM->aExtFileIcon[i].pszExt,
                                 strlen( pHAM->aExtFileIcon[i].pszExt )+1 );   /* +1 == null */

      if( !fOK )
         break;

      sprintf( Buffer, PRF_KEYNAME_ICON, i );

      fOK = PrfWriteProfileData( HINI_USERPROFILE,
                                 PRF_APPNAME,
                                 Buffer,
                                 pHAM->aExtFileIcon[i].pszIconFilename,
                                 strlen( pHAM->aExtFileIcon[i].pszIconFilename )+1 );  /* +1 == null */

   } /* endfor */


   if (fDeleteLastRecord)
   {
      sprintf( Buffer, PRF_KEYNAME_EXT, i );

      PrfWriteProfileData( HINI_USERPROFILE,
                           PRF_APPNAME,
                           Buffer,
                           NULL,
                           0 );


      sprintf( Buffer, PRF_KEYNAME_ICON, i );

      PrfWriteProfileData( HINI_USERPROFILE,
                           PRF_APPNAME,
                           Buffer,
                           NULL,
                           0 );
   }

   return( fOK );
}

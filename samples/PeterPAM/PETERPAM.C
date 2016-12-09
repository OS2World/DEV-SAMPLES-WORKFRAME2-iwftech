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
/*                  Include the C library header files                 */
/***********************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "peterpam.h"
#include "ppcnr.h"

#include "wkf.h"

/***********************************************************************/
/*                  Include the WF integration header files            */
/***********************************************************************/
#define INCL_WKFPAM
#include "wkf.h"


/*
** Local function prototypes
*/
static PPHAM *Init( PSZ pszProjectName );
static PPHAM *Close( PPHAM *pHAM );


static APIRET APIENTRY AddFiles( PVOID        pvPrivateData, 
                                 PSZ          pszContainer, 
                                 ULONG        ulCount,      
                                 PWKFPAM_PART pwppData );   



/* -------------------------------------------------------------------- */
/* WkfQueryHAM                                                          */
/* -------------------------------------------------------------------- */
/* PAM provided function for returning a caller unique HAM value.       */
/* This ham must be unique to each caller of this function.             */
/* If hamOld is not NULL, then any objects associated with this ham     */
/* is lost.                                                             */
/*                                                                      */
/* Parameters : HAM     hamOld         (I)  previously set ham value    */
/*                                          or NULL                     */
/*              PSZ     pszProjectName (I)  new project name to be      */
/*                                          assigned or NULL            */
/*                                                                      */
/* Returns :    NULL   If pszProjectName was NULL, then all data        */
/*                     associated with hamOld has been reset.           */
/*                     If pszProjectName was not NULL, then the         */
/*                     PAM failed.                                      */
/*              !NULL  A unique ham value; if hamOld was not NULL       */
/*                     then any data associated with hamOld has been    */
/*                     reset.                                           */
/* -------------------------------------------------------------------- */
extern HAM APIENTRY WKFQUERYHAM( HAM hamOld,
                                 PSZ pszProjectName )
{
   PPHAM  *pHAM;

   pHAM = (PPHAM*) hamOld;

   if (pszProjectName)
   {
      if (pHAM)
      {
         /*
         ** This is an existing, open PAM -- but the
         ** profile name has changed.  Save the new
         ** name, and pass the request on
         */
         strcpy( pHAM->szProjectFilename, pszProjectName );

         pHAM->BPAM.ham = (*pHAM->BPAM.QueryHam)( pHAM->BPAM.ham,
                                                  pszProjectName );

         if( !pHAM->BPAM.ham )
            return( NULLHANDLE );   /* BPAM Indicated error, so should we */

      }
      else
         pHAM = Init( pszProjectName );
   }
   else
   {
      /*
      ** We've been passed a HAM, but no project name --
      ** Thus, we have to free memory, and close down
      */
      pHAM = Close( pHAM );
   }

   return( (HAM)pHAM );
}



/*-------------------------------------------------------*/
/* Init - Do actual QueryHAM work                       */
/*-------------------------------------------------------*/
static PPHAM *Init( PSZ pszProjectName )
{
   PPHAM  *pHAM;
   APIRET  rc;
   ULONG   ulLen;
   HINI    hINI;
   ULONG   i;


   /*
   ** This is a a new, startup request (A name has been
   ** passed in, but no old HAM). 
   **
   ** First step: malloc some memory to use.
   */
   pHAM = calloc( 1, sizeof( PPHAM ) );
   if( !pHAM )
      rc = ERROR_NOT_ENOUGH_MEMORY;
   else
   {
      strcpy( pHAM->szProjectFilename, pszProjectName );

      pHAM->BPAM.szDLLName[0] = '\0';

      hINI = PrfOpenProfile( WinQueryAnchorBlock( HWND_DESKTOP ),
                             pszProjectName );

      if( hINI )
      {
         ulLen = sizeof( pHAM->BPAM.szDLLName );
   
         PrfQueryProfileData( hINI,
                              PRF_APPNAME,
                              PRF_KEYNAME_CHAINED,
                              pHAM->BPAM.szDLLName,
                             &ulLen );

         PrfCloseProfile( hINI );    /* All other data is saved in the USER INI file */
      }
      else
         WinAlarm( HWND_DESKTOP, WA_ERROR );
 
   
      if( !(pHAM->BPAM.szDLLName)[0] )
         strcpy( pHAM->BPAM.szDLLName, DEFAULT_DLLNAME );

      rc = LoadChainedPAM( &pHAM->BPAM );
   }


   if( rc )
   {
      if ( pHAM->BPAM.hmod )
         DosFreeModule( pHAM->BPAM.hmod );

      free( pHAM );

      return( NULL );
   }



   /*
   ** OK... last setp, load our information from the INI file
   **
   ** PAM specific information is usually stored on a project
   ** specific basis, and is therefore saved in the project
   ** filename passed in (which is an INI file for now).  For this
   ** PAM however, we want the information to be system wide,
   ** so the data is saved in the USER INI file
   */
   ulLen = sizeof( pHAM->ulNumEntries );

   if( !PrfQueryProfileData( HINI_USERPROFILE,
                             PRF_APPNAME,
                             PRF_KEYNAME_NUM,
                            &pHAM->ulNumEntries,
                            &ulLen ) )
      pHAM->ulNumEntries = 0;


   if( pHAM->ulNumEntries )
   {
      CHAR  NameBuffer[ 20 ];   /* Temp buffer to build the keyname */
      CHAR  DataBuffer[ CCHMAXPATH ];  /* Temp buffer to store data */
      BOOL  fOK;

      pHAM->aExtFileIcon = calloc( pHAM->ulNumEntries, 
                                   sizeof( EXTFILEICON ) );

      if( !pHAM->aExtFileIcon )
      {
         DosFreeModule( pHAM->BPAM.hmod );
         free( pHAM );

         return( NULL );
      }

      for (i=0; i < pHAM->ulNumEntries; i++)
      {
         sprintf( NameBuffer, PRF_KEYNAME_EXT, i );

         ulLen = sizeof( DataBuffer );
         fOK = PrfQueryProfileData( HINI_USERPROFILE,
                                    PRF_APPNAME,
                                    NameBuffer,
                                    DataBuffer,
                                   &ulLen );

         if( fOK )
         {
            DataBuffer[ulLen] = '\0';

            pHAM->aExtFileIcon[i].pszExt = strdup( DataBuffer );

            sprintf( NameBuffer, PRF_KEYNAME_ICON, i );

            ulLen = sizeof( DataBuffer );
            fOK = PrfQueryProfileData( HINI_USERPROFILE,
                                       PRF_APPNAME,
                                       NameBuffer,
                                       DataBuffer,
                                      &ulLen );

            if( fOK )
            {
               DataBuffer[ulLen] = '\0';
               pHAM->aExtFileIcon[i].pszIconFilename = strdup( DataBuffer );
            }
         }


         if( fOK )
         {
            /*
            ** Now load the icon handle
            */
            pHAM->aExtFileIcon[i].hptrIcon = WinLoadFileIcon( DataBuffer, FALSE );
         }


         if( !fOK )
            break;
 
      } /* endfor */

      if( !fOK )
      {
         /*
         ** An error occurred - free all memory obtained
         */
         do
         {
            free( pHAM->aExtFileIcon[i].pszExt          );
            free( pHAM->aExtFileIcon[i].pszIconFilename );
         } while ( i-- > 0 ); /* enddo */

         free( pHAM->aExtFileIcon );
         DosFreeModule( pHAM->BPAM.hmod );
         free( pHAM );

         return( NULL );
      }
   }


   /*
   ** Almost done -- initialize the chained PAM
   */
   pHAM->BPAM.ham = (*pHAM->BPAM.QueryHam)( NULLHANDLE,
                                            pszProjectName );

   if( pHAM->BPAM.ham == NULLHANDLE )
   {
      /*
      ** An error occurred - free all memory obtained
      */
      do
      {
         free( pHAM->aExtFileIcon[i].pszExt          );
         free( pHAM->aExtFileIcon[i].pszIconFilename );
      } while ( i-- > 0 ); /* enddo */

      free( pHAM->aExtFileIcon );
      DosFreeModule( pHAM->BPAM.hmod );
      free( pHAM );

      return( NULL );
   }

  
   return( pHAM );
}





/*---------------------------------------------------------------*/
/* LoadChainedPAM - load the PAM to chain to (bPAM in this case) */
/*                   and find its entrypoints.                   */
/*---------------------------------------------------------------*/
extern APIRET LoadChainedPAM( BPAM *pBPAM )
{
   APIRET rc;

   do
   {

      /*
      ** Now, load the chained PAM, and find the entry points
      */
      rc = DosLoadModule( NULL,  /* Not interested in failure information */
                          0,
                          pBPAM->szDLLName,
                         &pBPAM->hmod );
      if( rc )
         break;


      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_QUERYHAM,
                      (PFN*)&pBPAM->QueryHam );
      if( rc )
         break;

      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_QUERYTITLE,   
                      (PFN*)&pBPAM->QueryTitle );
      if( rc )
         break;

      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_SETTINGS,     
                      (PFN*)&pBPAM->Settings );
      if( rc )
         pBPAM->Settings = NULL;            /* This is an optional API   */

      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_QUERYFILES,   
                      (PFN*)&pBPAM->QueryFiles );
      if( rc )
         break;

      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_RUN,          
                      (PFN*)&pBPAM->Run );
      if( rc )
         break;

      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_WRITE,        
                      (PFN*)&pBPAM->Write );
      if( rc )
         break;

      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_STOP,         
                      (PFN*)&pBPAM->Stop );
      if( rc )
         break;

      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_QPATHINFO,    
                      (PFN*)&pBPAM->QueryPathInfo );
      if( rc )
         break;

      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_QRUNNABLE,
                      (PFN*)&pBPAM->QueryRunnable );
      if( rc )
         break;

      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_CREATEPATH,   
                      (PFN*)&pBPAM->CreatePath );
      if( rc )
         break;

      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_QCOPY,        
                      (PFN*)&pBPAM->QueryCopy );
      if( rc )
         break;

      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_COPYFILE,     
                      (PFN*)&pBPAM->CopyFile );
      if( rc )
         break;

      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_DELETE,       
                      (PFN*)&pBPAM->Delete );
      if( rc )
         break;

      rc = DosQueryProcAddr( pBPAM->hmod,
                             0,                 /* Load by name, not ordinal */
                             WKF_PFS_DEFAULTNAME,  
                      (PFN*)&pBPAM->DefaultName );
      if( rc )
         break;

   } while ( (0) ); /* enddo */

   return( rc );
}




/*---------------------------------------------------------------*/
/* Close - close chained PAM (bPAM in this case)                 */
/*---------------------------------------------------------------*/
static PPHAM *Close( PPHAM *pHAM )
{
   ULONG i;


   /*
   ** Almost done -- close the chained PAM
   */
   (*pHAM->BPAM.QueryHam)( pHAM->BPAM.ham,
                           NULL );



   for (i = 0; i < pHAM->ulNumEntries; i++)
   {
      free( pHAM->aExtFileIcon[i].pszExt          );
      free( pHAM->aExtFileIcon[i].pszIconFilename );
   } /* endfor */

   free( pHAM->aExtFileIcon );
   DosFreeModule( pHAM->BPAM.hmod );
   free( pHAM );

   return( NULL );
}



/* -------------------------------------------------------------------- */
/* WkfQueryFiles                                                        */
/* -------------------------------------------------------------------- */
/* PAM provided function for getting the list of files belonging to a   */
/* project.                                                             */
/* On entry, cbMessage contains the number of bytes allocated           */
/* for the pszMessageBuffer.  On exit, pszMessage buffer may contain    */
/* a single null delimited string to be displayed to the user.          */
/* The PAM function should check the length field prior to writing any  */
/* data since a count of 0 may imply a NULL pointer.                    */
/*                                                                      */
/* This is where all the work for PeterPAM gets done.  We use our       */
/* own callback routine, and patch any icons that match our list.       */
/*                                                                      */
/*                                                                      */
/* Parameters : HAM     hamOwner       (I)  project handle              */
/*              PSZ     pszMasks       (I)  file masks                  */
/*              PSZ     pszContainers  (I)  path names to be searched   */
/*              PWKF_ADDFILES pfnAddFiles (I) call back function for    */
/*                                          inserting file objects      */
/*                                          into the project container  */
/*              PVOID   pvPrivateData  (I)  parameter to be passed back */
/*                                          to the pfnAddMonitor        */
/*                                          callback function           */
/*              ULONG   cbMessage      (I)  size of message buffer      */
/*              PSZ     pszMessage     (O)  pointer to message buffer   */
/*                                                                      */
/* Returns :    APIRET  if return code is not 0, then an access method  */
/*                      specific message may be returned through        */
/*                      the pszMessage buffer.                          */
/*                                                                      */
/* -------------------------------------------------------------------- */
typedef struct _QFILES_CALLBACK
{
   PVOID         *pvPrivateData;
   WKF_ADDFILES  *pfnAddFiles;
   PPHAM         *pHAM;

} QFILES_CALLBACK;


extern APIRET APIENTRY WKFQUERYFILES( HAM     hamOwner,
                                      PSZ     pszMasks,
                                      PSZ     pszContainers,
                                      PWKF_ADDFILES pfnAddFiles,
                                      PVOID   pvPrivateData,
                                      ULONG   cbMessage,
                                      PSZ     pszMessage )
{
   PPHAM           *pHAM;
   QFILES_CALLBACK  QFiles;

   pHAM = (PPHAM*) hamOwner;

   QFiles.pvPrivateData = pvPrivateData;
   QFiles.pfnAddFiles   = pfnAddFiles;
   QFiles.pHAM          = pHAM;


   /*
   ** Chain to BPAM
   */
   return( (*pHAM->BPAM.QueryFiles)( pHAM->BPAM.ham,
                                     pszMasks,
                                     pszContainers,
                                     &AddFiles,        /* Use our own callback function */
                                     &QFiles,          /* Private data */
                                     cbMessage,
                                     pszMessage ) );

}


/* -------------------------------------------------------------------- */                           
/* WKF_ADDFILES                                                         */                           
/* -------------------------------------------------------------------- */                           
/* Callback function to add a file to the files container of a base     */
/* project. PeterPAM uses this function to associate the correct icons  */
/* with each file, so it does not use the WorkFrame/2 version of this   */
/* function.                                                            */
/*                                                                      */                           
/* This callback function is called to add files to WorkFrame.          */                           
/* Simply loop through all the files -- if any extensions match         */                           
/* those in our list, replace the icon handle.  Then pass all the       */                           
/* remaining data onto the real callback function.                      */                           
/*                                                                      */                           
/*                                                                      */                           
/* Parameters : PVOID   pvPrivateData  (I) pointer to caller specific   */                           
/*                                         data                         */                           
/*              PSZ     pszContainer   (I) path name                    */                           
/*              ULONG   ulCount        (I) number of WKF_PART entries   */                           
/*                                         in pBlock                    */                           
/*              PWKFPAM_PART pwppData  (I) start of data block          */                           
/*                                         containing WKF_PART list     */                           
/*                                                                      */                           
/* Returns :    NO_ERROR       - items added successfully               */                           
/*              other          - OS/2 return code                       */                           
/*                                                                      */                           
/* -------------------------------------------------------------------- */                           
static APIRET APIENTRY AddFiles( PVOID        pvPrivateData,                                         
                                 PSZ          pszContainer,                                          
                                 ULONG        ulCount,                                               
                                 PWKFPAM_PART pwppData )                                             
{                                                                                                    
   QFILES_CALLBACK  *pQFiles;                                                                        
   WKFPAM_PART      *pwppFirstData;                                                                  
   ULONG             i;                                                                              
   ULONG             ulProcessed=0;                                                                  
   PSZ               pszExtension;                                                                   
                                                                                                     
   pQFiles = (QFILES_CALLBACK*) pvPrivateData;                                                       
                                                                                                     
   pwppFirstData = pwppData;                                                                         
                                                                                                     
   while( ulProcessed < ulCount )                                                                    
   {                                                                                                 
      /*                                                                                             
      ** For every file (part) returned...                                                           
      */                                                                                             

      /* Find the last '.' */
      pszExtension = strrchr( pwppData->FileFind.achName, '.' );  
      if( pszExtension )                                                                             
         pszExtension++;      /* The extension starts after the last '.' */                          
      else                                                                                           
         pszExtension = "";   /* No extension */                                                     
                                                                                                     
      for (i=0; i < pQFiles->pHAM->ulNumEntries; i++)                                                
      {                                                                                              
         /*                                                                                          
         ** ...and for every extension in our list...                                                
         */                                                                                          
                                                                                                     
         if( !stricmp( pszExtension,                                                                 
                       pQFiles->pHAM->aExtFileIcon[i].pszExt ) )                                     
         {                                                                                           
            /*                                                                                       
            ** A match!  Use this extension (as long as the new icon is valid)                       
            */                                                                                       
            if ( pQFiles->pHAM->aExtFileIcon[i].hptrIcon )                                           
               pwppData->hIcon = pQFiles->pHAM->aExtFileIcon[i].hptrIcon;                            
                                                                                                     
            break;                                                                                   
         }                                                                                           
      } /* endfor */                                                                                 
                                                                                                     
      pwppData = WKFPAM_NEXTPART( pwppData );                                                        
      ulProcessed++;                                                                                 
                                                                                                     
   } /* endwhile */                                                                                  
                                                                                                     
                                                                                                     
   /*                                                                                                
   ** ..and we're done!  Pass the data to the real WKF_ADDFILES callback function
   */                                                                                                
   return( (*pQFiles->pfnAddFiles)( pQFiles->pvPrivateData,                                          
                                    pszContainer,                                                    
                                    ulCount,                                                         
                                    pwppFirstData ) );                                               
}                                                                                                    
                                                                                                     




/* -------------------------------------------------------------------- */
/* WkfQueryTitle                                                        */
/* -------------------------------------------------------------------- */
/* PAM provided function for querying the access method title or        */
/* description.                                                         */
/*                                                                      */
/* Parameters : HAM     hamOwner       (I)  project handle              */
/*              ULONG   ulTitleMax     (I)  number of bytes in the      */
/*                                          buffer allocated for the    */
/*                                          string                      */
/*              PSZ     pszTitle       (O)  address of the buffer       */
/*                                          containing the null         */
/*                                          delimited title string;     */
/*                                          on error, a 0 length        */
/*                                          string should be returned.  */
/*                                                                      */
/* Returns :    APIRET  NO_ERROR  success                               */
/*                                                                      */
/* -------------------------------------------------------------------- */
extern APIRET APIENTRY WKFQUERYTITLE( HAM     hamOwner,
                                      ULONG   ulTitleMax,
                                      PSZ     pszTitle )
{
   PPHAM           *pHAM;

   pHAM = (PPHAM*) hamOwner;

   memset( pszTitle, '\0', ulTitleMax );

   /*
   ** Copy our title, then append the chained PAM's title
   */
   strncpy( pszTitle, 
            TITLE, 
            max( 0, ulTitleMax-1 ) );

   return( (*pHAM->BPAM.QueryTitle)( pHAM->BPAM.ham,
                                     max( 0, ulTitleMax - sizeof( TITLE ) +1 ),
                                    &pszTitle[ sizeof( TITLE )-1 ] ) );
}





/* -------------------------------------------------------------------- */
/* WkfSettings                                                          */
/* -------------------------------------------------------------------- */
/* This entry point is optional.  If present, the base project Settings */
/* notebook Access page will enable an Options pushbutton.  Upon        */
/* invocation, the function can display a dialog to acquire and store   */
/* PeterPAM specific information.                                       */
/*                                                                      */
/* For this PAM, the entry point displays a dialog containing a list    */
/* of extensions to search for, and icon filenames to use.              */
/*                                                                      */
/* Parameters : HAM     hamOwner       (I)  project handle              */
/*              PSZ     pszProjectFile (I)  Project INI file name       */
/*              HWND    hwndParent     (I)  parent window handle        */
/*              HWND    hwndOwner      (I)  owner window handle         */
/*              ULONG   cbMessage      (I)  size of message buffer      */
/*              PSZ     pszMessage     (O)  pointer to message buffer   */
/*                                                                      */
/* Returns :    APIRET  if return code is not 0, then an access method  */
/*                      specific message may be returned through        */
/*                      the pszMessage buffer.                          */
/*                                                                      */
/* -------------------------------------------------------------------- */
extern APIRET APIENTRY WKFSETTINGS( HAM     hamOwner,
                                    PSZ     pszProjectFile,
                                    HWND    hwndParent,
                                    HWND    hwndOwner,
                                    ULONG   cbMessage,
                                    PSZ     pszMessage )
{
   PPHAM   *pHAM;
   APIRET   rc;

   pHAM = (PPHAM*) hamOwner;
   
   rc = Settings( pHAM,
                  pszProjectFile, 
                  hwndParent,     
                  hwndOwner,      
                  cbMessage,
                  pszMessage );

   /*
   ** This is an optional API -- only chain to bPAM if it exists there
   */
   if ( rc == NO_ERROR &&
        pHAM->BPAM.Settings )
   {
      rc = (*pHAM->BPAM.Settings)( pHAM->BPAM.ham,
                                   pszProjectFile, 
                                   hwndParent,     
                                   hwndOwner,      
                                   cbMessage,      
                                   pszMessage );
   }

   return( rc );
}





/*
** The rest of the file contains only chained jumps into BPAM
*/





/* -------------------------------------------------------------------- */
/* WkfRun                                                               */
/* -------------------------------------------------------------------- */
/* PAM provided function for executing an action                        */
/* On entry, cbMessage contains the number of bytes allocated           */
/* for the pszMessageBuffer.  On exit, pszMessage buffer may contain    */
/* a single null delimited string to be displayed to the user.          */
/* The PAM function should check the length field prior to writing any  */
/* data since a count of 0 may imply a NULL pointer.                    */
/*                                                                      */
/* Parameters : HAM     hamOwner       (I)  project handle              */
/*              PSZ     pszTitle       (I)  Title to be used or NULL    */
/*              PSZ     pszProgram     (I)  program to be executed      */
/*              PSZ     pszParameters  (I)  parameter string            */
/*              char   *pcEnvironment  (I)  environment block           */
/*              PSZ     psStartDir    (I)  startup directory           */
/*              PSZ     pszTermQueueName (I) termination queue name     */
/*                                          to be used for returning    */
/*                                          result codes as defined by  */
/*                                          DosStartSession.            */
/*              ULONG   ulFlags        (I)  = 0, Send message           */
/*                                          = 1, use full screen mode   */
/*                                               for DosStartSession    */
/*                                          = 2, use vio window         */
/*                                               for DosStartSession    */
/*                                          = 3, run cmd in a monitor   */
/*                                          = 255, use default mode for */
/*                                               DosStartSession        */
/*                                                                      */
/*              PWKF_ADDMONITOR pfnAddMonitor                           */
/*                                     (I)  call back function for      */
/*                                          inserting lines of output   */
/*                                          from the action invoked     */
/*                                          into the monitor container  */
/*                                          This parameter is only      */
/*                                          used if ulFlags = 3(Monitor)*/
/*                                          in which case it is required*/
/*              PVOID   pvPrivateData  (I)  parameter to be passed back */
/*                                          to the pfnAddMonitor        */
/*                                          callback function           */
/*              ULONG   cbMessage      (I)  size of message buffer      */
/*              PSZ     pszMessage     (O)  pointer to message buffer   */
/*                                                                      */
/* Returns :    APIRET  if return code is not 0, then an access method  */
/*                      specific message may be returned through        */
/*                      the pszMessage buffer.                          */
/*                                                                      */
/* -------------------------------------------------------------------- */
extern APIRET APIENTRY WKFRUN( HAM     hamOwner,
                               PSZ     pszTitle,
                               PSZ     pszProgram,
                               PSZ     pszParameters,
                               CHAR  * pcEnvironment,
                               PSZ     pszStartDir,
                               PSZ     pszTermQueueName,
                               ULONG   ulFlags,
                               PWKF_ADDMONITOR pfnAddMonitor,
                               PVOID   pvPrivateData,
                               ULONG   cbMessage,
                               PSZ     pszMessage )
{
   PPHAM           *pHAM;

   pHAM = (PPHAM*) hamOwner;

   return( (*pHAM->BPAM.Run)( pHAM->BPAM.ham,
                              pszTitle,             
                              pszProgram,           
                              pszParameters,        
                              pcEnvironment,        
                              pszStartDir,          
                              pszTermQueueName,     
                              ulFlags,              
                              pfnAddMonitor,
                              pvPrivateData,        
                              cbMessage,            
                              pszMessage ) );
}



/* -------------------------------------------------------------------- */
/* WkfWrite                                                             */
/* -------------------------------------------------------------------- */
/* PAM provided function for writing characters to the input pipe       */
/* of the program started through WkfRun.                               */
/*                                                                      */
/* Parameters : HAM     hamOwner       (I)  project handle              */
/*              ULONG   ulBytes        (I)  number of bytes to write    */
/*              char  * pcBuffer       (I)  data to be written          */
/*              ULONG   cbMessage      (I)  size of message buffer      */
/*              PSZ     pszMessage     (O)  pointer to message buffer   */
/*                                                                      */
/* Returns :    APIRET  if return code is not 0, then an access method  */
/*                      specific message may be returned through        */
/*                      the pszMessage buffer.                          */
/*                                                                      */
/* -------------------------------------------------------------------- */
extern APIRET APIENTRY WKFWRITE( HAM     hamOwner,
                                 ULONG   ulBytes,
                                 char  * pcBuffer,
                                 ULONG   cbMessage,
                                 PSZ     pszMessage )
{
   PPHAM           *pHAM;

   pHAM = (PPHAM*) hamOwner;

   return( (*pHAM->BPAM.Write)( pHAM->BPAM.ham,
                                ulBytes,     
                                pcBuffer,    
                                cbMessage,   
                                pszMessage ) );
}




/* -------------------------------------------------------------------- */
/* WkfStop                                                              */
/* -------------------------------------------------------------------- */
/* PAM provided function for stopping a process started through         */
/* WkfRun.  The HAM value uniquely identifies the task to be aborted.   */
/*                                                                      */
/* Parameters : HAM     hamOwner       (I)  project handle              */
/*              ULONG   cbMessage      (I)  size of message buffer      */
/*              PSZ     pszMessage     (O)  pointer to message buffer   */
/*                                                                      */
/* Returns :    APIRET  if return code is not 0, then an access method  */
/*                      specific message may be returned through        */
/*                      the pszMessage buffer.                          */
/*                                                                      */
/* -------------------------------------------------------------------- */
extern APIRET APIENTRY WKFSTOP( HAM     hamOwner,
                                ULONG   cbMessage,
                                PSZ     pszMessage )
{
   PPHAM           *pHAM;

   pHAM = (PPHAM*) hamOwner;

   return( (*pHAM->BPAM.Stop)( pHAM->BPAM.ham,
                               cbMessage,   
                               pszMessage ) );
}


/* -------------------------------------------------------------------- */
/* WkfQueryRunnable                                                     */
/* -------------------------------------------------------------------- */
/* PAM provided funtion for determining is a given file can be run      */
/*                                                                      */
/* Parameters : HAM     hamOwner       (I)  project handle              */
/*              PSZ     pszContainer   (I)  source path                 */
/*              PSZ     pszFilename    (I)  name of file to be queried  */
/*                                                                      */
/* Returns :    BOOL    TRUE  - file can be run (via the WkfRun API)    */
/*                      FALSE - file cannot be run                      */
/*                                                                      */
/* -------------------------------------------------------------------- */
extern BOOL APIENTRY WKFQUERYRUNNABLE( HAM     hamOwner,
                                       PSZ     pszContainer,
                                       PSZ     pszFilename )
{
   PPHAM           *pHAM;

   pHAM = (PPHAM*) hamOwner;

   return( (*pHAM->BPAM.QueryRunnable)( pHAM->BPAM.ham,
                                        pszContainer,
                                        pszFilename ) );
}




/* -------------------------------------------------------------------- */
/* WkfQueryPathInfo                                                     */
/* -------------------------------------------------------------------- */
/* PAM provided function for testing whether the filename is valid.     */
/* If the filename is valid, then the fully specified name is returned. */
/*                                                                      */
/* Parameters : HAM     hamOwner        (I)  project handle             */
/*              PSZ     pszPathName     (I)  file name to be validated  */
/*              ULONG   ulFullPathBufSize (I) size of following buffer  */
/*              PSZ     pszFullPathName (I)  address to the output      */
/*                                           buffer of size CCHMAXPATH  */
/*                                      (O)  *pszFullPathName contains  */
/*                                           the fully specified name   */
/*                                           if valid.                  */
/*              BOOL   *pfExists,      (O)  Does the file actually exist*/
/*              ULONG   cbMessage      (I)  size of message buffer      */
/*              PSZ     pszMessage     (O)  pointer to message buffer   */
/*                                                                      */
/* Returns :    APIRET  NO_ERROR                                        */
/*                      ERROR_PATH_NOT_FOUND                            */
/*                      ERROR_FILENAME_EXCED_RANGE                      */
/*                      ERROR_INSUFFICIENT_BUFFER                       */
/*                                                                      */
/* -------------------------------------------------------------------- */
extern APIRET APIENTRY WKFQUERYPATHINFO( HAM     hamOwner,
                                         PSZ     pszPathName,
                                         ULONG   ulFullPathBufSize,
                                         PSZ     pszFullPathName,
                                         BOOL   *pfExists,
                                         ULONG   cbMessage,
                                         PSZ     pszMessage )
{
   PPHAM           *pHAM;

   pHAM = (PPHAM*) hamOwner;

   return( (*pHAM->BPAM.QueryPathInfo)( pHAM->BPAM.ham,
                                        pszPathName,       
                                        ulFullPathBufSize, 
                                        pszFullPathName,   
                                        pfExists,          
                                        cbMessage,         
                                        pszMessage ) );
}



/* -------------------------------------------------------------------- */
/* WkfCreatePath                                                        */
/* -------------------------------------------------------------------- */
/* Creates all nodes along the path specified                           */
/*                                                                      */
/* Parameters : HAM     hamOwner       (I)  project handle              */
/*              PSZ     pszPath        (I)  path name to be created     */
/*              ULONG   cbMessage      (I)  size of message buffer      */
/*              PSZ     pszMessage     (O)  pointer to message buffer   */
/*                                                                      */
/* Returns :    APIRET  if return code is not 0, then an access method  */
/*                      specific message may be returned through        */
/*                      the pszMessage buffer.                          */
/*                                                                      */
/* -------------------------------------------------------------------- */
extern APIRET APIENTRY WKFCREATEPATH( HAM     hamOwner,
                                      PSZ     pszPath,
                                      ULONG   cbMessage,
                                      PSZ     pszMessage )
{
   PPHAM           *pHAM;

   pHAM = (PPHAM*) hamOwner;

   return( (*pHAM->BPAM.CreatePath)( pHAM->BPAM.ham,
                                     pszPath,     
                                     cbMessage,   
                                     pszMessage ) );
}


/* -------------------------------------------------------------------- */
/* WkfQueryCopy                                                         */
/* -------------------------------------------------------------------- */
/* PAM provided function for testing whether files can be copied into   */
/* the current container.  This function may be called as a result      */
/* of a DM_DRAGOVER message across a Base Project Window                */
/*                                                                      */
/* Parameters : HAM     hamOwner       (I)  project handle              */
/*              PSZ     pszFile        (I)  object to be copied         */
/*              PSZ     pszTargetPath  (I)  "target" path               */
/*              PULONG  pulMove        (O)  determines whether objects  */
/*                                          can be moved/copied/both    */
/*                                          or none                     */
/*                                          WKF_NOCOPY    : no copy/    */
/*                                                          move        */
/*                                          WKF_COPYFILES : copy only   */
/*                                          WKF_MOVEFILES : copy and    */
/*                                                          delete orig.*/
/*              ULONG   cbMessage      (I)  size of message buffer      */
/*              PSZ     pszMessage     (O)  pointer to message buffer   */
/*                                                                      */
/* Returns :    APIRET  if return code is not 0, then an access method  */
/*                      specific message may be returned through        */
/*                      the pszMessage buffer.                          */
/*                                                                      */
/* -------------------------------------------------------------------- */
extern APIRET APIENTRY WKFQUERYCOPY( HAM     hamOwner,
                                     PSZ     pszFile,
                                     PSZ     pszTargetPath,
                                     PULONG  pulMove,
                                     ULONG   cbMessage,
                                     PSZ     pszMessage )
{
   PPHAM           *pHAM;

   pHAM = (PPHAM*) hamOwner;

   return( (*pHAM->BPAM.QueryCopy)( pHAM->BPAM.ham,
                                    pszFile,       
                                    pszTargetPath, 
                                    pulMove,       
                                    cbMessage,     
                                    pszMessage ) );
}




/* -------------------------------------------------------------------- */
/* WkfCopyFile                                                          */
/* -------------------------------------------------------------------- */
/* PAM provided funtion for copying files from projects using the       */
/* same access methods                                                  */
/*                                                                      */
/* Parameters : HAM     hamOwner       (I)  project handle              */
/*              PSZ     pszPathFrom    (I)  source path                 */
/*              PSZ     pszFileFrom    (I)  name of file to be copied   */
/*              PSZ     pszPathTo      (I)  target path                 */
/*              PSZ     pszFileTo      (I)  target file name            */
/*              ULONG   ulMove         (I)  determines whether objects  */
/*                                          are to be moved or copied   */
/*                                          WKF_COPYFILES : copy only   */
/*                                          WKF_MOVEFILES : copy and    */
/*                                                          delete orig.*/
/*              ULONG   cbMessage      (I)  size of message buffer      */
/*              PSZ     pszMessage     (O)  pointer to message buffer   */
/*                                                                      */
/* Returns :    APIRET  if return code is not 0, then an access method  */
/*                      specific message may be returned through        */
/*                      the pszMessage buffer.                          */
/*                                                                      */
/* -------------------------------------------------------------------- */
extern APIRET APIENTRY WKFCOPYFILE( HAM     hamOwner,
                                    PSZ     pszPathFrom,
                                    PSZ     pszFileFrom,
                                    PSZ     pszPathTo,
                                    PSZ     pszFileTo,
                                    ULONG   ulMove,
                                    ULONG   cbMessage,
                                    PSZ     pszMessage )
{
   PPHAM           *pHAM;

   pHAM = (PPHAM*) hamOwner;

   return( (*pHAM->BPAM.CopyFile)( pHAM->BPAM.ham,
                                   pszPathFrom, 
                                   pszFileFrom, 
                                   pszPathTo,   
                                   pszFileTo,   
                                   ulMove,
                                   cbMessage,
                                   pszMessage ) );
}




/* -------------------------------------------------------------------- */
/* WkfDelete                                                            */
/* -------------------------------------------------------------------- */
/* PAM provided funtion for deleting file                               */
/*                                                                      */
/* Parameters : HAM     hamOwner       (I)  project handle              */
/*              PSZ     pszPath        (I)  source path                 */
/*              PSZ     pszFile        (I)  name of file to be deleted  */
/*              ULONG   cbMessage      (I)  size of message buffer      */
/*              PSZ     pszMessage     (O)  pointer to message buffer   */
/*                                                                      */
/* Returns :    APIRET  if return code is not 0, then an access method  */
/*                      specific message may be returned through        */
/*                      the pszMessage buffer.                          */
/*                                                                      */
/* -------------------------------------------------------------------- */
extern APIRET APIENTRY WKFDELETE( HAM     hamOwner,
                                  PSZ     pszPath,
                                  PSZ     pszFile,
                                  ULONG   cbMessage,
                                  PSZ     pszMessage )
{
   PPHAM           *pHAM;

   pHAM = (PPHAM*) hamOwner;

   return( (*pHAM->BPAM.Delete)( pHAM->BPAM.ham,
                                 pszPath,     
                                 pszFile,     
                                 cbMessage,   
                                 pszMessage ) );
}




/* -------------------------------------------------------------------- */
/* WkfDefaultName                                                       */
/* -------------------------------------------------------------------- */
/* PAM provided funtion for returning the default make file name or     */
/* the default target file name,                                        */
/*                                                                      */
/* Parameters : HAM     hamOwner       (I)  project handle              */
/*              PSZ     pszPrjFileName (I)  project file name           */
/*              ULONG   ulRequestFlag  (I)  indicates which default name*/
/*                                     (I)  to return                   */
/*              ULONG   cbDefault      (I)  size of pszTarget buffer    */
/*              PSZ     pszDefault     (O)  address to the buffer       */
/*                                          containing the default      */
/*                                          name requested              */
/*              ULONG   cbMessage      (I)  size of message buffer      */
/*              PSZ     pszMessage     (O)  pointer to message buffer   */
/*                                                                      */
/* Returns :    APIRET  if return code is not 0, then an access method  */
/*                      specific message may be returned through        */
/*                      the pszMessage buffer.                          */
/*                                                                      */
/* -------------------------------------------------------------------- */
extern APIRET APIENTRY WKFDEFAULTNAME( HAM     hamOwner,
                                       PSZ     pszPrjFileName,
                                       ULONG   ulRequestFlag,
                                       ULONG   cbDefault,
                                       PSZ     pszDefault,
                                       ULONG   cbMessage,
                                       PSZ     pszMessage )
{
   PPHAM           *pHAM;

   pHAM = (PPHAM*) hamOwner;

   return( (*pHAM->BPAM.DefaultName)( pHAM->BPAM.ham,
                                      pszPrjFileName,
                                      ulRequestFlag,
                                      cbDefault,
                                      pszDefault,
                                      cbMessage,
                                      pszMessage ) );
}





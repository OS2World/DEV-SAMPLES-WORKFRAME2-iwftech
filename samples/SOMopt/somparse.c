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

static APIRET ProcessLine( HWND hwndErrorBox,
                           HWND hwndFileNames,
                           USHORT Index,
                           USHORT *pusInsertedAt );


/******************************************************************************/
/*                                                                            */
/* FUNCTION: deflparParseError                                                */
/*                                                                            */
/*  The entrypoint             in the Compiler Options DLL is invoked during  */
/*  processing of the output of a Compile, Make or Build operation.           */
/*  The interface is provided with a line of output form the compiler         */
/*  and is expected to return the filename, line number, and column number    */
/*  associated with the error message on that line.                           */
/*                                                                            */
/*   Inputs:                                                                  */
/*                                                                            */
/*     HWND    - window handle of the error message listbox                   */
/*     PUSHORT - pointer to first line containing an error message            */
/*               in listbox                                                   */
/*     PUSHORT - pointer to last line containing an error message             */
/*     HWND    - window handle of listbox to insert filenames into            */
/*     PUSHORT - index of selected filename in filename listbox               */
/*     PVOID   - pointer to area containing options                           */
/*     PULONG  - validity of options (initialized or not)                     */
/*                                                                            */
/*   Returns:    0 if successful, 1 if cancel selected, 2 if error            */
/******************************************************************************/

extern ULONG APIENTRY WKFCOMPILEPARSEERROR( HWND     hwndErrorBox,
                                            PSZ      pszProject,
                                            USHORT * pusStartLine,
                                            USHORT * pusEndLine,
                                            HWND     hwndFileNames,
                                            USHORT * pusCurrentFile,
                                            VOID   * pvOptions,
                                            ULONG  * pulSetting )
{
   APIRET         rc;

   short          sIndex, i;
   short          sCount;
   USHORT         usInsertIndex;


   /**************************************/
   /* Get the index of the selected line */
   /**************************************/
   sIndex = WinQueryLboxSelectedItem( hwndErrorBox );
   if( sIndex == LIT_NONE )
      return WKFRC_PARSEERROR_ERROR;

   /*********************************/
   /* Get the total number of lines */
   /*********************************/
   sCount = WinQueryLboxCount( hwndErrorBox );

   rc = ProcessLine( hwndErrorBox,
                     hwndFileNames,
                     sIndex,
                     pusCurrentFile );
   if( rc )
      return( WKFRC_PARSEERROR_INVALID );

   i = sIndex ;

   *pusStartLine = 1;                     /* we will be processing all of list */
   *pusEndLine = sCount;

   /**************************************/
   /* Now, scan backwards from the       */
   /* selected line, and collect info    */
   /* about ALL errors that occurred.    */
   /* (line numbers and file names)      */
   /**************************************/

   while ( --i > 0)
   {
      rc = ProcessLine( hwndErrorBox,
                        hwndFileNames,
                        i,
                       &usInsertIndex );  /* Not needed here, just a place holder */

      if( rc & rc != WKFRC_PARSEERROR_INVALID )
         return( rc );
   }

   /*******************************************/
   /* Scan from 'LIT' line + 1 and forwards   */
   /* for all interesting lines.              */
   /*******************************************/

   i = sIndex ;

   while (++i < sCount)
   {
      rc = ProcessLine( hwndErrorBox,
                        hwndFileNames,
                        i,
                       &usInsertIndex );  /* Not needed here, just a place holder */

      if( rc & rc != WKFRC_PARSEERROR_INVALID )
         return( rc );
   }

   return( WKFRC_PARSEERROR_SUCCESS ); 
}



static APIRET ProcessLine( HWND hwndErrorBox,
                           HWND hwndFileNames,
                           USHORT Index,
                           USHORT *pusInsertedAt )
{
   WKF_TAGPARAM   mpParam;
   char           szBuffer[CCHMAXPATH];
   char           pszMessage[CCHMAXPATH];
   char           pszFilename[CCHMAXPATH];
   int            LineNumber;
   short          insertIndex;
   int            matched;

   /*****************************************/
   /* With Index, we can retrieve the line  */
   /* the user selected, and pick up the    */
   /* name of the file he is interested in. */
   /*****************************************/
   WinQueryLboxItemText( hwndErrorBox, 
                         Index, 
                         szBuffer, 
                         sizeof(szBuffer) );


   /**********************************************/
   /*   Now we actually parse the line.  This    */
   /*   example is hardcoded for the format of   */
   /*   the SOM compiler.  You can either        */
   /*   adjust the format to match the messages  */
   /*   you expect to parse, or develop a        */
   /*   generic pattern matching routine.        */
   /*                                            */
   /*   The following matches lines in the form: */
   /*    "som1.csc": 22: Error in pre-processor  */
   /**********************************************/
   memset( pszFilename, 0, sizeof( pszFilename ) );
   memset( pszMessage, 0, sizeof( pszMessage ) );

   matched = sscanf( szBuffer,
                     "\"%[^\"]\": %d: %[^\n]",
                     pszFilename, 
                    &LineNumber, 
                     pszMessage );

   if( matched != 3 )
      return WKFRC_PARSEERROR_INVALID;

   /**********************************************/
   /* Has this filename been inserted into the   */
   /* filename box yet?                          */
   /**********************************************/
   insertIndex = SHORT1FROMMR( WinSendMsg(hwndFileNames,
                               LM_SEARCHSTRING,
                               MPFROM2SHORT(LSS_CASESENSITIVE,LIT_FIRST),
                               MPFROMP( pszFilename )));

   if( insertIndex == LIT_ERROR )
      return WKFRC_PARSEERROR_ERROR;

   /************************************************/
   /*      Add the file name to the list box       */
   /************************************************/
   if( insertIndex == LIT_NONE )
      insertIndex = WinInsertLboxItem( hwndFileNames, 
                                       LIT_END, 
                                       pszFilename );


   /**************************************/
   /* Set the tag for the selected line. */
   /* Offset starts at zero.             */
   /**************************************/
   mpParam.sub.errorLine   = LineNumber;
   mpParam.sub.offset      = 0;
   mpParam.sub.fileNumber  = (char) (*pusInsertedAt = insertIndex) ;

   WinSendMsg( hwndErrorBox,
               LM_SETITEMHANDLE,
               MPFROMSHORT(Index),
               (MPARAM)(mpParam.tagParam) );

   return WKFRC_PARSEERROR_SUCCESS;
}

  

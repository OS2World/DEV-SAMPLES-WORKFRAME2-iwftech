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
/*|  This program illustrates adding or replacing actions to an actions      |*/
/*|  profile.                                                                |*/
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

#include "profile.h"

/* ----------------------------------------------------------------------- */
/* AddActionToProfile()                                                    */
/* ----------------------------------------------------------------------- */
BOOL _Optlink profileAdd( PSZ pszObject,
                          PSZ pszActionClass,
                          PSZ pszActionName,
                          PSZ pszProgram,
                          PSZ pszSourceType,
                          PSZ pszTargetType,
                          PSZ pszSupportMod,
                          PSZ pszSupportEntry,
                          PSZ pszActionType,
                          PSZ pszActionScope,
                          PSZ pszMenuOption,
                          PSZ pszRunMode )
   {
   HOBJECT   hProfileObject;
   PSZ       pszActionData;
   BOOL      bResult;

   /* ---------------------------------------------------------------------- */
   /* - allocate the maximum storage required to hold the setup data for   - */
   /* - a specific action                                                  - */
   /* ---------------------------------------------------------------------- */
   pszActionData = malloc( 12*CCHMAXPATH+sizeof(ACTION_DEFN_TEMPLATE) );
   if ( pszActionData == NULL )
      return( FALSE );

   /* ---------------------------------------------------------------------- */
   /* - make sure that the AP already exists before attempting to add an   - */
   /* - action to the profile                                              - */
   /* ---------------------------------------------------------------------- */
   hProfileObject = WinQueryObject( pszObject );
   if ( hProfileObject == NULLHANDLE )
      {
      bResult = FALSE;
      }
   else
      {
      /* ------------------------------------------------------------------- */
      /* - build up the setup data string for the action - the ORDER of    - */
      /* - the data elements are important                                 - */
      /* ------------------------------------------------------------------- */
      sprintf( pszActionData,
               ACTION_DEFN_TEMPLATE,
               pszActionName,
               pszActionClass,
               pszProgram,
               pszSourceType,
               pszTargetType,
               pszSupportMod,
               pszSupportEntry,
               pszActionType,
               pszActionScope,
               pszMenuOption,
               pszRunMode );

      /* ------------------------------------------------------------------- */
      /* - add the action to the profile                                   - */
      /* ------------------------------------------------------------------- */
      bResult = WinSetObjectData(hProfileObject, pszActionData );
      }


   /* ---------------------------------------------------------------------- */
   /* - free storage and return result                                     - */
   /* ---------------------------------------------------------------------- */
   free( pszActionData );

   return( bResult );
   }

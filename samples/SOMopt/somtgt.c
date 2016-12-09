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

#pragma title("IBM WorkFrame/2 Ver 2.1")
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



APIRET APIENTRY WKFQUERYCOMPILETARGET(
                             VOID           **ppvToolBuffer,
                             HWND             hwndWorkFrame,
                             HMODULE          hModHandle,
                             PSZ              pszProject,  
                             PSZ              pszTitle,
                             PWKF_ACTION      pwkfAction,   
                             ULONG            ulOptionsLength,
                             VOID            *pvOptions,
                             ULONG            ulCmdLineLength,
                             PSZ              pszCmdLine,
                             ULONG            ulSearchPath,
                             PSZ              pszSearchPath,
                             LIST_OF_SOURCE  *pstSourceObjects,
                             LIST_OF_TARGET  *pstTargetObjects,
                             LIST_OF_DEPEND  *pstDependObjects,
                             WKF_REALLOC     *pWkf_Realloc,
                             WKF_PRT_STATUS  *pWkf_PrtStatus,
                             BOOL             bFileScope ) 
{
   ULONG        ulTargetBufferNeeded = 0;
   ULONG        ulCmdLengthNeeded;
   SOM_OPTIONS *pOptions;
   ULONG        ulEmitFiles = 0;
   CHAR         szEmitFiles[ 100 ];   /* Enough to emit all types */


   szEmitFiles[0] = '\0';

   pOptions = (SOM_OPTIONS*)pvOptions;


   /*
   ** OK, the structures are all setup, now: do we need to initialize to the default?
   */
   if ( ulOptionsLength < sizeof(SOM_OPTIONS) ||
        strncmp( pOptions->szSignature,
                 SOM_OPTIONS_SIGNATURE,
                 sizeof(SOM_OPTIONS_SIGNATURE) ) )
   {
      /*
      ** Do we need to increase the size of the options block?
      */
      if ( ulOptionsLength < sizeof(SOM_OPTIONS) )
      {
         pOptions = (*pWkf_Realloc)( pOptions, 
                                     sizeof(SOM_OPTIONS) );
   
         if (!pOptions)
            return( ERROR_NOT_ENOUGH_MEMORY );
      }

      /*
      ** Use the default options
      */
      SetDefaultOptions( pOptions );
   }



   /*
   ** Mark the first file passed as used.
   */
   pstSourceObjects->ulNumUsed = 1;
   pstSourceObjects->stObject[0].fUsed = WKF_QT_FILE_USED;


   /*
   ** Now, make sure the buffers are big enough
   */
   ulCmdLengthNeeded = sizeof( DEFAULT_CMD_LINE );

   /*
   ** Was an output directory specified?
   */
   if( pOptions->szOutputDir[0] )
      ulCmdLengthNeeded += strlen( pOptions->szOutputDir ) + 
                           sizeof( SZ_OUTPUTDIR );

   if( pOptions->bVerifyRelease )
      ulCmdLengthNeeded += sizeof( SZ_VERIFYRELEASE );

   if( !pOptions->bDisplayWarnings )   /* Default is on */
      ulCmdLengthNeeded += sizeof( SZ_DISPLAYWARNINGS );

   if( pOptions->cCommentStyle != COMMENT_S ) /* default == s */
      ulCmdLengthNeeded += sizeof( SZ_COMMENTSTYLE )+1; 

      
   if( pOptions->bEmitFileDEF )
      { strcat( szEmitFiles, SZ_EMITFILEDEF ); ulEmitFiles++; } 

   if( pOptions->bEmitFileSC )
      { strcat( szEmitFiles, SZ_EMITFILESC ); ulEmitFiles++; } 

   if( pOptions->bEmitFilePSC )
      { strcat( szEmitFiles, SZ_EMITFILEPSC ); ulEmitFiles++; } 

   if( pOptions->bEmitFileCS2 )
      { strcat( szEmitFiles, SZ_EMITFILECS2 ); ulEmitFiles++; } 

   if( pOptions->bEmitFileC )
      { strcat( szEmitFiles, SZ_EMITFILEC ); ulEmitFiles++; } 

   if( pOptions->bEmitFileH )
      { strcat( szEmitFiles, SZ_EMITFILEH ); ulEmitFiles++; } 

   if( pOptions->bEmitFileIH )
      { strcat( szEmitFiles, SZ_EMITFILEIH ); ulEmitFiles++; } 

   if( pOptions->bEmitFilePH )
      { strcat( szEmitFiles, SZ_EMITFILEPH ); ulEmitFiles++; } 

   if( pOptions->bEmitFileCPP )
      { strcat( szEmitFiles, SZ_EMITFILECPP ); ulEmitFiles++; } 

   if( pOptions->bEmitFileXH )
      { strcat( szEmitFiles, SZ_EMITFILEXH ); ulEmitFiles++; } 

   if( pOptions->bEmitFileXIH )
      { strcat( szEmitFiles, SZ_EMITFILEXIH ); ulEmitFiles++; } 

   if( pOptions->bEmitFileXPH )
      { strcat( szEmitFiles, SZ_EMITFILEXPH ); ulEmitFiles++; } 

   if( ulEmitFiles )
      ulCmdLengthNeeded += strlen( szEmitFiles ) + 
                           sizeof( SZ_EMITFILE ) + 2;  /* +2 == quotes */

   if( ulCmdLineLength < (ulCmdLengthNeeded + 1) )   /* +1 == space for null */
   {
      pszCmdLine = (*pWkf_Realloc)( pszCmdLine, ulCmdLengthNeeded + 1 );    /* +1 == space for null */

      if( !pszCmdLine )
         return( ERROR_NOT_ENOUGH_MEMORY );
   }


   if ( pstTargetObjects )
   {
      /*
      ** The format we'll use in the output buffer is %n.XXX
      */
      ulTargetBufferNeeded = sizeof( "%n." ) * ulEmitFiles +   
                             strlen( szEmitFiles ) + 1;       /* +1 == null */
   
      if( pstTargetObjects->ulBufferSize < ulTargetBufferNeeded )
      {
         pstTargetObjects = (*pWkf_Realloc)( pstTargetObjects, ulTargetBufferNeeded );
   
         if( !pstTargetObjects )
            return( ERROR_NOT_ENOUGH_MEMORY );
   
         pstTargetObjects->ulBufferSize = ulTargetBufferNeeded;
      }
   }
   

   if( !ulSearchPath )                                      
   {                                                             
      pszSearchPath = (*pWkf_Realloc)( pszSearchPath, 1 );     
                                                                 
      if( !pszSearchPath )                                
         return( ERROR_NOT_ENOUGH_MEMORY );                      
   }                                                             



   /*
   ** Now start filling things in
   */


   /*
   ** Set the search path to NULL -- we are not parsing for dependencies.
   */
   pszSearchPath[0] = '\0';

   /*
   ** Set the dependencies (if we were passed a buffer) -- none.
   */
   if (pstDependObjects)
      pstDependObjects->ulNumOfEntries = 0;


   /*
   ** Set the command line
   */
   pszCmdLine[0] = '\0';

   if( pOptions->szOutputDir[0] )
      strcat( pszCmdLine, SZ_OUTPUTDIR );

   if( pOptions->bVerifyRelease )
      strcat( pszCmdLine, SZ_VERIFYRELEASE );

   if( !pOptions->bDisplayWarnings )   /* Default is on */
      strcat( pszCmdLine, SZ_DISPLAYWARNINGS );

   if( pOptions->cCommentStyle == COMMENT_C ) 
      strcat( pszCmdLine, SZ_COMMENTSTYLE SZ_COMMENT_C );
   else 
   if( pOptions->cCommentStyle == COMMENT_CPP ) 
      strcat( pszCmdLine, SZ_COMMENTSTYLE SZ_COMMENT_CPP );

   if( ulEmitFiles )
   {
      strcat( pszCmdLine, SZ_EMITFILE "\"" );
      strcat( pszCmdLine, &szEmitFiles[1] );      /* Skip the leading ; */
      strcat( pszCmdLine, "\"" );
   }


   /*
   ** Finally add the %f, signifying the source file
   */
   strcat( pszCmdLine, DEFAULT_CMD_LINE );



   /*
   ** Set the targets (if we were passed a buffer),
   */
   if( pstTargetObjects )
   {
      /*
      ** The target buffer should take the form
      **
      **      (filename_1)(filename_2)...(filename_n)\0   
      **                                                  
      ** where each filename_x is a null delimited string 
      */
      pstTargetObjects->cBuffer[0] = '\0';

      if( ulEmitFiles )
      {
         PSZ pszNextExt;
         PSZ pszTarget;

         pszNextExt = strtok( szEmitFiles, ";" );
         pszTarget = pstTargetObjects->cBuffer;

         while( pszNextExt )
         {
            /*
            ** Most of the extensions that are generated are
            ** the same as the command line option.  Rather
            ** than generate a seperate table, we'll use the 
            ** command line options, and adjust the one case
            ** that's different  (xc -> cpp)
            */
            if( !strcmp( "xc", pszNextExt ) )
               pszNextExt = "cpp";

            strcpy( pszTarget, "%n." );
            strcat( pszTarget, pszNextExt );

            pstTargetObjects->ulNumOfEntries++;

            /*
            ** Now setup for the next filename.
            */
            pszTarget += strlen( pszTarget ) +1;

            pszNextExt = strtok( NULL, ";" );
         } /* endwhile */

         /*
         ** Finally, append the extra (trailing) null
         */
         pszTarget[0] = '\0';
      }
      else
         pstTargetObjects->ulNumOfEntries = 0;
   }

   return( NO_ERROR );
}


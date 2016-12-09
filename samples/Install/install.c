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
/*|  This program illustrates installing a WF/2 V 2.1 enabled product.       |*/
/*|  A product folder is created in which a sample base project and an       |*/
/*|  actions profile is created.                                             |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/
/*|                                                                          |*/
/*|  ICC /Kb /Gm install.c project.c profile.c querywf.c                     |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/
#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_WIN
#include <os2.h>

/* ************************************************************************* */
/* - system includes                                                       - */
/* ************************************************************************* */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* ************************************************************************* */
/* - application includes                                                  - */
/* ************************************************************************* */
#include "install.h"

/* ************************************************************************* */
/* - STATIC function prototypes                                            - */
/* ************************************************************************* */
static HOBJECT  _Optlink CreateDefaultObject( PSZ pszTitle,
                                              PSZ pszObjectClass,
                                              PSZ pszLocation,
                                              PSZ pszObjectID );

static POPTIONS _Optlink MakeOptionsStruct( PSZ pszOptions );

static BOOL _Optlink SetOptions( HAB hAB,
                                 PSZ pszProjectFileName,
                                 PSZ pszToolClass,
                                 PSZ pszToolName,
                                 PSZ pszToolScope,
                                 PSZ pszToolOptions );

/* ************************************************************************* */
/* - main line; parameters are ignored                                     - */
/* ************************************************************************* */
int main (int argc, char *argv[] )
   {
   PSZ      pszProjectFileName = NULL;
   HAB      hAB;
   char     szWFDirectory[CCHMAXPATH];
   ULONG    ulMajorVersion;
   ULONG    ulMinorVersion;

   /* ---------------------------------------------------------------------- */
   /* - Just for fun, go find the latest version of the WF/2.              - */
   /* - To locate version 1 of WF/2, when both version 1 and a later       - */
   /* - version is installed, pass FALSE for the last parameter.           - */
   /* ---------------------------------------------------------------------- */
   if ( WkfQueryInstalledVersion( szWFDirectory,
                                  sizeof( szWFDirectory ),
                                  &ulMajorVersion,
                                  &ulMinorVersion,
                                  FALSE ) )
      {
      printf( "SAMPLE - WF/2 Ver %1d.%1d was found on %s\n",
              ulMajorVersion, ulMinorVersion,
              szWFDirectory );
      }
   else
      {
      puts( "ERROR - WF/2 is not installed on this machine" );
      return( 998 );
      }

   if ( ulMajorVersion < 2 )
      {
      puts( "ERROR - Version 2 of WF/2 or later is required to continue" );
      return( 998 );
      }

   /* ---------------------------------------------------------------------- */
   /* - obtain an anchor block since the base project I/O api's require it - */
   /* ---------------------------------------------------------------------- */
   hAB = WinInitialize( 0 );
   if ( hAB == NULLHANDLE )
      {
      puts( "ERROR - Can't initialize anchor block" );
      return( 999 );
      }

   /* ---------------------------------------------------------------------- */
   /* ---------------------------------------------------------------------- */
   /* -                                                                    - */
   /* -         add code to install the real product here                  - */
   /* -                                                                    - */
   /* ---------------------------------------------------------------------- */
   /* ---------------------------------------------------------------------- */

   /*  blah....                                                              */
   /*          blah....                                                      */
   /*                  blah....                                              */


   /* ---------------------------------------------------------------------- */
   /* - create our sample product folder on the desktop                    - */
   /* ---------------------------------------------------------------------- */
   puts("SAMPLE - creating main product folder");
   if ( NULLHANDLE == CreateDefaultObject( "Sample Product",
                                           "WPFolder",
                                           "<WP_DESKTOP>",
                                           ID_MAINFOLDER) )
      {
      puts( "ERROR - main product folder could not be created!" );
      return( 255 );
      }

   /* ---------------------------------------------------------------------- */
   /* - create the sample actions profile - it will be created inside      - */
   /* - our product folder.                                                - */
   /* ---------------------------------------------------------------------- */
   puts("SAMPLE - creating a sample actions profile");
   if ( NULLHANDLE == CreateDefaultObject( "Sample Profile",
                                           WKF_PROFILECLASS,
                                           ID_MAINFOLDER,
                                           ID_PROFILE ) )
      {
      puts( "ERROR - The sample action profile could not be created!" );
      return( 254 );
      }

   /* ---------------------------------------------------------------------- */
   /* - add our sample action  1 to the actions profile                    - */
   /* ---------------------------------------------------------------------- */
   puts("SAMPLE - adding a sample tool 1 to the actions profile");
   if ( !profileAdd( ID_PROFILE,
                     TOOL_CLASS,                  /* tool class              */
                     TOOL_NAME,                   /* tool name               */
                     TOOL_PROGRAM,                /* tool program name       */
                     TOOL_SOURCEMASK,             /* source masks            */
                     TOOL_TARGETMASK,             /* target masks            */
                     TOOL_SUPPORTDLL,             /* WF/2 support DLL        */
                     TOOL_SUPPORTENTRY,           /* entry point name in DLL */
                     TOOL_TYPE,                   /* always command          */
                     TOOL_ASCOPE,                 /* tool scope is file      */
                     TOOL_MENUTYPE,               /* add to all menus        */
                     TOOL_RUNMODE ) )             /* can run in monitor      */
      {
      puts( "ERROR - The sample action 1 could not be added to the AP!" );
      return( 253 );
      }

   /* ---------------------------------------------------------------------- */
   /* - add our sample action  2 to the actions profile                    - */
   /* ---------------------------------------------------------------------- */
   puts("SAMPLE - adding a sample tool 2 to the actions profile");
   if ( !profileAdd( ID_PROFILE,
                     TOOL2_CLASS,                  /* tool class              */
                     TOOL2_NAME,                   /* tool name               */
                     TOOL2_PROGRAM,                /* tool program name       */
                     TOOL2_SOURCEMASK,             /* source masks            */
                     TOOL2_TARGETMASK,             /* target masks            */
                     TOOL2_SUPPORTDLL,             /* WF/2 support DLL        */
                     TOOL2_SUPPORTENTRY,           /* entry point name in DLL */
                     TOOL2_TYPE,                   /* always command          */
                     TOOL2_ASCOPE,                 /* tool scope is file      */
                     TOOL2_MENUTYPE,               /* add to all menus        */
                     TOOL2_RUNMODE ) )             /* can run in monitor      */
      {
      puts( "ERROR - The sample action 2 could not be added to the AP!" );
      return( 253 );
      }

   puts("SAMPLE - setting up the global default options for tool2");
   if ( FALSE == WkfSetDefaultOption( NULLHANDLE,
                                      TOOL2_CLASS,
                                      TOOL2_PROGRAM,
                                      "-myspecialstuff %a %z",
                                      "",
                                      WKFDEF2_PROMPT_NO,
                                      WKFDEF2_EDITDDE_YES ) )
      {
      puts( "ERROR - could not set the global default options for TOOL2" );
      return( 253 );
      }

   /* ---------------------------------------------------------------------- */
   /* - allocate storage to hold the project file name                     - */
   /* ---------------------------------------------------------------------- */
   DosAllocSharedMem( (PPVOID)&pszProjectFileName,
                      (PSZ) NULL,
                      (ULONG) (CCHMAXPATH),
                      PAG_COMMIT | OBJ_GETTABLE | PAG_READ | PAG_WRITE );

   /* ---------------------------------------------------------------------- */
   /* - create the sample base project - assume that the project files     - */
   /* - have been or will be copied to c:\sample directory.                - */
   /* - Also assume that the target program can be run in a monitor.       - */
   /* - On successful return, the file name buffer will contain the        - */
   /* - the fully qualified project file name.  This name is required if   - */
   /* - some default options need to be set for a particular tool.         - */
   /* ---------------------------------------------------------------------- */
   puts("SAMPLE - creating a sample base project");
   if ( NULLHANDLE == projectCreate( "Sample Project",             /* title  */
                            ID_MAINFOLDER,           /* location             */
                            ID_PROJECT,              /* persistent handle    */
                            "c:\\sample",            /* source directory     */
                            "*.*",                   /* view mask            */
                            "SAMPLE.EXE",            /* target program       */
                            "c:\\sample",            /* target directory     */
                            "",                      /* target program parms */
                            WKF_PJIOVALUE_FALSE,     /* target prompt        */
                            WKF_PJIOPROG_MONITOR,    /* target program behave*/
                            "SAMPLE.MAK",            /* target makefile      */
                            NULL,                    /* monitor              */
                            WKF_PJIOMENU_DETAILED,   /* menu type            */
                            WKF_PJIOMENU_DETAILED,   /* menu type            */
                            "EDIT",                  /* non-exe dbl-click    */
                            "RUN",                   /* program dbl-click    */
                            NULL,                    /* PAM                  */
                            ID_PROFILE,              /* actions profile name */
                            pszProjectFileName ) )   /* buffer address where */
                                                     /* fully qualified      */
                                     /* project file name should be written  */
      {
      puts( "ERROR - The base project could not created successfully!" );
      if ( NULL!=pszProjectFileName )
         DosFreeMem( pszProjectFileName );
      return( 252 );
      }

   /* ---------------------------------------------------------------------- */
   /* - if a shared storage buffer was allocated then the fully qualified  - */
   /* - project file name should have been returned.  Use it to set        - */
   /* - the sample tools options in the base project.                      - */
   /* ---------------------------------------------------------------------- */
   if ( pszProjectFileName == NULL )
      {
      puts( "WARNING - couldn't allocate shared memory for the project "
            "file name buffer; tool options not set in project." );
      return( 251 );
      }

   /* ---------------------------------------------------------------------- */
   /* - set the options for the tool in the base project                   - */
   /* ---------------------------------------------------------------------- */
   puts("SAMPLE - writing the default tool options to the base project");
   if ( !SetOptions( hAB,
                     pszProjectFileName,
                     TOOL_CLASS,
                     TOOL_NAME,
                     WKF_SCOPE_FILE,
                     TOOL_OPTIONS ) )

      {
      puts( "WARNING - couldn't write the options to the project file" );
      return( 250 );
      }

   DosFreeMem( pszProjectFileName );

   puts( "Samples created successfully!" );

   WinTerminate( hAB );

   return( 0 );
   }   /* MAIN END */

/* ************************************************************************* */
/* STATIC function definitions                                               */
/* ************************************************************************* */

/* ------------------------------------------------------------------------- */
/* - MakeOptionsStruct()                                                   - */
/* ------------------------------------------------------------------------- */
/* - this code and structures are specific to the support DLL that the tool- */
/* - sample tool provides                                                  - */
/* -                                                                       - */
/* - PARAMETERS:                                                           - */
/* -                                                                       - */
/* -    PSZ pszOptions    - string buffer containing the command line opts - */
/* -                                                                       - */
/* ------------------------------------------------------------------------- */
static POPTIONS _Optlink MakeOptionsStruct( PSZ pszOptions )
   {
   POPTIONS pOptions;
   ULONG    size_of_struct;

   size_of_struct = 2 * sizeof(OPTIONS) + strlen( pszOptions );

   pOptions = calloc( 1, size_of_struct );

   if ( pOptions == NULL )
      return( NULL );

   pOptions->cb = size_of_struct;
   strcpy( pOptions->szSignature, OPTIONS_SIGNATURE );
   strcpy( pOptions->szData, pszOptions );
   pOptions->pszErrTemplate = (PSZ)strlen( pszOptions ) + 1;

   return( pOptions );
   }


/* ------------------------------------------------------------------------- */
/* - CreateDefaultObject()                                                 - */
/* ------------------------------------------------------------------------- */
/* - create any object with a persistent handle and use all default setup  - */
/* -                                                                       - */
/* - PARAMETERS:                                                           - */
/* -                                                                       - */
/* -    PSZ pszTitle       - title of the object                           - */
/* -    PSZ pszObjectClass - class of the object to be instantiated        - */
/* -    PSZ pszLocation    - location where the object should be created   - */
/* -    PSZ pszObjectID    - the name to assign the persistent object      - */
/* -                                                                       - */
/* ------------------------------------------------------------------------- */
#define ID_TEMPLATE "OBJECTID=%s"
static HOBJECT _Optlink CreateDefaultObject( PSZ pszTitle,
                                             PSZ pszObjectClass,
                                             PSZ pszLocation,
                                             PSZ pszObjectID )
   {
   HOBJECT hObject;
   char    szObjectID[255+sizeof(ID_TEMPLATE)];


   /* ---------------------------------------------------------------------- */
   /* - if the object already exists, then just return the existing handle   */
   /* ---------------------------------------------------------------------- */
   hObject = WinQueryObject( pszObjectID );

   if ( hObject == NULLHANDLE )
      {
      sprintf( szObjectID, ID_TEMPLATE, pszObjectID );
      hObject = WinCreateObject( pszObjectClass,
                                 pszTitle,
                                 szObjectID,
                                 pszLocation,
                                 CO_REPLACEIFEXISTS );
      }

   return( hObject );
   }

/* ------------------------------------------------------------------------- */
/* - SetOptions()                                                          - */
/* ------------------------------------------------------------------------- */
/* - set an action option within a base project                            - */
/* -                                                                       - */
/* - PARAMETERS:                                                           - */
/* -                                                                       - */
/* -    HAB hAb                - anchor block handle                       - */
/* -    PSZ pszProjectFileName - fully qualified OS/2 file name of b.p.    - */
/* -    PSZ pszToolClass       - action class name                         - */
/* -    PSZ pszToolName        - action name                               - */
/* -    PSZ pszToolScope       - scope of the action (file or project)     - */
/* -    PSZ pszToolOptions     - command line options string               - */
/* -                                                                       - */
/* ------------------------------------------------------------------------- */
static BOOL _Optlink SetOptions( HAB hAB,
                                 PSZ pszProjectFileName,
                                 PSZ pszToolClass,
                                 PSZ pszToolName,
                                 PSZ pszToolScope,
                                 PSZ pszToolOptions )
   {
   LHANDLE  hProject;
   POPTIONS pOptions;
   char     szOptionsKey[sizeof(WKF_OPTKEY_TEMPLATE)+CCHMAXPATH];

   /********************************************************************
   ** The WorkFrame integration can be done in two ways (for all WF APIs).
   **
   ** - You can use the WorkFrame APIs directly, and link with the 
   **   provided import library.   This will result in cleaner
   **   code, but your executable will be dependent on the WorkFrame
   **   DLLs being present at runtime.   This may cause problems if
   **   this executable will always be run, even if WorkFrame is not 
   **   installed on the user's machine.
   **
   ** - You can load the DLL, and find the WorkFrame entry points, 
   **   dynamically.  This will allow for easy recovery, if the 
   **   WorkFrame DLLs are not found (ie, if WorkFrame is not installed.)
   **
   ** These choices, and the solution, apply to ALL WorkFrame APIs.
   ** Both alternatives are shown below, controlled by the 
   ** macro USE_WKF_IMPLIB
   ********************************************************************/

#ifdef USE_WKF_IMPLIB
   /* Load the WorkFrame entrypoints dynamically */
   HMODULE              hMod;
   PWKFOPENPROJECTFILE  pfnOpenProjectFile;
   PWKFCLOSEPROJECTFILE pfnCloseProjectFile;
   PWKFWRITEPROJECTDATA pfnWriteProjectData;
   char                 szModError[CCHMAXPATH];

   if ( NO_ERROR==DosLoadModule( szModError, CCHMAXPATH, WKFDLL_PJIO, &hMod ) )
      {
      /* ---------------------------------------------------------------- */
      /* get the entry points for the project interfaces                  */
      /* ---------------------------------------------------------------- */
      if ( DosQueryProcAddr( hMod,
                             0,
                             WKFOPENPROJECTFILE_NAME,
                             (PFN*)&pfnOpenProjectFile ) ||
           DosQueryProcAddr( hMod,
                             0,
                             WKFCLOSEPROJECTFILE_NAME,
                             (PFN*)&pfnCloseProjectFile ) ||
           DosQueryProcAddr( hMod,
                             0,
                             WKFWRITEPROJECTDATA_NAME,
                             (PFN*)&pfnWriteProjectData ) )
         {
         puts("Incorrect WorkFrame DLL - " WKFDLL_PJIO );
         return( FALSE );
         }
      }
   else
      {
      puts("WorkFrame DLL not found - " WKFDLL_PJIO );
      return( FALSE );
      }
#endif

   /* Open project file for writing */
#ifdef USE_WKF_IMPLIB
   hProject = (*pfnOpenProjectFile)( hAB,
#else
   hProject =   WkfOpenProjectFile ( hAB,
#endif
                                     pszProjectFileName,
                                     WKF_ACCESS_ALL | WKF_SHARE_DENYWRITE );

   if ( hProject == NULLHANDLE )
      return( FALSE );


   /* ---------------------------------------------------------------------- */
   /* - create the key to be used for this action                            */
   /* ---------------------------------------------------------------------- */
   sprintf( szOptionsKey,
            WKF_OPTKEY_TEMPLATE,
            pszToolClass,
            pszToolName,
            pszToolScope );

   pOptions = MakeOptionsStruct( pszToolOptions );
   if ( pOptions != NULL )
      {
#ifdef USE_WKF_IMPLIB
      (*pfnWriteProjectData)( hProject,
#else
        WkfWriteProjectData ( hProject,
#endif
                              WKF_OPTIONS_APPNAME,
                              szOptionsKey,
                              (PVOID)pOptions,
                              pOptions->cb );
      free( pOptions );
      }

#ifdef USE_WKF_IMPLIB
   (*pfnCloseProjectFile)( hProject );
#else
     WkfCloseProjectFile ( hProject );
#endif

   return( TRUE );
   }

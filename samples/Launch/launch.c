/*+--------------------------------------------------------------------------+*/
/*|WF/2 Launching actions sample                                             |*/
/*|--------------------------------------------------------------------------|*/
/*|                                                                          |*/
/*| PROGRAM NAME: LAUNCH                                                     |*/
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
/*|  This program illustrates registering a non-PM program with WF/2 and     |*/
/*|  launching context sensitive actions. Also, this program does not        |*/
/*|  statically link to WF/2, and can be used where WF/2 support is optional.|*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/
#define INCL_ERRORS
#define INCL_DOS
#define INCL_WIN
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILES 10

extern BOOL _System WkfLaunchAction( PSZ   pszProject,
                                     PSZ   pszAction,
                                     PSZ * ppszFiles );

int main( int argc, char * argv[] )
   {
   char * ppFiles[MAX_FILES+1];
   char * pcProject;
   char * pcAction;
   int    i;

   if ( argc < 3 )
      {
      puts("<Project name> <action> <files...>");
      exit(255);
      }

   pcProject = argv[1];
   pcAction  = argv[2];
   for( i=3; i<argc && i<MAX_FILES; ++i )
      ppFiles[i-3] = argv[i];

   ppFiles[i-3] = NULL;

   if ( FALSE == WkfLaunchAction( pcProject, pcAction, ppFiles ) )
      puts("False returned" );
   return( 0 );
   }


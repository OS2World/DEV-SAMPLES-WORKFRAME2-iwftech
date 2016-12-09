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
/*|  This program illustrates creating a base project and setting options    |*/
/*|  for an action defined in the base project.                              |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/
#ifndef _PROJECT_H_
   #define _PROJECT_H_

   HOBJECT _Optlink projectCreate( PSZ pszTitle,
                                   PSZ pszLocation,
                                   PSZ pszObjectID,
                                   PSZ pszDIR,
                                   PSZ pszMASK,
                                   PSZ pszTARGETNAME,
                                   PSZ pszTARGETPATH,
                                   PSZ pszTARGETPARM,
                                   PSZ pszTARGETPROMPT,
                                   PSZ pszTARGETTYPE,
                                   PSZ pszMAKEFILE,
                                   PSZ pszMONITOR,
                                   PSZ pszFILEMENU,
                                   PSZ pszPROJECTMENU,
                                   PSZ pszOPENACTION,
                                   PSZ pszEXECACTION,
                                   PSZ pszPAM,
                                   PSZ pszPROFILE,
                                   PSZ pszPATHNAME_ADDR );
   #endif

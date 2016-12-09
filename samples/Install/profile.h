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
/*+--------------------------------------------------------------------------+*/
#ifndef _PROFILE_H_
   #define _PROFILE_H_

   /* ----------------------------------------------------------------------- */
   /* profileAdd()                                                            */
   /* ----------------------------------------------------------------------- */
   /*                                                                         */
   /* pszObject   - either the persistent object handle name                  */
   /*               (e.g. "<WKF_PROFILE_DFLT>") or the fully qualified        */
   /*               name of the actions profile                               */
   /*                                                                         */
   /* pszActionClass - Action class (e.g. "COMPILE", "BUILD",...)             */
   /*                                                                         */
   /* pszActionName  - Action name (e.g. "IBM C Set++"   )                    */
   /*                                                                         */
   /* pszProgram  - program name (e.g. "ICC.EXE")                             */
   /*                                                                         */
   /* pszSourceType - source file mask (e.g. "*.c\n*.cpp") or source type     */
   /*                 (e.g. "CSOURCE\nCPPSOURCE" - these must be predefined)  */
   /*                                                                         */
   /* pszTargetType - target file mask or target type                         */
   /*                                                                         */
   /* pszSupportMod - DLL containing the WF/2 action integration support      */
   /*                 functions (e.g. "DDE3DEF2")                             */
   /*                                                                         */
   /* pszSupportEntry - the stub name of the entry point containing the       */
   /*                   WF/2 action integration support function              */
   /*                   (e.g. "COMPILE")                                      */
   /*                                                                         */
   /* pszActionType must be one of the following:                             */
   /*                                                                         */
   /*    "WKF_ACTIONTYPE_CMD"   - action will be executed                     */
   /*                                                                         */
   /* pszActionScope must be one of the following strings:                    */
   /*                                                                         */
   /*    "WKF_ACTIONSCOPE_PROJECT" - action applies to projects only          */
   /*    "WKF_ACTIONSCOPE_FILE"    - action applies to files only             */
   /*    "WKF_ACTIONSCOPE_BOTH"    - action can apply to files and projects   */
   /*                                                                         */
   /*                                                                         */
   /* pszMenuOptions must be one of the following strings:                    */
   /*                                                                         */
   /*    "WKF_MENUSCOPE_DETAIL"    - action is shown only if the extended     */
   /*                                menu option is selected                  */
   /*    "WKF_MENUSCOPE_SHORT"     - action appears on all menus              */
   /*                                                                         */
   /*                                                                         */
   /* pszRunMode must be one of the following strings:                        */
   /*                                                                         */
   /*    "WKF_RUNMODE_FULLSCREEN"  - action should be started in full screen  */
   /*    "WKF_RUNMODE_WINDOW"      - action should be started in an AVIO win. */
   /*    "WKF_RUNMODE_MONITOR"     - action should run in a monitor           */
   /*    "WKF_RUNMODE_DEFAULT"     - let the PAM decide how to run program    */
   /*                                                                         */
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
                             PSZ pszRunMode );
   #endif

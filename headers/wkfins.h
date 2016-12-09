/****************************************************************************/
/* Integration Kit for IBM* WorkFrame/2* Version 2.1                        */
/*                                                                          */
/* The integration kit contains an information reference and sample         */
/* programs illustrating application programming interfaces to              */
/* provide assistance with product integration into IBM's                   */
/* WorkFrame/2 Version 2.1.                                                 */
/*                                                                          */
/* The information and sample programs are intended only for                */
/* Version 2.1, and are not applicable to any other IBM product or          */
/* product release.                                                         */
/*                                                                          */
/* The integration kit is provided as is, without any warranty of           */
/* merchantibility of fitness for any particular purpose. The kit           */
/* includes sample code. This code has not been tested, and IBM             */
/* makes no representation of its workability or fitness on any             */
/* system.                                                                  */
/*                                                                          */
/* You are permitted to make copies of the whole of this kit,               */
/* including this notice screen. You are also permitted to copy the         */
/* sample programs, and their source, contained in the kit for the          */
/* purpose of running those programs on your system.                        */
/*                                                                          */
/* You may distribute copies of the whole of this kit, or only of the       */
/* sample programs, in any country recognising the copyright of U.S.        */
/* nationals. Any copies distributed by you must include a copy of          */
/* this notice screen.                                                      */
/*                                                                          */
/* IBM will attempt to make future releases of the WorkFrame/2              */
/* product compatible, but cannot guarantee upward compatibility.           */
/*                                                                          */
/* COPYRIGHT: IBM WorkFrame/2 Version 2.1                                   */
/*            (C) COPYRIGHT IBM CORP 1990, 1991, 1992, 1993, 1994           */
/*            LICENSED MATERIALS - PROPERTY OF IBM                          */
/*            REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083         */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*                                                                          */
/* MODULE:    WKFINS.H - WorkFrame Support Installation interfaces          */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

#ifndef _WKFINS_H_
   #define _WKFINS_H_

   #define MAJ_VERKEY    "Major Version"
   #define MIN_VERKEY    "Minor Version"


   /* -------------------------------------------------------------------- */
   /* WkfQueryInstalledVersion: return the currently installed WF          */
   /* -------------------------------------------------------------------- */
   /*                                                                      */
   /* Parameters:                                                          */
   /* ----------                                                           */
   /* PSZ   pszDirectory - pointer to the buffer containing the            */
   /*                      the fully qualified path name of the WF's       */
   /*                      installation directory. This buffer should be   */
   /*                      at least CCHMAXPATH large to ensure success     */
   /* ULONG ulDirectory  - size of the above buffer                        */
   /* ULONG * pulMajorVersion - major version number returned              */
   /* ULONG * pulMinorVersion - minor version number returned              */
   /* BOOL  OnlyVersion1 - Set to TRUE, if you want to only look for       */
   /*                      the location of WF version 1.                   */
   /*                      If set to FALSE, then the most up to date       */
   /*                      version of the WF is located.                   */
   /*                                                                      */
   /* Returns:                                                             */
   /* -------                                                              */
   /* BOOL  TRUE  if WF found                                              */
   /*       FALSE if requested WF version not found                        */
   /*                                                                      */
   /* NOTE:                                                                */
   /* ----                                                                 */
   /* This function is not shipped in any of WF's DLLs.  The object module */
   /* is statically available in wf21.lib.  Hence, this function can be    */
   /* called whether WF is installed or not.                               */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   extern BOOL APIENTRY WkfQueryInstalledVersion( PSZ  pszDirectory,
                                          ULONG  ulDirectory,
                                          PULONG pulMajorVersion,
                                          PULONG pulMinorVersion,
                                          BOOL   OnlyVersion1 );


   /* -------------------------------------------------------------------- */
   /* Macros used for setting default options when using DDE3DEF2.DLL      */
   /* as the tool support dll                                              */
   /* -------------------------------------------------------------------- */
   #define WKFDEF2_APPNAME_TEMPLATE   "%s::%s"  /* Action_Class::EXE_name  */

   #define WKFDEF2_DEFAULTS "Wkf.Defaults"

   #define WKFDEF2_PARMS    "Wkf.CmdLine"       /* command line parameters */

   #define WKFDEF2_ERRTEMPL "Wkf.Errors"        /* error line template     */

   #define WKFDEF2_PROMPT   "Wkf.Prompt"        /* prompt required flag    */
   #define WKFDEF2_PROMPT_YES "1"
   #define WKFDEF2_PROMPT_NO  "0"

   #define WKFDEF2_EDITDDE  "Wkf.EditDDE"       /* tool supports editor DDE*/
   #define WKFDEF2_EDITDDE_YES "1"
   #define WKFDEF2_EDITDDE_NO  "0"

   #define WKFDEF2_FILE "%s\\DDE3DEF2.INI"

   /* -------------------------------------------------------------------- */
   /* WkfSetDefaultOption: set default options when using DDE3DEF2         */
   /* -------------------------------------------------------------------- */
   /*                                                                      */
   /* Parameters:                                                          */
   /* ----------                                                           */
   /*   HAB     hAB               - caller's anchor block or NULLHANDLE;   */
   /*                               if NULLHANDLE is passed, then this     */
   /*                               function will create its own hAB       */
   /*   PSZ     pszActionClass    - action class of the tool, e.g. "EDIT"  */
   /*   PSZ     pszEXEName        - UNQUALIFIED program file name (no ext.)*/
   /*                               e.g. "EPM"                             */
   /*   PSZ     pszParameters     - parameters to the program,             */
   /*                               e.g. "/W %a %z"                        */
   /*   PSZ     pszErrorTemplate  - error template (see default dialog     */
   /*                               for details)                           */
   /*   PSZ     pszPromptOnRun    - set to WKFDEF2_PROMPT_YES to prompt    */
   /*                               the user for parameters prior to tool  */
   /*                               invocation                             */
   /*                             - set to WKFDEF2_PROMPT_NO to invoke     */
   /*                               the the tool without promption         */
   /*   PSZ     pszSupportDDE     - set to WKFDEF2_EDITDDE_YES if this     */
   /*                               tool support WF DDE protocols          */
   /*                             - set to WKFDEF2_EDITDDE_NO for non      */
   /*                               EDIT class tools or for EDIT class     */
   /*                               tools which do not support WF DDE      */
   /*                                                                      */
   /* Returns:                                                             */
   /* -------                                                              */
   /* BOOL  TRUE  if successfully set                                      */
   /*       FALSE if unable to set the default options                     */
   /*                                                                      */
   /* NOTE:                                                                */
   /* ----                                                                 */
   /* This function is not shipped in any of WF's DLLs.  The object module */
   /* is statically available in wf21.lib.                                 */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   extern BOOL APIENTRY WkfSetDefaultOption( HAB hAB,
                                             PSZ pszActionClass,
                                             PSZ pszEXEName,
                                             PSZ pszParameters,
                                             PSZ pszErrorTemplate,
                                             PSZ pszPromptOnRun,
                                             PSZ pszSupportDDE );

#endif

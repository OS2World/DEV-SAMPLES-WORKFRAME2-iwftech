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
/* MODULE:    WKFOPT.H - Option Support DLL interfaces                      */
/*                                                                          */
/****************************************************************************/

#ifndef _WKFOPT_H_
   #define _WKFOPT_H_

   /* --------------------------------------------------------------------- */
   /* Version 1 API names for external tools support                        */
   /* --------------------------------------------------------------------- */
   #define WKF_ACTV1_COMPFUNCTIONNAME     "GETCOMPILEOPTS"
   #define WKF_ACTV1_LINKFUNCTIONNAME     "GETLINKOPTS"
   #define WKF_ACTV1_MAKEFUNCTIONNAME     "GETMAKEOPTS"
   #define WKF_ACTV1_DEBUGFUNCTIONNAME    "GETDEBUGOPTS"

   /* --------------------------------------------------------------------- */
   /* Version 1 API names for external compiler tools support               */
   /* --------------------------------------------------------------------- */
   #define WKF_ACTV1_PARSEFUNCTIONNAME    "PARSEERROR"
   #define WKF_ACTV1_INCLUDEFUNCTIONNAME  "FINDINCLUDE"
   #define WKF_ACTV1_FREEMEMFUNCTIONNAME  "FREEMEMORY"
   #define WKF_ACTV1_QUERYHELPFILE        "QUERYHELPFILE"
   #define WKF_ACTV1_QUERYRESOURCEID      "QUERYRESOURCEID"

   /* --------------------------------------------------------------------- */
   /* Version 2 API names for external tools support                        */
   /* --------------------------------------------------------------------- */
   #define WKF_ACT_PREFIX                 "WKF"
   #define WKF_ACT_PREFIX2                "QUERY"
   #define WKF_ACT_OPTIONS                "OPTIONS"
   #define WKF_ACT_TARGET                 "TARGET"
   #define WKF_ACT_RESOURCEID             "RESOURCEID"
   #define WKF_ACT_HELPFILE               "HELPFILE"
   #define WKF_ACT_TERMINATE              "TERMINATE"
   #define WKF_ACT_PARSEERROR             "PARSEERROR"

   /* --------------------------------------------------------------------- */
   /* Version 2 dynamic API name masks for external tools support           */
   /* --------------------------------------------------------------------- */
   #define WKF_MASK_QUERYOPT              "WKFQUERY%sOPTIONS"
   #define WKF_MASK_QUERYTARGET           "WKFQUERY%sTARGET"
   #define WKF_MASK_RESOURCEID            "WKFQUERY%sRESOURCEID"
   #define WKF_MASK_RESOURCEID2           "WKFQUERY%sRESOURCEID2"
   #define WKF_MASK_HELPFILE              "WKFQUERY%sHELPFILE"
   #define WKF_MASK_HELPFILE2             "WKFQUERY%sHELPFILE2"
   #define WKF_MASK_TERMINATE             "WKF%sTERMINATE"
   #define WKF_MASK_PARSEERROR            "WKF%sPARSEERROR"
   #define WKF_MASK_QUERYDDE              "WKF%sQUERYDDE"

   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* WKF_REALLOC -                                                          */
   /*                                                                        */
   /* Pointer to a callback function which reallocates the buffer passed in. */
   /* If successful, returns a pointer to the new buffer else it returns     */
   /* NULL.                                                                  */
   /* ---------------------------------------------------------------------- */
   typedef PVOID APIENTRY WKF_REALLOC( PVOID pvBuffer, ULONG ulNewSize );
   typedef WKF_REALLOC * PWKF_REALLOC;



   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* WKF_PRT_STATUS -                                                       */
   /*                                                                        */
   /* Pointer to a callback function which prints a status message and       */
   /* returns. Use when processing takes some time to complete. Pass only    */
   /* a simple text string -- no formatting will be done.                    */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   typedef VOID APIENTRY WKF_PRT_STATUS( PSZ pszStatusMsg );
   typedef WKF_PRT_STATUS * PWKF_PRT_STATUS;



   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* WkfQuery...Options -                                                   */
   /*                                                                        */
   /*       The actual entry point called will be named as above, with the   */
   /*       elipsis replaced by the tool class name, and the entire name     */
   /*       upper cased.  For example, when WorkFrame/2 needs to call this   */
   /*       function for a compiler, it will call WKFQUERYCOMPILEOPTIONS().  */
   /*       Any spaces in the class name will be converted to underscores.   */
   /*                                                                        */
   /* Optionally display a dialog and return the user selected options to    */
   /* be saved with the project file.                                        */
   /*                                                                        */
   /* If the pvOptions buffer is too small, then call pWkf_Realloc           */
   /* functions with the pvOptions buffer and the new buffer size required.  */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* rc = 0  no errors save file                                            */
   /*      1  no errors don't save file                                      */
   /*      2  CANCEL pressed                                                 */
   /*      8  ERROR (DLL displayed an error message)                         */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* Parameters:  Passed to DLL (Input)I/O(Output) Returned from DLL        */
   /*                                                                        */
   /*      HWND         hwndWorkFrame   -  I/   WF/2 frame handle            */
   /*                                           else HWND_DESKTOP if frame   */
   /*                                           handle is not available      */
   /*      HMODULE      hModHandle      -  I/   DLL module handle            */
   /*      PSZ          pszProject      -  I/   project file name            */
   /*      PSZ          pszTitle        -  I/   title to display             */
   /*      PWKF_ACTION  pwkfAction      -  I/   pointer to action structure  */
   /*      ULONG        ulSetting       -  I/   0x01 if default to be used   */
   /*      PULONG       pulOptionsLength-  I/O  initial buffer size (bytes)  */
   /*                                           on exit contains the actual  */
   /*                                           bytes of data                */
   /*      PVOID        pvOptions       -  I/O  options buffer               */
   /*      PWKF_REALLOC pWkf_Realloc    -  I/   realloc function             */
   /*      BOOL         bFileScope      -  I/   TRUE if invoked for file     */
   /*                                           file scoped actions          */
   /* ---------------------------------------------------------------------- */

   #define WKF_NOERROR    0
   #define WKF_DONTSAVE   1
   #define WKF_CANCEL     2
   #define WKF_ERROR      8

   #define WKF_QO_SETTING_USE_OPTIONS  0x00
   #define WKF_QO_SETTING_USE_DEFAULT  0x01

   typedef ULONG APIENTRY WKF_QUERY_OPTIONS( HWND          hwndWorkFrame,
                                             HMODULE       hModHandle,
                                             PSZ           pszProject,
                                             PSZ           pszTitle,
                                             PWKF_ACTION   pwkfAction,
                                             ULONG         ulSetting,
                                             PULONG        pulOptionsLength,
                                             PVOID         pvOptions,
                                             PWKF_REALLOC  pWkf_Realloc,
                                             BOOL          bFileScope );
   typedef WKF_QUERY_OPTIONS * PWKF_QUERY_OPTIONS;



   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* WkfQuery...Target -                                                    */
   /*                                                                        */
   /*       The actual entry point called will be named as above, with the   */
   /*       elipsis replaced by the tool class name, and the entire name     */
   /*       upper cased.  For example, when WorkFrame/2 needs to call this   */
   /*       function for a compiler, it will call WKFQUERYCOMPILETARGET().   */
   /*       Any spaces in the class name will be converted to underscores.   */
   /*                                                                        */
   /* Given a list of valid source files selected by the user, and options   */
   /* set by the user, return the following:                                 */
   /*                                                                        */
   /*    1. the command line parameters to be used by this action            */
   /*    2. from the list of input files, mark those which will be utilized  */
   /*       by this action, and those which should be discarded              */
   /*    3. if the target objects list pointer is not NULL, then return a    */
   /*       list of output files generated by this action.  Note: This list  */
   /*       should only include files created by a single invocation of the  */
   /*       action. For example, a LINK action may create .EXE and .MAP      */
   /*       files.                                                           */
   /*    4. if the dependency objects list pointer is not NULL, then return  */
   /*       a list of files which the action of building the TARGET file     */
   /*       depends on (excluding those marked in the list of source files). */
   /*       This list should be complete -- files returned will not be       */
   /*       parsed further by WorkFrame for nested includes.                 */
   /*                                                                        */
   /* NOTE: a) *ppvToolBuffer is a pvoid pointer which can be used by the    */
   /*          called DLL entry to store tool class and tool name specific   */
   /*          information across subsequent calls.  For instance,           */
   /*          information about imbedded dependency files can be saved      */
   /*          so that they would not have to be re-created on each call.    */
   /*          Subsequent calls to this function for the same tool class and */
   /*          name will be passed this value.  If at the end of this        */
   /*          process (e.g. creating make files), the pointer is not NULL,  */
   /*          the function WkfTerminate  will be called to cleanup the      */
   /*          memory used.                                                  */
   /*                                                                        */
   /*       b) cBuffer in both the list of target objects and dependency     */
   /*          objects list must have the form:                              */
   /*                                                                        */
   /*               (filename_1)(filename_2)...(filename_n)\0                */
   /*                                                                        */
   /*          where each filename_x is a null delimited string              */
   /*                                                                        */
   /*       c) ulBufferSize is the number bytes actually allocated to        */
   /*          cBuffer only.                                                 */
   /*                                                                        */
   /*       d) If any one of the output buffers are too small, then call     */
   /*          pWkf_Realloc function  with the field to be adjusted and the  */
   /*          new buffer size required in bytes.                            */
   /*                                                                        */
   /*          The field to be adjust must be one of the following:          */
   /*                                                                        */
   /*             WKF_REALLOC_TARGET                                         */
   /*             WKF_REALLOC_DEPEND                                         */
   /*             WKF_REALLOC_CMDLINE                                        */
   /*                                                                        */
   /*          For the target and depend fields, you do not need to account  */
   /*          for the 2 ULONG fields.  The bytes required should be for     */
   /*          the buffer itself only.                                       */
   /*                                                                        */
   /*       e) If processing takes a significant period of time, the         */
   /*          WKF_PRT_STATUS function should be called periodically to      */
   /*          provide ongoing status information to the user.  If           */
   /*          processing time is very small, use of this function is not    */
   /*          necessary.  Do not use this to report errors - the message    */
   /*          displayed will be temporary.exit                              */
   /*                                                                        */
   /*       f) The CmdLine returned can contain several special characters.  */
   /*          These provide extra flexability where necessary, but in       */
   /*          general should be avoided if possible.                        */
   /*                                                                        */
   /*            WKF_CMDLINE_EXECUTE    - The string returned is normally    */
   /*                                     taken as the parameters of the     */
   /*                                     command to be executed.  If the    */
   /*                                     first characters of the string are */
   /*                                     WKF_CMDLINE_EXECUTE, then the next */
   /*                                     word represents the command to     */
   /*                                     execute, and the rest of the       */
   /*                                     string represents the parameters   */
   /*                                     to be used.  This allows a command */
   /*                                     other than what the user entered   */
   /*                                     in the actions profile to be       */
   /*                                     executed.                          */
   /*                                                                        */
   /*            WKF_CMDLINE_INLINEFILE - WKF_CMDLINE_INLINEFILE must always */
   /*                                     be included in the command string  */
   /*                                     in pairs.  The characters will be  */
   /*                                     removed and replaced by a          */
   /*                                     temporary filename, and all the    */
   /*                                     characters in between will be      */
   /*                                     placed in this file (verbatum - no */
   /*                                     processing will be done on these   */
   /*                                     characters).  The temp file will   */
   /*                                     be deleted after the command has   */
   /*                                     completed execution.  This can be  */
   /*                                     used to execute a command via a    */
   /*                                     response file, and should be used  */
   /*                                     if the output length exceeds 1024  */
   /*                                     characters (which is the most      */
   /*                                     cmd.exe will accept, and thus the  */
   /*                                     most that can be placed in a       */
   /*                                     makefile)                          */
   /*                                                                        */
   /*            WKF_CMDLINE_NEXTCMD    - WKF_CMDLINE_NEXTCMD is used to     */
   /*                                     indicate that multiple commands    */
   /*                                     should be executed.  Unless other  */
   /*                                     special characters are used, each  */
   /*                                     "mini" command line between        */
   /*                                     WKF_CMDLINE_NEXTCMD's will be      */
   /*                                     interpreted exactly as normal -    */
   /*                                     that is, as parameters to the      */
   /*                                     command being executed.  Although  */
   /*                                     the commands will be executed      */
   /*                                     sequentially, they will be treated */
   /*                                     as one command.  For example, the  */
   /*                                     list of targets generated will     */
   /*                                     apply to the command sequence,     */
   /*                                     rather than any single command.    */
   /*                                     This capability should only be     */
   /*                                     used when no alternatives exist.   */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* rc = 0  no error                                                       */
   /*     !0  ERROR (DLL displayed an error message - abort processing)      */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   /* Parameters:   Passed to DLL (Input)I/O(Output) Returned from DLL       */
   /*                                                                        */
   /*      PVOID          * ppvToolBuffer    -  I/   Buffer created by the   */
   /*                                                tool DLL                */
   /*      HWND             hwndWorkFrame    -  I/   WF/2 frame handle       */
   /*                                                else HWND_DESKTOP if    */
   /*                                                frame handle is not     */
   /*                                                available               */
   /*      HMODULE          hModHandle       -  I/   DLL module handle       */
   /*      PSZ              pszProject       -  I/   project file name       */
   /*      PSZ              pszTitle         -  I/   project title           */
   /*      PWKF_ACTION      pwkfAction       -  I/   pointer to action struct*/
   /*      ULONG            ulOptionsLength  -  I/   options buffer size in  */
   /*                                                bytes                   */
   /*      PVOID            pvOptions        -  I/   options buffer          */
   /*      ULONG            ulCmdLineLength  -  I/   initial size of CmdLine */
   /*                                                buffer                  */
   /*      PSZ              pszCmdLine       -   /O  output command line     */
   /*      ULONG            ulSearchPath     -  I/   initial size of         */
   /*                                                SearchPath buffer       */
   /*      PSZ              pszSearchPath    -  I/O  address of the buffer   */
   /*                                                containing the search   */
   /*                                                path                    */
   /*      LIST_OF_SOURCE  *pstSourceObjects -  I/O  list of selected source */
   /*                                                files                   */
   /*      LIST_OF_TARGET  *pstTargetObjects -  I/   first entry contains    */
   /*                                                a new line delimited    */
   /*                                                string containing the   */
   /*                                                list of valid target    */
   /*                                                file masks (read from   */
   /*                                                the Actions Profile)    */
   /*                                            /O  list of output files    */
   /*      LIST_OF_DEPEND  *pstDependObjects -   /O  list of dependencies    */
   /*      PWKF_REALLOC     pWkf_Realloc     -  I/   realloc function        */
   /*      PWKF_PRT_STATUS  pWkf_PrtStatus   -  I/   print status function   */
   /*      BOOL             bFileScope       -  I/   TRUE if invoked for file*/
   /*                                                file scoped actions     */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */

   /* the following are the possible flags for the fUsed field of            */
   /* SOURCE_ENTRY                                                           */
   #define WKF_QT_FILE_NOTUSED 0   /* file not processed                     */
   #define WKF_QT_FILE_USED    1   /* file processed and required for command*/
   #define WKF_QT_FILE_DISCARD 2   /* file processed and to be discarded     */

   #define WKF_CMDLINE_EXECUTE    "\xFF\x01"
   #define WKF_CMDLINE_INLINEFILE "\xFF\x02"
   #define WKF_CMDLINE_NEXTCMD    "\xFF\x03"

   typedef _Packed struct _SOURCE_ENTRY       /* single source file entry    */
           {
           ULONG  fUsed;                      /* file used flag              */
           PSZ    pszFileName;                /* pointer to filename string  */
           } SOURCE_ENTRY, *pSOURCE_ENTRY;

   typedef _Packed struct _LIST_OF_SOURCE     /* source files list structure */
           {
           ULONG         ulNumOfEntries;      /* number of selected files    */
           ULONG         ulNumUsed;           /* number of files used        */
           SOURCE_ENTRY  stObject[1];         /* array of source structures  */
           } LIST_OF_SOURCE, *pLIST_OF_SOURCE;

   typedef _Packed struct _LIST_OF_TARGET     /* target files list structure */
           {
           ULONG ulNumOfEntries;              /* number of entries in the buf*/
           ULONG ulBufferSize;                /* size of cBuffer             */
           char  cBuffer[1];                  /* list of files produced      */
           } LIST_OF_TARGET, *pLIST_OF_TARGET;

   typedef _Packed struct _LIST_OF_DEPEND     /* dependency files list struc */
           {
           ULONG ulNumOfEntries;              /* number of entries in the buf*/
           ULONG ulBufferSize;                /* size of cBuffer             */
           char  cBuffer[1];                  /* list of dependency files    */
           } LIST_OF_DEPEND, *pLIST_OF_DEPEND;


   typedef APIRET APIENTRY WKF_QUERY_TARGET( PVOID           *ppvToolBuffer,
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
                                             BOOL             bFileScope );
   typedef WKF_QUERY_TARGET * PWKF_QUERY_TARGET;


   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* WKF...TERMINATE                                                        */
   /*                                                                        */
   /* If on a call to WkfQuery...Target function *ppvToolBuffer was set      */
   /* to a non-NULL value, at the end of the process (e.g. creating make     */
   /* files), this function will be called with the saved pointer value.     */
   /* This function is expected to free any memory it has allocated and/or   */
   /* display any saved error/warning messages.  Note that error messages    */
   /* should only be displayed if the fDisplayMessages flag is set.  Any     */
   /* aloocated memory should always be freed.                               */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* Parameters:      Passed to DLL (Input)I/O(Output) Returned from DLL    */
   /*                                                                        */
   /*      PVOID        pvToolBuffer     -  I/   Buffer created by tool DLL  */
   /*      BOOL         fDisplayMessages -  I/   Should msgs be displayed?   */
   /*      HWND         hwndWorkFrame    -  I/   WF/2 frame handle           */
   /*                                            else HWND_DESKTOP if        */
   /*                                            frame handle is not         */
   /*                                            available                   */
   /*      HMODULE      hModHandle       -  I/   DLL module handle           */
   /*      PSZ          pszTitle         -  I/   project title               */
   /*      PWKF_ACTION  pwkfAction       -  I/   pointer to action struct    */
   /*                                                                        */
   /* Returns:                                                               */
   /*                                                                        */
   /* rc = 0  no error                                                       */
   /*     !0  ERROR (DLL displayed an error message - abort processing)      */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */

   typedef APIRET APIENTRY WKF_TERMINATE( PVOID        pvToolBuffer,
                                          BOOL         fDisplayMessages,
                                          HWND         hwndWorkFrame,
                                          HMODULE      hModHandle,
                                          PSZ          pszTitle,
                                          PWKF_ACTION  pwkfAction );
   typedef WKF_TERMINATE * PWKF_TERMINATE;




   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* Wkf...ParseError                                                       */
   /*                                                                        */
   /*       The actual entry point called will be named as above, with the   */
   /*       elipsis replaced by the tool class name, and the entire name     */
   /*       upper cased.  For example, when WorkFrame/2 needs to call this   */
   /*       function for a compiler, it will call WKFCOMPILEPARSEERROR().    */
   /*       Any spaces in the class name will be converted to underscores.   */
   /*                                                                        */
   /* This entry point in the tool DLL is invoked during processing of the   */
   /* error messages output from the tool.  The interface is provided with   */
   /* a line of output from the tool and is expected to return the filename, */
   /* line number, and column number associated with the error message on    */
   /* that line.                                                             */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* rc = 0  no error                                                       */
   /*      1  error                                                          */
   /*      2  line invalid or not recognized                                 */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* Parameters:  Passed to DLL (Input)I/O(Output) Returned from DLL        */
   /*                                                                        */
   /*      HWND         hwndErrorBox    -  I/   window handle of the error   */
   /*                                           message listbox              */
   /*      PSZ          pszProject      -  I/   project file name            */
   /*      PUSHORT      pusStartLine    -   /O  pointer to first line        */
   /*                                           containing an error message  */
   /*                                           in listbox                   */
   /*      PUSHORT      pusEndLine      -   /O  pointer to last line         */
   /*                                           containing an error message  */
   /*      HWND         hwndFilenames   -  I/   window handle of listbox to  */
   /*                                           insert filenames into        */
   /*      PUSHORT      pusCurrentFile  -   /O  index of selected filename in*/
   /*                                           filename listbox             */
   /*      PVOID        pvOptions       -  I/   pointer to area containing   */
   /*                                           compiler options             */
   /*      PULONG       pulSetting      -  I/   validity of options          */
   /*                                           (initialized or not)         */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */

   #define WKFRC_PARSEERROR_SUCCESS  0
   #define WKFRC_PARSEERROR_ERROR    1
   #define WKFRC_PARSEERROR_INVALID  2

   #define WKF_SETTING_USE_OPTIONS  0x00
   #define WKF_SETTING_USE_DEFAULT  0x01

   typedef ULONG APIENTRY WKF_PARSEERROR( HWND    hwndErrorBox,
                                          PSZ     pszProject,
                                          PUSHORT pusStartLine,
                                          PUSHORT pusEndLine,
                                          HWND    hwndFilenames,
                                          PUSHORT pusCurrentFile,
                                          PVOID   pvOptions,
                                          PULONG  pulSetting );
   typedef WKF_PARSEERROR * PWKF_PARSEERROR;


   typedef  union  _WKF_TAGPARAM
            {
            unsigned long  tagParam ;
            struct {
                   USHORT           errorLine  ;
                   unsigned char    offset ;
                   char             fileNumber ;
                   } sub  ;
            } WKF_TAGPARAM;




   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* WkfQuery...HelpFile                                                    */
   /*                                                                        */
   /*       The actual entry point called will be named as above, with the   */
   /*       ellipsis replaced by the tool class name, and the entire name    */
   /*       upper cased.  For example, when WorkFrame/2 needs to call this   */
   /*       function for a compiler, it will call WKFQUERYCOMPILEHELPFILE(). */
   /*       Any spaces in the class name will be converted to underscores.   */
   /*                                                                        */
   /*                                                                        */
   /*  This interface obtains the error message help file. The WorkFrame/2   */
   /*  will invoke this interface after invoking the DLL to parse the error  */
   /*  message.  The interface is expected to place the string representing  */
   /*  the IPF .HLP file in the area provided and return TRUE (1) if help    */
   /*  is supported.  Returning FALSE (0) indicates that error message help  */
   /*  is not supported.                                                     */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* rc = FALSE   no help file support                                      */
   /*      TRUE    help file returned in pszHelpFile                         */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* Parameters:  Passed to DLL (Input)I/O(Output) Returned from DLL        */
   /*                                                                        */
   /*      HWND         hwndErrorBox    -  I/   Window handle of error       */
   /*                                           message listbox              */
   /*      PSZ          pszProject      -  I/   Name of project file         */
   /*      PSZ          pszHelpFile     -   /O  IPF help (.HLP) file name    */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */

   typedef BOOL APIENTRY WKF_QUERYHELPFILE(PSZ pszHelpFile);
   typedef WKF_QUERYHELPFILE * PWKF_QUERYHELPFILE;

   typedef ULONG APIENTRY WKF_QUERYHELPFILE2( HWND    hwndErrorBox,
                                              PSZ     pszProject,
                                              PSZ     pszHelpFile);
   typedef WKF_QUERYHELPFILE2 * PWKF_QUERYHELPFILE2;



   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* WkfQuery...ResourceID                                                  */
   /*                                                                        */
   /*      The actual entry point called will be named as above, with the    */
   /*      ellipsis replaced by the tool class name, and the entire name     */
   /*      upper cased.  For example, when WorkFrame/2 needs to call this    */
   /*      function for a compiler, it will call WKFQUERYCOMPILERESOURCEID().*/
   /*      Any spaces in the class name will be converted to underscores.    */
   /*                                                                        */
   /*  This interface is invoked whenever a resource id corresponding to a   */
   /*  particular error message is required.  The function must parse the    */
   /*  error message string and return the corresponding resource id within  */
   /*  the help file (obtained through the WkfQuery...HelpFile).             */
   /*  All resource ids must be greater than 20000 since values lower than   */
   /*  that are reserved for use by the editor.                              */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* Returns:                                                               */
   /*                                                                        */
   /* ULONG   0       no corresponding resource id                           */
   /*         >20000  help file resource id for the error message passed     */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* Parameters:  Passed to DLL (Input)I/O(Output) Returned from DLL        */
   /*                                                                        */
   /*      PSZ          pszErrorLine    -  I/   Error message for which help */
   /*                                           was requested                */
   /* ---------------------------------------------------------------------- */

   typedef ULONG APIENTRY WKF_QUERYRESOURCEID(PSZ pszErrorLine);
   typedef WKF_QUERYRESOURCEID * PWKF_QUERYRESOURCEID;

   typedef ULONG APIENTRY WKF_QUERYRESOURCEID2( HWND    hwndErrorBox,
                                                PSZ     pszProject,
                                                PSZ     pszErrorLine);
   typedef WKF_QUERYRESOURCEID2 * PWKF_QUERYRESOURCEID2;

   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* WkfQuery...DDE                                                         */
   /*                                                                        */
   /*  This interface is invoked to determine if a DDE session can be started*/
   /*  with a tool.  Currently this interface is supported only for EDIT     */
   /*  class tools. DDE sessions will be started with editors by the         */
   /*  WorkFrame/2 monitor when a file is selected for editing by selecting  */
   /*  an error message.                                                     */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /* Returns:                                                               */
   /*                                                                        */
   /*  BOOL                                                                  */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   /*                                                                        */
   /*                                                                        */
   /* Parameters:  Passed to DLL (Input)I/O(Output) Returned from DLL        */
   /*                                                                        */
   /*      VOID                                                              */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */

   typedef BOOL APIENTRY WKF_QUERYDDE( PVOID pvOptions );
   typedef WKF_QUERYDDE * PWKF_QUERYDDE;

#endif

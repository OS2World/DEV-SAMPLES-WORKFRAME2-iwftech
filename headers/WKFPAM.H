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
/* MODULE:    WKFPAM.H - Project Access Method interfaces                   */
/*                                                                          */
/****************************************************************************/

#ifndef _WKFPAM_H_
   #define _WKFPAM_H_

   /* -------------------------------------------------------------------- */
   /* WKFPAM_CCHMAXERROR - maximum error message string length which       */
   /*                      can be returned from a PAM function             */
   /* -------------------------------------------------------------------- */
   #define WKFPAM_CCHMAXERROR 255

   /* -------------------------------------------------------------------- */
   /* APIRET values from PAM functions                                     */
   /*                                                                      */
   /* WKFPAM_ERROR  - an error has occured, and an error message string    */
   /*                 may have been returned                               */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   #define WKFPAM_ERROR     ERROR_USER_DEFINED_BASE

   /* -------------------------------------------------------------------- */
   /*                                                                      */
   /* WKFPAM_PART                                                          */
   /*      This typedef represents a single element in the list of project */
   /*      parts passed back to the WKF_ADDFILES callback function.        */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   typedef _Packed struct _WKFPAM_PART
           {
           ULONG          cb;            /* The size of this element, which  */
                                         /* includes the FILEFINDBUF3 and    */
                                         /* other string data all of which   */
                                         /* contain variable length strings  */

           HPOINTER       hIcon;         /* A handle to a copy of an icon for*/
                                         /* the file -- does not need to be  */
                                         /* freed                            */

           PSZ            pszSystemData; /* Points to a string, contained    */
                                         /* within this structure; contains  */
                                         /* any spcecial data that the PAM   */
                                         /* wishes to display about the file.*/
                                         /* for example, a library system PAM*/
                                         /* may display information about    */
                                         /* the status of the file           */

           FILEFINDBUF3   FileFind;      /* The oNextEntryOffset field of    */
                                         /* this structure must be set to    */
                                         /* indicate the offset to the next  */
                                         /* FILEFINDBUF3 struct (if there is */
                                         /* one, else zero)                  */
           } WKFPAM_PART;

           typedef WKFPAM_PART *PWKFPAM_PART;


   /* -------------------------------------------------------------------- */
   /*                                                                      */
   /* WKFPAM_NEXTPART(p)                                                   */
   /*           This macro can be used to move from one part to the next   */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   #define WKFPAM_NEXTPART(p) (PWKFPAM_PART)((PCHAR)p+p->cb)


   /* -------------------------------------------------------------------- */
   /*                                                                      */
   /* HAM  - project PAM handle                                            */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   typedef LHANDLE HAM, *PHAM;


   /* -------------------------------------------------------------------- */
   /* WKF_ADDFILES                                                         */
   /* -------------------------------------------------------------------- */
   /* callback function to add a file to the files container of a base     */
   /* project                                                              */
   /*                                                                      */
   /* Parameters : PVOID   pvPrivateData  (I) pointer to caller specific   */
   /*                                         data                         */
   /*              PSZ     pszContainer   (I) path name                    */
   /*              ULONG   ulCount        (I) number of WKFPAM_PART entries*/
   /*                                         in pwppData                  */
   /*              PWKFPAM_PART pwppData  (I) start of data block          */
   /*                                         containing WKFPAM_PART list  */
   /*                                                                      */
   /* Returns :    NO_ERROR       - items added successfully               */
   /*              other          - OS/2 return code                       */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   typedef APIRET (APIENTRY WKF_ADDFILES)( PVOID        pvPrivateData,
                                           PSZ          pszContainer,
                                           ULONG        ulCount,
                                           PWKFPAM_PART pwppData );
   typedef WKF_ADDFILES * PWKF_ADDFILES;

   /* -------------------------------------------------------------------- */
   /* WKF_ADDMONITOR                                                       */
   /* -------------------------------------------------------------------- */
   /* callback function for adding output lines to the monitor listbox     */
   /*                                                                      */
   /* Parameters : PVOID   pvPrivateData  (I) pointer to caller specific   */
   /*                                         data                         */
   /*              PSZ     pszLine        (I) string to be displayed       */
   /*                                                                      */
   /* Returns :    NO_ERROR       - line added successfully                */
   /*              other          - OS/2 return code                       */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   typedef APIRET (APIENTRY WKF_ADDMONITOR)( PVOID   pvPrivateData,
                                             PSZ     pszLine );
   typedef WKF_ADDMONITOR * PWKF_ADDMONITOR;

   /* -------------------------------------------------------------------- */
   /* WkfQueryHAM                                                          */
   /* -------------------------------------------------------------------- */
   /* PAM provided function for returning a caller unique HAM value.       */
   /* This ham must be unique to each caller of this function.             */
   /* If hamOld is not NULL, then any objects associated with this ham     */
   /* are lost.                                                            */
   /*                                                                      */
   /* Parameters : HAM     hamOld         (I)  previously set ham value    */
   /*                                          or NULL                     */
   /*              PSZ     pszProjectName (I)  new project name to be      */
   /*                                          assigned or NULL            */
   /*                                                                      */
   /* Returns :    NULL   if pszProjectName was NULL, then all data        */
   /*                     associated with hamOld have been reset.          */
   /*                     if pszProjectName was not NULL, then the         */
   /*                     PAM failed.                                      */
   /*              !NULL  a unique ham value; if hamOld was not NULL       */
   /*                     then any data associated with hamOld has been    */
   /*                     reset.                                           */
   /* -------------------------------------------------------------------- */
   #define WKF_PFS_QUERYHAM       "WKFQUERYHAM"
   typedef HAM (APIENTRY WKF_QUERYHAM)( HAM hamOld,
                                        PSZ pszProjectName );
   typedef WKF_QUERYHAM *PWKF_QUERYHAM;

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
   /*                                          containing the null-        */
   /*                                          delimited title string;     */
   /*                                          on error, a zero length     */
   /*                                          string should be returned.  */
   /*                                                                      */
   /* Returns :    APIRET  NO_ERROR  success                               */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   #define WKF_PFS_QUERYTITLE     "WKFQUERYTITLE"
   typedef APIRET (APIENTRY WKF_QUERYTITLE)( HAM     hamOwner,
                                             ULONG   ulTitleMax,
                                             PSZ     pszTitle );
   typedef WKF_QUERYTITLE * PWKF_QUERYTITLE;

   /* -------------------------------------------------------------------- */
   /* WkfSettings                                                          */
   /* -------------------------------------------------------------------- */
   /* This entry point is optional.  If present, the Access page of the    */
   /* project's Settings notebook will enable an Options pushbutton.  Upon */
   /* invocation, the function can display a dialog to acquire and store   */
   /* any PAM specific information.  If a dialog is displayed, then        */
   /* the parent and owner of the dialog should be set to those passed     */
   /* in.  The dialog itself should be modal using WinDlgBox.              */
   /* When writing data to the project file using the project APIs, it is  */
   /* recommended that this DLL module name be used for the application    */
   /* name key required by the WkfWriteProjectData API to reduce the       */
   /* chances of another application using the same application name and   */
   /* overwriting your data.                                               */
   /*                                                                      */
   /* Parameters : HAM     hamOwner       (I)  Project handle              */
   /*              PSZ     pszProjectFile (I)  Project INI file name       */
   /*              HWND    hwndParent     (I)  Parent window handle        */
   /*              HWND    hwndOwner      (I)  Owner window handle         */
   /*              ULONG   cbMessage      (I)  Size of pszMessage buffer   */
   /*              PSZ     pszMessage     (O)  Pointer to message buffer   */
   /*                                                                      */
   /* Returns :    APIRET  if return code is not 0, then an access method  */
   /*                      specific message may be returned through        */
   /*                      the pszMessage buffer.                          */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   #define WKF_PFS_SETTINGS       "WKFSETTINGS"
   typedef APIRET (APIENTRY WKF_SETTINGS)( HAM     hamOwner,
                                           PSZ     pszProjectFile,
                                           HWND    hwndParent,
                                           HWND    hwndOwner,
                                           ULONG   cbMessage,
                                           PSZ     pszMessage );
   typedef WKF_SETTINGS * PWKF_SETTINGS;


   /* -------------------------------------------------------------------- */
   /* WkfQueryFiles                                                        */
   /* -------------------------------------------------------------------- */
   /* PAM provided function for getting the list of files or parts that    */
   /* belong to a project.                                                 */
   /* On entry, cbMessage contains the number of bytes allocated           */
   /* for the pszMessageBuffer.  On exit, pszMessage buffer may contain    */
   /* a single null delimited string to be displayed to the user.          */
   /* The PAM function should check the length field prior to writing any  */
   /* data since a count of 0 may imply a NULL pointer.                    */
   /*                                                                      */
   /* Parameters : HAM     hamOwner       (I)  Project handle              */
   /*              PSZ     pszMasks       (I)  File masks                  */
   /*              PSZ     pszContainers  (I)  Path names to be searched   */
   /*              PWKF_ADDFILES pfnAddFiles (I) Callback function for     */
   /*                                          inserting file objects      */
   /*                                          into the project container  */
   /*              PVOID   pvPrivateData  (I)  Parameter to be passed back */
   /*                                          to the pfnAddFiles callback */
   /*                                          function                    */
   /*              ULONG   cbMessage      (I)  Size of message buffer      */
   /*              PSZ     pszMessage     (O)  Pointer to message buffer   */
   /*                                                                      */
   /* Returns :    APIRET  if return code is not 0, then an access method  */
   /*                      specific message may be returned through        */
   /*                      the pszMessage buffer.                          */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   #define WKF_PFS_QUERYFILES      "WKFQUERYFILES"
   typedef APIRET (APIENTRY WKF_QUERYFILES)( HAM     hamOwner,
                                             PSZ     pszMasks,
                                             PSZ     pszContainers,
                                             PWKF_ADDFILES pfnAddFiles,
                                             PVOID   pvPrivateData,
                                             ULONG   cbMessage,
                                             PSZ     pszMessage );
   typedef WKF_QUERYFILES * PWKF_QUERYFILES;


   /* -------------------------------------------------------------------- */
   /* WkfRun                                                               */
   /* -------------------------------------------------------------------- */
   /* PAM-provided function for executing an action.                       */
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
   /*              PSZ     pszStartDir    (I)  startup directory           */
   /*              PSZ     pszTermQueueName (I) termination queue name     */
   /*                                          to be used for returning    */
   /*                                          result codes as defined by  */
   /*                                          DosStartSession.            */
   /*              ULONG   ulRunMode      (I)  = 0, Send message           */
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
   /*                                          used if ulRunMode=3(Monitor)*/
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
   #define WKF_RUNMODE_FULLSCREEN WKF_RUN_FULLSCREEN
   #define WKF_RUNMODE_WINDOW     WKF_RUN_WINDOW
   #define WKF_RUNMODE_MONITOR    WKF_RUN_MONITOR
   #define WKF_RUNMODE_MESSAGE    11
   #define WKF_RUNMODE_DEFAULT    255

   #define WKF_PFS_RUN             "WKFRUN"
   typedef APIRET (APIENTRY WKF_RUN)( HAM     hamOwner,
                                      PSZ     pszTitle,
                                      PSZ     pszProgram,
                                      PSZ     pszParameters,
                                      CHAR  * pcEnvironment,
                                      PSZ     pszStartDir,
                                      PSZ     pszTermQueueName,
                                      ULONG   ulRunMode,
                                      PWKF_ADDMONITOR pfnAddMonitor,
                                      PVOID   pvPrivateData,
                                      ULONG   cbMessage,
                                      PSZ     pszMessage );
   typedef WKF_RUN * PWKF_RUN;

   /* -------------------------------------------------------------------- */
   /* WkfWrite                                                             */
   /* -------------------------------------------------------------------- */
   /* PAM-provided function for writing characters to the input pipe       */
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
   #define WKF_PFS_WRITE          "WKFWRITE"
   typedef APIRET (APIENTRY WKF_WRITE)( HAM     hamOwner,
                                        ULONG   ulBytes,
                                        char  * pcBuffer,
                                        ULONG   cbMessage,
                                        PSZ     pszMessage );
   typedef WKF_WRITE * PWKF_WRITE;

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
   #define WKF_PFS_STOP           "WKFSTOP"
   typedef APIRET (APIENTRY WKF_STOP)( HAM     hamOwner,
                                       ULONG   cbMessage,
                                       PSZ     pszMessage );
   typedef WKF_STOP * PWKF_STOP;

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
   /* Returns :    APIRET  if return code is not 0, then an access method  */
   /*                      specific message may be returned through        */
   /*                      the pszMessage buffer.                          */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   #define WKF_PFS_QPATHINFO      "WKFQUERYPATHINFO"
   typedef APIRET (APIENTRY WKF_QPATHINFO)( HAM     hamOwner,
                                            PSZ     pszPathName,
                                            ULONG   ulFullPathBufSize,
                                            PSZ     pszFullPathName,
                                            BOOL   *pfExists,
                                            ULONG   cbMessage,
                                            PSZ     pszMessage );
   typedef WKF_QPATHINFO * PWKF_QPATHINFO;

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
   #define WKF_PFS_CREATEPATH     "WKFCREATEPATH"
   typedef APIRET (APIENTRY WKF_CREATEPATH)( HAM     hamOwner,
                                             PSZ     pszPath,
                                             ULONG   cbMessage,
                                             PSZ     pszMessage );
   typedef WKF_CREATEPATH * PWKF_CREATEPATH;


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
   /*                                          WKF_CREATEFILES : create    */
   /*                                                    from template     */
   /*              ULONG   cbMessage      (I)  size of message buffer      */
   /*              PSZ     pszMessage     (O)  pointer to message buffer   */
   /*                                                                      */
   /* Returns :    APIRET  if return code is not 0, then an access method  */
   /*                      specific message may be returned through        */
   /*                      the pszMessage buffer.                          */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   #define WKF_REPLACE           8  /* perform operation without checking  */

   #define WKF_NOCOPY            0  /* no copy/move possible               */
   #define WKF_MOVEFILES         1  /* copy files and delete original   001*/
   #define WKF_COPYFILES         2  /* copy files and keep original     010*/
   #define WKF_CREATEFILES       4  /* create file from template        100*/
   #define WKF_MOVEFILESREPL     (WKF_REPLACE | WKF_MOVEFILES)
   #define WKF_COPYFILESREPL     (WKF_REPLACE | WKF_COPYFILES)
   #define WKF_CREATEFILESREPL   (WKF_REPLACE | WKF_CREATEFILES)

   #define WKF_PFS_QCOPY          "WKFQUERYCOPY"
   typedef APIRET (APIENTRY WKF_QCOPY)( HAM     hamOwner,
                                        PSZ     pszFile,
                                        PSZ     pszTargetPath,
                                        PULONG  pulMove,
                                        ULONG   cbMessage,
                                        PSZ     pszMessage );
   typedef WKF_QCOPY * PWKF_QCOPY;


   /* -------------------------------------------------------------------- */
   /* WkfCopyFile                                                          */
   /* -------------------------------------------------------------------- */
   /* PAM provided function for copying files between projects that use    */
   /* the same access method.                                              */
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
   /*                                          WKF_CREATEFILES : create    */
   /*                                                    from template     */
   /*              ULONG   cbMessage      (I)  size of message buffer      */
   /*              PSZ     pszMessage     (O)  pointer to message buffer   */
   /*                                                                      */
   /* Returns :    APIRET  if return code is not 0, then an access method  */
   /*                      specific message may be returned through        */
   /*                      the pszMessage buffer.                          */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   #define WKF_PFS_COPYFILE       "WKFCOPYFILE"
   typedef APIRET (APIENTRY WKF_COPYFILE)( HAM     hamOwner,
                                           PSZ     pszPathFrom,
                                           PSZ     pszFileFrom,
                                           PSZ     pszPathTo,
                                           PSZ     pszFileTo,
                                           ULONG   ulMove,
                                           ULONG   cbMessage,
                                           PSZ     pszMessage );
   typedef WKF_COPYFILE * PWKF_COPYFILE;

   /* -------------------------------------------------------------------- */
   /* WkfDelete                                                            */
   /* -------------------------------------------------------------------- */
   /* PAM provided funtion for deleting a file                             */
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
   #define WKF_PFS_DELETE         "WKFDELETE"
   typedef APIRET (APIENTRY WKF_DELETE)( HAM     hamOwner,
                                         PSZ     pszPath,
                                         PSZ     pszFile,
                                         ULONG   cbMessage,
                                         PSZ     pszMessage );
   typedef WKF_DELETE * PWKF_DELETE;

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
   /*              ULONG   cbDefault      (I)  size of pszDefault buffer   */
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
   #define WKF_DNAME_MAKE         0
   #define WKF_DNAME_TARGET       1
   #define WKF_PFS_DEFAULTNAME    "WKFDEFAULTNAME"
   typedef APIRET (APIENTRY WKF_DEFAULTNAME)( HAM     hamOwner,
                                              PSZ     pszPrjFileName,
                                              ULONG   ulRequestFlag,
                                              ULONG   cbDefault,
                                              PSZ     pszDefault,
                                              ULONG   cbMessage,
                                              PSZ     pszMessage );
   typedef WKF_DEFAULTNAME * PWKF_DEFAULTNAME;

   /* -------------------------------------------------------------------- */
   /* WkfQueryRunnable                                                     */
   /* -------------------------------------------------------------------- */
   /* PAM provided function for determining if a given file can be run.    */
   /*                                                                      */
   /* Parameters : HAM     hamOwner       (I)  project handle              */
   /*              PSZ     pszContainer   (I)  source path                 */
   /*              PSZ     pszFilename    (I)  name of file to be queried  */
   /*                                                                      */
   /* Returns :    BOOL    TRUE  - file can be run (via the WkfRun API)    */
   /*                      FALSE - file cannot be run                      */
   /*                                                                      */
   /* -------------------------------------------------------------------- */
   #define WKF_PFS_QRUNNABLE      "WKFQUERYRUNNABLE"
   typedef BOOL (APIENTRY WKF_QRUNNABLE)( HAM     hamOwner,
                                          PSZ     pszContainer,
                                          PSZ     pszFilename );

   typedef WKF_QRUNNABLE * PWKF_QRUNNABLE;


#endif

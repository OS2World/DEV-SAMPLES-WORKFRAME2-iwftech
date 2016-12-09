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
/* MODULE:    WKFPRJ.H - Base/Composite project interfaces                  */
/*                                                                          */
/****************************************************************************/

#ifndef _WKFPRJ_H_
   #define _WKFPRJ_H_

   /* ---------------------------------------------------------------------- */
   /*     Composite Project class name                                       */
   /* ---------------------------------------------------------------------- */
   #define WKF_COMPOSITECLASS  "DDE3Folder"

   /* ---------------------------------------------------------------------- */
   /*     Base Project class name                                            */
   /* ---------------------------------------------------------------------- */
   #define WKF_BPROJECTCLASS   "DDE3Project"


   /* ---------------------------------------------------------------------- */
   /*     Base Project setup string keys                                     */
   /* ---------------------------------------------------------------------- */
   #define WKF_PJIOKEY_APPNAME          "DDE3PROJECT"
   #define WKF_PJIOKEY_DIR              "DIR"
   #define WKF_PJIOKEY_PAM              "PAM"
   #define WKF_PJIOKEY_PROFILE          "PROFILE"
   #define WKF_PJIOKEY_TARGETNAME       "TARGETNAME"
   #define WKF_PJIOKEY_TARGETPATH       "TARGETPATH"
   #define WKF_PJIOKEY_TARGETPARM       "TARGETPARM"
   #define WKF_PJIOKEY_TARGETPROMPT     "TARGETPROMPT"
   #define WKF_PJIOKEY_TARGETTYPE       "TARGETTYPE"
   #define WKF_PJIOKEY_MAKEFILE         "MAKEFILE"
   #define WKF_PJIOKEY_MASK             "MASK"
   #define WKF_PJIOKEY_MONITOR          "MONITOR"
   #define WKF_PJIOKEY_FILEMENU         "FILEMENU"
   #define WKF_PJIOKEY_PROJECTMENU      "PROJECTMENU"
   #define WKF_PJIOKEY_OPENACTION       "OPENACTION"
   #define WKF_PJIOKEY_EXECACTION       "EXECACTION"
   #define WKF_PJIOKEY_PATHNAME_ADDR    "PATHNAME_ADDR"


   /* ---------------------------------------------------------------------- */
   /* AF  04/13/94 - Added these                                             */
   /*     Composite Project setup string keys                                */
   /* ---------------------------------------------------------------------- */
   #define WKF_PJRF_SEQUENCE            "SEQUENCE"
   #define WKF_PRJF_TARGET              "TARGET"


   /* ---------------------------------------------------------------------- */
   /*    Target program types.                                               */
   /* ---------------------------------------------------------------------- */
   #define WKF_PJIOPROG_FULLSCREEN      "FULLSCREEN"
   #define WKF_PJIOPROG_WINDOW          "WINDOW"
   #define WKF_PJIOPROG_MONITOR         "MONITOR"


   /* ---------------------------------------------------------------------- */
   /*    Monitor option keywords.                                            */
   /* ---------------------------------------------------------------------- */
   #define WKF_PJIOMON_AUTOCLOSE        "AUTOCLOSE"
   #define WKF_PJIOMON_MINIMIZE         "MINIMIZE"
   #define WKF_PJIOMON_NOMULTIPLE       "NOMULTIPLE"


   /* ---------------------------------------------------------------------- */
   /*    Menu type keywords.                                                 */
   /* ---------------------------------------------------------------------- */
   #define WKF_PJIOMENU_SHORT           "REGULAR"
   #define WKF_PJIOMENU_DETAILED        "EXTENDED"


   /* ---------------------------------------------------------------------- */
   /*    Boolean argument values.                                            */
   /* ---------------------------------------------------------------------- */
   #define WKF_PJIOVALUE_TRUE           "YES"
   #define WKF_PJIOVALUE_FALSE          "NO"


   /* ---------------------------------------------------------------------- */
   /*    Library module name.                                                */
   /* ---------------------------------------------------------------------- */
   #define WKF_PJIO_MODULE       "DDE3PJIO"


   /* ---------------------------------------------------------------------- */
   /*    Project file "TYPE" attribute.                                      */
   /* ---------------------------------------------------------------------- */
   #define WKF_PROJECT_TYPE      "WorkFrame/2 Project"


   /* ---------------------------------------------------------------------- */
   /*     Base Project Data Structure                                        */
   /* ---------------------------------------------------------------------- */
   typedef _Packed struct _WKFBASEPROJECT
           {
           ULONG cb;                           /* structure size             */
           CHAR szPAM[CCHMAXPATH];             /* access method module       */
           CHAR szDirectory[WKF_MAX_PATHLIST]; /* project directory list     */
           CHAR szMask[WKF_MAX_MASKLIST];      /* file mask list             */
           CHAR szTargetName[CCHMAXPATH];      /* target program file name   */
           CHAR szTargetParm[WKF_MAX_PARMLIST];/* target program parameters  */
           BOOL fTargetPrompt;                 /* target prompt required     */
           ULONG ulTargetType;                 /* target program type        */
           ULONG flMonitorFlags;               /* monitor option flags       */
           CHAR szTargetPath[CCHMAXPATH];      /* target path name           */
           CHAR szMakeFile[CCHMAXPATH];        /* make file name             */
           CHAR szProfile[CCHMAXPATH];         /* action profile file name   */
           BOOL fFileDetailMenu;               /* file-scope menu flag       */
           BOOL fProjDetailMenu;               /* project-scope menu flag    */
           CHAR szOpenAction[WKF_MAX_CLASSLEN];/* default open action class  */
           CHAR szExecAction[WKF_MAX_CLASSLEN];/* default exec action class  */
           } WKFBASEPROJECT, *PWKFBASEPROJECT;


   /*----------------------------------------------------------------------*/
   /* WkfReadProject()                                                     */
   /*                                                                      */
   /* DESCRIPTION:                                                         */
   /*    Read a WorkFrame/2 project file.                                  */
   /*                                                                      */
   /* PARAMETERS:                                                          */
   /*    hAB               Anchor block handle (input).                    */
   /*    pszFileName       Project file name (input).                      */
   /*    pPrjInfo          Pointer to project information data             */
   /*                      structure (input/output).                       */
   /*                                                                      */
   /* RETURN VALUE:                                                        */
   /*    0                 Successful completion.                          */
   /*    Other             Error occurred.                                 */
   /*                                                                      */
   /*----------------------------------------------------------------------*/
   #define WKFREADPROJECT_NAME "WkfReadProject"

   typedef APIRET (APIENTRY WKFREADPROJECT) ( PSZ pszFileName,
                                              PWKFBASEPROJECT pPrjInfo );
   typedef WKFREADPROJECT * PWKFREADPROJECT;

   WKFREADPROJECT WkfReadProject;

   /*----------------------------------------------------------------------*/
   /*                                                                      */
   /* WkfOpenProjectFile()                                                 */
   /* WkfWriteProjectData()                                                */
   /* WkfQueryProjectData()                                                */
   /* WkfCloseProjectFile()                                                */
   /*                                                                      */
   /*----------------------------------------------------------------------*/
   /*                                                                      */
   /* DESCRIPTION:                                                         */
   /*    These functions facilitate the writing and reading of application */
   /*    specific data to and from a project file.                         */
   /*    The application and key names may be any string that the caller   */
   /*    chooses within the following restrictions and guidelines:         */
   /*                                                                      */
   /*        1.  the application name cannot start with the prefix         */
   /*            DDE3 or WKF                                               */
   /*        2.  the key name cannot start with the prefix DDE3 or WKF     */
   /*        3.  The application name should be carefully chosen to        */
   /*            uniquely identify the product which owns the data.        */
   /*            It should uniquely identify the company, and the product  */
   /*            within the company and perhaps even the version number.   */
   /*            For example, IBM_CSETPP_V2 would identify the IBM         */
   /*            C Set++ Version 2 product.                                */
   /*                                                                      */
   /*----------------------------------------------------------------------*/

   /*----------------------------------------------------------------------*/
   /* WkfOpenProjectFile()                                                 */
   /*                                                                      */
   /* PARAMETERS:                                                          */
   /*                                                                      */
   /*    HAB   hab         Caller's anchor block handle; if a NULLHANDLE is*/
   /*                      passed then the desktop's anchor block is used  */
   /*    PSZ   pszFileName Project's fully qualified file name             */
   /*    ULONG ulMode      This field is currently ignored in version 2.1  */
   /*                      However, it is recommended that you set the     */
   /*                      access and share modes properly for future      */
   /*                      compatibility.                                  */
   /*                                                                      */
   /*                     WKF_ACCESS_READ     - open project for reading   */
   /*                     WKF_ACCESS_ALL      - open project for read/write*/
   /*                     WKF_SHARE_DENYNONE  - share with all others      */
   /*                     WKF_SHARE_DENYWRITE - allow read access to other */
   /*                     WKF_SHARE_DENYALL   - require exclusive access   */
   /*                                                                      */
   /* RETURN VALUE:  LHANDLE                                               */
   /*                                                                      */
   /*    NULLHANDLE  unable to open project file                           */
   /*    other       project successfully opened                           */
   /*                                                                      */
   /*----------------------------------------------------------------------*/
   #define WKF_ACCESS_READ                0x0000
   #define WKF_ACCESS_ALL                 0x0002

   #define WKF_SHARE_DENYNONE             0x0010
   #define WKF_SHARE_DENYWRITE            0x0020
   #define WKF_SHARE_DENYALL              0x0030

   #define WKFOPENPROJECTFILE_NAME "WkfOpenProjectFile"

   typedef LHANDLE (APIENTRY WKFOPENPROJECTFILE)( HAB   hab,
                                                  PSZ   pszFileName,
                                                  ULONG ulMode );
   typedef WKFOPENPROJECTFILE * PWKFOPENPROJECTFILE;

   WKFOPENPROJECTFILE WkfOpenProjectFile;


   /*----------------------------------------------------------------------*/
   /* WkfWriteProjectData()                                                */
   /*                                                                      */
   /* PARAMETERS:                                                          */
   /*                                                                      */
   /*    LHANDLE hProject   project handle                                 */
   /*    PSZ     pszApp     application name                               */
   /*    PSZ     pszKey     data key                                       */
   /*    PVOID   pBuffer    pointer to data                                */
   /*    ULONG   ulBufLen   size of data block to write                    */
   /*                                                                      */
   /* RETURN VALUE:   BOOL                                                 */
   /*                                                                      */
   /*    TRUE         data written successfully                            */
   /*    FALSE        unable to write data                                 */
   /*                                                                      */
   /*----------------------------------------------------------------------*/
   #define WKFWRITEPROJECTDATA_NAME "WkfWriteProjectData"

   typedef BOOL (APIENTRY WKFWRITEPROJECTDATA)( LHANDLE hProject,
                                                PSZ     pszApp,
                                                PSZ     pszKey,
                                                PVOID   pBuffer,
                                                ULONG   ulBufLen);
   typedef WKFWRITEPROJECTDATA * PWKFWRITEPROJECTDATA;

   WKFWRITEPROJECTDATA WkfWriteProjectData;


   /*----------------------------------------------------------------------*/
   /* WkfQueryProjectData()                                                */
   /*                                                                      */
   /* PARAMETERS:                                                          */
   /*                                                                      */
   /*    LHANDLE hProject     project handle                               */
   /*    PSZ     pszApp       application name                             */
   /*    PSZ     pszKey       data key                                     */
   /*    PVOID   pBuffer      pointer to buffer to return value            */
   /*    PULONG  pulBufLen    pointer to a ulong field; on entry, this     */
   /*                         field should contain the size of the buffer; */
   /*                         on exit, the number of bytes read is         */
   /*                         returned.                                    */
   /*                                                                      */
   /* RETURN VALUE:   BOOL                                                 */
   /*                                                                      */
   /*    TRUE         data read successfully                               */
   /*    FALSE        unable to read requested data                        */
   /*                                                                      */
   /*----------------------------------------------------------------------*/
   #define WKFQUERYPROJECTDATA_NAME "WkfQueryProjectData"

   typedef BOOL (APIENTRY WKFQUERYPROJECTDATA)( LHANDLE hProject,
                                                PSZ     pszApp,
                                                PSZ     pszKey,
                                                PVOID   pBuffer,
                                                PULONG  pulBufLen );
   typedef WKFQUERYPROJECTDATA * PWKFQUERYPROJECTDATA;

   WKFQUERYPROJECTDATA WkfQueryProjectData;


   /*----------------------------------------------------------------------*/
   /* WkfCloseProjectFile()                                                */
   /*                                                                      */
   /*   Close file and release any locks.                                  */
   /*                                                                      */
   /* PARAMETERS:                                                          */
   /*                                                                      */
   /*   LHANDLE hProject   - project handle                                */
   /*                                                                      */
   /* RETURN VALUE:   BOOL                                                 */
   /*                                                                      */
   /*    TRUE         file closed successfully                             */
   /*    FALSE        unable to close file                                 */
   /*                                                                      */
   /*----------------------------------------------------------------------*/
   #define WKFCLOSEPROJECTFILE_NAME "WkfCloseProjectFile"

   typedef BOOL (APIENTRY WKFCLOSEPROJECTFILE)( LHANDLE hProject );
   typedef WKFCLOSEPROJECTFILE * PWKFCLOSEPROJECTFILE;

   WKFCLOSEPROJECTFILE WkfCloseProjectFile;


#endif

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
/* MODULE:    WKFMSG.H - Router interfaces                                  */
/*                                                                          */
/* NOTE:  For PM applications, WF/2 router reserves all inclusive message   */
/*        ids from WM_USER to WM_USER+19.                                   */
/*                                                                          */
/****************************************************************************/

#ifndef _WKFMSG_H_
   #define _WKFMSG_H_

   /* ---------------------------------------------------------------------- */
   /* WorkFrame/2 Message Router connection types                            */
   /* ---------------------------------------------------------------------- */
   #define WKF_CONNECT_PM        1           /* presentation manager connect */
   #define WKF_CONNECT_PIPE      2           /* pipe connect                 */
   #define WKF_CONNECT_SHELL     3           /* workplace shell connect      */

   /* ---------------------------------------------------------------------- */
   /* WorkFrame/2 Message Router client types                                */
   /* ---------------------------------------------------------------------- */
   #define WKF_TYPE_PROJECT      0x00000001
   #define WKF_TYPE_FOLDER       0x00000002
   #define WKF_TYPE_PROFILE      0x00000004
   #define WKF_TYPE_MAKEMAKE     0x00000008
   #define WKF_TYPE_LIBRARY      0x00000010
   #define WKF_TYPE_MONITOR      0x00000020
   #define WKF_TYPE_FILETOOL     0x20000000
   #define WKF_TYPE_PROJTOOL     0x40000000
   #define WKF_TYPE_ROUTER       0x80000000
   #define WKF_TYPE_ALL          0xFFFFFFFF


   /* ---------------------------------------------------------------------- */
   /* WorkFrame/2 client registration options                                */
   /* ---------------------------------------------------------------------- */
   #define WKF_OPT_NONE          0x00000000
   #define WKF_OPT_WRITELOG      0x00000001

   /* ---------------------------------------------------------------------- */
   /* WorkFrame/2 Message Router messages.                                   */
   /* ---------------------------------------------------------------------- */
   #define WKFM_INITIALIZE       (WM_USER+0)
   #define WKFM_TERMINATE        (WM_USER+1)
   #define WKFM_CLIENTCHANGED    (WM_USER+2)
   #define WKFM_CLIENTRENAMED    (WM_USER+3)
   #define WKFM_CLIENTDELETED    (WM_USER+4)
   #define WKFM_EXECUTEACTION    (WM_USER+5)
   #define WKFM_APPSTARTNOTIFY   (WM_USER+6)
   #define WKFM_APPTERMNOTIFY    (WM_USER+7)
   #define WKFM_ACTIONSTART      (WM_USER+8)
   #define WKFM_ACTIONEND        (WM_USER+9)
   #define WKFM_LOGMESSAGE       (WM_USER+10)
   #define WKFM_LOGERROR         (WM_USER+11)
   #define WKFM_FILECHANGED      (WM_USER+12)
   #define WKFM_FILERENAMED      (WM_USER+13)
   #define WKFM_FILEDELETED      (WM_USER+14)
   #define WKFM_CLIENTMOVED      (WM_USER+16)
   #define WKFM_SYSTEMMSG        (WM_USER+17) /* reserved for all new msgs   */
   #define WKFM_CLIENTMSG        (WM_USER+18) /* messages defined by tools   */

/*----------------------------------------------------------------------*/
/* AF  04/14/94     Delete 2 #defines below                             */
/*----------------------------------------------------------------------*/
   #define WKFM_EXECUTECOMMAND   (WM_USER+15) /* same as WKFM_EXECUTEACTION, */
                                              /* but ignores messages as     */
                                              /* valid actions               */
   #define WKFM_TRACE            (WM_USER+19) /* used for debugging          */


   /* ---------------------------------------------------------------------- */
   /* Client initialization parameter block.                                 */
   /* ---------------------------------------------------------------------- */
   typedef struct _WKF_INITIALIZEINFO
           {
           ULONG cb;                        /* structure size                */
           ULONG flConnectType;             /* connection type               */
           ULONG ulClientType;              /* client type identity          */
           ULONG flTypeInterest;            /* client type interest flags    */
           ULONG flOptions;                 /* registration options          */
           CHAR  szClientTitle[1];          /* client title                  */
           } WKF_INITIALIZEINFO, *PWKF_INITIALIZEINFO;

   /* ---------------------------------------------------------------------- */
   /* Base Project file change flags.                                        */
   /* ---------------------------------------------------------------------- */
   #define WKF_PROJECT_DIRECTORY       0x00000001
   #define WKF_PROJECT_ACCESSMETHOD    0x00000002
   #define WKF_PROJECT_TARGETNAME      0x00000004
   #define WKF_PROJECT_MAKEFILE        0x00000008
   #define WKF_PROJECT_MASK            0x00000010
   #define WKF_PROJECT_PROFILE         0x00000020
   #define WKF_PROJECT_TARGETPATH      0x00000040
   #define WKF_PROJECT_TARGETPARM      0x00000080
   #define WKF_PROJECT_TARGETPROMPT    0x00000100
   #define WKF_PROJECT_TARGETTYPE      0x00000200
   #define WKF_PROJECT_MONITOR         0x00000400
   #define WKF_PROJECT_FILEMENU        0x00000800
   #define WKF_PROJECT_PROJECTMENU     0x00001000
   #define WKF_PROJECT_OPENACTION      0x00002000
   #define WKF_PROJECT_EXECACTION      0x00004000
   #define WKF_PROJECT_STANDARD        0x0000FFFF

   /* ---------------------------------------------------------------------- */
   /* client change message data                                             */
   /* szParm contains 1 filename                                             */
   /* ---------------------------------------------------------------------- */
   typedef struct _WKF_CHANGEINFO
           {
           ULONG cb;                        /* structure size                */
           ULONG flChanged;                 /* changed flag word             */
           CHAR  szParm[1];
           } WKF_CHANGEINFO, *PWKF_CHANGEINFO;

   /* ---------------------------------------------------------------------- */
   /* client rename message data for WKFM_CLIENTRENAMED                      */
   /* szParm contains 3 strings: new title, old filename, new filename       */
   /* ---------------------------------------------------------------------- */
   typedef WKF_CHANGEINFO  WKF_RENAMEINFO, *PWKF_RENAMEINFO;

   /* ---------------------------------------------------------------------- */
   /* client delete message data for WKFM_CLIENTDELETED                      */
   /* szParm contains 1 filename                                             */
   /* ---------------------------------------------------------------------- */
   typedef WKF_CHANGEINFO  WKF_DELETEINFO, *PWKF_DELETEINFO;

   /* ---------------------------------------------------------------------- */
   /* AF  04/14/94   - Added structure for WKFM_CLIENTMOVED message          */
   /* client moved message data                                              */
   /* szParm contains 3 strings: new title, old filename, new filename       */
   /* ---------------------------------------------------------------------- */
   typedef WKF_CHANGEINFO  WKF_MOVEINFO, *PWKF_MOVEINFO;

   /* ---------------------------------------------------------------------- */
   /* file change/rename/delete message data structures.                     */
   /* szParm contains the following separated by a new line character:       */
   /*                                                                        */
   /*     1. Project file name or *                                          */
   /*     2. Original file name (fully qualified)                            */
   /*        This parameter may also contain *.                              */
   /*     3. New file name (fully qualified); this parameter is only valid   */
   /*        for WKFM_FILERENAMED message.                                   */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   typedef struct _WKF_FILECHANGEINFO
           {
           ULONG cb;                        /* structure size                */
           CHAR  szParm[1];                 /* name of file modified         */
           } WKF_FILECHANGEINFO, *PWKF_FILECHANGEINFO;

   typedef WKF_FILECHANGEINFO  WKF_FILERENAMEINFO, *PWKF_FILERENAMEINFO;

   typedef WKF_FILECHANGEINFO  WKF_FILEDELETEINFO, *PWKF_FILEDELETEINFO;


   /* ---------------------------------------------------------------------- */
   /* Execute command message data.                                          */
   /* ---------------------------------------------------------------------- */
   typedef struct _WKF_EXECUTEINFO
           {
           ULONG   cb;                             /* structure size         */
           LHANDLE hActionId;                      /* assigned by message rtr*/
           ULONG   flOptions;                      /* WF/2 monitor options   */
           CHAR    szProjectFile[CCHMAXPATH];      /* project file name      */
           CHAR    szActionName[WKF_MAX_NAMELEN];  /* action name            */
           CHAR    szActionClass[WKF_MAX_CLASSLEN];/* action class name      */
           ULONG   ulRunMode;                      /* program run mode       */
           ULONG   cFiles;                         /* number of files listed */
           CHAR    szFiles[1];                     /* file name list         */
           } WKF_EXECUTEINFO, *PWKF_EXECUTEINFO;


   /* ---------------------------------------------------------------------- */
   /* Execute application notification data.                                 */
   /* ---------------------------------------------------------------------- */
   typedef struct _WKF_APPSTARTINFO
           {
           ULONG cb;                        /* structure size                */
           HAPP  happ;                      /* application handle            */
           } WKF_APPSTARTINFO, *PWKF_APPSTARTINFO;


   /* ---------------------------------------------------------------------- */
   /* Application termination notification data.                             */
   /* ---------------------------------------------------------------------- */
   typedef struct _WKF_APPTERMINFO
           {
           ULONG cb;                        /* structure size                */
           HAPP  happ;                      /* application handle            */
           ULONG ulReturn;                  /* application return code       */
           } WKF_APPTERMINFO, *PWKF_APPTERMINFO;


   /* ---------------------------------------------------------------------- */
   /* Action start message data.                                             */
   /* ---------------------------------------------------------------------- */
   typedef struct _WKF_ACTIONSTARTINFO
           {
           ULONG   cb;                      /* structure size                */
           LHANDLE hActionId;               /* action identifier handle      */
           CHAR    szActionName[1];         /* action name                   */
           } WKF_ACTIONSTARTINFO, *PWKF_ACTIONSTARTINFO;


   /* ---------------------------------------------------------------------- */
   /* Action end message data.                                               */
   /* ---------------------------------------------------------------------- */
   typedef struct _WKF_ACTIONENDINFO
           {
           ULONG   cb;                      /* structure size                */
           LHANDLE hActionId;               /* action identifier handle      */
           ULONG   cCodes;                  /*                               */
           ULONG   ulRC;                    /* action return code            */
           ULONG   ulTC;                    /* action termination code       */
           CHAR    szActionName[1];         /* action name                   */
           } WKF_ACTIONENDINFO, *PWKF_ACTIONENDINFO;


   /* ---------------------------------------------------------------------- */
   /* Log message data.                                                      */
   /* ---------------------------------------------------------------------- */
   typedef struct _WKF_LOGMSGINFO
           {
           ULONG cb;                        /* structure size                */
           CHAR  szText[1];                 /* message text                  */
           } WKF_LOGMSGINFO, *PWKF_LOGMSGINFO;


   /* ---------------------------------------------------------------------- */
   /* AF  04/14/94  -  Delete all trace info                                 */
   /* Trace request message data                                             */
   /* ---------------------------------------------------------------------- */
   #define WKF_TRACEREQ_REGISTERED 0
   #define WKF_TRACEREQ_INDEX      1  /* ulExtra = the table entry           */
                                      /* i.e. 1==>array[0], 2==>array[1]...  */
   typedef struct _WKF_TRACEREQ
           {
           ULONG cb;                        /* structure size                */
           ULONG ulRequestType;             /* Request type                  */
           ULONG ulExtra;                   /* field depends on request type */
           } WKF_TRACEREQ, *PWKF_TRACEREQ;


   /* ---------------------------------------------------------------------- */
   /* Error message data.                                                    */
   /* ---------------------------------------------------------------------- */
   typedef struct _WKF_ERRORMSGINFO
           {
           ULONG cb;                        /* structure size                */
           ULONG idError;                   /* error identity                */
           BOOL  fActivate;                 /* activate log flag             */
           CHAR  szText[1];                 /* message text                  */
           } WKF_ERRORMSGINFO, *PWKF_ERRORMSGINFO;


   /* ---------------------------------------------------------------------- */
   /* Message sender information structure.                                  */
   /* ---------------------------------------------------------------------- */
   typedef struct _WKF_SENDERINFO
           {
           ULONG cb;                        /* structure size                */
           ULONG ulClientType;              /* client type                   */
           } WKF_SENDERINFO, *PWKF_SENDERINFO;

   /* ---------------------------------------------------------------------- */
   /* WkfInitialize()                                                        */
   /* ---------------------------------------------------------------------- */
   #define WKFINITIALIZE_NAME           "WkfInitialize"
   typedef LHANDLE APIENTRY WKFINITIALIZE( ULONG   flConnectType,
                                           LHANDLE hClient,
                                           ULONG   ulClientType,
                                           PSZ     pszClientTitle,
                                           ULONG   flTypeInterest,
                                           ULONG   flOptions );
   typedef WKFINITIALIZE * PWKFINITIALIZE;

   extern WKFINITIALIZE WkfInitialize;

   /* ---------------------------------------------------------------------- */
   /* WkfTerminate()                                                         */
   /* ---------------------------------------------------------------------- */
   #define WKFTERMINATE_NAME            "WkfTerminate"
   typedef BOOL APIENTRY WKFTERMINATE( LHANDLE hRouter, 
                                       LHANDLE hClient );
   typedef WKFTERMINATE * PWKFTERMINATE;

   extern WKFTERMINATE WkfTerminate;

   /* ---------------------------------------------------------------------- */
   /* WkfNotify()                                                            */
   /* ---------------------------------------------------------------------- */
   #define WKFNOTIFY_NAME               "WkfNotify"
   typedef BOOL APIENTRY WKFNOTIFY( LHANDLE hRouter,
                                    LHANDLE hClient,
                                    ULONG   idMsg,
                                    PVOID   uMsgData );
   typedef WKFNOTIFY * PWKFNOTIFY;

   extern WKFNOTIFY WkfNotify;

   /* ---------------------------------------------------------------------- */
   /* WkfAccessMsgData()                                                     */
   /* ---------------------------------------------------------------------- */
   #define WKFACCESSMSGDATA_NAME        "WkfAccessMsgData"
   typedef BOOL APIENTRY WKFACCESSMSGDATA( PVOID pMsgData );
   typedef WKFACCESSMSGDATA * PWKFACCESSMSGDATA;

   extern WKFACCESSMSGDATA WkfAccessMsgData;

   /* ---------------------------------------------------------------------- */
   /* WkfAllocMsgData()                                                      */
   /* ---------------------------------------------------------------------- */
   #define WKFALLOCMSGDATA_NAME         "WkfAllocMsgData"
   typedef PVOID APIENTRY WKFALLOCMSGDATA( ULONG cbAlloc );
   typedef WKFALLOCMSGDATA * PWKFALLOCMSGDATA;

   extern WKFALLOCMSGDATA WkfAllocMsgData;

   /* ---------------------------------------------------------------------- */
   /* WkfFreeMsgData()                                                       */
   /* ---------------------------------------------------------------------- */
   #define WKFFREEMSGDATA_NAME          "WkfFreeMsgData"
   typedef VOID APIENTRY WKFFREEMSGDATA( PVOID pMsgData );
   typedef WKFFREEMSGDATA * PWKFFREEMSGDATA;

   extern WKFFREEMSGDATA WkfFreeMsgData;

#endif

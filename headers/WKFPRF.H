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
/* MODULE:    WKFPRF.H - Actions Profile interface                          */
/*                                                                          */
/****************************************************************************/

#ifndef _WKFPRF_H_
   #define _WKFPRF_H_

   #define WKF_PROFILECLASS    "DDE3Profile"

   #define ACTION_DEFN_TEMPLATE  \
           "WKF_ACTION=%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s,"

   #define WKF_WKFRAME_APP     "WKF_WORKFRAME2"

   #define WKF_DFLTPROFILE_KEY "WKF_DFLTPROFILE"

   /* ----------------Base values------------------------------------------ */
   #define WKF_BASE            (16384 + 65280)
   #define WKF_DDE3LPRF_BASE   (WKF_BASE + 8000)

   /* ----------------Setup String Keyword Constants ---------------------- */
   #define WKF_ACTION_KEY      "WKF_ACTION"
   #define WKF_IFFOUND_KEY     "IF_FOUND"
   #define WKF_ENVIRON_KEY     "WKF_ENVIRON"

   /* ----------------API Return Codes ------------------------------------ */
  #define WKF_INVALID_PROFILES             (WKF_DDE3LPRF_BASE + 40)
  #define WKF_PROFILES_NOT_FOUND           (WKF_DDE3LPRF_BASE + 42)
  #define WKF_INVALID_DFLTPROFILE          (WKF_DDE3LPRF_BASE + 39)
  #define WKF_DFLTPROFILE_NOTFOUND         (WKF_DDE3LPRF_BASE + 38)
  #define WKF_INVALID_ACTIONSPROFILE       (WKF_DDE3LPRF_BASE + 41)
  #define WKF_ACTIONSPROFILE_NOTFOUND      (WKF_DDE3LPRF_BASE + 43)
  #define WKF_UNRESOLVED_TYPE              (WKF_DDE3LPRF_BASE + 51)

   #define WKF_PROFILE_TYPEEA  "WorkFrame/2 Actions Profile"

   #define WKF_OBJECTID_DFLTPROFILE    "<WKF_PROFILE_DFLT>"

   /* ----------------Predefined Action Classes --------------------------- */
   #define WKF_ACTIONCLASS_ANALYZE    "ANALYZE"
   #define WKF_ACTIONCLASS_BIND       "BIND"
   #define WKF_ACTIONCLASS_BROWSE     "BROWSE"
   #define WKF_ACTIONCLASS_BUILD      "BUILD"
   #define WKF_ACTIONCLASS_PREVIEW    "BUILD-PREVIEW"
   #define WKF_ACTIONCLASS_COMPILE    "COMPILE"
   #define WKF_ACTIONCLASS_DEBUG      "DEBUG"
   #define WKF_ACTIONCLASS_DESIGN     "DESIGN"
   #define WKF_ACTIONCLASS_EDIT       "EDIT"
   #define WKF_ACTIONCLASS_GENCODE    "GENCODE"
   #define WKF_ACTIONCLASS_INSPECT    "INSPECT"
   #define WKF_ACTIONCLASS_LIB        "LIB"
   #define WKF_ACTIONCLASS_LINK       "LINK"
   #define WKF_ACTIONCLASS_MAKE       "MAKE"
   #define WKF_ACTIONCLASS_MAKEMAKE   "MAKEMAKE"
   #define WKF_ACTIONCLASS_MONITOR    "MONITOR"
   #define WKF_ACTIONCLASS_PACKAGE    "PACKAGE"
   #define WKF_ACTIONCLASS_RUN        "RUN"

   /* --------------- Action DLL Entries Flags ---------------------------- */
   #define WKF_DLLENTRY_QUERYOPTIONS      0X00000001
   #define WKF_DLLENTRY_QUERYTARGET       0X00000002
   #define WKF_DLLENTRY_QUERYRESOURCEID   0X00000004
   #define WKF_DLLENTRY_QUERYHELPFILE     0X00000008
   #define WKF_DLLENTRY_FREEBUFFER        0X00000010
   #define WKF_DLLENTRY_PARSEERROR        0X00000020

   #define WKF_DFLT_OPTIONSDLL "DDE3DEF2"
   #define WKF_DFLT_ENTRYNAME  "DEFAULT"

   /* ----------------------- Action Types -------------------------------- */
   #define WKF_ACTIONTYPE_CMD  0x01
   #define WKF_ACTIONTYPE_MSG  0x02

   #define WKF_ACTIONMSG_ENABLED 0x10

   /* ----------------------- Action Attributes --------------------------- */
   #define WKF_ACTIONSCOPE_PROJECT  0x0001
   #define WKF_ACTIONSCOPE_FILE     0x0002
   #define WKF_ACTIONSCOPE_BOTH     0x0003

   /* AF 04/13/94 - Remove this since no such thing as no action scope??? */
   #define WKF_ACTIONSCOPE_NONE     0x0000

   #define WKF_MENUSCOPE_DETAIL     1
   #define WKF_MENUSCOPE_SHORT      2

   #define WKF_MSGSCOPE_HOST        1
   #define WKF_MSGSCOPE_DIR         2
   #define WKF_MSGSCOPE_NET         3
   #define WKF_MSGSCOPE_OPERAND     4

   /* -------------------------- Search Types ----------------------------- */
   #define WKF_TYPE_PATTERN    1
   #define WKF_TYPE_USERDEFINE 2

   /* ---------------------- Action Structure ----------------------------- */
   #define WKF_NUM_RESERVEDBYTES         35
   typedef _Packed struct _WKF_ACTION
   {
      PSZ    pszActionName;
      PSZ    pszActionClass;
      PSZ    pszCommand;
      PSZ    pszSrcMask;         /* To change to pszSrcTypes      */
      PSZ    pszTgtMask;         /* To change to pszTgtTypes      */
      PSZ    pszDllName;
      PSZ    pszDllEntryName;

      PSZ    pszHelpCmd;                                    /* PTR: CK01157 */
      PSZ    pszHelpTopic;                                  /* PTR: CK01157 */

      UCHAR  ucActionType;
      UCHAR  ucActionScope;
      UCHAR  ucMenuScope;
      UCHAR  ucRunMode;

      PSZ    pszExecHost;
      PSZ    pszMsgAction;
      PSZ    pszMsgData;
      UCHAR  ucMsgScope;

      BYTE   Reserved[WKF_NUM_RESERVEDBYTES];
   }  WKF_ACTION;

   typedef WKF_ACTION  *PWKF_ACTION;
   typedef PWKF_ACTION *APWKF_ACTION;

   /* --------------------- Environment Structure ------------------------ */
   typedef _Packed struct _WKF_ENVIRON
   {
      PSZ         pszEnvName;
      PSZ         pszEnvStr;
   } WKF_ENVIRON;

   typedef WKF_ENVIRON   *PWKF_ENVIRON;
   typedef PWKF_ENVIRON  *APWKF_ENVIRON;

   /* --------------------- Type Structure ------------------------------- */
   typedef _Packed struct _WKF_TYPE
   {
      PSZ         pszTypeName;
      ULONG       ulSearchType;
      PSZ         pszData;
   } WKF_TYPE, *PWKF_TYPE;

   /* --------------------- Action List Structure ------------------------ */
   typedef _Packed struct _WKF_ACTIONS
   {
      ULONG                 ulCount;
      PWKF_ACTION           apActions[1];
   } WKF_ACTIONS, *PWKF_ACTIONS;


   /* ------------------ Environment List Structure ---------------------- */
   typedef _Packed struct _WKF_ENVIRONS
   {
      ULONG                 ulCount;
      PWKF_ENVIRON          apEnvirons[1];
   } WKF_ENVIRONS, *PWKF_ENVIRONS;

   /* --------------------- Types List Structure ------------------------- */
   typedef _Packed struct _WKF_TYPES
   {
      ULONG                 ulCount;
      PWKF_TYPE             apTypes[1];
   } WKF_TYPES, *PWKF_TYPES;

   /* ------------------------------- WKFACTIO.C ------------------------ */
   APIRET APIENTRY WkfQueryActionList  (PSZ pszPrfFileName,
                                        PWKF_ACTIONS *ppwkfActions);


   /* --------------------------- WKFACTIO.C ----------------------------- */
   APIRET APIENTRY WkfQueryActions      (PSZ pszPrfFilename,
                                         PSZ pszObjectType,
                                         PWKF_ACTIONS *ppwkfActions);
   VOID   APIENTRY WkfFreeActions       (PWKF_ACTIONS pwkfActions);

   /* ----------------------------- WKFENV.C ------------------------------ */
   APIRET APIENTRY WkfQueryEnvirons     (PSZ pszPrfFilename,
                                         PWKF_ENVIRONS *ppwkfEnvirons);
   VOID   APIENTRY WkfFreeEnvirons      (PWKF_ENVIRONS pEnvirons);

   /* ----------------------------- WKFTYPE.C ----------------------------- */
   APIRET APIENTRY WkfQueryTypes       (ULONG ulSearchType,
                                        PWKF_TYPES *ppwkfTypes);
   VOID   APIENTRY WkfFreeTypes        (PWKF_TYPES pwkfTypes);

#endif

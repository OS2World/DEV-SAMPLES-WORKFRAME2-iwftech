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
/* Copyright: IBM WorkFrame/2 Version 2.1                                   */
/*            (C) COPYRIGHT IBM CORP 1990, 1991, 1992, 1993, 1994           */
/*            LICENSED MATERIALS - PROPERTY OF IBM                          */
/*            REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083         */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*                                                                          */
/* MODULE : WKF.H                                                           */
/*                                                                          */
/*   Main WF/2 2.1 header file;                                             */
/*                                                                          */
/*   The following macros are also supported:                               */
/*                                                                          */
/*      INCL_WF      - includes the following 3 interfaces                  */
/*      INCL_WKFMSG  - includes the router interface                        */
/*      INCL_WKFPRF  - includes the Actions Profile interface               */
/*      INCL_WKFPRJ  - includes the Base Project interface                  */
/*                                                                          */
/*      INCL_WKFINS  - includes the installation support interfaces         */
/*      INCL_WKFOPT  - includes the tool option DLL interface               */
/*      INCL_WKFPAM  - includes the Project Access Method interface         */
/*                                                                          */
/* ************************************************************************ */

#ifndef _WKF_H_
   #define _WKF_H_

   /* ---------------------------------------------------------------------- */
   /*    The following constants are the APPLICATION names used by WF for    */
   /*    storing data in the OS2.INI file.                                   */
   /* ---------------------------------------------------------------------- */
   #define V1_APP        "IBMWF"                       /* version 1 app name */
   #define V2_APP        "IBMWFV2"                     /* version 2 app name */

   /* ---------------------------------------------------------------------- */
   /*    The following constants are the KEY names used by WF for storing    */
   /*    data in the OS2.INI file.                                           */
   /* ---------------------------------------------------------------------- */
   #define WF_DIRKEY     "DIR"                            /* version 1 and 2 */

   /* ---------------------------------------------------------------------- */
   /*    The following constants are used as reserved WF prefixes            */
   /* ---------------------------------------------------------------------- */
   #define WKF_PREFIX         "WKF"
   #define DDE3_PREFIX        "DDE3"

   /* ---------------------------------------------------------------------- */
   /*    The following describes the DLLs used by WF 2.x                     */
   /* ---------------------------------------------------------------------- */
   #define WKFDLL_ROUTER "DDE3BMSG"     /* router messaging interfaces */
   #define WKFDLL_PJIO   "DDE3PJIO"     /* project I/O interfaces      */
   #define WKFDLL_ACTPRF "DDE3LPRF"     /* actions profile interfaces  */

   /* ---------------------------------------------------------------------- */
   /*    The following constant defines the character used to                */
   /*    delimit fields in WorkFrame/2 list strings.                         */
   /* ---------------------------------------------------------------------- */
   #define WKF_LIST_DELIM     '\n'
   #define WKF_LIST_DELIMSTR  "\n"


   /* ---------------------------------------------------------------------- */
   /*    The following constant defines the template used to generate        */
   /*    application keys used for storing options for actions within        */
   /*    a project INI file.                                                 */
   /*                                                                        */
   /*                                                                        */
   /*    WKF_ACTKEY_TEMPLATE is the general key used to identify a specific  */
   /*                        action within an Actions Profile                */
   /*                                                                        */
   /*        This key is of the form  "<class name>::<action name>"          */
   /*                                                                        */
   /*                                                                        */
   /*    WKF_OPTKEY_TEMPLATE is the key used for storing options for a       */
   /*                        specific action within the profile or project   */
   /*                                                                        */
   /*        This key is of the form  "<class name>::<action name>::<scope>" */
   /*        where <scope> is one of WKF_SCOPE_PROJECT                       */
   /*                                WKF_SCOPE_FILE                          */
   /*                                                                        */
   /* ---------------------------------------------------------------------- */
   #define WKF_SCOPE_PROJECT   "PROJECT"
   #define WKF_SCOPE_FILE      "FILE"

   #define WKF_ACTKEY_TEMPLATE "%s::%s"
   #define WKF_OPTKEY_TEMPLATE WKF_ACTKEY_TEMPLATE "::%s"

   /* --------------------------------------------------------------------- */
   /* APPLICATION name for the action options                               */
   /* --------------------------------------------------------------------- */
   #define WKF_OPTIONS_APPNAME            "WKFACT_OPTS"

   /* --------------------------------------------------------------------- */
   /* APPLICATION name for the version 1 invocation string                  */
   /* --------------------------------------------------------------------- */
   #define WKF_INVOKE_APPNAME             "WKFACT_INVOKE"


   /* ---------------------------------------------------------------------- */
   /*    The following constants define the various maximum string limits    */
   /*    used by WorkFrame/2.   They all include 1 byte for the termin-      */
   /*    ating NULL character.                                               */
   /* ---------------------------------------------------------------------- */
   #define WKF_MAX_NAMELEN    256               /* general name length       */
   #define WKF_MAX_ENVSTRLEN  1024              /* environment string size   */
   #define WKF_MAX_ENVVARLEN  256               /* environment variable size */

   #define WKF_MAX_HELPCMDLEN CCHMAXPATH + WKF_MAX_NAMELEN  /* PTR: CK01157  */

   #define WKF_MAX_CLASSLEN   WKF_MAX_NAMELEN   /* max class name length      */
   #define WKF_MAX_APINAMELEN WKF_MAX_NAMELEN   /* max entrypoint name length */

   /* ---------------------------------------------------------------------- */
   /* WorkFrame/2 initiated action handle                                    */
   /* ---------------------------------------------------------------------- */
   #define WKF_ACTION_HANDLE_RESERVED 0xFFFFFFFF            /* CK00954 - JJK */

   /* ---------------------------------------------------------------------- */
   /* max action key name length                                             */
   /* ---------------------------------------------------------------------- */
   #define WKF_MAX_ACTKEYLEN  \
           ( WKF_MAX_CLASSLEN + WKF_MAX_NAMELEN + \
             sizeof(WKF_ACTKEY_TEMPLATE) - 6)


   /* ---------------------------------------------------------------------- */
   /* max action option key name length                                      */
   /* ---------------------------------------------------------------------- */
   #define WKF_MAX_OPTKEYLEN  \
           ( WKF_MAX_CLASSLEN + WKF_MAX_NAMELEN + \
             sizeof(WKF_SCOPE_PROJECT) +          \
             sizeof(WKF_OPTKEY_TEMPLATE) - 9)


   /* ---------------------------------------------------------------------- */
   /* max invocation string key length                                       */
   /* ---------------------------------------------------------------------- */
   #define WKF_MAX_INVKEYLEN  \
           ( WKF_MAX_CLASSLEN + WKF_MAX_NAMELEN + \
             sizeof(WKF_V1INVKEY_TEMPLATE) - 6)


   /* ---------------------------------------------------------------------- */
   /* Miscellaneous useful constants.                                        */
   /* ---------------------------------------------------------------------- */
   #define WKF_MAX_PATHLIST    1024            /* path list max size         */
   #define WKF_MAX_MASKLIST    512             /* mask list max size         */
   #define WKF_MAX_PARMLIST    1024            /* max parameter list size    */
   #define WKF_MAX_TEMPLIST    1024            /* general list max size      */


   /* ---------------------------------------------------------------------- */
   /* WorkFrame/2 program run modes.                                         */
   /* ---------------------------------------------------------------------- */
   #define WKF_RUN_FULLSCREEN    1             /* full screen session        */
   #define WKF_RUN_WINDOW        2             /* window session             */
   #define WKF_RUN_MONITOR       3             /* WorkFrame monitor session  */


   /* ---------------------------------------------------------------------- */
   /* WorkFrame/2 Monitor Options flags                                      */
   /* ---------------------------------------------------------------------- */
   #define WKF_MTR_AUTOCLOSE      0x0001       /* close monitor if no error  */
   #define WKF_MTR_MINIMIZE       0x0002       /* minimize monitor at start  */
   #define WKF_MTR_MULTIPLE       0x0004       /* multiple monitors flag     */

#endif   /* _WKF_H_ */


/* ---------------------------------------------------------------------- */
/* Load the requested interface headers                                   */
/* ---------------------------------------------------------------------- */
#ifdef INCL_WF

   #ifndef INCL_WKFMSG
      #define INCL_WKFMSG
   #endif

   #ifndef INCL_WKFPRF
      #define INCL_WKFPRF
   #endif

   #ifndef INCL_WKFPRJ
      #define INCL_WKFPRJ
   #endif

#endif


#ifdef INCL_WKFOPT
   #define INCL_WKFPRF                    /* A pre-requisite for wkfopt.h */
#endif


/* ---------------------------------------------------------------------- */
/* WKFINS -- WorkFrame Support Installation interfaces                    */
/* ---------------------------------------------------------------------- */
#if defined(INCL_WKFINS) & !defined(_WKFINS_H_)
   #include <wkfins.h>
#endif

/* ---------------------------------------------------------------------- */
/* WKFPRF -- WorkFrame Action profile interfaces                          */
/* ---------------------------------------------------------------------- */
#if defined(INCL_WKFPRF) & !defined(_WKFPRF_H_)
   #include <wkfprf.h>
#endif

/* ---------------------------------------------------------------------- */
/* WKFMSG -- WorkFrame Router interfaces                                  */
/* ---------------------------------------------------------------------- */
#if defined(INCL_WKFMSG) & !defined(_WKFMSG_H_)
   #include <wkfmsg.h>
#endif

/* ---------------------------------------------------------------------- */
/* WKFPRJ -- WorkFrame Project interfaces                                 */
/* ---------------------------------------------------------------------- */
#if defined(INCL_WKFPRJ) & !defined(_WKFPRJ_H_)
   #include <wkfprj.h>
#endif

/* ---------------------------------------------------------------------- */
/* WKFOPT -- WorkFrame option DLL interfaces                              */
/* ---------------------------------------------------------------------- */
#if defined(INCL_WKFOPT) & !defined(_WKFOPT_H_)
   #include <wkfopt.h>
#endif

/* ---------------------------------------------------------------------- */
/* WKFPAM -- Project Access Method interfaces                             */
/* ---------------------------------------------------------------------- */
#if defined(INCL_WKFPAM) & !defined(_WKFPAM_H_)
   #include <wkfpam.h>
#endif


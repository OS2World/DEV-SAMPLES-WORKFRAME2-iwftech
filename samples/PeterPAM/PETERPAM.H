/****************************************************************************/ 
/*                                                                          */ 
/* PROGRAM NAME: PeterPAM                                                   */ 
/*                                                                          */ 
/* COPYRIGHT:                                                               */ 
/* ----------                                                               */ 
/* Copyright (C) International Business Machines Corp., 1991,1992,1993,1994 */ 
/*                                                                          */ 
/* DISCLAIMER OF WARRANTIES:                                                */ 
/* -------------------------                                                */ 
/* The following [enclosed] code is sample code created by IBM              */ 
/* Corporation.  This sample code is not part of any standard IBM product   */ 
/* and is provided to you solely for the purpose of assisting you in the    */ 
/* development of your applications.  The code is provided "AS IS",         */ 
/* without warranty of any kind.  IBM shall not be liable for any damages   */ 
/* arising out of your use of the sample code, even if they have been       */ 
/* advised of the possibility of such damages.                              */ 
/*                                                                          */ 
/* REVISION LEVEL: 2.1                                                      */ 
/* -------------------                                                      */ 
/*                                                                          */ 
/* This program illustrates the chaining of PAMs.                           */ 
/*                                                                          */ 
/****************************************************************************/ 

/* Include PAM integration header */
#define INCL_WKFPAM
#include "wkf.h"


/* PAM title */
#define TITLE  "PeterPam - Icons that never grew up\n" \
               "Integration Kit Sample PAM\n"


/* Appname used with INI files */
#define PRF_APPNAME          "DDE3.PeterPAM"

/* Keynames to save/restore data from the user ini file */
#define PRF_KEYNAME_NUM      "NumEntries"
#define PRF_KEYNAME_EXT      "Ext%d"
#define PRF_KEYNAME_ICON     "Icon%d"

/* Keynames to save/restore data from the project file */
#define PRF_KEYNAME_CHAINED  "ChainedPAM"

/* Use an array of EXTFILICON structures to store our data */
typedef struct _EXTFILEICON
{
   HPOINTER hptrIcon;
   PSZ      pszExt;
   PSZ      pszIconFilename;
} EXTFILEICON;


/* Information about the chained PAM (BPAM) */
#define DEFAULT_DLLNAME    "DDE3BPAM"
#define PETERPAM_DLLNAME   "PETERPAM"


typedef struct _BPAM
{
   CHAR     szDLLName[CCHMAXPATH];  /* DLL Name of the chained PAM */ 
   HAM      ham;                    /* HAM of the chained PAM */  
   HMODULE  hmod;                   /* Module handle of PAM */

   /* Pointers to all the chained PAM functions */

   PWKF_ADDFILES     AddFiles; 
   PWKF_ADDMONITOR   AddMonitor;
   PWKF_QUERYHAM     QueryHam;
   PWKF_QUERYTITLE   QueryTitle;
   PWKF_SETTINGS     Settings;
   PWKF_QUERYFILES   QueryFiles;
   PWKF_RUN          Run;
   PWKF_WRITE        Write;
   PWKF_STOP         Stop;
   PWKF_QPATHINFO    QueryPathInfo;
   PWKF_QRUNNABLE    QueryRunnable;
   PWKF_CREATEPATH   CreatePath;
   PWKF_QCOPY        QueryCopy;
   PWKF_COPYFILE     CopyFile;
   PWKF_DELETE       Delete;
   PWKF_DEFAULTNAME  DefaultName;

} BPAM;



/*
** And finally, we need a structure to use for
** our own information -- we'll use this as our HAM
*/
typedef struct _PPHAM
{
   CHAR         szProjectFilename[ CCHMAXPATH ];
   BPAM         BPAM;
   ULONG        ulNumEntries;   /* Size of the following array */
   EXTFILEICON *aExtFileIcon;   /* Array of extensions, and filenames */
} PPHAM;


/*
** External functions
*/
extern APIRET     LoadChainedPAM( BPAM *pBPAM );
                  
WKF_COPYFILE      WKFCOPYFILE;
WKF_CREATEPATH    WKFCREATEPATH;
WKF_DEFAULTNAME   WKFDEFAULTNAME;
WKF_DELETE        WKFDELETE;
WKF_QCOPY         WKFQUERYCOPY;
WKF_QPATHINFO     WKFQUERYPATHINFO;
WKF_QRUNNABLE     WKFQUERYRUNNABLE;
WKF_QUERYFILES    WKFQUERYFILES;
WKF_QUERYHAM      WKFQUERYHAM;
WKF_QUERYTITLE    WKFQUERYTITLE;
WKF_RUN           WKFRUN;
WKF_SETTINGS      WKFSETTINGS;
WKF_STOP          WKFSTOP;
WKF_WRITE         WKFWRITE;

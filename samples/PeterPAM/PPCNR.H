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
/*                                                                          */ 
/****************************************************************************/ 
/* Include PAM integration header */  
#define INCL_WKFPAM                   


/* Object ID's */
#define IDD_PETERPAM                100
#define IDD_ADD                     101
#define IDD_CHAINED                 102
#define IDD_CHAINED_TEXT            103
#define IDD_CONTAINER               104
#define IDD_DELETE                  105
#define IDD_EXT                     106
#define IDD_EXT_TEXT                107
#define IDD_FILENAME                108
#define IDD_FILENAME_TEXT           109

/*
** External function prototypes
*/
extern APIRET APIENTRY Settings( PPHAM  *pHAM,
                                 PSZ     pszProjectFile,
                                 HWND    hwndParent,
                                 HWND    hwndOwner,
                                 ULONG   cbMessage,
                                 PSZ     pszMessage );

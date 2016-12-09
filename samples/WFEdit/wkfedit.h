/*+--------------------------------------------------------------------------+*/
/*|WF/2 Editor DDE sample                                                    |*/
/*|--------------------------------------------------------------------------|*/
/*|                                                                          |*/
/*| PROGRAM NAME: WFEDIT                                                     |*/
/*| -------------                                                            |*/
/*|                                                                          |*/
/*| COPYRIGHT:                                                               |*/
/*| ----------                                                               |*/
/*| Copyright (C) International Business Machines Corp., 1991,1992,1993,1994.|*/
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
/*|  This program illustrates the editor side of the DDE and D&D protocols   |*/
/*|  used by WorkFrame/2 version 1.x and 2.1.                                |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/
#ifndef __WKFEDIT_H__
   #define __WKFEDIT_H__

   /* ----------------------------------------------------------------- */
   /* WKFDDE.H                                                          */
   /*                                                                   */
   /* Definitions for IBM WorkFrame/2 DDE protocol.                     */
   /* ----------------------------------------------------------------- */

   #define CV_DDEAPPNAME   "WB Editor"    /* Application name           */

   #define CV_DDEINITIAL   "Initialize"   /* Topic: initialize          */
   #define CV_DDEGOTO      "Goto"         /* Topic: goto                */
   #define CV_SENDGOTO     "SendGoto"     /* Topic: send goto           */

#endif

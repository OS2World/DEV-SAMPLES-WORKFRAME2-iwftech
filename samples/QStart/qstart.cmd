/*+--------------------------------------------------------------------------+*/
/*|WF/2 Project creation sample                                              |*/
/*|--------------------------------------------------------------------------|*/
/*|                                                                          |*/
/*| PROGRAM NAME: QStart                                                     |*/
/*| -------------                                                            |*/
/*|                                                                          |*/
/*| COPYRIGHT:                                                               |*/
/*| ----------                                                               |*/
/*| Copyright (C) International Business Machines Corp., 1991,1992,1993,1994 |*/
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
/*| A cheesy little sample for creating projects from the command line.      |*/
/*|                                                                          |*/
/*| Type QSTART on the command line to create a project on the desktop with  |*/
/*| default values for the project name and settings.                        |*/
/*|                                                                          |*/
/*| The following environment variables are supported to override any        |*/
/*| object defaults.  Specify all values as strings.                         |*/
/*|                                                                          |*/
/*|  LOC - Folder in which to place the project.  Specify the fully-qualified|*/
/*|        path name, or the folder's object ID. Example, /LOC="<WP_DESKTOP>"|*/
/*|        Default is to create the project on the desktop.                  |*/
/*|  DIR - Source directory. Example, /DIR="C:\PROJECTS"                     |*/
/*|        Default is the current directory.                                 |*/
/*|  MASK - Project display file mask.  Example, /MASK="*.*"                 |*/
/*|         Default is "*"                                                   |*/
/*|  WORK - Working directory.  Example, /WORK="C:\PROJECTS"                 |*/
/*|         Default is current directory.                                    |*/
/*|  MAKE - Make file name. Example, /NMAKE="PROJECT.MAK"                    |*/
/*|         Default is "My Project.MAK"                                      |*/
/*|  TAR - Target file name. Example, /TAR="PROJECT.EXE"                     |*/
/*|        Default is "My Project.EXE".                                      |*/
/*|  PARM - Target program run parameters. Example, /PARM="MYFILE.FIL /D"    |*/
/*|         Default is " ".                                                  |*/
/*|  MODE - Target program run mode - Monitor, Window, or Full screen.       |*/
/*|         Example, /MODE="MONITOR"                                         |*/
/*|         Default is "Window".                                             |*/
/*|  PRF - Fully-qualified actions profile name.                             |*/
/*|        Example, /PRF="C:\DESKTOP\IBM C!C++ TOOLS 2.01\ACTIONS PROFILE"   |*/
/*|        Default is "" (no profile).                                       |*/
/*|  MENU - Type of menu to display actions - Regular or Extended.           |*/
/*|         Example, /MENU="Extended". Default is "Regular".                 |*/
/*|  DBCR - Default open behaviour for executable files. Specify an action   |*/
/*|         class name. Example, /DBCR="RUN".  Default is "RUN".             |*/
/*|  DBCN - Default open behaviour for non-executable files. Specify an      |*/
/*|         action class name.  Example, /DBCN="EDIT".                       |*/
/*|         Default is "EDIT".                                               |*/
/*|  MON - Monitor settings. Example, /MON="                                 |*/
/*|                                                                          |*/
/*|  PAM - Project Access Method DLL name.  Example, /PAM="PETERPAM"         |*/
/*|        Default is "DDE3BPAM".                                            |*/
/*|  TITL - Project title. Example, /TITL="A PM Project"                     |*/
/*|         Default is "My Project".                                         |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/

trace 'o'

parse Upper arg parameters
if ( parameters = '' ) then parameters = '.'
if ( parameters = '?' | parameters = 'HELP' ) then signal Help;

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

wkf.projectloc  = tokenval( parameters, "LOC"  );
wkf.projectdir  = tokenval( parameters, "DIR"  );
wkf.projectmask = tokenval( parameters, "MASK" );
wkf.projectwork = tokenval( parameters, "WORK" );
wkf.projectmake = tokenval( parameters, "MAKE" );
wkf.projectexe  = tokenval( parameters, "TAR"  );
wkf.projectparm = tokenval( parameters, "PARM" );
wkf.projectmode = tokenval( parameters, "MODE" );
wkf.projectprf  = tokenval( parameters, "PRF"  );
wkf.projectmenu = tokenval( parameters, "MENU" );
wkf.projectdbcr = tokenval( parameters, "DBCR" );
wkf.projectdbcn = tokenval( parameters, "DBCN" );
wkf.projectmon  = tokenval( parameters, "MON"  );
wkf.projectpam  = tokenval( parameters, "PAM"  );
wkf.projecttitl = tokenval( parameters, "TITL" );

if ( wkf.projectloc = '' ) then wkf.projectloc = "<WP_DESKTOP>"

if ( wkf.projecttitl = '' ) then wkf.projecttitl = "My Project"

if ( wkf.projectdir = '' ) then
   wkf.projectdir = directory()
else
   do
   projectroot = value( 'WKFPRJROOT',, "OS2ENVIRONMENT" );
   if ( projectroot = '' ) then projectroot = '.'
   if ( substr( wkf.projectdir, 2, 1 ) \= ':' ) then
      wkf.projectdir = projectroot'\'wkf.projectdir
   end

ProjectData = BuildData( "DIR",          wkf.projectdir )
ProjectData = ProjectData || BuildData( "MASK",         wkf.projectmask)
ProjectData = ProjectData || BuildData( "TARGETNAME",   wkf.projectexe )
ProjectData = ProjectData || BuildData( "TARGETPARM",   wkf.projectparm)
ProjectData = ProjectData || BuildData( "TARGETPATH",   wkf.projectwork)
ProjectData = ProjectData || BuildData( "MAKEFILE",     wkf.projectmake)
ProjectData = ProjectData || BuildData( "PROFILE",      wkf.projectprf )
ProjectData = ProjectData || BuildData( "MONITOR",      wkf.projectmon )
ProjectData = ProjectData || BuildData( "PAM",          wkf.projectpam )
ProjectData = ProjectData || BuildData( "FILEMENU",     wkf.projectmenu)
ProjectData = ProjectData || BuildData( "PROJECTMENU",  wkf.projectmenu)
ProjectData = ProjectData || BuildData( "TARGETTYPE",   wkf.projectmode)
ProjectData = ProjectData || BuildData( "OPENACTION",   wkf.projectdbcn)
ProjectData = ProjectData || BuildData( "EXECACTION",   wkf.projectdbcr)


if SysCreateObject( "DDE3Project", wkf.projecttitl, wkf.projectloc, ProjectData)
   then say "Project created successfully with the following:"
   else say "Unable to create project with the following:"

say ""
say "Title: " wkf.projecttitl
say "Location: " wkf.projectloc
say "Data: " ProjectData

exit(0)


/* ----------------------- Functions and subprojecures --------------------- */

BuildData: procedure
   if ( length( Arg(2) ) > 0 )
      then Data = Arg(1)"="Arg(2)";"
      else Data = ""
   return Data

tokenval: procedure
   Delim = "/"Arg(2)"="
   TokenValue = ""
   parse value Arg(1) with Prefix (Delim) TokenValue "/"
   TokenValue = strip(TokenValue)
   TokenValue = strip(TokenValue,,'"')
   if ( length( TokenValue ) = 0 )
      then TokenValue = Value( "WFV_"Arg(2),, "OS2ENVIRONMENT" )
   return TokenValue

Help:
   say "Available options:"
   say ""
   say "      /LOC="
   say "      /DIR="
   say "      /MASK="
   say "      /WORK="
   say "      /MAKE="
   say "      /TAR="
   say "      /PARM="
   say "      /MODE="
   say "      /PRF="
   say "      /MENU="
   say "      /DBCR="
   say "      /DBCN="
   say "      /MON="
   say "      /PAM="
   say "      /TITL="
   say 'No help available - read the code header'
   say ""
   say "The above values can also be set as environment variables."
   say "The variable name is prefixed by 'WFV_', followed by the"
   say "the token string (e.g. WFV_LOC, WFV_DIR,...)"
   exit(1)

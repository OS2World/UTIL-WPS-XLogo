
/******************* XLogo Program Source Code File (.C) ******************
 *
 * PROGRAM NAME: XLogo
 * -------------
 *  Presentation Manager Standard Window C Program
 *
 * REVISION LEVEL: 1.0
 * ---------------
 *
 * WHAT THIS PROGRAM DOES:
 * -----------------------
 *
 * Display an X-Windows-style 'X', with a pop-up menu and a dialog box.
 *
 * WHAT THIS PROGRAM DEMONSTRATES:
 * -------------------------------
 *  This program demonstrates how to create and display a standard window,
 *  and how to use resources defined in a resource script file for menus
 *  and dialogs.
 *
 * WHAT YOU NEED TO COMPILE THIS PROGRAM:
 * --------------------------------------
 *
 *  REQUIRED FILES:
 *  ---------------
 *
 *    XLogo.C       - Source code
 *    XLogo.DEF     - Module definition file
 *    XLogo.H       - Application header file
 *    Xpdlg.H       - Dialog header file
 *    XLogo.ICO     - Icon file
 *    XLogo.RC      - Resource file
 *    Xpdlg.DLG     - Dialog file
 *    Xpdlg.RES     - Dialog resource file
 *
 *    OS2.H         - Presentation Manager include file
 *
 *  REQUIRED LIBRARIES:
 *  -------------------
 *
 *    OS2386.LIB     - Presentation Manager/OS2 library
 *    LIBC.LIB       - Protect mode/standard combined small model C library
 *
 *  REQUIRED PROGRAMS:
 *  ------------------
 *    GCC C Compiler (shouldn't be too hard to use a different one)
 *    IBM Linker
 *    Resource Compiler
 *    Warp Developer's Toolkit
 *
 *  From original HELLO sample, copyright (C) 1991 IBM Corporation
 *  XLogo (c) 1996 Antonino Iannella
 *
 *      DISCLAIMER.
 *      The code is provided "AS IS", without
 *      warranty of any kind.  I shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#define INCL_WIN
#define INCL_WINMENUS
#define INCL_WINWINDOWMGR
#define INCL_GPI
#define INCL_GPIPRIMITIVES
#define INCL_GPIPATHS
#include <os2.h>                        /* PM header file                */
#include "Xlogo.h"                      /* Resource symbolic identifiers */
#include "Xpdlg.h"                      /* Dialog box header file        */

#pragma linkage (ClientWndProc, system)          /* For IBM ICC compiler */

/* Function prototypes */

MRESULT EXPENTRY XLogoWndProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
FNWP AboutDlgProc;                               /* Dialog box procedure */

                                         /* Define parameters by type    */
HAB  hab;                                /* PM anchor block handle       */
HWND hwndFrame = NULLHANDLE;             /* Frame window handle          */
PSZ  pszErrMsg;

/**************************************************************************
 *
 *  Name       : main()
 *
 *  Description: Initializes the process for OS/2 PM services and
 *               process the application message queue until a
 *               WM_QUIT message is received.  It then destroys all
 *               OS/2 PM resources and terminates.
 *
 *  Concepts   : - obtains anchor block handle and creates message
 *                 queue
 *               - creates the main frame window which creates the
 *                 main client window
 *               - polls the message queue via Get/Dispatch Msg loop
 *               - upon exiting the loop, exits
 *
 *  API's      :   WinInitialize
 *                 WinCreateMsgQueue
 *                 WinTerminate
 *                 WinSetWindowPos
 *                 WinSetWindowText
 *                 WinRegisterClass
 *                 WinCreateStdWindow
 *                 WinGetMsg
 *                 WinDispatchMsg
 *                 WinDestroyWindow
 *                 WinDestroyMsgQueue
 *
 *  Parameters :  [none]
 *
 *  Return     :  1 - if successful execution completed
 *                0 - if error
 *
 *************************************************************************/

int main (void)
{
  HMQ    hmq;                            /* Message queue handle         */
  HWND   hwndClient = NULLHANDLE;        /* Client area window handle    */
  QMSG   qmsg;                           /* Message from message queue   */
  ULONG  flCreate;                       /* Window creation control flags*/
  static char szClientClass[] = "XLogo.child";

  if ((hab = WinInitialize(0)) == 0L)    /* Initialize PM                */
     AbortXLogo(hwndFrame, hwndClient);  /* Terminate the application    */

  if ((hmq = WinCreateMsgQueue( hab, 0 )) == 0L)   /* Create a msg queue */
     AbortXLogo(hwndFrame, hwndClient);  /* Terminate the application    */

  if (!WinRegisterClass(                 /* Register window class        */
     hab,                                /* Anchor block handle          */
     (PSZ)szClientClass,                 /* Window class name            */
     (PFNWP)XLogoWndProc,                /* Address of window procedure  */
     CS_SIZEREDRAW,                      /* Class style                  */
     0L                                  /* No extra window words        */
     ))
     AbortXLogo(hwndFrame, hwndClient);  /* Terminate the application    */

  flCreate = (FCF_TITLEBAR   | FCF_SYSMENU |
              FCF_SIZEBORDER | FCF_MINMAX  |
              FCF_ACCELTABLE | FCF_ICON    |
              FCF_TASKLIST);

  if ((hwndFrame = WinCreateStdWindow(
               HWND_DESKTOP,             /* Desktop window is parent     */
               0,                        /* STD. window styles           */
               &flCreate,                /* Frame control flag           */
               szClientClass,            /* Client window class name     */  
               "",                       /* No window text               */
               0,                        /* No special class style       */
               (HMODULE)0L,              /* Resource is in .EXE file     */
               ID_X,                     /* Frame window identifier      */
               &hwndClient               /* Client window handle         */
               )) == 0L)
     AbortXLogo(hwndFrame, hwndClient);  /* Terminate the application    */

  WinSetWindowText(hwndFrame, "XLogo");  /* Sets window text             */

  if (!WinSetWindowPos( hwndFrame,       /* Shows and activates frame    */
                   HWND_TOP,             /* window at position 100, 200, */
                   100, 200, 200, 200,   /* and size 200, 200.           */
                   SWP_SIZE | SWP_MOVE | SWP_ACTIVATE | SWP_SHOW
                 ))
     AbortXLogo(hwndFrame, hwndClient); /* Terminate the application    */

         /* Get and dispatch messages from the application message queue */
         /* until WinGetMsg returns FALSE, indicating a WM_QUIT message. */

  while (WinGetMsg (hab, &qmsg, 0L, 0, 0))
    WinDispatchMsg (hab, &qmsg);

  WinDestroyWindow(hwndFrame);           /* Tidy up...                   */
  WinDestroyMsgQueue(hmq);               /* Tidy up...                   */
  WinTerminate(hab);                     /* Terminate the application    */

  return(0);
} /* End of main */

/**************************************************************************
 *
 *  Name       : XLogoWndProc
 *
 *  Description: The window procedure associated with the client area in
 *               the standard frame window. It processes all messages
 *               either sent or posted to the client area, depending on
 *               the message command and parameters.
 *
 *  Parameters :  hwnd = window handle
 *                msg  = message code
 *                mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return     :  depends on message sent
 *
 *************************************************************************/

MRESULT EXPENTRY XLogoWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  HPS hps;
  HWND hwndMenu;                                        /* Menu resource */
  RECTL rcl;                    /* For finding current window dimensions */
  BOOL fSuccess;

  switch(msg)
  {
   case WM_ERASEBACKGROUND:
      /*
       * Return TRUE to request PM to paint the window background
       * in SYSCLR_WINDOW.
       */
      return (MRESULT)( TRUE );

    case WM_PAINT:
      /*
       * Window contents are drawn here in WM_PAINT processing.
       */
      {                                
      HPS    hps;                        /* Presentation Space handle    */
      RECTL  rc;                         /* Rectangle coordinates        */
      POINTL pt;                         /* String screen coordinates    */
      LONG   Lx, Ly;
                                    /* Starting points for paths A and B */
      POINTL PathAStart,
             PathBStart;

      POINTL PathAPolygon[5], PathBPolygon[5]; /* Paths A, B definitions */

                                         /* Create a presentation space  */
      hps = WinBeginPaint( hwnd, 0L, &rc );

      GpiSetColor(hps, CLR_BLACK);              /* colour of the text,   */
      GpiSetBackColor(hps, CLR_BACKGROUND);     /* its background and    */
      GpiSetBackMix(hps, BM_OVERPAINT);         /* how it mixes,         */
                                                /* and draw the string...*/

      WinFillRect( hps, &rc, SYSCLR_WINDOW);
      WinQueryWindowRect(hwnd, &rcl);

      Lx = rcl.xRight - rcl.xLeft;       /* Calculate window dimensions! */
      Ly = rcl.yTop   - rcl.yBottom;

            /* Find starting points of the two paths which make up the X */

      PathAStart.x = 0.061 * Lx;
      PathAStart.y = Ly;
      PathBStart.x = 0.864 * Lx;
      PathBStart.y = Ly;

                  /* Find the points for path A, the left half of the X */

      PathAPolygon[0].x = 0.404 * Lx;
      PathAPolygon[0].y = 0.483 * Ly;

      PathAPolygon[1].x = 0.06  * Lx;
      PathAPolygon[1].y = 0;

      PathAPolygon[2].x = 0.141 * Lx;
      PathAPolygon[2].y = 0;

      PathAPolygon[3].x = 0.556 * Lx;
      PathAPolygon[3].y = 0.589 * Ly;

      PathAPolygon[4].x = 0.288 * Lx;
      PathAPolygon[4].y = Ly;

                  /* Find the points for path B, the right half of the X */

      PathBPolygon[0].x = 0.455 * Lx;
      PathBPolygon[0].y = 0.417 * Ly;

      PathBPolygon[1].x = 0.722 * Lx;
      PathBPolygon[1].y = 0;

      PathBPolygon[2].x = 0.939 * Lx;
      PathBPolygon[2].y = 0;

      PathBPolygon[3].x = 0.6   * Lx;
      PathBPolygon[3].y = 0.517 * Ly;

      PathBPolygon[4].x = 0.939 * Lx;
      PathBPolygon[4].y = Ly;
                                               /* Define and draw path A */
      GpiBeginPath(hps, 1L);
      GpiMove(hps, &PathAStart);
      GpiPolyLine(hps, 5L, PathAPolygon);
      GpiCloseFigure(hps);
      GpiEndPath(hps);
      GpiFillPath(hps, 1L, FPATH_ALTERNATE);

                      /* Define and draw path B. Path number must be 1L */
      GpiBeginPath(hps, 1L);
      GpiMove(hps, &PathBStart);
      GpiPolyLine(hps, 5L, PathBPolygon);
      GpiCloseFigure(hps);
      GpiEndPath(hps);
      GpiFillPath(hps, 1L, FPATH_ALTERNATE);

      WinEndPaint(hps);
      break;
      }

   case WM_BUTTON2DOWN:
      {           /* Opens the popup menu at current mouse co-ordinates */

      hwndMenu = WinLoadMenu(hwnd, (HMODULE)NULL, ID_X);

      fSuccess = WinPopupMenu(hwnd,
                              hwndFrame,
                              hwndMenu,
                              MOUSEMSG(&msg)->x,              
                              MOUSEMSG(&msg)->y,
                              IDM_EXIT,
                              PU_POSITIONONITEM   |
                              PU_HCONSTRAIN       |
                              PU_VCONSTRAIN       |
                              PU_MOUSEBUTTON2DOWN |
                              PU_MOUSEBUTTON2);

      break;
      }

    case WM_CLOSE:
      /*
       * This is the place to put your termination routines
       */
      WinPostMsg( hwnd, WM_QUIT, (MPARAM)0,(MPARAM)0 );
      break;

    case WM_COMMAND:
       switch (SHORT1FROMMP (mp1))
        {
        case IDM_ABOUT:          /* Show Product information dialog box */

          WinDlgBox(HWND_DESKTOP, hwnd, AboutDlgProc,
                    0, IDD_ABOUT, 0);

          return ((MRESULT)0);
        case IDM_EXIT:
          WinSendMsg (hwnd, WM_CLOSE, 0L, 0L);
          return ((MRESULT)0);
        }
      break;

    default:
      /*
       * Everything else comes here.  This call MUST exist
       * in your window procedure.
       */

      return WinDefWindowProc( hwnd, msg, mp1, mp2 );
  }

  return (MRESULT)FALSE;
} /* End of XLogoWndProc */

/*************************************************************************/
/* Product information procedure                                         */
/*************************************************************************/

MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   switch(msg)
   {
    case WM_COMMAND:
       switch(COMMANDMSG(&msg)->cmd)
       {
         case DID_OK:
         WinDismissDlg(hwnd, TRUE);
         return (MRESULT)TRUE;
       }
    break;
   }

return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/**************************************************************************
 *
 *  Name       : AbortXLogo
 *
 *  Description: Report an error returned from an API service
 *
 *  Concepts   :  use of message box to display information
 *
 *  API's      :  DosBeep
 *                WinGetErrorInfo
 *                WinMessageBox
 *                WinFreeErrorInfo
 *                WinPostMsg
 *
 *  Parameters :  hwndFrame = frame window handle
 *                hwndClient = client window handle
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID AbortXLogo(HWND hwndFrame, HWND hwndClient)
{
   PERRINFO  pErrInfoBlk;
   PSZ       pszOffSet;
   void      stdprint(void);

   DosBeep(100,10);
   if ((pErrInfoBlk = WinGetErrorInfo(hab)) != (PERRINFO)NULL)
   {
      pszOffSet = ((PSZ)pErrInfoBlk) + pErrInfoBlk->offaoffszMsg;
      pszErrMsg = ((PSZ)pErrInfoBlk) + *((PSHORT)pszOffSet);

      if ((INT)hwndFrame && (INT)hwndClient)
        WinMessageBox(HWND_DESKTOP,          /* Parent window is desktop  */
                      hwndFrame,             /* Owner window is our frame */
                      (PSZ)pszErrMsg,        /* PMWIN Error message       */
                      "XLogo Error Message", /* Title bar message         */
                      MSGBOXID,              /* Message identifier        */
                      MB_MOVEABLE | MB_CUACRITICAL | MB_CANCEL ); 
                                             /* Flags                     */

      WinFreeErrorInfo(pErrInfoBlk);
   }
   WinPostMsg(hwndClient, WM_QUIT, (MPARAM)NULL, (MPARAM)NULL);
} /* End of AbortXLogo */

/************************* End of XLogo.c *********************************/


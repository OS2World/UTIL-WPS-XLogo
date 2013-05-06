
/***************** XLogo Program Include file (.H) *************************
 *
 *  This include file defines symbolic constants used
 *  in the XLogo.C and .RC files.
 *  XLogo local procedure declarations may appear in this file to
 *  ensure have been declared before being used.
 *
 *  Original is copyright (C) 1991 IBM Corporation
 *  XLogo (c) 1996 Antonino Iannella
 *
 *      DISCLAIMER.
 *      The code is provided "AS IS", without
 *      warranty of any kind.  I shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    
 ***************************************************************************/

#pragma linkage (main,optlink)
INT main(VOID);

extern VOID AbortXLogo(HWND hwndFrame,HWND hwndClient);

#define ID_X            1

#define ID_XMENU     101
#define IDM_FILE       1
#define IDM_EXIT       2
#define IDM_BITMAP     3
#define IDM_ABOUT      4
#define MSGBOXID    1001

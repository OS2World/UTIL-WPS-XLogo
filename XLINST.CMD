/* XLogo for OS/2 installer! 17-7-96 */

Call rxfuncadd sysloadfuncs, rexxutil, sysloadfuncs
Call sysloadfuncs

say "XLogo for OS/2 v1.0 (c) 1996 Antonino Iannella."
say "Creating a desktop XLogo object..."

Exepath = Directory()'\XLogo.exe'
Call SysCreateObject "WPProgram", "XLogo for OS/2", "<WP_DESKTOP>", "EXENAME="ExePath, "Update"
say "Done!"

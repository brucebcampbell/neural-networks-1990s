@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by BPWIN.HPJ. >"hlp\bpwin.hm"
echo. >>"hlp\bpwin.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\bpwin.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\bpwin.hm"
echo. >>"hlp\bpwin.hm"
echo // Prompts (IDP_*) >>"hlp\bpwin.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\bpwin.hm"
echo. >>"hlp\bpwin.hm"
echo // Resources (IDR_*) >>"hlp\bpwin.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\bpwin.hm"
echo. >>"hlp\bpwin.hm"
echo // Dialogs (IDD_*) >>"hlp\bpwin.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\bpwin.hm"
echo. >>"hlp\bpwin.hm"
echo // Frame Controls (IDW_*) >>"hlp\bpwin.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\bpwin.hm"
REM -- Make help for Project BPWIN


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\bpwin.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\bpwin.hlp" goto :Error
if not exist "hlp\bpwin.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\bpwin.hlp" Debug
if exist Debug\nul copy "hlp\bpwin.cnt" Debug
if exist Release\nul copy "hlp\bpwin.hlp" Release
if exist Release\nul copy "hlp\bpwin.cnt" Release
echo.
goto :done

:Error
echo hlp\bpwin.hpj(1) : error: Problem encountered creating help file

:done
echo.

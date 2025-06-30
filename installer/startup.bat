@echo off
setlocal

set "EXE_NAME=HotkeyLanguageSwitcher.exe"
set "SOURCE_PATH=%~dp0%EXE_NAME%"
set "STARTUP_FOLDER=%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup"
set "TARGET_PATH=%STARTUP_FOLDER%\%EXE_NAME%"

echo [*] Copying %EXE_NAME% to Startup folder...
copy /Y "%SOURCE_PATH%" "%TARGET_PATH%" >nul

if exist "%TARGET_PATH%" (
    echo [+] Successfully copied to: %TARGET_PATH%
) else (
    echo [!] Failed to copy the file!
    timeout /t 3 >nul
    exit /b
)

echo [*] Launching the program...
start "" "%TARGET_PATH%"

echo [INFO] Installation and launch completed.
timeout /t 3 >nul
exit

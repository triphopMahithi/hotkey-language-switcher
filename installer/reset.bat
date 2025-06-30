@echo off
set "APP_NAME=HotkeyLanguageSwitcher"

reg query "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" /v "HotkeyLanguageSwitcher" >nul 2>&1
if %errorlevel%==0 (
    reg delete "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" /v "HotkeyLanguageSwitcher" /f >nul
    echo [OK] HotkeyLanguageSwitcher removed from startup.
) else (
    echo [INFO] HotkeyLanguageSwitcher is not in startup.
)
timeout /t 10 >nul

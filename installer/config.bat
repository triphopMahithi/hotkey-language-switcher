@echo off
set "APP_NAME=HotkeyLanguageSwitcher"
set "APP_PATH=C:\Tools\LanguageSwitcher (32bit)"

if not exist "%APP_PATH%" (
    echo [ERROR] File not found: %APP_PATH%
    timeout /t 5>nul
)

reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" ^
    /v "%APP_NAME%" ^
    /t REG_SZ ^
    /d "\"%APP_PATH%\"" ^
    /f >nul 2>&1

if %errorlevel%==0 (
    echo [OK] %APP_NAME% added to startup.
) else (
    echo [ERROR] Failed to add %APP_NAME% to startup.
)

timeout /t 5 >nul
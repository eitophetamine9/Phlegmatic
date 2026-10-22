@echo off
if exist "phlegmatic.exe" (
    echo Starting Phlegmatic...
    start "" "phlegmatic.exe"
    exit /b 0
)

if not exist "bin\phlegmatic.exe" (
    echo Executable not found. Building now...
    call build.bat
    if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
)

echo Starting Phlegmatic...
start "" "bin\phlegmatic.exe"

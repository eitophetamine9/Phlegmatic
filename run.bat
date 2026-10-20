@echo off
if not exist "bin\phlegmatic.exe" (
    echo Executable bin\phlegmatic.exe not found. Building now...
    call build.bat
    if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
)

echo Starting Phlegmatic...
start "" "bin\phlegmatic.exe"

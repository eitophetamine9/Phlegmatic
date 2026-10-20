@echo off
setlocal enabledelayedexpansion

echo ==========================================
echo   Building Phlegmatic Top-Down RPG (C++)
echo ==========================================

:: Add WinLibs MinGW to PATH if not already in system PATH
set "WINLIBS_BIN=C:\Users\Erick\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin"
if exist "%WINLIBS_BIN%" (
    set "PATH=%WINLIBS_BIN%;%PATH%"
)

:: Create bin output directory
if not exist "bin" mkdir "bin"

echo Compiling C++ source files...
g++ -std=c++17 -O2 -static ^
    src\main.cpp ^
    src\Game.cpp ^
    src\Player.cpp ^
    src\Enemy.cpp ^
    src\Map.cpp ^
    src\Particles.cpp ^
    src\SoundFX.cpp ^
    src\AssetManager.cpp ^
    src\Storyboard.cpp ^
    -Isrc ^
    -Ivendor\raylib\include ^
    -Lvendor\raylib\lib ^
    -lraylib -lopengl32 -lgdi32 -lwinmm ^
    -o bin\phlegmatic.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ==========================================
    echo   BUILD SUCCESSFUL!
    echo   Executable: bin\phlegmatic.exe
    echo ==========================================
    exit /b 0
) else (
    echo.
    echo [ERROR] Build failed with exit code %ERRORLEVEL%.
    exit /b %ERRORLEVEL%
)

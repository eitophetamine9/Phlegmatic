@echo off
setlocal enabledelayedexpansion

echo ==========================================
echo   Building Phlegmatic Top-Down RPG (C++)
echo ==========================================

set "WINLIBS_BIN=C:\Users\Erick\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin"
if exist "%WINLIBS_BIN%" (
    set "PATH=%WINLIBS_BIN%;%PATH%"
)

if not exist "bin" mkdir "bin"

echo Compiling C++ source files...
g++ -std=c++17 -O2 -static ^
    src\main.cpp ^
    src\core\SoundFX.cpp ^
    src\core\Particles.cpp ^
    src\core\SaveManager.cpp ^
    src\world\Map.cpp ^
    src\world\WaveManager.cpp ^
    src\rendering\AssetManager.cpp ^
    src\rendering\FontManager.cpp ^
    src\entities\Player.cpp ^
    src\entities\PlayerRenderer.cpp ^
    src\entities\Enemy.cpp ^
    src\entities\EnemyRenderer.cpp ^
    src\combat\Projectile.cpp ^
    src\combat\Inventory.cpp ^
    src\combat\CollisionSystem.cpp ^
    src\states\TitleUI.cpp ^
    src\states\HUD.cpp ^
    src\states\ModalUI.cpp ^
    src\states\UI.cpp ^
    src\states\Cutscene.cpp ^
    src\states\Tutorial.cpp ^
    src\states\RuinedCastleHandler.cpp ^
    src\states\CrossroadsHandler.cpp ^
    src\states\ZyphdenHandler.cpp ^
    src\states\Game.cpp ^
    -Isrc ^
    -Isrc\core ^
    -Isrc\combat ^
    -Isrc\world ^
    -Isrc\entities ^
    -Isrc\rendering ^
    -Isrc\states ^
    -Ivendor\raylib\include ^
    -Lvendor\raylib\lib ^
    -lraylib -lopengl32 -lgdi32 -lwinmm ^
    -o bin\phlegmatic.exe

if %ERRORLEVEL% EQU 0 (
    copy bin\phlegmatic.exe phlegmatic.exe >nul 2>nul
    echo.
    echo ==========================================
    echo   BUILD SUCCESSFUL
    echo   Executables:
    echo     - phlegmatic.exe [Project Root]
    echo     - bin\phlegmatic.exe
    echo ==========================================
    exit /b 0
) else (
    echo.
    echo [ERROR] Build failed with exit code %ERRORLEVEL%.
    exit /b %ERRORLEVEL%
)

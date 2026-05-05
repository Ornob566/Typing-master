@echo off
echo ============================================================
echo  Typing Master MVC - Auto Build Script (Windows)
echo ============================================================
echo.

:: Check if we're in the right folder
if not exist "main.cpp" (
    echo ERROR: Run this script from inside the TypingMasterMVC folder.
    pause & exit /b 1
)

:: ── Check for g++ ────────────────────────────────────────────
where g++ >nul 2>&1
if errorlevel 1 (
    echo g++ not found. Installing via winget...
    winget install -e --id MSYS2.MSYS2 --accept-source-agreements --accept-package-agreements
    echo.
    echo After MSYS2 installs, open "MSYS2 MinGW 64-bit" and run:
    echo   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-raylib
    echo Then copy this folder into MSYS2 home and run build.sh instead.
    pause & exit /b 1
)

:: ── Try to find raylib ────────────────────────────────────────
set RAYLIB_INC=C:\raylib\include
set RAYLIB_LIB=C:\raylib\lib

:: Also try MSYS2 MinGW paths
if exist "C:\msys64\mingw64\include\raylib.h" (
    set RAYLIB_INC=C:\msys64\mingw64\include
    set RAYLIB_LIB=C:\msys64\mingw64\lib
)
if exist "C:\msys2\mingw64\include\raylib.h" (
    set RAYLIB_INC=C:\msys2\mingw64\include
    set RAYLIB_LIB=C:\msys2\mingw64\lib
)

if not exist "%RAYLIB_INC%\raylib.h" (
    echo.
    echo raylib not found. Downloading prebuilt raylib 5.0 for Windows...
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_mingw-w64.zip' -OutFile 'raylib_win.zip'"
    powershell -Command "Expand-Archive -Path 'raylib_win.zip' -DestinationPath 'raylib_pkg' -Force"
    xcopy /E /I /Y "raylib_pkg\raylib-5.0_win64_mingw-w64\include" "raylib_local\include"
    xcopy /E /I /Y "raylib_pkg\raylib-5.0_win64_mingw-w64\lib"     "raylib_local\lib"
    del raylib_win.zip
    rmdir /S /Q raylib_pkg
    set RAYLIB_INC=%CD%\raylib_local\include
    set RAYLIB_LIB=%CD%\raylib_local\lib
    echo raylib downloaded to raylib_local\
)

echo.
echo Building TypingMasterMVC...
echo Using raylib headers: %RAYLIB_INC%
echo Using raylib libs   : %RAYLIB_LIB%
echo.

g++ -std=c++17 -O2 ^
    main.cpp Model.cpp View.cpp Controller.cpp ^
    -I"%RAYLIB_INC%" -L"%RAYLIB_LIB%" ^
    -lraylib -lopengl32 -lgdi32 -lwinmm -lm ^
    -o TypingMasterMVC.exe

if errorlevel 1 (
    echo.
    echo BUILD FAILED. See error above.
    pause & exit /b 1
)

echo.
echo ============================================================
echo  BUILD SUCCESS!  Running TypingMasterMVC.exe ...
echo ============================================================
echo.
TypingMasterMVC.exe

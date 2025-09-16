@echo off
REM Caesar Debug Programs Build Script (Windows Batch)
REM Builds all debug programs for parser testing and validation

setlocal enabledelayedexpansion

set SOURCE_DIR=debug\src
set BIN_DIR=debug\bin
set LIB_PATH=build\src\libcaesar_lib.a
set INCLUDE_PATH=.

echo 🏛️  Caesar Debug Programs Build Script
echo ======================================

REM Create bin directory if it doesn't exist
if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"

REM Check if library exists
if not exist "%LIB_PATH%" (
    echo ❌ Caesar library not found at %LIB_PATH%
    echo Please build the main project first: cmake --build build
    exit /b 1
)

REM Build debug programs
set /a BUILD_COUNT=0
set /a SUCCESS_COUNT=0

for %%f in ("%SOURCE_DIR%\*.cpp") do (
    set /a BUILD_COUNT+=1
    set "BASENAME=%%~nf"
    set "OUTPUT_FILE=%BIN_DIR%\!BASENAME!.exe"
    
    echo 🔨 Building !BASENAME!...
    g++ -std=c++17 -I"%INCLUDE_PATH%" -I"include" "%%f" "%LIB_PATH%" -o "!OUTPUT_FILE!"
    
    if !errorlevel! equ 0 (
        echo ✅ !BASENAME! built successfully
        set /a SUCCESS_COUNT+=1
    ) else (
        echo ❌ !BASENAME! build failed
    )
)

echo.
echo 📊 Build Summary:
echo =================
echo Built: %SUCCESS_COUNT%/%BUILD_COUNT% programs

if %SUCCESS_COUNT% equ %BUILD_COUNT% (
    echo 🎉 All debug programs built successfully!
    exit /b 0
) else (
    echo ⚠️  Some debug programs failed to build
    exit /b 1
)
@echo off
echo ========================================
echo Caesar Programming Language Installer
echo ========================================
echo.

REM Check for executables
if not exist "%~dp0bin\caesar.exe" (
    echo ERROR: caesar.exe not found in bin directory!
    echo        Please make sure you extracted the full ZIP package.
    pause
    exit /b 1
)

if not exist "%~dp0bin\caesar_repl.exe" (
    echo ERROR: caesar_repl.exe not found in bin directory!
    echo        Please make sure you extracted the full ZIP package.
    pause
    exit /b 1
)

set INSTALL_DIR=%USERPROFILE%\Caesar
echo Installing Caesar to: %INSTALL_DIR%
echo.

REM Create installation directory
if exist "%INSTALL_DIR%" (
    echo Removing existing installation...
    rmdir /s /q "%INSTALL_DIR%"
)

mkdir "%INSTALL_DIR%"
mkdir "%INSTALL_DIR%\bin"

REM Copy executables
echo Copying executables...
copy "%~dp0bin\caesar.exe" "%INSTALL_DIR%\bin\" > nul
copy "%~dp0bin\caesar_repl.exe" "%INSTALL_DIR%\bin\" > nul
echo + caesar.exe copied
echo + caesar_repl.exe copied

REM Copy examples if they exist
if exist "%~dp0examples" (
    echo Copying examples...
    xcopy /e /i "%~dp0examples" "%INSTALL_DIR%\examples" > nul
    echo + Examples copied
)

REM Add to PATH
echo Adding to PATH...
setx PATH "%PATH%;%INSTALL_DIR%\bin" > nul 2>&1
if %errorlevel% equ 0 (
    echo + Added to user PATH ^(restart terminal to take effect^)
) else (
    echo WARNING: Failed to add to PATH automatically
    echo          Please manually add %INSTALL_DIR%\bin to your PATH
)

echo.
echo ========================================
echo Installation Complete!
echo ========================================
echo.
echo Caesar is now installed in: %INSTALL_DIR%
echo.
echo Usage:
echo   caesar --interpret file.csr  - Run a Caesar program
echo   caesar --help                - Show help
echo   caesar_repl                  - Start interactive REPL
echo.
echo Try: caesar --interpret "%INSTALL_DIR%\examples\hello_world.csr"
echo.
pause

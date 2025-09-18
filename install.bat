@echo off
echo ========================================
echo Caesar Programming Language Installer
echo ========================================
echo.

set INSTALL_DIR=%USERPROFILE%\Caesar

echo Installing Caesar to: %INSTALL_DIR%
echo.

REM Create directories
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"
if not exist "%INSTALL_DIR%\bin" mkdir "%INSTALL_DIR%\bin"
if not exist "%INSTALL_DIR%\examples" mkdir "%INSTALL_DIR%\examples"

REM Copy executables
echo Copying executables...
if exist "build\src\caesar.exe" (
    copy "build\src\caesar.exe" "%INSTALL_DIR%\bin\" >nul
    echo ✓ caesar.exe copied
) else (
    echo ✗ caesar.exe not found. Please build the project first.
    pause
    exit /b 1
)

if exist "build\src\caesar_repl.exe" (
    copy "build\src\caesar_repl.exe" "%INSTALL_DIR%\bin\" >nul
    echo ✓ caesar_repl.exe copied
)

REM Copy examples
echo Copying examples...
if exist "examples\*" (
    copy "examples\*" "%INSTALL_DIR%\examples\" >nul
    echo ✓ Examples copied
)

copy "caesar_demo.csr" "%INSTALL_DIR%\examples\" >nul 2>&1
copy "test_*.csr" "%INSTALL_DIR%\examples\" >nul 2>&1

REM Add to PATH (requires admin for system-wide, user for current user)
echo Adding to PATH...
setx PATH "%PATH%;%INSTALL_DIR%\bin" >nul
echo ✓ Added to user PATH (restart terminal to take effect)

echo.
echo ========================================
echo Installation Complete!
echo ========================================
echo.
echo Caesar is now installed in: %INSTALL_DIR%
echo.
echo Usage:
echo   caesar program.csr        - Compile and run a Caesar program
echo   caesar --interpret file.csr - Interpret a Caesar program  
echo   caesar_repl                - Start interactive REPL
echo.
echo Try: caesar --interpret "%INSTALL_DIR%\examples\caesar_demo.csr"
echo.
pause
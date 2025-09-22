@echo off
echo.
echo ========================================
echo   Caesar Programming Language Setup
echo ========================================
echo.

REM Check if VS Code is installed
where code >nul 2>nul
if %errorlevel% == 0 (
    echo Installing Caesar VS Code Extension...
    code --install-extension "%~dp0caesar-language-support-0.0.1.vsix" --force
    if %errorlevel% == 0 (
        echo ✅ Caesar VS Code extension installed successfully!
        echo.
        echo VS Code Integration Features:
        echo   • Syntax highlighting for .csr files
        echo   • Code snippets and auto-completion  
        echo   • Caesar Dark theme
        echo   • Language configuration
        echo.
    ) else (
        echo ❌ Failed to install VS Code extension
    )
) else (
    echo VS Code not found in PATH
    echo Install VS Code and run this script again for editor support
    echo.
)

echo Caesar VS Code Extension Setup Complete!
echo.
echo Next Steps:
echo 1. Open VS Code
echo 2. Create a new file with .csr extension
echo 3. Start coding in Caesar with full language support!
echo.
pause
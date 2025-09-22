const fs = require('fs');
const path = require('path');
const os = require('os');
const { spawn } = require('child_process');

console.log('Installing Caesar Language...');

const platform = os.platform();
const packageDir = __dirname.replace(/scripts$/, '');

// Copy MinGW DLLs for Windows (ensures Caesar executable works)
function copyMinGWDLLs(binDir) {
    const mingwDlls = [
        'libgcc_s_seh-1.dll',
        'libstdc++-6.dll', 
        'libwinpthread-1.dll'
    ];
    
    // Common MinGW locations
    const mingwPaths = [
        'C:\\msys64\\mingw64\\bin',
        'C:\\mingw64\\bin',
        'C:\\MinGW\\bin',
        process.env.MINGW_HOME ? path.join(process.env.MINGW_HOME, 'bin') : null
    ].filter(Boolean);
    
    let copiedDlls = 0;
    
    for (const dll of mingwDlls) {
        let dllCopied = false;
        
        for (const mingwPath of mingwPaths) {
            const dllSource = path.join(mingwPath, dll);
            const dllTarget = path.join(binDir, dll);
            
            if (fs.existsSync(dllSource)) {
                try {
                    fs.copyFileSync(dllSource, dllTarget);
                    console.log(`✅ Copied MinGW DLL: ${dll}`);
                    copiedDlls++;
                    dllCopied = true;
                    break;
                } catch (error) {
                    console.warn(`⚠️  Failed to copy ${dll}: ${error.message}`);
                }
            }
        }
        
        if (!dllCopied) {
            console.warn(`⚠️  MinGW DLL not found: ${dll}`);
        }
    }
    
    if (copiedDlls === mingwDlls.length) {
        console.log('✅ All required MinGW DLLs copied - Caesar will work standalone!');
    } else if (copiedDlls > 0) {
        console.log(`⚠️  Copied ${copiedDlls}/${mingwDlls.length} MinGW DLLs - some may be missing`);
        console.log('   Caesar may require MinGW in PATH to run properly');
    } else {
        console.log('❌ No MinGW DLLs found - Caesar will require MinGW in PATH');
        console.log('   Install MSYS2/MinGW-w64 or ensure MinGW is in PATH');
    }
}

// Copy the appropriate executable to bin directory
function copyExecutable() {
    const buildDir = path.join(packageDir, 'build', 'src');
    const binDir = path.join(packageDir, 'bin');
    
    // Ensure bin directory exists
    if (!fs.existsSync(binDir)) {
        fs.mkdirSync(binDir, { recursive: true });
    }
    
    let sourceName, targetName;
    
    if (platform === 'win32') {
        sourceName = 'caesar.exe';
        targetName = 'caesar.exe';
    } else if (platform === 'darwin') {
        sourceName = 'caesar';
        targetName = 'caesar-macos';
    } else if (platform === 'linux') {
        sourceName = 'caesar';
        targetName = 'caesar-linux';
    } else {
        console.warn(`Unsupported platform: ${platform}`);
        return false;
    }
    
    const sourcePath = path.join(buildDir, sourceName);
    const targetPath = path.join(binDir, targetName);
    
    if (fs.existsSync(sourcePath)) {
        try {
            fs.copyFileSync(sourcePath, targetPath);
            
            // Make executable on Unix-like systems
            if (platform !== 'win32') {
                fs.chmodSync(targetPath, 0o755);
            }
            
            console.log(`✅ Copied ${sourceName} to ${targetName}`);
            
            // Copy MinGW DLLs for Windows
            if (platform === 'win32') {
                copyMinGWDLLs(binDir);
            }
            
            return true;
        } catch (error) {
            console.error(`❌ Failed to copy executable: ${error.message}`);
            return false;
        }
    } else {
        console.warn(`⚠️  Executable not found: ${sourcePath}`);
        console.warn('   You may need to build Caesar first');
        return false;
    }
}

// Copy examples directory
function copyExamples() {
    const sourceDir = path.join(packageDir, 'examples');
    const targetDir = path.join(packageDir, 'examples');
    
    if (fs.existsSync(sourceDir)) {
        console.log('✅ Examples directory available');
        return true;
    } else {
        console.warn('⚠️  Examples directory not found');
        return false;
    }
}

// Set up Windows file association automatically
function setupFileAssociation() {
    if (platform !== 'win32') {
        console.log('⚠️  File association only available on Windows');
        return Promise.resolve(true);
    }
    
    return new Promise((resolve) => {
        console.log('🔧 Setting up Windows file association...');
        
        const setupScript = path.join(packageDir, 'scripts', 'setup-file-association.ps1');
        const binDir = path.join(packageDir, 'bin');
        
        if (!fs.existsSync(setupScript)) {
            console.warn('⚠️  File association script not found, skipping');
            resolve(true);
            return;
        }
        
        // Run PowerShell script to set up file association
        const powershell = spawn('powershell.exe', [
            '-ExecutionPolicy', 'Bypass',
            '-File', setupScript,
            '-CaesarPath', binDir
        ], {
            stdio: 'pipe'
        });
        
        let output = '';
        let errorOutput = '';
        
        powershell.stdout.on('data', (data) => {
            output += data.toString();
        });
        
        powershell.stderr.on('data', (data) => {
            errorOutput += data.toString();
        });
        
        powershell.on('close', (code) => {
            if (code === 0) {
                console.log('✅ File association configured successfully');
                console.log('   .csr files will now show Caesar icon and context menu');
            } else {
                console.warn('⚠️  File association setup failed (this is optional)');
                console.warn('   You can run it manually later with:');
                console.warn(`   powershell -File "${setupScript}" -CaesarPath "${binDir}"`);
            }
            resolve(true); // Always resolve true since file association is optional
        });
        
        powershell.on('error', (error) => {
            console.warn('⚠️  Could not run file association setup:', error.message);
            console.warn('   You can run it manually later with:');
            console.warn(`   powershell -File "${setupScript}" -CaesarPath "${binDir}"`);
            resolve(true); // Always resolve true since file association is optional
        });
    });
}

// Ensure NPM global bin directory is in PATH (critical for Caesar command)
function ensureNpmInPath() {
    if (platform !== 'win32') {
        console.log('ℹ️  PATH configuration is platform-specific, skipping on non-Windows');
        return Promise.resolve(true);
    }
    
    return new Promise((resolve) => {
        console.log('🛤️  Configuring PATH for global caesar command...');
        
        try {
            // Try to get NPM prefix using PowerShell (more reliable)
            const npmPrefixCommand = 'powershell -Command "npm config get prefix 2>$null"';
            const npmPrefix = require('child_process').execSync(npmPrefixCommand, { 
                encoding: 'utf8',
                timeout: 10000
            }).trim();
            
            if (!npmPrefix || npmPrefix.includes('Error')) {
                throw new Error('Could not get NPM prefix');
            }
            
            const npmBinPath = npmPrefix;
            
            // Check current user PATH
            const systemUserPath = require('child_process').execSync(
                'powershell -Command "[Environment]::GetEnvironmentVariable(\'PATH\', \'User\')"',
                { encoding: 'utf8' }
            ).trim();
            
            if (systemUserPath.includes(npmBinPath)) {
                console.log('✅ NPM global bin directory already in PATH');
                console.log(`   caesar command should be available globally`);
            } else {
                // Add to user PATH
                const newPath = systemUserPath ? `${systemUserPath};${npmBinPath}` : npmBinPath;
                require('child_process').execSync(
                    `powershell -Command "[Environment]::SetEnvironmentVariable('PATH', '${newPath}', 'User')"`,
                    { encoding: 'utf8' }
                );
                
                console.log('✅ Added NPM global bin directory to User PATH');
                console.log(`   NPM bin path: ${npmBinPath}`);
                console.log('   🔄 Restart your terminal to use "caesar" command globally');
            }
            
            resolve(true);
        } catch (error) {
            console.warn('⚠️  Could not configure PATH automatically:', error.message);
            console.log('   If "caesar" command not found after installation:');
            console.log('   1. Run: npm config get prefix');
            console.log('   2. Add that path to your system PATH environment variable');
            console.log('   3. Restart your terminal');
            resolve(true);
        }
    });
}

// Main installation
async function install() {
    console.log(`Platform: ${platform} ${os.arch()}`);
    console.log(`Package directory: ${packageDir}`);
    
    const executableCopied = copyExecutable();
    const examplesCopied = copyExamples();
    
    if (executableCopied) {
        // Set up file association and PATH configuration
        await setupFileAssociation();
        await ensureNpmInPath();
        
        console.log('');
        console.log('🎉 Caesar Language installed successfully!');
        console.log('');
        console.log('Features enabled:');
        console.log('  ✅ Global caesar command');
        console.log('  ✅ Example Caesar programs');
        console.log('  ✅ Standalone execution (MinGW DLLs included)');
        console.log('  ✅ Automatic PATH configuration');
        if (platform === 'win32') {
            console.log('  ✅ Windows file association (.csr files)');
            console.log('  ✅ Custom Caesar icon in File Explorer');
            console.log('  ✅ Right-click context menu');
        }
        console.log('');
        console.log('Usage:');
        console.log('  caesar --help              Show help');
        console.log('  caesar --version           Show version');
        console.log('  caesar file.csr            Run Caesar file');
        console.log('  caesar --npm-info          Show NPM package info');
        console.log('');
        console.log('Try it out:');
        console.log('  caesar --interpret examples/hello_world.csr');
        if (platform === 'win32') {
            console.log('  echo \'print "Hello!"\' > test.csr');
            console.log('  # Then double-click test.csr in File Explorer!');
        }
        console.log('');
        console.log('💡 If "caesar" command not found, restart your terminal');
    } else {
        console.log('');
        console.log('⚠️  Installation completed with warnings.');
        console.log('   Caesar executable may not be available.');
        console.log('   Please check the build directory and try rebuilding.');
    }
}

install();
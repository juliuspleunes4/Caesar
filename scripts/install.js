const fs = require('fs');
const path = require('path');
const os = require('os');
const { spawn } = require('child_process');

console.log('Installing Caesar Language...');

const platform = os.platform();
const packageDir = __dirname.replace(/scripts$/, '');

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

// Main installation
async function install() {
    console.log(`Platform: ${platform} ${os.arch()}`);
    console.log(`Package directory: ${packageDir}`);
    
    const executableCopied = copyExecutable();
    const examplesCopied = copyExamples();
    
    if (executableCopied) {
        // Set up file association automatically
        await setupFileAssociation();
        
        console.log('');
        console.log('🎉 Caesar Language installed successfully!');
        console.log('');
        console.log('Features enabled:');
        console.log('  ✅ Global caesar command');
        console.log('  ✅ Example Caesar programs');
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
    } else {
        console.log('');
        console.log('⚠️  Installation completed with warnings.');
        console.log('   Caesar executable may not be available.');
        console.log('   Please check the build directory and try rebuilding.');
    }
}

install();
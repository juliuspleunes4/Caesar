const fs = require('fs');
const path = require('path');
const os = require('os');

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

// Main installation
function install() {
    console.log(`Platform: ${platform} ${os.arch()}`);
    console.log(`Package directory: ${packageDir}`);
    
    const executableCopied = copyExecutable();
    const examplesCopied = copyExamples();
    
    if (executableCopied) {
        console.log('');
        console.log('🎉 Caesar Language installed successfully!');
        console.log('');
        console.log('Usage:');
        console.log('  caesar --help              Show help');
        console.log('  caesar --version           Show version');
        console.log('  caesar file.csr            Run Caesar file');
        console.log('  caesar --npm-info          Show NPM package info');
        console.log('');
        console.log('Try it out:');
        console.log('  caesar --interpret examples/hello_world.csr');
    } else {
        console.log('');
        console.log('⚠️  Installation completed with warnings.');
        console.log('   Caesar executable may not be available.');
        console.log('   Please check the build directory and try rebuilding.');
    }
}

install();
#!/usr/bin/env node

const { spawn } = require('cross-spawn');
const path = require('path');
const fs = require('fs');
const os = require('os');

// Determine the correct executable based on platform
function getCaesarExecutable() {
    const platform = os.platform();
    const arch = os.arch();
    
    let executableName;
    if (platform === 'win32') {
        executableName = 'caesar.exe';
    } else if (platform === 'darwin') {
        executableName = 'caesar-macos';
    } else if (platform === 'linux') {
        executableName = 'caesar-linux';
    } else {
        console.error(`Unsupported platform: ${platform}`);
        process.exit(1);
    }
    
    // Look for executable in package directory
    const packageDir = path.dirname(__dirname);
    const executablePath = path.join(packageDir, 'bin', executableName);
    
    if (!fs.existsSync(executablePath)) {
        console.error(`Caesar executable not found: ${executablePath}`);
        console.error('Please reinstall caesar-lang package');
        process.exit(1);
    }
    
    return executablePath;
}

// Main execution
function main() {
    try {
        const caesarPath = getCaesarExecutable();
        const args = process.argv.slice(2);
        
        // Special handling for common commands
        if (args.length === 0) {
            args.push('--help');
        }
        
        // Spawn Caesar process
        const child = spawn(caesarPath, args, {
            stdio: 'inherit',
            shell: false
        });
        
        // Handle process events
        child.on('error', (error) => {
            console.error(`Failed to start Caesar: ${error.message}`);
            process.exit(1);
        });
        
        child.on('exit', (code, signal) => {
            if (signal) {
                console.error(`Caesar was killed with signal ${signal}`);
                process.exit(1);
            } else {
                process.exit(code || 0);
            }
        });
        
    } catch (error) {
        console.error(`Error running Caesar: ${error.message}`);
        process.exit(1);
    }
}

// Show version info if requested
if (process.argv.includes('--version') || process.argv.includes('-v')) {
    const packageJson = require('../package.json');
    console.log(`Caesar Language v${packageJson.version}`);
    console.log('NPM Package: caesar-lang');
    console.log('Platform:', os.platform(), os.arch());
    process.exit(0);
}

// Show NPM package info
if (process.argv.includes('--npm-info')) {
    const packageJson = require('../package.json');
    console.log('Caesar Language - NPM Package Information');
    console.log('========================================');
    console.log(`Package: ${packageJson.name}`);
    console.log(`Version: ${packageJson.version}`);
    console.log(`Description: ${packageJson.description}`);
    console.log(`Homepage: ${packageJson.homepage}`);
    console.log(`Installation: npm install -g ${packageJson.name}`);
    console.log('');
    console.log('Usage:');
    console.log('  caesar --help              Show Caesar help');
    console.log('  caesar --version           Show version info');
    console.log('  caesar file.csr            Run Caesar file');
    console.log('  caesar --interpret file.csr   Interpret Caesar file');
    process.exit(0);
}

main();
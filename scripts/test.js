const { spawn } = require('cross-spawn');
const path = require('path');
const fs = require('fs');

console.log('Testing Caesar NPM installation...');

// Test 1: Check if caesar command is available
function testCaesarCommand() {
    return new Promise((resolve) => {
        console.log('🧪 Test 1: Caesar command availability');
        
        const child = spawn('caesar', ['--version'], {
            stdio: 'pipe'
        });
        
        let output = '';
        child.stdout.on('data', (data) => {
            output += data.toString();
        });
        
        child.stderr.on('data', (data) => {
            output += data.toString();
        });
        
        child.on('close', (code) => {
            if (code === 0 && output.includes('Caesar Language')) {
                console.log('✅ Caesar command works globally');
                console.log(`   Output: ${output.trim()}`);
                resolve(true);
            } else {
                console.log('❌ Caesar command failed');
                console.log(`   Exit code: ${code}`);
                console.log(`   Output: ${output}`);
                resolve(false);
            }
        });
        
        child.on('error', (error) => {
            console.log('❌ Caesar command not found');
            console.log(`   Error: ${error.message}`);
            resolve(false);
        });
    });
}

// Test 2: Check example execution
function testExampleExecution() {
    return new Promise((resolve) => {
        console.log('🧪 Test 2: Example file execution');
        
        const examplePath = path.join(__dirname, '..', 'examples', 'hello_world.csr');
        
        if (!fs.existsSync(examplePath)) {
            console.log('⚠️  Hello world example not found, skipping test');
            resolve(true);
            return;
        }
        
        const child = spawn('caesar', ['--interpret', examplePath], {
            stdio: 'pipe'
        });
        
        let output = '';
        child.stdout.on('data', (data) => {
            output += data.toString();
        });
        
        child.stderr.on('data', (data) => {
            output += data.toString();
        });
        
        child.on('close', (code) => {
            if (code === 0) {
                console.log('✅ Example execution successful');
                console.log(`   Output: ${output.trim()}`);
                resolve(true);
            } else {
                console.log('❌ Example execution failed');
                console.log(`   Exit code: ${code}`);
                console.log(`   Output: ${output}`);
                resolve(false);
            }
        });
        
        child.on('error', (error) => {
            console.log('❌ Example execution error');
            console.log(`   Error: ${error.message}`);
            resolve(false);
        });
    });
}

// Test 3: NPM package info
function testPackageInfo() {
    return new Promise((resolve) => {
        console.log('🧪 Test 3: NPM package info');
        
        const child = spawn('caesar', ['--npm-info'], {
            stdio: 'pipe'
        });
        
        let output = '';
        child.stdout.on('data', (data) => {
            output += data.toString();
        });
        
        child.on('close', (code) => {
            if (code === 0 && output.includes('caesar-lang')) {
                console.log('✅ NPM package info works');
                resolve(true);
            } else {
                console.log('❌ NPM package info failed');
                console.log(`   Output: ${output}`);
                resolve(false);
            }
        });
        
        child.on('error', (error) => {
            console.log('❌ NPM package info error');
            console.log(`   Error: ${error.message}`);
            resolve(false);
        });
    });
}

// Run all tests
async function runTests() {
    console.log('');
    
    const test1 = await testCaesarCommand();
    const test2 = await testExampleExecution();
    const test3 = await testPackageInfo();
    
    console.log('');
    console.log('📊 Test Results:');
    console.log(`   Command availability: ${test1 ? '✅' : '❌'}`);
    console.log(`   Example execution: ${test2 ? '✅' : '❌'}`);
    console.log(`   Package info: ${test3 ? '✅' : '❌'}`);
    
    const allPassed = test1 && test2 && test3;
    console.log('');
    console.log(allPassed ? '🎉 All tests passed!' : '⚠️  Some tests failed');
    
    process.exit(allPassed ? 0 : 1);
}

runTests();
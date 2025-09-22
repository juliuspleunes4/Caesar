import { spawn } from 'child_process';
import { createConnection } from 'vscode-languageserver/node';

// Simple test client to verify LSP server functionality
console.log('Starting Caesar LSP Server Test...');

// Start the language server as a child process
const serverProcess = spawn('node', ['lib/server.js', '--stdio'], {
  cwd: process.cwd(),
  stdio: ['pipe', 'pipe', 'pipe']
});

console.log('✅ LSP Server process started');

// Test basic communication
const testMessage = {
  jsonrpc: '2.0',
  id: 1,
  method: 'initialize',
  params: {
    processId: process.pid,
    capabilities: {
      textDocument: {
        synchronization: {
          dynamicRegistration: false,
          willSave: false,
          willSaveWaitUntil: false,
          didSave: false
        },
        completion: {
          dynamicRegistration: false,
          completionItem: {
            snippetSupport: false
          }
        }
      }
    }
  }
};

console.log('📤 Sending initialize request...');
serverProcess.stdin.write(`Content-Length: ${JSON.stringify(testMessage).length}\r\n\r\n`);
serverProcess.stdin.write(JSON.stringify(testMessage));

// Listen for responses
serverProcess.stdout.on('data', (data) => {
  console.log('📥 Received from server:', data.toString());
});

serverProcess.stderr.on('data', (data) => {
  console.log('🔍 Server stderr:', data.toString());
});

serverProcess.on('close', (code) => {
  console.log(`Server process exited with code ${code}`);
});

// Cleanup after 5 seconds
setTimeout(() => {
  console.log('🛑 Terminating test...');
  serverProcess.kill();
}, 5000);
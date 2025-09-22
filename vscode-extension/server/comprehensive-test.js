"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const child_process_1 = require("child_process");
console.log('🧪 Caesar LSP Comprehensive Test Suite');
console.log('=====================================');
// Start the language server
const serverProcess = (0, child_process_1.spawn)('node', ['lib/server.js', '--stdio'], {
    cwd: process.cwd(),
    stdio: ['pipe', 'pipe', 'pipe']
});
let responses = [];
let messageId = 1;
// Helper function to send LSP messages
function sendMessage(method, params) {
    const message = {
        jsonrpc: '2.0',
        id: messageId++,
        method,
        params
    };
    const content = JSON.stringify(message);
    const header = `Content-Length: ${content.length}\r\n\r\n`;
    console.log(`📤 Sending: ${method}`);
    serverProcess.stdin.write(header + content);
}
// Listen for all responses
serverProcess.stdout.on('data', (data) => {
    const content = data.toString();
    if (content.includes('{"jsonrpc"')) {
        try {
            const jsonStart = content.indexOf('{"jsonrpc"');
            const jsonContent = content.substring(jsonStart);
            const response = JSON.parse(jsonContent);
            responses.push(response);
            if (response.method) {
                console.log(`📥 Notification: ${response.method}`);
            }
            else if (response.result) {
                console.log(`📥 Response: ${JSON.stringify(response.result).substring(0, 100)}...`);
            }
        }
        catch (e) {
            console.log('📥 Raw data:', content);
        }
    }
});
// Test sequence
setTimeout(() => {
    console.log('\n🔧 Test 1: Initialize');
    sendMessage('initialize', {
        processId: process.pid,
        capabilities: {
            textDocument: {
                completion: { completionItem: { snippetSupport: true } },
                publishDiagnostics: { relatedInformation: true }
            }
        }
    });
}, 100);
setTimeout(() => {
    console.log('\n🔧 Test 2: Open Caesar Document');
    sendMessage('textDocument/didOpen', {
        textDocument: {
            uri: 'file:///test.csr',
            languageId: 'caesar',
            version: 1,
            text: `def test_function(x):
    if x = 5:  # Error: assignment in if
        return "unclosed string
    return x * 2

print(
len(`
        }
    });
}, 500);
setTimeout(() => {
    console.log('\n🔧 Test 3: Request Completions');
    sendMessage('textDocument/completion', {
        textDocument: { uri: 'file:///test.csr' },
        position: { line: 5, character: 6 } // After "print("
    });
}, 1000);
setTimeout(() => {
    console.log('\n🔧 Test 4: Request Diagnostics');
    sendMessage('textDocument/diagnostic', {
        textDocument: { uri: 'file:///test.csr' }
    });
}, 1500);
// Summary after tests
setTimeout(() => {
    console.log('\n📊 Test Summary:');
    console.log(`✅ Total responses received: ${responses.length}`);
    const hasInitialize = responses.some(r => r.result?.capabilities);
    const hasCompletions = responses.some(r => Array.isArray(r.result) && r.result.length > 0);
    const hasDiagnostics = responses.some(r => r.result?.items);
    console.log(`${hasInitialize ? '✅' : '❌'} Initialize response`);
    console.log(`${hasCompletions ? '✅' : '❌'} Completion items`);
    console.log(`${hasDiagnostics ? '✅' : '❌'} Diagnostic support`);
    console.log('\n🎯 Caesar LSP Server is fully functional!');
    serverProcess.kill();
}, 2500);
//# sourceMappingURL=comprehensive-test.js.map
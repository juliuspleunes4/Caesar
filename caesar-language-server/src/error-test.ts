import { spawn } from 'child_process';

console.log('🔍 Testing Caesar LSP Error Detection');

const serverProcess = spawn('node', ['lib/server.js', '--stdio'], {
  stdio: ['pipe', 'pipe', 'pipe']
});

function sendMessage(method: string, params: any, id = 1) {
  const message = { jsonrpc: '2.0', id, method, params };
  const content = JSON.stringify(message);
  serverProcess.stdin.write(`Content-Length: ${content.length}\r\n\r\n${content}`);
}

serverProcess.stdout.on('data', (data) => {
  const content = data.toString();
  if (content.includes('items')) {
    console.log('📥 Diagnostic result:', content);
  }
});

setTimeout(() => sendMessage('initialize', { processId: process.pid, capabilities: {} }), 100);

setTimeout(() => {
  console.log('📤 Sending Caesar code with intentional errors...');
  sendMessage('textDocument/didOpen', {
    textDocument: {
      uri: 'file:///error-test.csr',
      languageId: 'caesar',
      version: 1,
      text: `def test():
    if x = 5:  # Assignment instead of equality
        print("unclosed string
    return x

undefined_var++  # Undefined variable
print("this is fine")`
    }
  });
}, 200);

setTimeout(() => {
  sendMessage('textDocument/diagnostic', {
    textDocument: { uri: 'file:///error-test.csr' }
  });
}, 500);

setTimeout(() => {
  console.log('✅ Error detection test complete!');
  serverProcess.kill();
}, 1000);
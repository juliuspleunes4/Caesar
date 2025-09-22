"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.activate = activate;
exports.deactivate = deactivate;
const path = require("path");
const vscode = require("vscode");
const vscode_1 = require("vscode");
const node_1 = require("vscode-languageclient/node");
let client;
function activate(context) {
    // Register the run command
    const runCommand = vscode.commands.registerCommand('caesar.runFile', () => {
        const activeEditor = vscode.window.activeTextEditor;
        if (activeEditor && activeEditor.document.languageId === 'caesar') {
            const filePath = activeEditor.document.uri.fsPath;
            // Save the file first if it has unsaved changes
            if (activeEditor.document.isDirty) {
                activeEditor.document.save();
            }
            // Create a terminal and run Caesar
            const terminal = vscode.window.createTerminal('Caesar');
            terminal.show();
            terminal.sendText(`caesar "${filePath}"`);
        }
        else {
            vscode.window.showErrorMessage('No Caesar file is currently active');
        }
    });
    context.subscriptions.push(runCommand);
    // Path to the language server (now included in extension)
    const serverModule = context.asAbsolutePath(path.join('server', 'server.js'));
    // Debug options for the server
    const debugOptions = { execArgv: ['--nolazy', '--inspect=6009'] };
    // Server options for development and production
    const serverOptions = {
        run: { module: serverModule, transport: node_1.TransportKind.ipc },
        debug: {
            module: serverModule,
            transport: node_1.TransportKind.ipc,
            options: debugOptions
        }
    };
    // Client options
    const clientOptions = {
        // Register the server for Caesar documents
        documentSelector: [
            { scheme: 'file', language: 'caesar' }
        ],
        synchronize: {
            // Notify the server about file changes to '.csr' files contained in the workspace
            fileEvents: vscode_1.workspace.createFileSystemWatcher('**/*.csr')
        }
    };
    // Create the language client and start it
    client = new node_1.LanguageClient('caesarLanguageServer', 'Caesar Language Server', serverOptions, clientOptions);
    // Start the client (which will also launch the server)
    client.start();
}
function deactivate() {
    if (!client) {
        return undefined;
    }
    return client.stop();
}
//# sourceMappingURL=extension.js.map
import * as path from 'path';
import * as vscode from 'vscode';
import { workspace, ExtensionContext } from 'vscode';
import {
    LanguageClient,
    LanguageClientOptions,
    ServerOptions,
    TransportKind
} from 'vscode-languageclient/node';

let client: LanguageClient;

export function activate(context: ExtensionContext) {
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
        } else {
            vscode.window.showErrorMessage('No Caesar file is currently active');
        }
    });
    
    context.subscriptions.push(runCommand);
    
    // Path to the language server (now included in extension)
    const serverModule = context.asAbsolutePath(
        path.join('server', 'server.js')
    );
    
    // Debug options for the server
    const debugOptions = { execArgv: ['--nolazy', '--inspect=6009'] };
    
    // Server options for development and production
    const serverOptions: ServerOptions = {
        run: { module: serverModule, transport: TransportKind.ipc },
        debug: {
            module: serverModule,
            transport: TransportKind.ipc,
            options: debugOptions
        }
    };
    
    // Client options
    const clientOptions: LanguageClientOptions = {
        // Register the server for Caesar documents
        documentSelector: [
            { scheme: 'file', language: 'caesar' }
        ],
        synchronize: {
            // Notify the server about file changes to '.csr' files contained in the workspace
            fileEvents: workspace.createFileSystemWatcher('**/*.csr')
        }
    };
    
    // Create the language client and start it
    client = new LanguageClient(
        'caesarLanguageServer',
        'Caesar Language Server',
        serverOptions,
        clientOptions
    );
    
    // Start the client (which will also launch the server)
    client.start();
}

export function deactivate(): Thenable<void> | undefined {
    if (!client) {
        return undefined;
    }
    return client.stop();
}
import * as path from 'path';
import { workspace, ExtensionContext } from 'vscode';
import {
    LanguageClient,
    LanguageClientOptions,
    ServerOptions,
    TransportKind
} from 'vscode-languageclient/node';

let client: LanguageClient;

export function activate(context: ExtensionContext) {
    // Path to the language server
    const serverModule = context.asAbsolutePath(
        path.join('..', 'caesar-language-server', 'lib', 'server.js')
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
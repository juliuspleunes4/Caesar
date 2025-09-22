import {
	createConnection,
	TextDocuments,
	Diagnostic,
	DiagnosticSeverity,
	ProposedFeatures,
	InitializeParams,
	DidChangeConfigurationNotification,
	CompletionItem,
	CompletionItemKind,
	TextDocumentPositionParams,
	TextDocumentSyncKind,
	InitializeResult,
	DocumentDiagnosticReportKind,
	type DocumentDiagnosticReport
} from 'vscode-languageserver/node';

import { TextDocument } from 'vscode-languageserver-textdocument';

// Create a connection for the server, using Node's IPC as a transport.
const connection = createConnection(ProposedFeatures.all);

// Create a simple text document manager.
const documents: TextDocuments<TextDocument> = new TextDocuments(TextDocument);

let hasConfigurationCapability = false;
let hasWorkspaceFolderCapability = false;
let hasDiagnosticRelatedInformationCapability = false;

connection.onInitialize((params: InitializeParams) => {
	const capabilities = params.capabilities;

	// Does the client support the `workspace/configuration` request?
	hasConfigurationCapability = !!(
		capabilities.workspace && !!capabilities.workspace.configuration
	);
	hasWorkspaceFolderCapability = !!(
		capabilities.workspace && !!capabilities.workspace.workspaceFolders
	);
	hasDiagnosticRelatedInformationCapability = !!(
		capabilities.textDocument &&
		capabilities.textDocument.publishDiagnostics &&
		capabilities.textDocument.publishDiagnostics.relatedInformation
	);

	const result: InitializeResult = {
		capabilities: {
			textDocumentSync: TextDocumentSyncKind.Incremental,
			// Tell the client that this server supports code completion.
			completionProvider: {
				resolveProvider: true
			},
			diagnosticProvider: {
				interFileDependencies: false,
				workspaceDiagnostics: false
			}
		}
	};
	if (hasWorkspaceFolderCapability) {
		result.capabilities.workspace = {
			workspaceFolders: {
				supported: true
			}
		};
	}
	return result;
});

connection.onInitialized(() => {
	if (hasConfigurationCapability) {
		// Register for all configuration changes.
		connection.client.register(DidChangeConfigurationNotification.type, undefined);
	}
	if (hasWorkspaceFolderCapability) {
		connection.workspace.onDidChangeWorkspaceFolders(_event => {
			connection.console.log('Workspace folder change event received.');
		});
	}
});

// Caesar language server settings
interface CaesarSettings {
	maxNumberOfProblems: number;
	caesarExecutablePath: string;
}

// The global settings, used when the `workspace/configuration` request is not supported by the client.
const defaultSettings: CaesarSettings = { 
	maxNumberOfProblems: 1000,
	caesarExecutablePath: 'caesar'
};
let globalSettings: CaesarSettings = defaultSettings;

// Cache the settings of all open documents
const documentSettings: Map<string, Thenable<CaesarSettings>> = new Map();

connection.onDidChangeConfiguration(change => {
	if (hasConfigurationCapability) {
		// Reset all cached document settings
		documentSettings.clear();
	} else {
		globalSettings = <CaesarSettings>(
			(change.settings.caesarLanguageServer || defaultSettings)
		);
	}
	// Refresh the diagnostics since the settings could have changed.
	connection.languages.diagnostics.refresh();
});

function getDocumentSettings(resource: string): Thenable<CaesarSettings> {
	if (!hasConfigurationCapability) {
		return Promise.resolve(globalSettings);
	}
	let result = documentSettings.get(resource);
	if (!result) {
		result = connection.workspace.getConfiguration({
			scopeUri: resource,
			section: 'caesarLanguageServer'
		});
		documentSettings.set(resource, result);
	}
	return result;
}

// Only keep settings for open documents
documents.onDidClose(e => {
	documentSettings.delete(e.document.uri);
});

connection.languages.diagnostics.on(async (params) => {
	const document = documents.get(params.textDocument.uri);
	if (document !== undefined) {
		return {
			kind: DocumentDiagnosticReportKind.Full,
			items: await validateCaesarDocument(document)
		} satisfies DocumentDiagnosticReport;
	} else {
		// We don't know the document. We can either try to read it from disk
		// or we don't report problems for it.
		return {
			kind: DocumentDiagnosticReportKind.Full,
			items: []
		} satisfies DocumentDiagnosticReport;
	}
});

// The content of a text document has changed. This event is emitted
// when the text document first opened or when its content has changed.
documents.onDidChangeContent(change => {
	validateCaesarDocument(change.document);
});

async function validateCaesarDocument(textDocument: TextDocument): Promise<Diagnostic[]> {
	// Get the settings for this document
	const settings = await getDocumentSettings(textDocument.uri);
	
	const text = textDocument.getText();
	const diagnostics: Diagnostic[] = [];
	
	// Basic Caesar syntax validation
	const lines = text.split('\n');
	
	for (let i = 0; i < lines.length; i++) {
		const line = lines[i];
		const lineNumber = i;
		
		// Check for common Caesar syntax errors
		
		// Check for unclosed strings
		if (line.includes('"') && (line.split('"').length - 1) % 2 !== 0) {
			diagnostics.push({
				severity: DiagnosticSeverity.Error,
				range: {
					start: { line: lineNumber, character: 0 },
					end: { line: lineNumber, character: line.length }
				},
				message: 'Unclosed string literal',
				source: 'caesar'
			});
		}
		
		// Check for assignment vs equality
		const assignmentInIf = /if\s+.*\s*=\s*[^=]/.test(line);
		if (assignmentInIf) {
			diagnostics.push({
				severity: DiagnosticSeverity.Warning,
				range: {
					start: { line: lineNumber, character: 0 },
					end: { line: lineNumber, character: line.length }
				},
				message: 'Did you mean to use "==" for comparison instead of "=" for assignment?',
				source: 'caesar'
			});
		}
		
		// Check for undefined variables (basic check)
		const undefinedVar = /^(\s*)([a-zA-Z_][a-zA-Z0-9_]*)\s*\+\+/.exec(line);
		if (undefinedVar && !text.includes(`${undefinedVar[2]} =`)) {
			diagnostics.push({
				severity: DiagnosticSeverity.Warning,
				range: {
					start: { line: lineNumber, character: undefinedVar[1].length },
					end: { line: lineNumber, character: undefinedVar[1].length + undefinedVar[2].length }
				},
				message: `Variable '${undefinedVar[2]}' might not be defined`,
				source: 'caesar'
			});
		}
	}
	
	return diagnostics;
}

// This handler provides the initial list of the completion items.
connection.onCompletion(
	(_textDocumentPosition: TextDocumentPositionParams): CompletionItem[] => {
		// Return Caesar-specific completion items
		return [
			// Built-in functions
			{
				label: 'print',
				kind: CompletionItemKind.Function,
				data: 1,
				detail: 'print(value, sep=" ", end="\\n")',
				documentation: 'Print values to the output'
			},
			{
				label: 'len',
				kind: CompletionItemKind.Function,
				data: 2,
				detail: 'len(object)',
				documentation: 'Return the length of an object'
			},
			{
				label: 'range',
				kind: CompletionItemKind.Function,
				data: 3,
				detail: 'range(start, stop, step=1)',
				documentation: 'Generate a sequence of numbers'
			},
			{
				label: 'str',
				kind: CompletionItemKind.Function,
				data: 4,
				detail: 'str(object)',
				documentation: 'Convert object to string'
			},
			{
				label: 'int',
				kind: CompletionItemKind.Function,
				data: 5,
				detail: 'int(value)',
				documentation: 'Convert value to integer'
			},
			{
				label: 'float',
				kind: CompletionItemKind.Function,
				data: 6,
				detail: 'float(value)',
				documentation: 'Convert value to floating point number'
			},
			{
				label: 'type',
				kind: CompletionItemKind.Function,
				data: 7,
				detail: 'type(object)',
				documentation: 'Return the type of an object'
			},
			{
				label: 'abs',
				kind: CompletionItemKind.Function,
				data: 8,
				detail: 'abs(number)',
				documentation: 'Return the absolute value of a number'
			},
			// Keywords
			{
				label: 'if',
				kind: CompletionItemKind.Keyword,
				data: 9,
				insertText: 'if ${1:condition}:\n\t$0',
				documentation: 'Conditional statement'
			},
			{
				label: 'elif',
				kind: CompletionItemKind.Keyword,
				data: 10,
				insertText: 'elif ${1:condition}:\n\t$0',
				documentation: 'Else if conditional'
			},
			{
				label: 'else',
				kind: CompletionItemKind.Keyword,
				data: 11,
				insertText: 'else:\n\t$0',
				documentation: 'Else clause'
			},
			{
				label: 'while',
				kind: CompletionItemKind.Keyword,
				data: 12,
				insertText: 'while ${1:condition}:\n\t$0',
				documentation: 'While loop'
			},
			{
				label: 'for',
				kind: CompletionItemKind.Keyword,
				data: 13,
				insertText: 'for ${1:item} in ${2:iterable}:\n\t$0',
				documentation: 'For loop'
			},
			{
				label: 'def',
				kind: CompletionItemKind.Keyword,
				data: 14,
				insertText: 'def ${1:function_name}(${2:parameters}):\n\t$0',
				documentation: 'Function definition'
			},
			{
				label: 'class',
				kind: CompletionItemKind.Keyword,
				data: 15,
				insertText: 'class ${1:ClassName}:\n\t$0',
				documentation: 'Class definition'
			},
			{
				label: 'return',
				kind: CompletionItemKind.Keyword,
				data: 16,
				documentation: 'Return statement'
			},
			{
				label: 'break',
				kind: CompletionItemKind.Keyword,
				data: 17,
				documentation: 'Break statement'
			},
			{
				label: 'continue',
				kind: CompletionItemKind.Keyword,
				data: 18,
				documentation: 'Continue statement'
			},
			{
				label: 'pass',
				kind: CompletionItemKind.Keyword,
				data: 19,
				documentation: 'Pass statement (no operation)'
			}
		];
	}
);

// This handler resolves additional information for the item selected in
// the completion list.
connection.onCompletionResolve(
	(item: CompletionItem): CompletionItem => {
		if (item.data === 1) {
			item.detail = 'print(value, sep=" ", end="\\n")';
			item.documentation = 'Print values to the output. Multiple values can be printed separated by the sep parameter.';
		} else if (item.data === 2) {
			item.detail = 'len(object)';
			item.documentation = 'Return the number of items in a container (string, list, etc.)';
		}
		return item;
	}
);

// Make the text document manager listen on the connection
// for open, change and close text document events
documents.listen(connection);

// Listen on the connection
connection.listen();

connection.console.log('Caesar Language Server started and listening...');
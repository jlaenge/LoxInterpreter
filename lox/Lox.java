package lox;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import lox.parse.ParseError;
import lox.parse.Parser;
import lox.pass.Interpreter;

public class Lox {
	
	private static Interpreter interpreter = new Interpreter();
	
	private static boolean hadError = false;
	private static boolean hadRuntimeError = false;

	public static void main(String[] args) throws IOException {
		
		if(args.length == 0) {
			runPrompt();
		} else if(args.length == 1) {
			runFile(args[0]);
		} else {
			System.out.println("Usage: jlox [script]");
			System.exit(64);
		}
		
	}
	
	private static void runFile(String path) throws IOException {
		byte[] bytes = Files.readAllBytes(Paths.get(path));
		run(new String(bytes, Charset.defaultCharset()));
		
		// Indicate an error in the exit code.
		if(hadError) System.exit(65);
		if(hadRuntimeError) System.exit(70);
	}
	
	private static void runPrompt() throws IOException {
		InputStreamReader input = new InputStreamReader(System.in);
		BufferedReader reader = new BufferedReader(input);
		
		String source = "";
		while(true) {
	
			if(source.isEmpty()) {
				System.out.print("> ");
			} else {
				System.out.print("\t");
			}
			
			String line = reader.readLine();
			if(line == null) break;
			
			source += line;
			Scanner scanner = new Scanner(source);
			List<Token> tokens = scanner.scanTokens();
			
			Parser parser = new Parser(tokens);
			ParseError parseError = null;
			try {
				parser.tryParse();
			} catch(ParseError err) {
				parseError = err;
			}
			
			if(parseError == null) {
				run(source);
				source = "";
			} else {
				switch(parseError.type) {
					case INCOMPLETE_PROGRAM:
						break;
					default:
						Lox.error(parseError.token, parseError.message);
						source = "";
				}
			}
			hadError = false;
	
		}
	}
	
	private static void run(String source) {
		
		Scanner scanner = new Scanner(source);
		List<Token> tokens = scanner.scanTokens();
		
		Parser parser = new Parser(tokens);
		List<Stmt> statements = parser.parse();
		if(hadError) return;
		
		interpreter.interpret(statements);
		
	}
	
	public static void error(int line, String message) {
		report(line, "", message);
	}
	public static void error(Token token, String message) {
		if(token.type == TokenType.EOF) {
			report(token.line, " at end", message);
		} else {
			report(token.line, " at '" + token.lexeme + "'", message);
		}
	}
	
	public static void runtimeError(RuntimeError err) {
		System.err.println(
			err.getMessage() + "\n[line " + err.token.line + "]"
		);
		hadRuntimeError = true;
	}
	
	private static void report(int line, String where, String message) {
		System.err.println("[line " + line + "] Error" + where + ": " + message);
		hadError = true;
	}
	
}

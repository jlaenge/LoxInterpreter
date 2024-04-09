package lox;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class Lox {
	
	private static boolean hadError = false;

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
	}
	
	private static void runPrompt() throws IOException {
		InputStreamReader input = new InputStreamReader(System.in);
		BufferedReader reader = new BufferedReader(input);
		
		while(true) {
			System.out.print("> ");
			String line = reader.readLine();
			if(line != null) {
				run(line);
				hadError = false;
			} else {
				break;
			}
		}
	}
	
	private static void run(String source) {
		
		Scanner scanner = new Scanner(source);
		List<Token> tokens = scanner.scanTokens();
		
		Parser parser = new Parser(tokens);
		Expr expression = parser.parse();
		if(hadError) return;
		
		Interpreter interpreter = new Interpreter();
		interpreter.interpret(expression);
		
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
		
	}
	
	private static void report(int line, String where, String message) {
		System.err.println("[line " + line + "] Error" + where + ": " + message);
		hadError = true;
	}
	
}

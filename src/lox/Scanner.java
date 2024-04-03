package lox;

import java.util.ArrayList;
import java.util.List;

import static lox.TokenType.*;

public class Scanner {
	
	private final String source;
	private final List<Token> tokens = new ArrayList<Token>();
	
	private int start = 0;
	private int current = 0;
	private int line = 1;
	
	public Scanner(String source) {
		this.source = source;
	}
	
	public List<Token> scanTokens() {
		
		while(!isAtEnd()) {
			// We are at the beginning of next lexeme.
			start = current;
			scanToken();
		}
		
		tokens.add(new Token(EOF, "", null, line));
		return tokens;
		
	}
	
	private boolean isAtEnd() {
		return (current >= source.length());
	}
	
	private void scanToken() {}
	
	

}

package lox;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static lox.TokenType.*;

public class Scanner {
	
	private static final Map<String, TokenType> keywords;
	static {
		keywords = new HashMap<String, TokenType>();
		keywords.put("and", AND);
		keywords.put("class", CLASS);
		keywords.put("else", ELSE);
		keywords.put("false", FALSE);
		keywords.put("for", FOR);
		keywords.put("fun", FUNCTION);
		keywords.put("if", IF);
		keywords.put("nil", NIL);
		keywords.put("or", OR);
		keywords.put("print", PRINT);
		keywords.put("return", RETURN);
		keywords.put("super", SUPER);
		keywords.put("this", THIS);
		keywords.put("true", TRUE);
		keywords.put("var", VAR);
		keywords.put("while", WHILE);
	}
	
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
	
	private void scanToken() {
		char c = advance();
		switch(c) {
		
			// Single-character tokens.
			case '(': addToken(LEFT_PARENTHESIS); break;
			case ')': addToken(RIGHT_PARENTHESIS); break;
			case '{': addToken(LEFT_BRACE); break;
			case '}': addToken(RIGHT_BRACE); break;
			case ',': addToken(COMMA); break;
			case '.': addToken(DOT); break;
			case '-': addToken(MINUS); break;
			case '+': addToken(PLUS); break;
			case ';': addToken(SEMICOLON); break;
			case '*': addToken(STAR); break;
			
			// One or two character tokens.
			case '!':
				if(match('=')) addToken(BANG_EQUAL);
				else addToken(BANG);
				break;
			case '=':
				if(match('=')) addToken(EQUAL_EQUAL);
				else addToken(EQUAL);
				break;
			case '<':
				if(match('=')) addToken(LESS_EQUAL);
				else addToken(LESS);
				break;
			case '>':
				if(match('=')) addToken(GREATER_EQUAL);
				else addToken(GREATER);
				break;
				
			// Comments.
			case '/':
				if(match('/')) {
					while(!isAtEnd() && peek() != '\n') advance();
				} else {
					addToken(SLASH);
				}
				break;
				
			// Whitespaces.
			case ' ':
			case '\r':
			case '\t':
				break;
				
			case '\n':
				line++;
				break;
				
			// String Literals.
			case '"': string(); break;
			
			// Literals, Keywords and Erros.
			default:
				
				// numbers
				if(isDigit(c)) number();
				
				// identifiers
				else if(isAlpha(c)) identifier();
				
				// errors
				else Lox.error(line, "Unexpected character: '" + c + "'.");
				break;
			
		}
	}
	
	private boolean isDigit(char c) {
		return '0' <= c && c <= '9';
	}
	private boolean isAlpha(char c) {
		if('a' <= c && c <= 'z') return true;
		if('A' <= c && c <= 'Z') return true;
		if(c == '_')             return true;
		return false;
	}
	private boolean isAlphaNumeric(char c) {
		return (isDigit(c) || isAlpha(c));
	}
	
	private void string() {
		while(!isAtEnd() && peek() != '"') {
			if(peek() == '\n') line++;
			advance();
		}
		
		if(isAtEnd()) {
			Lox.error(line, "Unterminated string.");
			return;
		}
		
		advance();
		String value = source.substring(start+1, current-1);
		addToken(STRING, value);
	}
	private void identifier() {
		while(isAlphaNumeric(peek())) advance();
		
		String text = source.substring(start, current);
		TokenType type = keywords.get(text);
		if(type == null) type = IDENTIFIER;
		addToken(type);
		
	}
	
	private void number() {
		while(isDigit(peek())) advance();
		
		if(peek() == '.' && isDigit(peekNext())) {
			advance();
			while(isDigit(peek())) advance();
		}
		
		addToken(NUMBER, Double.parseDouble(source.substring(start, current)));
	}
	
	private char advance() {
		assert(current < source.length());
		current++;
		return source.charAt(current - 1);
	}
	
	private void addToken(TokenType type) {
		addToken(type, null);
	}
	
	private void addToken(TokenType type, Object literal) {
		String text = source.substring(start, current);
		tokens.add(new Token(type, text, literal, line));
	}
	
	private boolean match(char expected) {
		if(isAtEnd()) return false;
		if(source.charAt(current) != expected) return false;
		
		current++;
		return true;
	}
	
	private char peek() {
		if(isAtEnd()) return '\0';
		return source.charAt(current);
	}
	private char peekNext() {
		if(current+1 >= source.length()) return '\0';
		return source.charAt(current+1);
	}

}

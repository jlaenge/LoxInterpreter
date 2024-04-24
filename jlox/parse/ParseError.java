package jlox.parse;

import jlox.Token;

public class ParseError extends RuntimeException {
	
	private static final long serialVersionUID = 1L;
	
	public ParseErrorType type;
	public Token token;
	public String message;
	
	public ParseError(ParseErrorType type, Token token, String message) {
		this.type = type;
		this.token = token;
		this.message = message;
	}
	
}

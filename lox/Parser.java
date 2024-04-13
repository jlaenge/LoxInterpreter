package lox;

import java.util.ArrayList;
import java.util.List;

import static lox.TokenType.*;

public class Parser {
	
	public static class ParseError extends RuntimeException {
		private static final long serialVersionUID = 1L;
	}

	private final List<Token> tokens;
	private int current = 0;
	
	public Parser(List<Token> tokens) {
		this.tokens = tokens;
	}
	
	public List<Stmt> parse() {
		List<Stmt> statements = new ArrayList<Stmt>();
		while(!isAtEnd()) {
			statements.add(declaration());
		}
		return statements;
	}
	
	private Stmt declaration() {
		try {
			if(match(VAR)) {
				return varDeclaration();
			} else {
				return statement();
			}
		} catch(ParseError err) {
			synchronize();
			return null;
		}
	}
	
	private Stmt varDeclaration() {
		Token name = consume(IDENTIFIER, "Expected name variable.");
		
		Expr initializer = null;
		if(match(EQUAL)) {
			initializer = expression();
		}
		
		consume(SEMICOLON, "Expected ';' after variable declaration.");
		return new Stmt.Var(name, initializer);
	}
	
	private Stmt statement() {
		if(match(PRINT)) {
			return printStatement();
		}
		return expressionStatement();
	}
	
	private Stmt printStatement() {
		Expr expr = expression();
		consume(SEMICOLON, "Expected ';' after value.");
		return new Stmt.Print(expr);
	}
	
	private Stmt expressionStatement() {
		Expr expr = expression();
		consume(SEMICOLON, "Expected ';' after expression.");
		return new Stmt.Expression(expr);
	}
	
	private Expr expression() {
		return equality();
	}
	
	private Expr equality() {
		Expr expr = comparison();
		
		while(match(BANG_EQUAL, EQUAL_EQUAL)) {
			Token operator = previous();
			Expr right = comparison();
			expr = new Expr.Binary(expr, operator, right);
		}
		
		return expr;
	}
	
	private Expr comparison() {
		Expr expr = term();
		
		while(match(LESS, LESS_EQUAL, GREATER, GREATER_EQUAL)) {
			Token operator = previous();
			Expr right = term();
			expr = new Expr.Binary(expr, operator, right);
		}
		
		return expr;
	}
	
	private Expr term() {
		Expr expr = factor();
		
		while(match(MINUS, PLUS)) {
			Token operator = previous();
			Expr right = factor();
			expr = new Expr.Binary(expr, operator, right);
		}
		
		return expr;
	}
	
	private Expr factor() {
		Expr expr = unary();
		
		while(match(SLASH, STAR)) {
			Token operator = previous();
			Expr right = unary();
			expr = new Expr.Binary(expr, operator, right);
		}
		
		return expr;
	}
	
	private Expr unary() {
		
		if(match(BANG, MINUS)) {
			Token operator = previous();
			Expr right = unary();
			return new Expr.Unary(operator, right);
		} else {
			return primary();
		}
		
	}
	
	private Expr primary() {
		if(match(FALSE)) return new Expr.Literal(false);
		if(match(TRUE))  return new Expr.Literal(true);
		if(match(NIL))   return new Expr.Literal(null);
		
		if(match(NUMBER, STRING)) {
			return new Expr.Literal(previous().literal);
		}
		
		if(match(IDENTIFIER)) {
			return new Expr.Variable(previous());
		}
		
		if(match(LEFT_PARENTHESIS)) {
			Expr expr = expression();
			consume(RIGHT_PARENTHESIS, "Expected ')' after expression.");
			return new Expr.Grouping(expr);
		}
		
		throw error(peek(), "Expected expression.");
	}
	
	private ParseError error(Token token, String message) {
		Lox.error(token, message);
		return new ParseError();
	}
	
	private void synchronize() {
		advance();
		
		while(!isAtEnd()) {
			if(previous().type == SEMICOLON) return;
			
			switch(peek().type) {
				case CLASS:
				case FUNCTION:
				case VAR:
				case FOR:
				case IF:
				case PRINT:
				case RETURN:
					return;
				default:
			}
			
			advance();
		}
	}
	
	private boolean match(TokenType... types) {
		for(TokenType type : types) {
			if(check(type)) {
				advance();
				return true;
			}
		}
		return false;
	}
	
	private Token consume(TokenType type, String message) {
		if(check(type)) {
			return advance();
		} else {
			throw error(peek(), message);
		}
	}
		
	private Token advance() {
		assert(current + 1 < tokens.size());
		assert(!isAtEnd());
		current++;
		return previous();
	}
	
	private boolean check(TokenType type) {
		return (peek().type == type);
	}
	
	private boolean isAtEnd() {
		return (peek().type == EOF);
	}
	
	private Token peek() {
		return tokens.get(current);
	}
	
	private Token previous() {
		assert(current > 0);
		return tokens.get(current - 1);
	}
	
}

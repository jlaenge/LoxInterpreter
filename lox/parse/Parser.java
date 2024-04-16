package lox.parse;

import java.util.ArrayList;
import java.util.List;

import lox.Expr;
import lox.Stmt;
import lox.Token;
import lox.TokenType;

import static lox.TokenType.*;

public class Parser {

	private final List<Token> tokens;
	private int current = 0;
	
	private ParseErrorHandler errorHandler;
	
	public Parser(List<Token> tokens) {
		this.tokens = tokens;
		this.errorHandler = ParseErrorHandler.get(ParseErrorHandler.Type.SYNCHRONIZE);
	}

	public List<Stmt> tryParse() {
		errorHandler = ParseErrorHandler.get(ParseErrorHandler.Type.THROW);
		return parse();
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
			errorHandler.handleError(this, err);
			return null;
		}
	}
	
	private Stmt varDeclaration() {
		Token name = consume(IDENTIFIER, "Expected name variable.");
		
		consume(EQUAL, "Expected '=' after variable declaration.");
		Expr initializer = expression();
		
		consume(SEMICOLON, "Expected ';' after variable definition.");
		return new Stmt.Var(name, initializer);
	}
	
	private Stmt statement() {
		if(match(LEFT_BRACE)) {
			return blockStatement();
		} else if(match(IF)) {
			return ifStatement();
		} else if(match(PRINT)) {
			return printStatement();
		}
		return expressionStatement();
	}
	
	private Stmt blockStatement() {
		List<Stmt> statements = new ArrayList<Stmt>();
		
		while(!check(RIGHT_BRACE) && !isAtEnd()) {
			statements.add(declaration());
		}
		
		consume(RIGHT_BRACE, "Expected '}' after block.");
		return new Stmt.Block(statements);
	}
	
	private Stmt ifStatement() {
		consume(LEFT_PARENTHESIS, "Expected '(' after 'if'.");
		Expr condition = expression();
		consume(RIGHT_PARENTHESIS, "Expected')' after condition.");
		
		Stmt thenBranch = statement();
		Stmt elseBranch = null;
		if(match(ELSE)) {
			elseBranch = statement();
		}
		
		return new Stmt.If(condition, thenBranch, elseBranch);
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
		return assignment();
	}
	
	private Expr assignment() {
		Expr expr = equality();
		
		if(match(EQUAL)) {
			Token equals = previous();
			Expr value = expression();
			
			if(expr instanceof Expr.Variable) {
				Token name = ((Expr.Variable)expr).name;
				return new Expr.Assign(name, value);
			}
			
			error(equals, "Invalid lhs of assignment.");
		}
		
		return expr;
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
		ParseErrorType type = ParseErrorType.GENERAL;
		if(isAtEnd()) {
			type = ParseErrorType.INCOMPLETE_PROGRAM;
		}
		return new ParseError(type, token, message);
	}
	
	public void synchronize() {
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

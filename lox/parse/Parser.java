package lox.parse;

import java.util.ArrayList;
import java.util.Arrays;
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
			if(match(FUNCTION)) {
				return funDeclaration("function");
			} else if(match(VAR)) {
				return varDeclaration();
			} else {
				return statement();
			}
		} catch(ParseError err) {
			errorHandler.handleError(this, err);
			return null;
		}
	}
	
	private Stmt funDeclaration(String kind) {
		Token name = consume(IDENTIFIER, "Expected " + kind + " name.");
		
		List<Token> parameters = new ArrayList<Token>();
		consume(LEFT_PARENTHESIS, "Expected '(' after " + kind + " name.");
		if(!check(RIGHT_PARENTHESIS)) {
			if(parameters.size() >= 255) {
				error(peek(), "Exceeded parameter limit of '255'.");
			}
			do {
				Token parameter = consume(IDENTIFIER, "Expected parameter name.");
				parameters.add(parameter);
			} while(match(COMMA));
		}
		consume(RIGHT_PARENTHESIS, "Expected ')' after parameters.");
		
		consume(LEFT_BRACE, "Expected '{' before " + kind + " body.");
		List<Stmt> body = block();
		
		return new Stmt.Function(name, parameters, body);
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
		} else if(match(FOR)) {
			return forStatement();
		} else if(match(IF)) {
			return ifStatement();
		} else if(match(WHILE)) {
			return whileStatement();
		} else if(match(PRINT)) {
			return printStatement();
		} else if(match(RETURN)) {
			return returnStatement();
		}
		return expressionStatement();
	}
	
	private Stmt blockStatement() {
		return new Stmt.Block(block());
	}
	
	private List<Stmt> block() {
		List<Stmt> statements = new ArrayList<Stmt>();
		
		while(!check(RIGHT_BRACE) && !isAtEnd()) {
			statements.add(declaration());
		}
		
		consume(RIGHT_BRACE, "Expected '}' after block.");
		return statements;
	}
	
	private Stmt forStatement() {
		consume(LEFT_PARENTHESIS, "Expected '(' after 'for'.");
		
		Stmt initializer = null;
		if(!match(SEMICOLON)) {
			if(match(VAR)) {
				initializer = varDeclaration();
			} else {
				initializer = expressionStatement();
			}
		}
		
		Expr condition = new Expr.Literal(true);
		if(!check(SEMICOLON)) {
			condition = expression();
		}
		consume(SEMICOLON, "Expected ';' after loop condition.");
		
		Expr increment = null;
		if(!check(RIGHT_PARENTHESIS)) {
			increment = expression();
		}
		consume(RIGHT_PARENTHESIS, "Expected ')' after for clauses.");
		
		Stmt body = statement();
		
		if(increment != null) {
			body = new Stmt.Block(Arrays.asList(
				body,
				new Stmt.Expression(increment)
			));
		}
		body = new Stmt.While(condition, body);
		if(initializer != null) {
			body = new Stmt.Block(Arrays.asList(
				initializer,
				body
			));
		}
		
		return body;
		
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
	
	private Stmt whileStatement() {
		consume(LEFT_PARENTHESIS, "Expected '(' after 'while'.");
		Expr condition = expression();
		consume(RIGHT_PARENTHESIS, "Expected ')' after 'condition'.");
		
		Stmt body = statement();
		return new Stmt.While(condition, body);
	}
	
	private Stmt printStatement() {
		Expr expr = expression();
		consume(SEMICOLON, "Expected ';' after value.");
		return new Stmt.Print(expr);
	}
	
	private Stmt returnStatement() {
		Token keyword = previous();
		Expr expr = null;
		if(!check(SEMICOLON)) {
			expr = expression();
		}
		consume(SEMICOLON, "Expected ';' after return.");
		return new Stmt.Return(keyword, expr);
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
		Expr expr = or();
		
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
	
	private Expr or() {
		Expr expr = and();
		
		while(match(OR)) {
			Token operator = previous();
			Expr rhs = and();
			expr = new Expr.Logical(expr, operator, rhs);
		}
		
		return expr;
	}
	
	private Expr and() {
		Expr expr = equality();
		
		while(match(AND)) {
			Token operator = previous();
			Expr rhs = equality();
			expr = new Expr.Logical(expr, operator, rhs);
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
			return call();
		}
		
	}
	
	private Expr call() {
		Expr expr = primary();
		
		while(match(LEFT_PARENTHESIS)) {
			expr = finishCall(expr);
		}
		
		return expr;
	}
	
	private Expr finishCall(Expr callee) {
		
		List<Expr> arguments = new ArrayList<Expr>();
		if(!check(RIGHT_PARENTHESIS)) {
			do {
				if(arguments.size() >= 255) {
					error(peek(), "Maximum argument count of '255' exceeded.");
				}
				Expr argument = expression();
				arguments.add(argument);
			} while(match(COMMA));
		}
		
		Token paren = consume(RIGHT_PARENTHESIS, "Expected ')' after arguments.");
		
		return new Expr.Call(callee, paren, arguments);
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

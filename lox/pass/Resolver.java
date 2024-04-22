package lox.pass;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;

import lox.Expr;
import lox.Expr.Assign;
import lox.Expr.Binary;
import lox.Expr.Call;
import lox.Expr.Grouping;
import lox.Expr.Literal;
import lox.Expr.Logical;
import lox.Expr.Unary;
import lox.Expr.Variable;
import lox.Lox;
import lox.Stmt;
import lox.Stmt.Block;
import lox.Stmt.Class;
import lox.Stmt.Expression;
import lox.Stmt.Function;
import lox.Stmt.If;
import lox.Stmt.Print;
import lox.Stmt.Return;
import lox.Stmt.Var;
import lox.Stmt.While;
import lox.Token;

public class Resolver implements Expr.Visitor<Void>, Stmt.Visitor<Void> {
	
	private final Interpreter interpreter;
	private final Stack<Map<String, Boolean>> scopes = new Stack<Map<String,Boolean>>();
	private FunctionType currentFunction = FunctionType.NONE;
	
	private enum FunctionType {
		NONE,
		FUNCTION
	}
	
	public Resolver(Interpreter interpreter) {
		this.interpreter = interpreter;
	}
	
	public void resolve(List<Stmt> statements) {
		resolveStatements(statements);
	}

	@Override
	public Void visitStmtBlock(Block stmt) {
		beginScope();
		resolveStatements(stmt.statements);
		endScope();
		return null;
	}
	
	@Override
	public Void visitStmtVar(Var stmt) {
		declare(stmt.name);
		if(stmt.initializer != null) {
			resolve(stmt.initializer);
		}
		define(stmt.name);
		return null;
	}
	
	@Override
	public Void visitExprVariable(Variable expr) {
		if(!scopes.isEmpty()) {
			Map<String, Boolean> scope = scopes.peek();
			if(scope.get(expr.name.lexeme) == Boolean.FALSE) {
				Lox.error(expr.name, "Can't read local variable in it's own initializer.");
			}
		}
		
		resolveLocal(expr, expr.name);
		return null;
	}
	
	@Override
	public Void visitExprAssign(Assign expr) {
		resolve(expr.expression);
		resolveLocal(expr.expression, expr.name);
		return null;
	}


	@Override
	public Void visitStmtClass(Class stmt) {
		declare(stmt.name);
		define(stmt.name);
		return null;
	}
	
	@Override
	public Void visitStmtFunction(Function stmt) {
		declare(stmt.name);
		define(stmt.name);
		resolveFunction(stmt, FunctionType.FUNCTION);
		return null;
	}

	@Override
	public Void visitStmtExpression(Expression stmt) {
		resolve(stmt.expression);
		return null;
	}
	@Override
	public Void visitStmtIf(If stmt) {
		resolve(stmt.condition);
		resolve(stmt.thenBranch);
		if(stmt.elseBranch != null) {
			resolve(stmt.elseBranch);
		}
		return null;
	}
	@Override
	public Void visitStmtPrint(Print stmt) {
		resolve(stmt.expression);
		return null;
	}
	@Override
	public Void visitStmtReturn(Return stmt) {
		
		if(currentFunction == FunctionType.NONE) {
			Lox.error(stmt.keyword, "Return statement outside of function or method.");
		}
		if(stmt.expression != null) {
			resolve(stmt.expression);
		}
		return null;
		
	}
	@Override
	public Void visitStmtWhile(While stmt) {
		resolve(stmt.condition);
		resolve(stmt.body);
		return null;
	}
	@Override
	public Void visitExprBinary(Binary expr) {
		resolve(expr.left);
		resolve(expr.right);
		return null;
	}
	@Override
	public Void visitExprCall(Call expr) {
		resolve(expr.callee);
		resolveExpressions(expr.arguments);
		return null;
	}
	@Override
	public Void visitExprGrouping(Grouping expr) {
		resolve(expr.expression);
		return null;
	}
	@Override
	public Void visitExprLiteral(Literal expr) {
		return null;
	}
	@Override
	public Void visitExprLogical(Logical expr) {
		resolve(expr.left);
		resolve(expr.right);
		return null;
	}
	@Override
	public Void visitExprUnary(Unary expr) {
		resolve(expr.right);
		return null;
	}

	private void resolveStatements(List<Stmt> statements) {
		for(Stmt statement : statements) {
			resolve(statement);
		}
	}
	private void resolve(Stmt statement) {
		statement.accept(this);
	}
	private void resolveExpressions(List<Expr> expressions) {
		for(Expr expression : expressions) {
			resolve(expression);
		}
	}
	private void resolve(Expr expression) {
		expression.accept(this);
	}
	private void resolveLocal(Expr expression, Token name) {
		for(int i=scopes.size()-1; i>=0; i--) {
			Map<String, Boolean> scope = scopes.get(i);
			if(scope.containsKey(name.lexeme)) {
				interpreter.resolve(expression, scopes.size() - i - 1);
				return;
			}
		}
	}
	private void resolveFunction(Function function, FunctionType functionType) {
		
		FunctionType encolsingFunction = currentFunction;
		currentFunction = functionType;
		beginScope();
		
		for(Token parameter : function.parameters) {
			declare(parameter);
			define(parameter);
		}
		resolveStatements(function.body);
		
		endScope();
		currentFunction = encolsingFunction;
		
	}
	
	private void beginScope() {
		scopes.push(new HashMap<String, Boolean>());
	}
	private void endScope() {
		scopes.pop();
	}
	
	private void declare(Token name) {
		if(!scopes.isEmpty()) {
			
			Map<String, Boolean> scope = scopes.peek();
			if(scope.containsKey(name.lexeme)) {
				Lox.error(name, "Redeclaration of variable.");
			}
			
			scope.put(name.lexeme, false);
			
		}
	}
	private void define(Token name) {
		if(!scopes.isEmpty()) {
			Map<String, Boolean> scope = scopes.peek();
			scope.put(name.lexeme, true);
		}
	}

}

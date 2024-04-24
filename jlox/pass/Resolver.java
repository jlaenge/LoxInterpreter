package jlox.pass;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;

import jlox.Expr;
import jlox.Lox;
import jlox.Stmt;
import jlox.Token;
import jlox.Expr.Assign;
import jlox.Expr.Binary;
import jlox.Expr.Call;
import jlox.Expr.Get;
import jlox.Expr.Grouping;
import jlox.Expr.Literal;
import jlox.Expr.Logical;
import jlox.Expr.Set;
import jlox.Expr.Super;
import jlox.Expr.This;
import jlox.Expr.Unary;
import jlox.Expr.Variable;
import jlox.Stmt.Block;
import jlox.Stmt.Class;
import jlox.Stmt.Expression;
import jlox.Stmt.Function;
import jlox.Stmt.If;
import jlox.Stmt.Print;
import jlox.Stmt.Return;
import jlox.Stmt.Var;
import jlox.Stmt.While;

public class Resolver implements Expr.Visitor<Void>, Stmt.Visitor<Void> {
	
	private final Interpreter interpreter;
	private final Stack<Map<String, Boolean>> scopes = new Stack<Map<String,Boolean>>();
	
	private FunctionType currentFunction = FunctionType.NONE;
	private ClassType currentClass = ClassType.NONE;
	private enum FunctionType {
		NONE,
		FUNCTION,
		INITIALIZER,
		METHOD
	}
	
	private enum ClassType {
		NONE,
		CLASS,
		SUBCLASS
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
		
		ClassType enclosingClass = currentClass;
		currentClass = (stmt.superclass == null) ? ClassType.CLASS : ClassType.SUBCLASS;
		declare(stmt.name);
		define(stmt.name);
		
		if(stmt.superclass != null) {
			if(stmt.superclass.name.lexeme.equals(stmt.name.lexeme)) {
				Lox.error(stmt.superclass.name, "A class can not inherit itself.");
			}
			beginScope();
			scopes.peek().put("super", true);
			resolve(stmt.superclass);
		}
	
		beginScope();
		scopes.peek().put("this", true);
		for(Function method : stmt.methods) {
			boolean isInitializer = method.name.lexeme.equals("init");
			FunctionType type = isInitializer ? FunctionType.INITIALIZER : FunctionType.METHOD;
			resolveFunction(method, type);
		}
		endScope();
		if(stmt.superclass != null) {
			endScope();
		}
		currentClass = enclosingClass;
		
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

		switch(currentFunction) {
		
			case NONE:
				Lox.error(stmt.keyword, "Return statement outside of function or method.");
				break;
				
			case INITIALIZER:
				if(stmt.expression != null) {
					Lox.error(stmt.keyword, "Return statement in constructor not allowed.");
				}
				break;
				
			default:
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
	public Void visitExprGet(Get expr) {
		resolve(expr.object);
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
	public Void visitExprSet(Set expr) {
		resolve(expr.object);
		resolve(expr.value);
		return null;
	}
	
	@Override
	public Void visitExprSuper(Super expr) {
		switch(currentClass) {
			case NONE:
				Lox.error(expr.keyword, "Can not use 'super' outside of class.");
				break;
			case CLASS:
				Lox.error(expr.keyword, "Class has no superclass.");
				break;
			default:
		}
		resolveLocal(expr, expr.keyword);
		return null;
	}

	@Override
	public Void visitExprThis(This expr) {
		if(currentClass == ClassType.NONE) {
			Lox.error(expr.keyword, "The 'this' keyword can only be used within a class.");
		}
		resolveLocal(expr, expr.keyword);
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

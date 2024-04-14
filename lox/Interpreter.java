package lox;

import java.util.List;

import lox.Expr.*;
import lox.Stmt.*;

public class Interpreter implements Expr.Visitor<Object>, Stmt.Visitor<Void> {
	
	private Environment environment = new Environment();
	
	public void interpret(List<Stmt> statements) {
		try {
			for(Stmt statement : statements) {
				execute(statement);
			}
		} catch(RuntimeError err) {
			Lox.runtimeError(err);
		}
	}


	@Override
	public Void visitStmtBlock(Block stmt) {
		executeBlock(stmt.statements, new Environment(environment));
		return null;
	}

	
	@Override
	public Void visitStmtExpression(Expression stmt) {
		evaluate(stmt.expression);
		return null;
	}

	@Override
	public Void visitStmtPrint(Print stmt) {
		Object value = evaluate(stmt.expression);
		System.out.println(stringify(value));
		return null;
	}
	
	@Override
	public Void visitStmtVar(Var stmt) {
		
		Object value = null;
		if(stmt.initializer != null) {
			value = evaluate(stmt.initializer);
		}
		
		environment.define(stmt.name.lexeme, value);
		return null;
	}
	

	@Override
	public Object visitExprAssign(Assign expr) {
		Object value = evaluate(expr.expression);
		environment.assign(expr.name, value);
		return value;
	}
	
	@Override
	public Object visitExprBinary(Binary expr) {
		Object left = evaluate(expr.left);
		Object right = evaluate(expr.right);
		switch(expr.operator.type) {
		
			// boolean
			case LESS:
				return isLess(left, right);
			case LESS_EQUAL:
				return isLess(left, right) || isEqual(left, right);
			case EQUAL_EQUAL:
				return isEqual(left, right);
			case BANG_EQUAL:
				return !isEqual(left, right);
			case GREATER_EQUAL:
				return isLess(right, left) || isEqual(right, left);
			case GREATER:
				return isLess(right, left);
			
			// arithmetic
			case MINUS:
				checkNumberOperands(expr.operator, left, right);
				return (double)left - (double)right;
			case PLUS:
				if(left instanceof Double && right instanceof Double) {
					return (double)left + (double)right;
				}
				if(left instanceof String && right instanceof String) {
					return (String)left + (String)right;
				}
				throw new RuntimeError(expr.operator, "Operands must be two numbers or two strings.");
			case SLASH:
				checkNumberOperands(expr.operator, left, right);
				if((double)right == 0) {
					throw new RuntimeError(expr.operator, "Division by 0.");
				}
				return (double)left / (double)right;
			case STAR:
				checkNumberOperands(expr.operator, left, right);
				return (double)left * (double)right;
			default:
				break;
		}
		return null;
	}

	@Override
	public Object visitExprGrouping(Grouping expr) {
		return evaluate(expr.expression);
	}

	@Override
	public Object visitExprLiteral(Literal expr) {
		return expr.value;
	}

	@Override
	public Object visitExprUnary(Unary expr) {
		Object right = evaluate(expr.right);
		switch(expr.operator.type) {
			case BANG:
				return isTruthy(right);
			case MINUS:
				checkNumberOperand(expr.operator, right);
				return -(double)right;
			default:
				break;
		}
		return null;
	}
	

	@Override
	public Object visitExprVariable(Variable expr) {
		return environment.get(expr.name);
	}
	
	private void checkNumberOperand(Token operator, Object operand) {
		if(operand instanceof Double) return;
		throw new RuntimeError(operator, "Operand must be a number.");
	}
	private void checkNumberOperands(Token operator, Object left, Object right) {
		if(left instanceof Double && right instanceof Double) return;
		throw new RuntimeError(operator, "Operands must be a numbers.");
	}
	
	private boolean isLess(Object left, Object right) {
		if(left == null || right == null) {
			return (left == null && right == null);
		}
		if(left instanceof Boolean && right instanceof Boolean) {
			return !(boolean)left && (boolean)right;
		}
		if(left instanceof Double && right instanceof Double) {
			return (double)left < (double)right;
		}
		if(left instanceof String && right instanceof String) {
			return ((String)left).compareTo((String)right) < 0;
		}
		return false;
	}
	private boolean isEqual(Object left, Object right) {
		if(left == null) {
			return (right == null);
		}
		if(right == null) {
			return false;
		}
		return left.equals(right);
	}
	
	private boolean isTruthy(Object object) {
		if(object == null) return false;
		if(object instanceof Boolean) return (boolean)object;
		return false;
	}
	
	private void execute(Stmt stmt) {
		stmt.accept(this);
	}
	private void executeBlock(List<Stmt> statements, Environment environment) {
		Environment previous = this.environment;
		
		try {
			this.environment = environment;
			for(Stmt statement : statements) {
				execute(statement);
			}
		} finally {
			this.environment = previous;
		}
	}
	
	private Object evaluate(Expr expr) {
		return expr.accept(this);
	}
	
	public String stringify(Object value) {
		if(value == null) return "nil";
		if(value instanceof Double) {
			String text = value.toString();
			if(text.endsWith(".0")) {
				return text.substring(0, text.length()-2);
			}
		}
		return value.toString();
	}

}

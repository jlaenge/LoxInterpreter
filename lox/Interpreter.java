package lox;

import lox.Expr.Binary;
import lox.Expr.Grouping;
import lox.Expr.Literal;
import lox.Expr.Unary;

public class Interpreter implements Expr.Visitor<Object> {
	
	public void interpret(Expr expr) {
		try {
			Object value = evaluate(expr);
			System.out.println(stringify(value));
		} catch(RuntimeError err) {
			Lox.runtimeError(err);
		}
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

package lox;

import lox.Expr.Assign;
import lox.Expr.Binary;
import lox.Expr.Grouping;
import lox.Expr.Literal;
import lox.Expr.Unary;
import lox.Expr.Variable;

public class ASTPrinter implements Expr.Visitor<String> {
	
	public static void main(String[] args) {
		
		// -123 * (45.67)
		Expr expression = new Expr.Binary(
			new Expr.Unary(
				new Token(TokenType.MINUS, "-", null, 1),
				new Expr.Variable(new Token(TokenType.IDENTIFIER, "tmp", null, 1))
			),
			new Token(TokenType.STAR, "*", null, 1),
			new Expr.Grouping(
				new Expr.Literal(45.67)
			)
		);
		System.out.println(new ASTPrinter().print(expression));
		
	}
	
	public String print(Expr expr) {
		return expr.accept(this);
	}

	@Override
	public String visitExprAssign(Assign expr) {
		return parenthesize(expr.name.lexeme, expr.expression);
	}
	
	@Override
	public String visitExprBinary(Binary expr) {
		return parenthesize(expr.operator.lexeme, expr.left, expr.right);
	}

	@Override
	public String visitExprGrouping(Grouping expr) {
		return parenthesize("group", expr.expression);
	}

	@Override
	public String visitExprLiteral(Literal expr) {
		if(expr.value == null) return "nil";
		return expr.value.toString();
	}

	@Override
	public String visitExprUnary(Unary expr) {
		return parenthesize(expr.operator.lexeme, expr.right);
	}
	
	private String parenthesize(String name, Expr... exprs) {
		StringBuilder builder = new StringBuilder();
		
		builder.append("(");
		builder.append(name);
		for(Expr expr : exprs) {
			builder.append(" ");
			builder.append(expr.accept(this));
		}
		builder.append(")");
		
		return builder.toString();
	}

	@Override
	public String visitExprVariable(Variable expr) {
		return expr.name.lexeme;
	}

}

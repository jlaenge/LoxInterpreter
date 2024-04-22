package lox;

import java.util.Arrays;
import java.util.List;

import lox.Expr.Assign;
import lox.Expr.Binary;
import lox.Expr.Call;
import lox.Expr.Get;
import lox.Expr.Grouping;
import lox.Expr.Literal;
import lox.Expr.Logical;
import lox.Expr.Set;
import lox.Expr.This;
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
		return parenthesize(expr.name.lexeme, Arrays.asList(expr.expression));
	}
	
	@Override
	public String visitExprBinary(Binary expr) {
		return parenthesize(expr.operator.lexeme, Arrays.asList(expr.left, expr.right));
	}
	
	@Override
	public String visitExprCall(Call expr) {
		String callee = expr.callee.accept(this);
		return parenthesize(callee, expr.arguments);
	}
	
	@Override
	public String visitExprGet(Get expr) {
		return parenthesize(".", expr.object, expr.name);
	}

	@Override
	public String visitExprGrouping(Grouping expr) {
		return parenthesize("group", Arrays.asList(expr.expression));
	}

	@Override
	public String visitExprLiteral(Literal expr) {
		if(expr.value == null) return "nil";
		return expr.value.toString();
	}
	
	@Override
	public String visitExprLogical(Logical expr) {
		return parenthesize(expr.operator.lexeme, Arrays.asList(expr.left, expr.right));
	}
	

	@Override
	public String visitExprSet(Set expr) {
		return parenthesizeStrings("=", Arrays.asList(
			parenthesize(".", expr.object, expr.name),
			expr.value.accept(this)
		));
	}
	
	@Override
	public String visitExprThis(This expr) {
		return "this";
	}

	@Override
	public String visitExprUnary(Unary expr) {
		return parenthesize(expr.operator.lexeme, Arrays.asList(expr.right));
	}
	
	private String parenthesize(String name, List<Expr> exprs) {
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
	private String parenthesize(String name, Expr expr, Token token) {
		StringBuilder builder = new StringBuilder();
		
		builder.append("(");
		builder.append(name);
		builder.append(" ");
		builder.append(expr.accept(this));
		builder.append(" ");
		builder.append(token.literal);
		builder.append(")");
		
		return builder.toString();
	}
	private String parenthesizeStrings(String name, List<String> subStrings) {
		StringBuilder builder = new StringBuilder();
		
		builder.append("(");
		builder.append(name);
		for(String subString : subStrings) {
			builder.append(" ");
			builder.append(subString);
		}
		builder.append(")");
		
		return builder.toString();
	}

	@Override
	public String visitExprVariable(Variable expr) {
		return expr.name.lexeme;
	}

}

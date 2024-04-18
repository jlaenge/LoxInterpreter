package lox.callable;

import java.util.List;

import lox.Environment;
import lox.Interpreter;
import lox.Stmt;

public class LoxFunction implements LoxCallable {
	
	private Stmt.Function function;
	
	public LoxFunction(Stmt.Function function) {
		this.function = function;
	}

	@Override
	public Object call(Interpreter interpreter, List<Object> arguments) {
		Environment environment = new Environment(interpreter.globals);
		for(int i=0; i<arity(); i++) {
			environment.define(function.parameters.get(i).lexeme, arguments.get(i));
		}
		interpreter.executeBlock(function.body, environment);
		return null;
	}

	@Override
	public int arity() {
		return function.parameters.size();
	}
	
	@Override
	public String toString() {
		return "<fn" + function.name.lexeme + ">";
	}

}

package lox.callable;

import java.util.List;

import lox.Environment;
import lox.Interpreter;
import lox.Stmt;
import lox.error.Return;

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
		
		try {
			interpreter.executeBlock(function.body, environment);
		} catch (Return error) {
			return error.value;
		}
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

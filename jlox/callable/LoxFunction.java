package jlox.callable;

import java.util.List;

import jlox.Environment;
import jlox.LoxClassInstance;
import jlox.Stmt;
import jlox.error.Return;
import jlox.pass.Interpreter;

public class LoxFunction implements LoxCallable {
	
	private final Stmt.Function function;
	private final Environment closure;
	
	private final boolean isInitializer;
	
	public LoxFunction(Stmt.Function function, Environment closure, boolean isInitializer) {
		this.function = function;
		this.closure = closure;
		this.isInitializer = isInitializer;
	}

	@Override
	public Object call(Interpreter interpreter, List<Object> arguments) {
		
		Environment environment = new Environment(closure);
		for(int i=0; i<arity(); i++) {
			environment.define(function.parameters.get(i).lexeme, arguments.get(i));
		}
		
		try {
			interpreter.executeBlock(function.body, environment);
		} catch (Return error) {
			return error.value;
		}
		
		if(isInitializer) {
			return closure.getAt(0, "this");
		}
		
		return null;
		
	}

	@Override
	public int arity() {
		return function.parameters.size();
	}
	
	public LoxFunction bind(LoxClassInstance instance) {
		Environment environment = new Environment(closure);
		environment.define("this", instance);
		return new LoxFunction(function, environment, isInitializer);
	}
	
	@Override
	public String toString() {
		return "<fn" + function.name.lexeme + ">";
	}

}

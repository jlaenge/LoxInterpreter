package lox.callable;

import java.util.List;

import lox.pass.Interpreter;

public interface LoxCallable {

	public Object call(Interpreter interpreter, List<Object> arguments);
	
	public int arity();
	
}

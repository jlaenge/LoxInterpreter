package jlox.callable;

import java.util.List;

import jlox.pass.Interpreter;

public interface LoxCallable {

	public Object call(Interpreter interpreter, List<Object> arguments);
	
	public int arity();
	
}

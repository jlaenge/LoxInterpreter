package lox.callable;

import java.util.List;

import lox.LoxClassInstance;
import lox.pass.Interpreter;

public class LoxClass implements LoxCallable {
	
	private final String name;

	public LoxClass(String name) {
		this.name = name;
	}
	
	@Override
	public Object call(Interpreter interpreter, List<Object> arguments) {
		LoxClassInstance instance = new LoxClassInstance(this);
		return instance;
	}

	@Override
	public int arity() {
		return 0;
	}
	
	@Override
	public String toString() {
		return "<cl " + name + ">";
	}

}

package lox.callable;

import java.util.List;
import java.util.Map;

import lox.LoxClassInstance;
import lox.Token;
import lox.pass.Interpreter;

public class LoxClass implements LoxCallable {
	
	private final String name;
	private final Map<String, LoxFunction> methods;

	public LoxClass(String name, Map<String, LoxFunction> methods) {
		this.name = name;
		this.methods = methods;
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
	
	public LoxFunction findMethod(Token name) {
		if(methods.containsKey(name.lexeme)) {
			return methods.get(name.lexeme);
		}
		return null;
	}
	
	@Override
	public String toString() {
		return "<cl " + name + ">";
	}

}

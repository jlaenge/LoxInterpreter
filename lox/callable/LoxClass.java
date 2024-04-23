package lox.callable;

import java.util.List;
import java.util.Map;

import lox.LoxClassInstance;
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
		LoxFunction initializer = findMethod("init");
		if(initializer != null) {
			initializer.bind(instance).call(interpreter, arguments);
		}
		return instance;
	}

	@Override
	public int arity() {
		LoxFunction initializer = findMethod("init");
		if(initializer != null) {
			return initializer.arity();
		} else {
			return 0;
		}
	}
	
	public LoxFunction findMethod(String name) {
		if(methods.containsKey(name)) {
			return methods.get(name);
		}
		return null;
	}
	
	@Override
	public String toString() {
		return "<cl " + name + ">";
	}

}

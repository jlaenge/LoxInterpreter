package jlox.callable;

import java.util.List;
import java.util.Map;

import jlox.LoxClassInstance;
import jlox.pass.Interpreter;

public class LoxClass implements LoxCallable {
	
	private final String name;
	private final LoxClass superclass;
	private final Map<String, LoxFunction> methods;

	public LoxClass(String name, LoxClass superclass, Map<String, LoxFunction> methods) {
		this.name = name;
		this.superclass = superclass;
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
		} else if(superclass != null) {
			return superclass.findMethod(name);
		}
		return null;
	}
	
	@Override
	public String toString() {
		return "<cl " + name + ">";
	}

}

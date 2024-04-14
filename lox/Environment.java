package lox;

import java.util.HashMap;
import java.util.Map;

public class Environment {

	private final Environment enclosing;
	private final Map<String, Object> values = new HashMap<String, Object>();
	
	public Environment() {
		enclosing = null;
	}
	public Environment(Environment enclosing) {
		this.enclosing = enclosing;
	}
	
	public void define(String name, Object value) {
		values.put(name, value);
	}
	
	public Object get(Token name) {
		if(values.containsKey(name.lexeme)) {
			return values.get(name.lexeme);
		} else if(enclosing != null) {
			return enclosing.get(name);
		}
		throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
	}
	
	public void assign(Token name, Object value) {
		if(values.containsKey(name.lexeme)) {
			values.put(name.lexeme, value);
		} else if(enclosing != null) {
			enclosing.assign(name, value);
		}
		throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
	}
	
}

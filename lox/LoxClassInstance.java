package lox;

import java.util.HashMap;
import java.util.Map;

import lox.callable.LoxClass;
import lox.callable.LoxFunction;

public class LoxClassInstance {
	
	private final LoxClass loxClass;
	private final Map<String, Object> fields = new HashMap<String, Object>();
	
	public LoxClassInstance(LoxClass loxClass) {
		this.loxClass = loxClass;
	}
	
	public void set(Token name, Object value) {
		fields.put(name.lexeme, value);
	}
	public Object get(Token name) {
		if(fields.containsKey(name.lexeme)) {
			return fields.get(name.lexeme);
		}
		
		LoxFunction method = loxClass.findMethod(name.lexeme);
		if(method != null) {
			return method.bind(this);
		}
		
		throw new RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
	}
	
	@Override
	public String toString() {
		return loxClass.toString() + " instance";
	}

}

package lox.callable;

public class LoxClass {
	
	private final String name;

	public LoxClass(String name) {
		this.name = name;
	}
	
	@Override
	public String toString() {
		return "<cl " + name + ">";
	}

}

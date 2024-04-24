package jlox.error;

public class Return extends RuntimeException {

	private static final long serialVersionUID = 1L;
	
	public final Object value;
	
	public Return(Object value) {
		super(null, null, false, false);
		this.value = value;
	}
	
}

package lox.parse;

import lox.Lox;

public abstract class ParseErrorHandler {
	
	private static ParseErrorHandler[] instances = {
		new ParseErrorThrow(), new ParseErrorSynchronize()	
	};
	
	public enum Type {
		THROW,
		SYNCHRONIZE
	}
	public static ParseErrorHandler get(Type type) {
		return instances[type.ordinal()];
	}
	
	
	public abstract void handleError(Parser parser, ParseError err);
	
	static class ParseErrorThrow extends ParseErrorHandler {
		public void handleError(Parser parser, ParseError err) {
			throw err;
		}
	}
	
	static class ParseErrorSynchronize extends ParseErrorHandler {
		public void handleError(Parser parser, ParseError err) {
			Lox.error(err.token, err.message);
			parser.synchronize();
		}
	}

}

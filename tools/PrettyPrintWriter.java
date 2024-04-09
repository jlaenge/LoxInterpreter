package tools;

import java.io.PrintWriter;

public class PrettyPrintWriter {
	
	private final String indent = "    ";
	
	private PrintWriter writer;
	private int depth;
	
	public PrettyPrintWriter(PrintWriter writer) {
		this.writer = writer;
		depth = 0;
	}
	
	public void indent() {
		indent(1);
	}
	public void indent(int levels) {
		depth += levels;
	}
	
	public void outdent() {
		outdent(1);
	}
	public void outdent(int levels) {
		assert(levels <= depth);
		depth -= levels;
	}

	public void println() {
		//System.out.println();
		writer.println();
	}
	public void println(String line) {
		line = indent.repeat(depth) + line;
		//System.out.println(line);
		writer.println(line);
	}
	
	public void flush() {
		writer.flush();
	}

}

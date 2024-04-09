package tools;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.List;

public class GenerateAST {
	
	public static void main(String[] args) throws IOException {
		
		if(args.length != 1) {
			System.err.println("Usage: generate_ast <output_directory>");
			System.exit(64);
		}
		
		String outputDirectory = args[0];
		defineAST(outputDirectory, "Expr", Arrays.asList(
			"Binary : Expr left, Token operator, Expr right",
			"Grouping : Expr expression",
			"Literal : Object value",
			"Unary : Token operator, Expr right"
		));
		
		System.out.println("GenerateAST Terminated");
		
	}
	
	private static void defineAST(String outputDirectory, String baseName, List<String> types) throws IOException {
		
		String path = outputDirectory + "/" + baseName + ".java";
		PrettyPrintWriter writer = new PrettyPrintWriter(new PrintWriter(path, "UTF-8"));
		
		writer.println("package lox;");
		writer.println();
		
		// CLASS: open
		writer.println("public abstract class " + baseName + " {");
		writer.println();
		writer.indent();
		
		// visitor pattern
		defineVisitor(writer, baseName, types);
		writer.println("abstract <T> T accept(Visitor<T> visitor);");
		writer.println();
		
		// sub-classes
		for(String type : types) {
			String className = type.split(":")[0].trim();
			String fields = type.split(":")[1].trim();
			defineType(writer, baseName, className, fields);
		}
		
		// CLASS: close
		writer.outdent();
		writer.println("}");
		
		writer.flush();
		
	}
	
	private static void defineVisitor(PrettyPrintWriter writer, String baseName, List<String> types) {
		
		// INTERFACE: open
		writer.println("interface Visitor<T> {");
		writer.indent();
		
		// visit-methods
		for(String type : types) {
			type = type.trim();
			String className = type.split(":")[0].trim();
			writer.println(
				"public T visit" + baseName + className + "(" +
					className + " " + baseName.toLowerCase() + 
				");"
			);
		}
		
		// INTERFACE: close
		writer.outdent();
		writer.println("}");
		writer.println();
		
	}
	
	private static void defineType(PrettyPrintWriter writer, String baseName, String className, String fieldsString) throws IOException {
		
		String[] fields = fieldsString.split(",");
		
		// CLASS: open
		writer.println("static class " + className + " extends " + baseName + " {");
		writer.indent();
		
		// ATTRIBUTES
		for(String field : fields) {
			field = field.trim();
			writer.println(field + ";");
		}
		
		// CONSTRUCTOR: open
		writer.println(className + "(" + fieldsString + ") {");
		writer.indent();
		
		for(String field : fields) {
			field = field.trim();
			String name = field.split(" ")[1].trim();
			writer.println("this." + name + " = " + name + ";");
		}
		
		// CONSTRUCTOR: close
		writer.outdent();
		writer.println("}");
		
		// VISITOR: open
		writer.println("<T> T accept(Visitor<T> visitor) {");
		writer.indent();
		
		writer.println("return visitor.visit" + baseName + className + "(this);");
		
		// VISITOR: close
		writer.outdent();
		writer.println("}");
		
		// CLASS: close
		writer.outdent();
		writer.println("}");
		writer.println();
		
	}

}
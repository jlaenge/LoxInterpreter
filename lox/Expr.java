package lox;

import java.util.List;

public abstract class Expr {

    public interface Visitor<T> {
        public T visitExprAssign(Assign expr);
        public T visitExprBinary(Binary expr);
        public T visitExprCall(Call expr);
        public T visitExprGrouping(Grouping expr);
        public T visitExprLiteral(Literal expr);
        public T visitExprLogical(Logical expr);
        public T visitExprUnary(Unary expr);
        public T visitExprVariable(Variable expr);
    }

    public abstract <T> T accept(Visitor<T> visitor);

    public static class Assign extends Expr {
        public Token name;
        public Expr expression;
        public Assign(Token name, Expr expression) {
            this.name = name;
            this.expression = expression;
        }
        public <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprAssign(this);
        }
    }

    public static class Binary extends Expr {
        public Expr left;
        public Token operator;
        public Expr right;
        public Binary(Expr left, Token operator, Expr right) {
            this.left = left;
            this.operator = operator;
            this.right = right;
        }
        public <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprBinary(this);
        }
    }

    public static class Call extends Expr {
        public Expr callee;
        public Token paren;
        public List<Expr> arguments;
        public Call(Expr callee, Token paren, List<Expr> arguments) {
            this.callee = callee;
            this.paren = paren;
            this.arguments = arguments;
        }
        public <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprCall(this);
        }
    }

    public static class Grouping extends Expr {
        public Expr expression;
        public Grouping(Expr expression) {
            this.expression = expression;
        }
        public <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprGrouping(this);
        }
    }

    public static class Literal extends Expr {
        public Object value;
        public Literal(Object value) {
            this.value = value;
        }
        public <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprLiteral(this);
        }
    }

    public static class Logical extends Expr {
        public Expr left;
        public Token operator;
        public Expr right;
        public Logical(Expr left, Token operator, Expr right) {
            this.left = left;
            this.operator = operator;
            this.right = right;
        }
        public <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprLogical(this);
        }
    }

    public static class Unary extends Expr {
        public Token operator;
        public Expr right;
        public Unary(Token operator, Expr right) {
            this.operator = operator;
            this.right = right;
        }
        public <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprUnary(this);
        }
    }

    public static class Variable extends Expr {
        public Token name;
        public Variable(Token name) {
            this.name = name;
        }
        public <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprVariable(this);
        }
    }

}

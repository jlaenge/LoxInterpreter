package lox;

import java.util.List;

public abstract class Expr {

    interface Visitor<T> {
        public T visitExprAssign(Assign expr);
        public T visitExprBinary(Binary expr);
        public T visitExprGrouping(Grouping expr);
        public T visitExprLiteral(Literal expr);
        public T visitExprUnary(Unary expr);
        public T visitExprVariable(Variable expr);
    }

    abstract <T> T accept(Visitor<T> visitor);

    public static class Assign extends Expr {
        public Token name;
        public Expr expression;
        public Assign(Token name, Expr expression) {
            this.name = name;
            this.expression = expression;
        }
        <T> T accept(Visitor<T> visitor) {
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
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprBinary(this);
        }
    }

    public static class Grouping extends Expr {
        public Expr expression;
        public Grouping(Expr expression) {
            this.expression = expression;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprGrouping(this);
        }
    }

    public static class Literal extends Expr {
        public Object value;
        public Literal(Object value) {
            this.value = value;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprLiteral(this);
        }
    }

    public static class Unary extends Expr {
        public Token operator;
        public Expr right;
        public Unary(Token operator, Expr right) {
            this.operator = operator;
            this.right = right;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprUnary(this);
        }
    }

    public static class Variable extends Expr {
        public Token name;
        public Variable(Token name) {
            this.name = name;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprVariable(this);
        }
    }

}

package lox;

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

    static class Assign extends Expr {
        Token name;
        Expr expression;
        Assign(Token name, Expr expression) {
            this.name = name;
            this.expression = expression;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprAssign(this);
        }
    }

    static class Binary extends Expr {
        Expr left;
        Token operator;
        Expr right;
        Binary(Expr left, Token operator, Expr right) {
            this.left = left;
            this.operator = operator;
            this.right = right;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprBinary(this);
        }
    }

    static class Grouping extends Expr {
        Expr expression;
        Grouping(Expr expression) {
            this.expression = expression;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprGrouping(this);
        }
    }

    static class Literal extends Expr {
        Object value;
        Literal(Object value) {
            this.value = value;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprLiteral(this);
        }
    }

    static class Unary extends Expr {
        Token operator;
        Expr right;
        Unary(Token operator, Expr right) {
            this.operator = operator;
            this.right = right;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprUnary(this);
        }
    }

    static class Variable extends Expr {
        Token name;
        Variable(Token name) {
            this.name = name;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitExprVariable(this);
        }
    }

}

package lox;

import java.util.List;

public abstract class Expr {

    static class Binary extends Expr {
        Expr left;
        Token operator;
        Expr right;
        Binary(Expr left, Token operator, Expr right) {
            this.left = left;
            this.operator = operator;
            this.right = right;
        }
    }

    static class Grouping extends Expr {
        Expr expression;
        Grouping(Expr expression) {
            this.expression = expression;
        }
    }

    static class Literal extends Expr {
        Object value;
        Literal(Object value) {
            this.value = value;
        }
    }

    static class Unary extends Expr {
        Token operator;
        Expr right;
        Unary(Token operator, Expr right) {
            this.operator = operator;
            this.right = right;
        }
    }

}

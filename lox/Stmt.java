package lox;

public abstract class Stmt {

    interface Visitor<T> {
        public T visitStmtExpression(Expression stmt);
        public T visitStmtPrint(Print stmt);
    }

    abstract <T> T accept(Visitor<T> visitor);

    static class Expression extends Stmt {
        Expr expression;
        Expression(Expr expression) {
            this.expression = expression;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitStmtExpression(this);
        }
    }

    static class Print extends Stmt {
        Expr expression;
        Print(Expr expression) {
            this.expression = expression;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitStmtPrint(this);
        }
    }

}

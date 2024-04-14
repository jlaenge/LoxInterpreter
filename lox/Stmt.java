package lox;

import java.util.List;

public abstract class Stmt {

    interface Visitor<T> {
        public T visitStmtBlock(Block stmt);
        public T visitStmtExpression(Expression stmt);
        public T visitStmtPrint(Print stmt);
        public T visitStmtVar(Var stmt);
    }

    abstract <T> T accept(Visitor<T> visitor);

    static class Block extends Stmt {
        List<Stmt> statements;
        Block(List<Stmt> statements) {
            this.statements = statements;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitStmtBlock(this);
        }
    }

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

    static class Var extends Stmt {
        Token name;
        Expr initializer;
        Var(Token name, Expr initializer) {
            this.name = name;
            this.initializer = initializer;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitStmtVar(this);
        }
    }

}

package lox;

import java.util.List;

public abstract class Stmt {

    interface Visitor<T> {
        public T visitStmtBlock(Block stmt);
        public T visitStmtExpression(Expression stmt);
        public T visitStmtFunction(Function stmt);
        public T visitStmtIf(If stmt);
        public T visitStmtPrint(Print stmt);
        public T visitStmtReturn(Return stmt);
        public T visitStmtVar(Var stmt);
        public T visitStmtWhile(While stmt);
    }

    abstract <T> T accept(Visitor<T> visitor);

    public static class Block extends Stmt {
        public List<Stmt> statements;
        public Block(List<Stmt> statements) {
            this.statements = statements;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitStmtBlock(this);
        }
    }

    public static class Expression extends Stmt {
        public Expr expression;
        public Expression(Expr expression) {
            this.expression = expression;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitStmtExpression(this);
        }
    }

    public static class Function extends Stmt {
        public Token name;
        public List<Token> parameters;
        public List<Stmt> body;
        public Function(Token name, List<Token> parameters, List<Stmt> body) {
            this.name = name;
            this.parameters = parameters;
            this.body = body;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitStmtFunction(this);
        }
    }

    public static class If extends Stmt {
        public Expr condition;
        public Stmt thenBranch;
        public Stmt elseBranch;
        public If(Expr condition, Stmt thenBranch, Stmt elseBranch) {
            this.condition = condition;
            this.thenBranch = thenBranch;
            this.elseBranch = elseBranch;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitStmtIf(this);
        }
    }

    public static class Print extends Stmt {
        public Expr expression;
        public Print(Expr expression) {
            this.expression = expression;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitStmtPrint(this);
        }
    }

    public static class Return extends Stmt {
        public Token keyword;
        public Expr expression;
        public Return(Token keyword, Expr expression) {
            this.keyword = keyword;
            this.expression = expression;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitStmtReturn(this);
        }
    }

    public static class Var extends Stmt {
        public Token name;
        public Expr initializer;
        public Var(Token name, Expr initializer) {
            this.name = name;
            this.initializer = initializer;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitStmtVar(this);
        }
    }

    public static class While extends Stmt {
        public Expr condition;
        public Stmt body;
        public While(Expr condition, Stmt body) {
            this.condition = condition;
            this.body = body;
        }
        <T> T accept(Visitor<T> visitor) {
            return visitor.visitStmtWhile(this);
        }
    }

}

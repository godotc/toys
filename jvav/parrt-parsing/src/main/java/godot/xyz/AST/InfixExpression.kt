package godot.xyz.AST


class InfixExpression : Expression() {

    var left = Expression()
    var operator = String()
    var right = Expression()

    override fun toString(): String {
        return StringBuilder()
                .append('(')
                .append(left)
                .append(" " + operator + " ")
                .append(right)
                .append(')')
                .append('\n')
                .toString()
    }
}

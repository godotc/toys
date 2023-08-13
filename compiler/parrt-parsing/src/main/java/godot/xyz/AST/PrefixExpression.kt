package godot.xyz.AST


class PrefixExpression : Expression() {

    var operator = String()
    var right = Expression()

    override fun toString(): String {
        return StringBuilder()
            .append('(')
            .append(operator)
            .append(right)
            .append(')')
            .append('\n')
            .toString()
    }
}

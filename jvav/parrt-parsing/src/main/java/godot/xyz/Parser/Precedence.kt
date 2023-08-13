package godot.xyz.Parser

import godot.xyz.Token.TokenType


class Precedence {

    enum class Priority(val priority: Int) {
        LOWEST(0),
        EQUALS(1),
        LESS_GREATER(2),
        SUM(3),

        PRODUCT(4),

        POWER(5),

        PREFIX(6),
        CALL(7),
    }

    companion object {
        var precedence = HashMap<TokenType, Priority>()

        init {
            precedence.put(TokenType.PLUS, Priority.SUM);
            precedence.put(TokenType.MINUS, Priority.SUM);
            precedence.put(TokenType.SLASH, Priority.PRODUCT);
            precedence.put(TokenType.ASTERISK, Priority.PRODUCT);
            precedence.put(TokenType.HAT, Priority.POWER);
            precedence.put(TokenType.LPAREN, Priority.CALL);

        }

        fun GetPrecedence(type: TokenType): Priority? {
            return precedence.get(type)
        }
    }

}

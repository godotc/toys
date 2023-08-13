package godot.xyz.Parser

import godot.xyz.Token.TokenType


class Precidence {

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
        var precidence = HashMap<TokenType, Priority>()

        init {
            precidence.put(TokenType.PLUS, Priority.SUM);
            precidence.put(TokenType.MINUS, Priority.SUM);
            precidence.put(TokenType.SLASH, Priority.PRODUCT);
            precidence.put(TokenType.ASTERISK, Priority.PRODUCT);
            precidence.put(TokenType.HAT, Priority.POWER);
            precidence.put(TokenType.LPAREN, Priority.CALL);

        }

        fun GetPrecedence(type: TokenType): Priority? {
            return precidence.get(type)
        }
    }

}

package godot.xyz.lexer

import godot.xyz.Token.Token
import godot.xyz.Token.TokenType


class Lexer(var input: String) {

    var ch: Char;
    var cur_pos: Int;
    var peek_pos: Int;

    init {
        cur_pos = 0;
        peek_pos = 0;
        ch = ' ';
        ReadChar();
    }

    public fun PeekChar(): Char {
        return input.get(peek_pos);
    }

    public fun NextToken(): Token {
        skip_white_space();

        val token = when (ch) {
            '+' -> Token(TokenType.PLUS, "+");
            '-' -> Token(TokenType.MINUS, "-");
            '*' -> Token(TokenType.ASTERISK, "*");
            '/' -> Token(TokenType.SLASH, "/");
            '(' -> Token(TokenType.LPAREN, "(");
            ')' -> Token(TokenType.RPAREN, ")");
            '^' -> Token(TokenType.HAT, "^");
            '\u0000' -> Token(TokenType.EOF, " ")
            else -> {
                if (ch.isDigit()) {
                    val num = read_num();
                    return Token(TokenType.NUM, num)
                } else {
                    throw Exception("Lexer error")
                }

            }
        }

        ReadChar();
        return token;

    }

    public fun HasNext(): Boolean {
        return peek_pos <= input.length;
    }

    public fun ReadChar() {
        this.ch = if (peek_pos >= input.length) {
            0.toChar();
        } else {
            input.get(peek_pos);
        }
        cur_pos = peek_pos;
        ++peek_pos;
    }

    fun skip_white_space() {
        while (HasNext()) {
            if (" \t\n".contains(ch)) {
                ReadChar();
            } else break
        }
    }

    fun read_num(): String {
        val num = StringBuilder()
        while (ch.isDigit()) {
            num.append(ch);
            ReadChar();
        }
        return num.toString();
    }

}
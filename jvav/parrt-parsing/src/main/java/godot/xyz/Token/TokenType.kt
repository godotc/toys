package godot.xyz.Token

enum class TokenType() {
    UNKOWN,
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    LPAREN,
    RPAREN,
    EOF,
    HAT,
    NUM,
}

//enum class TokenType(var value: String) {
//    PLUS("+"),
//    MINUS("-"),
//    ASTERISK("*"), // *
//    SLASH("/"),
//    LPAREN("("),
//    RPAREN(")"),
//    EOF("\u0000"),
//    HAT("^"),
//    NUM(" ");
//}

/*
I want this:
if (ch.IsDigital()){
    var Token =  TokenType.NUM("123")
}

 */

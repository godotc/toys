package godot.xyz.lexer

import godot.xyz.Evaluator.Evaluator
import godot.xyz.Parser.Parser
import org.junit.jupiter.api.Test

class LexerTest {
    @Test
    public fun test() {
        val code = "10+1 * 2";
        var lexer = Lexer(code);
        while (lexer.HasNext()) {
            println(lexer.NextToken());
        }

        lexer = Lexer(code)
        val parser = Parser(lexer)


        val expression = parser.Parse();
        println(expression)
        println(Evaluator.eval(expression));
    }

}
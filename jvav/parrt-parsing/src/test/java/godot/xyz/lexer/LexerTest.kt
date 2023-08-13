package godot.xyz.lexer

import godot.xyz.Evaluator.Evaluator
import godot.xyz.Parser.Parser
import org.junit.jupiter.api.Test

class LexerTest {
    @Test
    public fun test() {
        val code = "10+1 * 2";
        val lexer = Lexer(code);
        while (lexer.HasNext()) {
            println(lexer.NextToken());
        }

        val parser = Parser(lexer)

        val expression = parser.ParseMain();
        println(Evaluator.eval(expression));
    }

}
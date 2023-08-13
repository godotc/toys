package godot.xyz;

import godot.xyz.Evaluator.Evaluator
import godot.xyz.Object.GInt
import godot.xyz.Parser.Parser
import godot.xyz.lexer.Lexer


fun parse_string(string: String) {
    if (string.isEmpty()) return
    var lexer = Lexer(string)
    while (lexer.HasNext()) {
        println(lexer.NextToken())
    }

    lexer = Lexer(string)

    val parser = Parser(lexer)
    val expr = parser.Parse()
    println(expr)
    println(
        (Evaluator.eval(expr) as GInt).value
    )


}

fun main() {
    println("Enter your expression...");


    while (true) {
        print("-> ");
        val input = readln();
        parse_string(input)
    }
}
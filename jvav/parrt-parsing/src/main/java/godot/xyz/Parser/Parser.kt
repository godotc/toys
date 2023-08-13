package godot.xyz.Parser

import godot.xyz.AST.Expression
import godot.xyz.AST.InfixExpression
import godot.xyz.AST.IntegerExpression
import godot.xyz.AST.PrefixExpression
import godot.xyz.Token.Token
import godot.xyz.Token.TokenType
import godot.xyz.lexer.Lexer
import java.lang.RuntimeException
import java.math.BigInteger

class Parser(private final val lexer: Lexer) {

    private val prefix_parser_fn_map = HashMap<TokenType, () -> Expression>();
    private val infix_parser_fn_map = HashMap<TokenType, (Expression) -> Expression>();

    var cur = Token()
    var peek = Token()


    //    public interface InfixParseFn {
//        fun parse(expression: Expression): Expression;
//    }
    init {
        prefix_parser_fn_map.put(TokenType.PLUS, ::parse_integer);
        prefix_parser_fn_map.put(TokenType.MINUS, ::parse_prefix_expr);
        prefix_parser_fn_map.put(TokenType.LPAREN, ::parse_group_expr);

        infix_parser_fn_map.put(TokenType.PLUS, this::parse_infix_expr);
        infix_parser_fn_map.put(TokenType.MINUS, this::parse_infix_expr);
        infix_parser_fn_map.put(TokenType.ASTERISK, this::parse_infix_expr);
        infix_parser_fn_map.put(TokenType.SLASH, this::parse_infix_expr);
        infix_parser_fn_map.put(TokenType.HAT, this::parse_infix_expr);

        // Init cur and peek
        next_token()
        next_token()
    }

    public fun ParseExpression(precedence: int): Expression {
        val prefix_parse_fn = prefix_parser_fn_map.get(cur.type);
        if (prefix_parse_fn == null) {
            throw RuntimeException("No prefix parse fn for " + cur.type)
        }
        var left = prefix_parse_fn();

        while (!is_peek_token(TokenType.EOF) && precedence < peek_precedence()) {
            val inifix_parse_fn = infix_parser_fn_map.get(peek.type);
            if (inifix_parse_fn == null) {
                return left;
            }
            next_token();
            left = inifix_parse_fn(left)
        }
        return left
    }

    fun parse_prefix_expr(): Expression {
        val prefix_expr = PrefixExpression()
        prefix_expr.operator = cur.value;
        next_token()
        prefix_expr.right = ParseExpression(Precidence.Priority.PREFIX)

        return prefix_expr
    }

    fun parse_infix_expr(left: Expression): Expression {
        val infix_expr = InfixExpression()
        infix_expr.left = left
        infix_expr.operator = cur.value;

        val precedence = Precidence.GetPrecedence(cur.type)
        next_token()
        infix_expr.right = ParseExpression(precedence)

        return infix_expr
    }

    fun parse_group_expr(): Expression {
        next_token();
        val expr = ParseExpression(Precidence.Priority.LOWEST);
        next_token();
        return expr;
    }

    fun parse_integer(): Expression {
        val value = BigInteger(cur.value);
        val integerExpression = IntegerExpression(value)
        return integerExpression
    }


    private fun next_token() {
        cur = peek;
        peek = lexer.NextToken();
    }

    private fun is_cur_token(type: TokenType): bool {
        return cur.type == type
    }

    private fun is_peek_token(type: TokenType): bool {
        return peek.type == type
    }

    private fun peek_precedence(): Precidence.Priority? {
        return Precidence.GetPrecedence(peek.type)
    }

}



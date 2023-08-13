package godot.xyz.Parser

import godot.xyz.AST.Expression
import godot.xyz.AST.InfixExpression
import godot.xyz.AST.IntegerExpression
import godot.xyz.AST.PrefixExpression
import godot.xyz.Token.Token
import godot.xyz.Token.TokenType
import godot.xyz.lexer.Lexer
import java.math.BigInteger

class Parser(private final val lexer: Lexer) {

    private val prefix_parser_fn_map = HashMap<TokenType, () -> Expression>();
    private val infix_parser_fn_map = HashMap<TokenType, (Expression) -> Expression>();

    var cur = Token()
    var peek = Token()


    init {
        prefix_parser_fn_map.put(TokenType.NUM, ::parse_integer);
        prefix_parser_fn_map.put(TokenType.MINUS, ::parse_prefix_expr);
        prefix_parser_fn_map.put(TokenType.PLUS, ::parse_prefix_expr);
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

    public fun Parse(): Expression {
        return parse_expression(Precedence.Priority.LOWEST);
    }

    fun parse_expression(precedence: Precedence.Priority): Expression {
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
        prefix_expr.right = parse_expression(Precedence.Priority.PREFIX)

        return prefix_expr
    }

    fun parse_infix_expr(left: Expression): Expression {
        val infix_expr = InfixExpression()
        infix_expr.left = left
        infix_expr.operator = cur.value;

        val precedence = Precedence.GetPrecedence(cur.type)!!
        next_token()
        infix_expr.right = parse_expression(precedence)

        return infix_expr
    }

    fun parse_group_expr(): Expression {
        next_token();
        val expr = parse_expression(Precedence.Priority.LOWEST);
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

    private fun is_cur_token(type: TokenType): Boolean {
        return cur.type == type
    }

    private fun is_peek_token(type: TokenType): Boolean {
        return peek.type == type
    }

    private fun peek_precedence(): Precedence.Priority {
        return Precedence.GetPrecedence(peek.type)!!
    }

}



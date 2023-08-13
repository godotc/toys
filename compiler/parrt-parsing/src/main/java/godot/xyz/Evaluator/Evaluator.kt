package godot.xyz.Evaluator

import godot.xyz.AST.InfixExpression
import godot.xyz.AST.IntegerExpression
import godot.xyz.AST.Node
import godot.xyz.AST.PrefixExpression
import godot.xyz.Object.GInt
import godot.xyz.Object.GObj

class Evaluator {


    companion object {

        fun eval(node: Node): GObj? = when (node) {
            is IntegerExpression -> GInt(node.value)

            is PrefixExpression -> {
                when (node.operator) {
                    "-" -> minus(node)
                    "+" -> eval(node.right)
                    else -> null
                }
            }

            is InfixExpression -> {
                val left = eval(node.left)!!;
                val right = eval(node.right)!!;
                op(left, right, node.operator)
            }

            else -> null
        }

        fun op(left: GObj, right: GObj, operator: String): GObj? {
            if (left is GInt && right is GInt) {
                return when (operator) {
                    "+" -> GInt(left.value.add(right.value))
                    "-" -> GInt(left.value.minus(right.value))
                    "*" -> GInt(left.value.multiply(right.value))
                    "/" -> GInt(left.value.divide(right.value))
                    "^" -> GInt(left.value.pow(right.value.toInt()))
                    else -> null
                }
            }
            return null;
        }


        fun minus(node: Node): GInt? {
            if (node is PrefixExpression) {
                val obj = eval((node as PrefixExpression).right)
                if (obj is GInt) {
                    return GInt(obj.value.negate())
                }
            }
            return null
        }

    }
}
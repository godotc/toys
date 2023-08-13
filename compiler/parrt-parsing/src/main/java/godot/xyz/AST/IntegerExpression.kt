package godot.xyz.AST

import java.math.BigInteger


class IntegerExpression(var value: BigInteger) : Expression() {


    fun IntegerExpression(value: String) {
        this.value = BigInteger(value);
    }
    fun IntegerExpression(value: BigInteger) {
        this.value = value;
    }

}

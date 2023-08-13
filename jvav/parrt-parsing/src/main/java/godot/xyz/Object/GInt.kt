package godot.xyz.Object

import java.math.BigInteger

class GInt(var value: BigInteger) : GObj() {

    fun GInt(value: String) {
        this.value = BigInteger(value)
    }


    override fun toString(): String {
        return "Mint($value)";
    }

}



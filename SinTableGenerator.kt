import kotlin.math.PI
import kotlin.math.sin
import java.io.File

fun main() {
    //#define fxpt_shift 15
    val fxpt_shift = 15
    //#define fxpt(X) ((fxpt_t)(X * (1 << fxpt_shift)))
    fun fxpt(x: Double): Long = (x * (1L shl fxpt_shift)).toLong()
    //#define unfxpt(X) ((float)X / (1 << fxpt_shift))
    fun unfxpt(x: Long): Double = x.toDouble() / (1L shl fxpt_shift)

    println("Step Value:")
    val stepValue = readLine()!!.toLong()

    val oneHalfPi = "fxpt_half_pi"
    val pi = "fxpt_pi"
    val threeHalfPi = "fxpt_three_half_pi"
    val twoPi = "fxpt_two_pi"

    val trig_t = "int16_t"
    val dest = "dest"
    val theta_t = "theta_t"
    val sinf = "lookup_sin"
    val cosf = "lookup_cos"
    val table = "sin_table"

    // memcpy_P (void *dest, uint_farptr_t src, size_t len)
    fun q1SinLookUp(theta: String) = "pgm_read_dword(&$table[($theta) / $stepValue])"

    val writer = File("sin.ino").printWriter()

    writer.println("""
    #include "fxpt.h"

    // DO NOT EDIT. THIS FILE IS AUTO GENERATED:
    // kotlinc SinTableGenerator.kt; kotlin SinTableGeneratorKt

    const PROGMEM $trig_t $table[] = {
    """.trimIndent());

    var count = 0;
    (0L until Long.MAX_VALUE step stepValue)
            .takeWhile { it <= fxpt(PI / 2) + stepValue }
            .map { fxpt(sin(unfxpt(it))) }
            .onEach { count++ }
            .joinToString()
            .also { writer.println(it) }
    println("Generated $count values.")

    writer.println("""
    };

    $trig_t $sinf($theta_t t) {
        $trig_t $dest = 0;

        if(t <= $oneHalfPi) {
            return ${q1SinLookUp("t")};
        }
        else if(t <= $pi) {
            return ${q1SinLookUp("$pi - t")};
        }
        else if(t <= $threeHalfPi) {
            return -${q1SinLookUp("t - $pi")};
        }
        else if(t <= $twoPi) {
            return -${q1SinLookUp("$twoPi - t")};
        }

        return 0;
    }

    $trig_t $cosf($theta_t t) {
        if(t <= $oneHalfPi) {
            return ${q1SinLookUp("$oneHalfPi - t")};
        }
        else if(t <= $pi) {
            return -${q1SinLookUp("t - $oneHalfPi")};;
        }
        else if(t <= $threeHalfPi) {
            return -${q1SinLookUp("$threeHalfPi - t")};
        }
        else if(t <= $twoPi) {
            return ${q1SinLookUp("t - $threeHalfPi")};;
        }

        return 0;
    }""".trimIndent())

    writer.close()
}

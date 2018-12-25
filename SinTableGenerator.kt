import kotlin.math.PI
import kotlin.math.sin
import java.io.File

fun main() {
  println("SIN(X) Table Resolution:")
  val numEntries = readLine()!!.toInt()

  val lookupFactor = numEntries / (PI/2)

  val oneHalfPi = "HALF_PI"
  val pi = "PI"
  val threeHalfPi = "3*HALF_PI"
  val twoPi = "TWO_PI"

  val trig_t = "float"
  val theta_t = "theta_t"
  val sinf = "lookup_sin"
  val cosf = "lookup_cos"
  val table = "sin_table"

  val writer = File("sin.ino").printWriter()

  writer.println("""
    // DO NOT EDIT. THIS FILE IS AUTO GENERATED:
    // kotlinc SinTableGenerator.kt; kotlin SinTableGeneratorKt
    """);

  writer.println(
    DoubleArray(numEntries+1) { sin(it / lookupFactor) }
    .joinToString(
      prefix = "const PROGMEM $trig_t $table[] = {",
      separator = ",\n",
      postfix = "};"
    )
  )

  writer.println("""
  $trig_t $sinf($theta_t t) {
    if(t < 0) return -$sinf(-t);
    if(t <= $oneHalfPi) return pgm_read_float($table + round(t * $lookupFactor));
    if(t <= $pi) return $sinf($pi - t);
    if(t <= $threeHalfPi) return -$sinf(t - $pi);
    if(t <= $twoPi) return -$sinf($twoPi - t);

    return 0;
  }

  $trig_t $cosf($theta_t t) {
    return $sinf($oneHalfPi - t);
  }"""
  .trimMargin())
  writer.close()
}

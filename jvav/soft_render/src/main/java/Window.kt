import java.awt.Color
import java.awt.Dimension
import java.awt.Toolkit
import java.awt.image.BufferedImage
import java.awt.image.DataBufferInt
import javax.swing.JFrame
import javax.swing.JPanel
import kotlin.math.abs

class Window : JFrame() {
    init {
        this.setTitle("JVAV SOFT RENDER")
        m_Pannel = this.contentPane as JPanel
        m_Pannel.preferredSize = Dimension(WIN_W, WIN_H)
        m_Pannel.minimumSize = Dimension(WIN_W, WIN_H)
        m_Pannel.setLayout(null)
        setResizable(true)
        pack()
        isVisible = true
        val dim = Toolkit.getDefaultToolkit().screenSize
        setLocation(dim.width / 2 - this.size.width / 2, dim.height / 2 - this.size.height / 2)
        setDefaultCloseOperation(EXIT_ON_CLOSE)

        // BufferImage
        m_ScreenBuffer = BufferedImage(WIN_W, WIN_H, BufferedImage.TYPE_INT_RGB)
        val dest = m_ScreenBuffer.raster.dataBuffer
        m_Screen = (dest as DataBufferInt).getData()

        while (true) {

            val sky_blue = Color(163, 216, 239)
            val orange = Color(255, 128, 0)
            for (i in m_Screen.indices) {
                m_Screen[i] = sky_blue.rgb
            }

            // gradually from blue to orange
            for (i in 0 until WIN_W) {
                for (j in 0 until WIN_H) {
                    val alpha = abs(((half_win_width - i).toFloat() / half_win_width).toDouble()).toFloat()
                    val color = (sky_blue.red * alpha + orange.red * (1 - alpha)).toInt() shl 16 or
                            ((sky_blue.green * alpha + orange.green * (1 - alpha)).toInt() shl 8) or
                            (sky_blue.blue * alpha + orange.blue * (1 - alpha)).toInt()

                    m_Screen[j * WIN_W + i] = color
                }
            }

            for (i in 0 until WIN_W) {
                val p = (i + frameIndex * 8) % WIN_W
                for (j in 0 until WIN_H) {
                    val alpha = abs(((half_win_width - p).toFloat() / half_win_width).toDouble()).toFloat()
                    val color =
                        (sky_blue.red * alpha + orange.red * (1 - alpha)).toInt() shl 16 or ((sky_blue.green * alpha + orange.green * (1 - alpha)).toInt() shl 8) or (sky_blue.blue * alpha + orange.blue * (1 - alpha)).toInt()
                    m_Screen[j * WIN_W + i] = color
                }
            }
            ++frameIndex

            // Draw image to graphic memory
            m_Pannel.getGraphics().drawImage(m_ScreenBuffer, 0, 0, this)
        }
    }

    companion object {
        var WIN_W = 800
        var WIN_H = 600

        var half_win_width = WIN_W / 2
        var half_win_height = WIN_H / 2

        var frameIndex = 0

        var m_Pannel = JPanel()
        lateinit var m_Screen: IntArray
        lateinit var m_ScreenBuffer: BufferedImage

    }

}

fun main(args: Array<String>) {
    Window()
}

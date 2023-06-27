import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.DataBuffer;
import java.awt.image.DataBufferInt;

public class Main extends JFrame {

    public static int WIN_W = 800;
    public static int WIN_H = 600;
    public static int half_win_width = WIN_W / 2;
    public static int half_win_height = WIN_H / 2;

    public static int frameIndex = 0;

    public static JPanel m_Pannel;
    public static int[] m_Screen;
    public static BufferedImage m_ScreenBuffer;

    public static void main(String[] args) {
        new Main();
    }

    public Main() {
        setTitle("JVAV SOFT RENDER");
        m_Pannel = (JPanel) this.getContentPane();
        m_Pannel.setPreferredSize(new Dimension(WIN_W, WIN_H));
        m_Pannel.setMinimumSize(new Dimension(WIN_W, WIN_H));
        m_Pannel.setLayout(null);

        setResizable(true);
        pack();
        setVisible(true);
        Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
        setLocation(dim.width / 2 - this.getSize().width / 2, dim.height / 2 - this.getSize().height / 2);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // BufferImage
        m_ScreenBuffer = new BufferedImage(WIN_W, WIN_H, BufferedImage.TYPE_INT_RGB);
        DataBuffer dest = m_ScreenBuffer.getRaster().getDataBuffer();
        m_Screen = ((DataBufferInt) dest).getData();


        while (true) {
            Color sky_blue = new Color(163, 216, 239);
            Color orange = new Color(255, 128, 0);
            for (int i = 0; i < m_Screen.length; ++i) {
                m_Screen[i] = sky_blue.getRGB();
            }

            // gradually from blue to orange
            for (int i = 0; i < WIN_W; ++i) {
                for (int j = 0; j < WIN_H; ++j) {
                    float alpha = Math.abs((float) (half_win_width - i) / half_win_width);

                    int color = (int) (sky_blue.getRed() * alpha + orange.getRed() * (1 - alpha)) << 16 |
                            (int) (sky_blue.getGreen() * alpha + orange.getGreen() * (1 - alpha)) << 8 |
                            (int) (sky_blue.getBlue() * alpha + orange.getBlue() * (1 - alpha));

                    m_Screen[j * WIN_W + i] = color;
                }
            }
            for (int i = 0; i < WIN_W; ++i) {
                int p = (i + frameIndex * 8) % WIN_W;
                for (int j = 0; j < WIN_H; ++j) {
                    float alpha = Math.abs((float) (half_win_width - p) / half_win_width);

                    int color = (int) (sky_blue.getRed() * alpha + orange.getRed() * (1 - alpha)) << 16 |
                            (int) (sky_blue.getGreen() * alpha + orange.getGreen() * (1 - alpha)) << 8 |
                            (int) (sky_blue.getBlue() * alpha + orange.getBlue() * (1 - alpha));

                    m_Screen[j * WIN_W + i] = color;

                }
            }

            ++frameIndex;

            // Draw image to graphic memory
            m_Pannel.getGraphics().drawImage(m_ScreenBuffer, 0, 0, this);
        }
    }

}

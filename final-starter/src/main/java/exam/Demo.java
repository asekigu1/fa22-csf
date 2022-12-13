package exam;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.IOException;

public class Demo {

  /**
   * Execution starts here.
   *
   * @param args - command-line arguments (not used here).
   * @throws IOException if an error occurs during reading image file.
   */
  public static void main(String[] args) throws IOException {
    BufferedImage image = ImageUtility.getBufferedImage("tiny/black.png");
    Color[][] pixels = ImageUtility.getPixels(image);
    ImageUtility.printColors(pixels);
    ImageUtility.printPixelIDs(pixels);
  }
}

package exam;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.nio.file.Path;
import java.nio.file.Paths;
import javax.imageio.ImageIO;
import org.apache.commons.lang3.StringUtils;

public class ImageUtility {

  /**
   * Get the image data from the image file.
   * @param imageFile the image filename (can include sub-directory in resources folder).
   * @return a BufferedImage object containing image data.
   * @throws IOException if an error occurs during reading image file.
   */
  public static BufferedImage getBufferedImage(String imageFile) throws IOException {
    String path = getPathToResourcesFolder();
    Path dataFile = Paths.get(path, imageFile);
    // TODO: On Windows, use the statement below instead of the one above!
    // Path dataFile = Paths.get(path.substring(1), imageFile);
    return ImageIO.read(dataFile.toFile());
  }

  private static String getPathToResourcesFolder() {
    URL url = Thread.currentThread().getContextClassLoader().getResource("");
    return url.getPath().replace("%20", " ")
        .replace("classes", "resources");
  }

  /**
   * Generate a 2D color array of image pixels.
   * @param image a BufferedImage object.
   * @return a 2D array of colors.
   */
  public static Color[][] getPixels(BufferedImage image) {
    int width = image.getWidth();
    int height = image.getHeight();
    Color[][] pixels = new Color[height][width];
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        pixels[y][x] = new Color(image.getRGB(x, y));
      }
    }
    return pixels;
  }

  /**
   * Print RGB values.
   * @param pixels a 2D array of color pixels.
   */
  public static void printColors(Color[][] pixels) {
    print(pixels, "COLOR");
  }

  private static void print(Color[][] pixels, String info) {
    for (int y = 0; y < getHeight(pixels); y++) {
      for (int x = 0; x < getWidth(pixels); x++) {
        switch (info) {
          case "PIXEL_ID":
            System.out.printf("%3d\t", y * getWidth(pixels) + x);
            break;
          case "COORDINATE":
            System.out.printf("(%d,%d)\t", x, y);
            break;
          case "COLOR":
          default:
            System.out.printf("[%-3d %3s %3d]\t", pixels[y][x].getRed(),
                StringUtils.center("" + pixels[y][x].getGreen(), 3), pixels[y][x].getBlue()
            );
        }
      }
      System.out.print("\n\n");
    }
  }

  /**
   * Get Height.
   * @param pixels a 2d array.
   * @return number of rows.
   */
  public static int getHeight(Color[][] pixels) {
    return pixels.length;
  }

  /**
   * Get Width.
   * @param pixels a 2d array.
   * @return number of columns.
   */
  public static int getWidth(Color[][] pixels) {
    return pixels[0].length;
  }

  /**
   * Print (x,y) coordinate of pixels.
   * @param pixels a 2D array of color pixels.
   */
  public static void printCoordinates(Color[][] pixels) {
    print(pixels, "COORDINATE");
  }

  /**
   * Print pixelIDs.
   * @param pixels a 2D array of color pixels.
   */
  public static void printPixelIDs(Color[][] pixels) {
    print(pixels, "PIXEL_ID");
  }

  /**
   * Print a 2D grid corresponding to image pixels where each cells contains
   *   the label (region ID) that pixel belongs to.
   * @param regions a UnionFind data structure.
   * @param width the width of the image.
   * @param height the height of the image.
   */
  public static void printLabels(UnionFind<Integer> regions, int width, int height) {
    System.out.printf("Number of regions: %d\n", regions.count());
    System.out.print("Region labels:\n\n");
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int pixelID = y * width + x;
        int label = regions.find(pixelID);
        System.out.printf("%3d\t", label);
      }
      System.out.print("\n\n");
    }
  }

  /**
   * Compute the Euclidean distance between two colors.
   * @param color1 a Color object.
   * @param color2 a Color object.
   * @return a double value.
   */
  public static double distance(Color color1, Color color2) {
    return Math.sqrt(Math.pow(color1.getRed() - color2.getRed(), 2)
        + Math.pow(color1.getGreen() - color2.getGreen(), 2)
        + Math.pow(color1.getBlue() - color2.getBlue(), 2)
    );
  }

  /**
   * Compute the average of two colors.
   * @param color1 a Color object.
   * @param color2 a Color object.
   * @return a Color object.
   */
  public static Color average(Color color1, Color color2) {
    return new Color((color1.getRed() + color2.getRed()) / 2,
        (color1.getGreen() + color2.getGreen()) / 2,
        (color1.getBlue() + color2.getBlue()) / 2);
  }


  /**
   * Save the image data to an image file.
   * @param filename the output image filename.
   * @param out the BufferedImage object to be saved to a file.
   * @throws IOException if an error occurs during saving the image file.
   */
  public static void save(String filename, BufferedImage out) throws IOException {
    String path = getPathToResourcesFolder();
    ImageIO.write(out, "png", new File(path + "/" + filename));
  }
}

package exam;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.IOException;

public class ImageRegions {
  private static final double SIMILARITY_THRESHOLD = 1;

  /**
   * Execution starts here.
   * @param args - command-line arguments (not used here).
   * @throws IOException if an error occurs during reading image file.
   */
  public static void main(String[] args) throws IOException {
    BufferedImage image = ImageUtility.getBufferedImage("tiny/bw.png");
    Color[][] pixels = ImageUtility.getPixels(image);
    UnionFind<Integer> regions = new RasterUnionFind(image.getWidth(), image.getHeight());
    labelRegions(pixels, regions);
    ImageUtility.printLabels(regions, image.getWidth(), image.getHeight());
  }

  // Go over every pixel and call union to merge it with its neighbours if
  //  this pixel has the same color as its neighbors
  private static void labelRegions(Color[][] pixels, UnionFind<Integer> regions) {
    int height = ImageUtility.getHeight(pixels);
    int width = ImageUtility.getWidth(pixels);
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int pixelID1 = y * width + x;

        // right
        if (x + 1 < width) {
          if (isSameColor(pixels[y][x], pixels[y][x + 1])) {
            regions.union(pixelID1, y * width + x + 1);
          }
        }

        // bottom
        if (y + 1 < height) {
          if (isSameColor(pixels[y][x], pixels[y + 1][x])) {
            regions.union(pixelID1, (y + 1) * width + x);
          }
        }
      }
    }
  }

  // Return true if the two colors are similar enough to be considered the same.
  //  Otherwise returns false.
  private static boolean isSameColor(Color color1, Color color2) {
    return ImageUtility.distance(color1, color2) <= SIMILARITY_THRESHOLD;
  }
}

package exam;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.HashMap;
import java.util.PriorityQueue;

public class ImageSegmentation {

  /**
   * Execution starts here.
   * @param args - command-line arguments (not used here).
   * @throws IOException if an error occurs during reading image file.
   */
  public static void main(String[] args) throws IOException {
    BufferedImage image = ImageUtility.getBufferedImage("small/gradient.png");
    kruskal(image, 10);
    ImageUtility.save("gradient.png", image);
  }

  private static void kruskal(BufferedImage image, int numberOfDesiredRegions) {
    Color[][] pixels = ImageUtility.getPixels(image);
    PriorityQueue<Edge> edges = addAllEdges(pixels);
    UnionFind<Integer> regions = new RasterUnionFind(image.getWidth(), image.getHeight());

    while (!edges.isEmpty()) {
      Edge e = edges.poll();
      regions.union(e.fromPixelID, e.toPixelID);
      if (regions.count() == numberOfDesiredRegions) {
        break;
      }
    }

    HashMap<Integer, Color> colors = assignColorToEachRegion(pixels, regions);
    reColor(image, regions, colors);
  }

  // Go over every pixel and add an edge between the pixel and its neighbors.
  // Store the edges in a priority queue.
  private static PriorityQueue<Edge> addAllEdges(Color[][] pixels) {
    int height = ImageUtility.getHeight(pixels);
    int width = ImageUtility.getWidth(pixels);
    PriorityQueue<Edge> edges = new PriorityQueue<>();
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        connectToNeighbors(pixels, edges, y, x);
      }
    }
    return edges;
  }

  // Go over every pixel and retrieve it's region label.
  // Store the labels in a hash table and map them to the average color of all the pixels in that region.
  private static HashMap<Integer, Color> assignColorToEachRegion(Color[][] pixels, UnionFind<Integer> regions) {
    int height = ImageUtility.getHeight(pixels);
    int width = ImageUtility.getWidth(pixels);
    HashMap<Integer, Color> colors = new HashMap<>();
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int pixelID = y * width + x;
        int label = regions.find(pixelID);
        if (!colors.containsKey(label)) {
          colors.put(label, pixels[y][x]);
        } else {
          Color color1 = colors.get(label);
          Color color2 = pixels[y][x];
          Color color = ImageUtility.average(color1, color2);
          colors.put(label, color);
        }
      }
    }
    return colors;
  }

  // Go over every pixel and replace its color with the color of its region label.
  private static void reColor(BufferedImage image, UnionFind<Integer> regions, HashMap<Integer, Color> colors) {
    int width = image.getWidth();
    int height = image.getHeight();
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int pixelID = y * width + x;
        int label = regions.find(pixelID);
        Color color = colors.get(label);
        image.setRGB(x, y, color.getRGB());
      }
    }
  }

  // Add an edge between pixel at (x,y) and its neighbors to north, south, east, or west (if such pixels exist).
  // The color distance between the endpoint pixels are set as the edge weight.
  private static void connectToNeighbors(Color[][] pixels, PriorityQueue<Edge> edges, int y, int x) {
    int width = ImageUtility.getWidth(pixels);
    int pixelID1 = y * width + x;

    if (x - 1 >= 0) {     // left
      int pixelID2 = y * width + x - 1;
      edges.add(new Edge(pixelID1, pixelID2, ImageUtility.distance(pixels[y][x], pixels[y][x - 1])));
    }

    if (y - 1 >= 0) {     // top
      int pixelID2 = (y - 1) * width + x;
      edges.add(new Edge(pixelID1, pixelID2, ImageUtility.distance(pixels[y][x], pixels[y - 1][x])));
    }

    if (x + 1 < width) {     // right
      int pixelID2 = y * width + x + 1;
      edges.add(new Edge(pixelID1, pixelID2, ImageUtility.distance(pixels[y][x], pixels[y][x + 1])));
    }

    if (y + 1 < ImageUtility.getHeight(pixels)) {     // bottom
      int pixelID2 = (y + 1) * width + x;
      edges.add(new Edge(pixelID1, pixelID2, ImageUtility.distance(pixels[y][x], pixels[y + 1][x])));
    }
  }

  private static class Edge implements Comparable<Edge> {
    int fromPixelID;
    int toPixelID;
    double weight;

    Edge(int fromPixelID, int toPixelID, double weight) {
      this.fromPixelID = fromPixelID;
      this.toPixelID = toPixelID;
      this.weight = weight;
    }

    @Override
    public int compareTo(Edge other) {
      return (int) Math.signum(this.weight - other.weight);
    }

    @Override
    public String toString() {
      return String.format("(%d,%d):%.1f", fromPixelID, toPixelID, weight);
    }
  }
}

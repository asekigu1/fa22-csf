package exam;

/**
 * A raster-based implementation of hw9.UnionFind ADT.
 */
public class RasterUnionFind implements UnionFind<Integer> {
  // Do not rename these fields!
  private final int[][] parentID;
  private int count;

  /**
   * Construct a hw9.RasterUnionFind.
   *
   * @param width number of columns.
   * @param height number of rows. Pre: width and height are positive values.
   */
  public RasterUnionFind(int width, int height) {
    count = height * width;
    parentID = new int[height][width];
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        parentID[i][j] = i * width + j;
      }
    }
  }

  private int getX(int pixelID) {
    return pixelID / parentID[0].length;
  }

  private int getY(int pixelID) {
    return pixelID % parentID[0].length;
  }

  @Override
  public int find(Integer pixelID) {
    if (pixelID == parentID[getX(pixelID)][getY(pixelID)]) {
      return pixelID;
    }
    return find(parentID[getX(pixelID)][getY(pixelID)]);
  }

  @Override
  public boolean union(Integer pixelID1, Integer pixelID2) {
    int root1 = find(pixelID1);
    int root2 = find(pixelID2);
    if (root1 == root2) {
      return false;
    }
    int larger = Math.max(root1, root2);
    parentID[getX(larger)][getY(larger)] = Math.min(root1, root2);
    count--;
    return true;
  }

  @Override
  public int count() {
    return count;
  }
}

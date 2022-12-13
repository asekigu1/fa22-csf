package exam;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class UnionFindTest {
  private UnionFind<Integer> unionFind;

  @Test
  @DisplayName("A 2-by-2 raster with two regions")
  void sample1() {
    //   b  w
    //   w  w
    unionFind = new RasterUnionFind(2, 2);
    unionFind.union(1, 3);
    unionFind.union(2, 3);
    assertEquals(unionFind.count(), 2);
    assertEquals(unionFind.find(0), 0);
    assertEquals(unionFind.find(1), 1);
    assertEquals(unionFind.find(2), 1);
    assertEquals(unionFind.find(3), 1);
  }

  @Test
  @DisplayName("A 3-by-3 raster with three regions")
  void sample2() {
    //   w  w  b
    //   w  w  w
    //   b  w  w
    unionFind = new RasterUnionFind(3, 3);
    unionFind.union(0, 1);
    unionFind.union(0, 3);
    unionFind.union(1, 0);
    unionFind.union(1, 4);
    unionFind.union(3, 0);
    unionFind.union(3, 4);
    unionFind.union(4, 3);
    unionFind.union(4, 1);
    unionFind.union(4, 5);
    unionFind.union(4, 7);
    unionFind.union(5, 4);
    unionFind.union(5, 8);
    unionFind.union(7, 4);
    unionFind.union(7, 8);
    unionFind.union(8, 5);
    unionFind.union(8, 7);
    assertEquals(unionFind.count(), 3);
    assertEquals(unionFind.find(0), 0);
    assertEquals(unionFind.find(1), 0);
    assertEquals(unionFind.find(2), 2);
    assertEquals(unionFind.find(3), 0);
    assertEquals(unionFind.find(4), 0);
    assertEquals(unionFind.find(5), 0);
    assertEquals(unionFind.find(6), 6);
    assertEquals(unionFind.find(7), 0);
    assertEquals(unionFind.find(8), 0);
  }

  @Test
  @DisplayName("A 3-by-3 raster with nine regions")
  void sample3() {
    //   a  b  c
    //   d  e  f
    //   g  h  i
    unionFind = new RasterUnionFind(3, 3);
    assertEquals(unionFind.count(), 9);
    assertEquals(unionFind.find(0), 0);
    assertEquals(unionFind.find(1), 1);
    assertEquals(unionFind.find(2), 2);
    assertEquals(unionFind.find(3), 3);
    assertEquals(unionFind.find(4), 4);
    assertEquals(unionFind.find(5), 5);
    assertEquals(unionFind.find(6), 6);
    assertEquals(unionFind.find(7), 7);
    assertEquals(unionFind.find(8), 8);
  }

  @Test
  @DisplayName("A 3-by-3 raster with one region")
  void sample4() {
    //   w  w  w
    //   w  w  w
    //   w  w  w
    unionFind = new RasterUnionFind(3, 3);
    unionFind.union(0, 1);
    unionFind.union(0, 3);
    unionFind.union(1, 0);
    unionFind.union(1, 4);
    unionFind.union(1, 2);
    unionFind.union(2, 1);
    unionFind.union(2, 5);
    unionFind.union(3, 0);
    unionFind.union(3, 4);
    unionFind.union(3, 6);
    unionFind.union(4, 3);
    unionFind.union(4, 1);
    unionFind.union(4, 5);
    unionFind.union(4, 7);
    unionFind.union(5, 2);
    unionFind.union(5, 4);
    unionFind.union(5, 8);
    unionFind.union(6, 3);
    unionFind.union(6, 7);
    unionFind.union(7, 6);
    unionFind.union(7, 4);
    unionFind.union(7, 8);
    unionFind.union(8, 5);
    unionFind.union(8, 7);
    assertEquals(unionFind.count(), 1);
    assertEquals(unionFind.find(0), 0);
    assertEquals(unionFind.find(1), 0);
    assertEquals(unionFind.find(2), 0);
    assertEquals(unionFind.find(3), 0);
    assertEquals(unionFind.find(4), 0);
    assertEquals(unionFind.find(5), 0);
    assertEquals(unionFind.find(6), 0);
    assertEquals(unionFind.find(7), 0);
    assertEquals(unionFind.find(8), 0);
  }

  @Test
  @DisplayName("A 3-by-3 raster with three regions")
  void sample5() {
    //   w  w  w
    //   b  b  b
    //   r  r  r
    unionFind = new RasterUnionFind(3, 3);
    unionFind.union(0, 1);
    unionFind.union(1, 0);
    unionFind.union(1, 2);
    unionFind.union(2, 1);
    unionFind.union(3, 4);
    unionFind.union(4, 3);
    unionFind.union(4, 5);
    unionFind.union(5, 4);
    unionFind.union(6, 7);
    unionFind.union(7, 6);
    unionFind.union(7, 8);
    unionFind.union(8, 7);
    assertEquals(unionFind.count(), 3);
    assertEquals(unionFind.find(0), 0);
    assertEquals(unionFind.find(1), 0);
    assertEquals(unionFind.find(2), 0);
    assertEquals(unionFind.find(3), 3);
    assertEquals(unionFind.find(4), 3);
    assertEquals(unionFind.find(5), 3);
    assertEquals(unionFind.find(6), 6);
    assertEquals(unionFind.find(7), 6);
    assertEquals(unionFind.find(8), 6);
  }
}

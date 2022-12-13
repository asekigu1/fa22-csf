package exam;

/**
 * Represents a collection of non-overlapping (disjoint) sets.
 *
 * @param <T> The type of the items contained.
 */
public interface UnionFind<T> {

  /**
   * Returns the label representative of the set containing the given item.
   *
   * @param item to look in this data structure. Pre: item is valid and contained in this data structure.
   * @return the label of the set containing the given item.
   */
  int find(T item);

  /**
   * If the given items are in different sets, merges those sets and returns `true`. Otherwise, does nothing and returns
   * `false`.
   *
   * @param item1 an item in this data structure.
   * @param item2 an item in this data structure. Pre: item1 and item2 are valid and contained in this data
   *     structure.
   * @return true if merge happened and false otherwise.
   */
  boolean union(T item1, T item2);

  /**
   * Returns the number of disjoint sets (connected components).
   *
   * @return the number of disjoint sets.
   */
  int count();
}

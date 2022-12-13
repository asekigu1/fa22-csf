# Final Exam

1. Briefly describe how the minimum spanning tree-based segmentation works. 
   Please describe the algorithm at high-level (don't describe the code). 
   Keep the answer to a paragraph or two; be brief and to the point.

   kruskal() first sorts all edges in ascending order, based on the edge weight
   (here the weight is the difference in color). Pick the smallest edge, and 
   the vertices of the edge with the smallest color difference are joined by 
   union(). Repeat last step until the regions.count() matches 
   numberOfDesiredRegions.

---


2. Carefully inspect the `addAllEdges` method. 
   "It seems that each edge is added twice to the priority queue." 
   Is this statement true or false. If it is false, state why. 
   If it is true, state why this is not a problem (not a bug).

   True but not a problem because the call to union() on line 29
   will not change anything about the trees. The parentID element is 
   just reassigned to the same ID it was already previously assigned to.
   
----


3. Carefully inspect the `kruskal` method. 
   "It seems the implementation of kruskal is missing the step to check for cycles and avoid it." 
   Is this statement true or false. If it is false, state why. 
   If it is true, state why this is not a problem (not a bug).

   True but not a problem because we are stopping when the regions.count()
   hits a threshold, not when we've added (N-1) edges. Creating a cycle will
   not affect the regions.count(), so cycles aren't problematic.
   
----


4. Consider this statement: "Kruskal is a better choice compared to Prim's algorithm for 
   minimum spanning tree-based segmentation." 
   Is this statement true or false. Justify your answer. 
   Keep the answer to a paragraph; be brief and to the point.
   
   True because Kruksal's algorithm allows a forest but Prim's will continue to expand
   the graph started from the first random vertex. If we implemented Prim's algorithm,
   and similar to Kruksal, made it exit the algorithm once the desired region number 
   is reached, the region including the first random vertex will continue to expand
   itself with some other regions untouched, regardless of how close together other 
   regions may be. For example, if we ran ImageSegmentation with Prim's algorithm on 
   gradient.png for 3 regions, it would continue to union() the neighbors to the first 
   random vertex until only 2 pixels are left. This is not a good segmentation as some
   pixels are closer to the remaining 2 pixels than the initial random vertex.

---


5. Finally, complete the following code snippet and provide the missing running times. 
   Please describe the running time as a function of image width ($w$) and image height ($h$). 
   Assume Java's priority queue is implemented as a binary heap where all core operations are logarithmic. 
   Assume Java's hash table (HashMap) operates under uniform hashing assumption with 
   excellent collision resolution that results in constant time operations!

```java
Color[][] pixels = ImageUtility.getPixels(image); // O(w x h)
PriorityQueue<Edge> edges = addAllEdges(pixels);  // O(log(w x h))
UnionFind<Integer> regions = new RasterUnionFindSln(image.getWidth(), image.getHeight()); // O(w x h)

while (!edges.isEmpty()) { // O(w x h)
  Edge e = edges.poll();   // O(1)
  regions.union(e.fromPixelID, e.toPixelID);  // O(log(w x h))
  if (regions.count() == numberOfDesiredRegions) { // O(1)
    break; // O(1)
  }
}

HashMap<Integer, Color> colors = assignColorToEachRegion(pixels, regions); // O(w x h x log(w x h))
reColor(image, regions, colors); // O(w x h)
```
 

---


6. In the implementation of `RasterUnionFind`, we did not incorporate any of the 
   improvements (the union-by-size and path compression). 
   That said, it seems our implementation is performing path compression! 
   For example, consider the output of `ImageRegions` for the `color.png` file:
   Briefly elaborate if this is a lucky incidence or there is any aspect of our implementation 
   which increases the likelihood of this unplanned path compression. 
   Keep the answer to a paragraph; be brief and to the point.
   
This is not a lucky incidence; in fact, our implementation of ImageRegions not only increases the likelihood
of a flat tree but guarantees that max height is 1. In union(), the merging is done by 
setting the root having the smaller pixelID value be the parent of the other root. This means 
ImageRegions.labelRegions() iterates over the grid from upper left to bottom right, in 
other words in the pixelID's ascending order. This means for every region, the smallest pixelID
becomes the first root after the first union(), and for every proceeding union(a, b) a < b, 
a continues to be the direct child of the original root, and by the implementation of union(),
b also becomes the direct child of the original root. The smallest value in the region 
is maintained as the only parent node for the entire region.
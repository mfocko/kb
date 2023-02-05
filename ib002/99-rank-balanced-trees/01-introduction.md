---
title: Introduction
description: |
  Summing up the contents of the „Rank-Balanced Trees“ chapter.
tags:
- balanced trees
---

Data structures have become a regular part of the essential toolbox for
problem-solving. In many cases, they also help to improve the existing algorithm's
performance, e.g. using a priority queue in _Dijkstra's algorithm for the shortest_
_path_. We will mainly discuss the implementation of a set.

Currently, the most commonly used implementations of sets use hash tables, but we
will talk about another common alternative, implementation via self-balancing
search trees. Compared to a hash table, they provide consistent time complexity,
but at the cost of a requirement for ordering on the elements. The most implemented
self-balancing binary tree is a _red-black tree_, as described by _Guibas and_
_Sedgewick_[^1]. Among other alternatives, we can find (non-binary) _B-tree_[^2]
and _AVL tree_[^3].

We will start by discussing the properties of the AVL tree[^3], then we will
follow up on that with a definition of a _rank-balanced tree_, including
implementation of algorithms for implementing commonly known trees such as red-black
tree and AVL tree using the given representation. Afterwards we will delve into
the details of the _Weak AVL (WAVL) tree_[^4] that has relaxed requirements
compared to the AVL tree.

[^1]: GUIBAS, Leo J.; SEDGEWICK, Robert. A dichromatic framework for balanced trees. In: _19th Annual Symposium on Foundations of Computer Science (sfcs 1978)_. 1978, pp. 8–21. Available from doi: [10.1109/SFCS.1978.3](https://doi.org/10.1109/SFCS.1978.3).
[^2]: BAYER, R.; MCCREIGHT, E. Organization and Maintenance of Large Ordered Indices. In: _Proceedings of the 1970 ACM SIGFIDET (Now SIGMOD) Workshop on Data Description, Access and Control_. Houston, Texas: Association for Computing Machinery, 1970, pp. 107–141. SIGFIDET ’70. isbn 9781450379410. Available from doi: [10.1145/1734663.1734671](https://doi.org/10.1145/1734663.1734671).
[^3]: ADELSON-VELSKIJ, Georgij; LANDIS, Evgenij. An algorithm for the organization of information. _Doklady Akad. Nauk SSSR_. 1962, vol. 146, pp. 263–266.
[^4]: HAEUPLER, Bernhard; SEN, Siddhartha; TARJAN, Robert E. Rank-Balanced Trees. _ACM Trans. Algorithms_. 2015, vol. 11, no. 4. issn 1549-6325. Available from doi: [10.1145/2689412](https://doi.org/10.1145/2689412).
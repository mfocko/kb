---
title: Distance boundaries from BFS tree on undirected graphs
description: |
  Short explanation of distance boundaries deduced from a BFS tree.
tags:
- graphs
- bfs
---

## Introduction

As we have talked on the seminar, if we construct from some vertex $u$ BFS tree on an undirected graph, we can obtain:

- lower bound of length of the shortest path between 2 vertices from the _height difference_
- upper bound of length of the shortest path between 2 vertices from the _path through the root_

## Lower bound

Consider the following graph:

![BFS graph](/files/ib002/bfs-tree/bfs_graph_light.svg#gh-light-mode-only)![BFS graph](/files/ib002/bfs-tree/bfs_graph_dark.svg#gh-dark-mode-only)

We run BFS from the vertex $a$ and obtain the following BFS tree:

![BFS tree](/files/ib002/bfs-tree/bfs_tree_light.svg#gh-light-mode-only)![BFS tree](/files/ib002/bfs-tree/bfs_tree_dark.svg#gh-dark-mode-only)

Let's consider pair of vertices $e$ and $h$. For them we can safely lay, from the BFS tree, following properties:

- lower bound: $2$
- upper bound: $4$

By having a look at the graph we started from, we can see that we have a path ‹$e, j, h$› that has a length 2. Apart from that we can also notice there is another path from $e$ to $h$ and that is ‹$e, a, c, i, d, h$›. And that path has a length of $5$. Doesn't this break our statements at the beginning? (_I'm leaving that as an exercise ;)_)

## Proof by contradiction

Let's keep the same graph, but break the lower bound, i.e. I have gotten a lower bound $2$, but „there must be a shorter path“! ;)

Now the more important question, is there a shorter path in that graph? The answer is no, there's no shorter path than the one with length $2$. So what can we do about it? We'll add an edge to have a shorter path. Now we have gotten a lower bound of $2$, which means the only shorter path we can construct has $1$ edge and that is ‹$e, h$› (no intermediary vertices). Let's do this!

![BFS tree](/files/ib002/bfs-tree/bfs_graph_with_additional_edge_light.svg#gh-light-mode-only)![BFS tree](/files/ib002/bfs-tree/bfs_graph_with_additional_edge_dark.svg#gh-dark-mode-only)

Okay, so we have a graph that breaks the rule we have laid. However, we need to run BFS to obtain the new BFS tree, since we have changed the graph.

:::tip

Do we need to run BFS after **every** change?

­I am leaving that as an exercise ;)

:::

![BFS tree](/files/ib002/bfs-tree/bfs_tree_with_additional_edge_light.svg#gh-light-mode-only)![BFS tree](/files/ib002/bfs-tree/bfs_tree_with_additional_edge_dark.svg#gh-dark-mode-only)

Oops, we have gotten a new BFS tree, that has a height difference of 1.

:::tip

Try to think about a way this can be generalized for shortening of minimal length 3 to minimal length 2 ;)

:::

---
title: Iterative algorithms via iterators
description: |
  Examples of iterative implementations of DFS using iterators.
---

## Introduction

- [Source code used later on.](pathname:///files/ib002/iterative-and-iterators/)

As we have talked on the seminar, iterative approach to implementing DFS is not very intuitive and is a very easy way how to create an incorrect implementation.

On the other hand, we have seen iterative implementation in the exercises and I have also prepared two from which one was similar to recursive implementation without colors from exercises and the other one used features of high-level languages.

## Different implementations

### Recursive DFS implementation from exercises without colors

```
FUNCTION VisitedDFS(u, Visited) IS
    Visited <- Union(Visited, { u })
    FOR v IN u.successors DO
        IF v NOT IN Visited THEN
            Visited <- VisitedDFS(v, Visited)
        FI
    OD

    RETURN Visited
END
```

This implementation is correct, does the DFS traversal as it should, however it has one „smallish“ downside and that is the time complexity. The usage of set raises the time complexity, of course it is implementation dependant. However in case of either RB-tree or hash-table implementation, we get look-up in time $\mathcal{O}(n)$ for hash-table in worst-case or $\mathcal{O}(\log n)$ for the other in the worst-case. Both are not ideal compared to checking color on vertex.

### Iterative DFS from the exercises

```
PROCEDURE IterDFS(u)
    stack <- empty stack
    Push(stack, (u, 0))
    u.color <- gray
    time <- 1
    u.d <- time

    WHILE NOT Empty(stack) DO
        (u, k) <- Pop(stack)

        IF k < Length(u.successors) THEN
            // search is not finished, is pushed back to stack
            Push(stack, (u, k + 1))

            v <- u.successors[k + 1]
            IF v.color = white THEN
                Push(stack, (v, 0))
                v.color <- gray
                time <- time + 1
                v.d <- time
            FI
        ELSE
            // u has no other successors, we can finish the search
            time <- time + 1
            u.f <- time
            u.color <- black
        FI
    OD
END
```

As we can see, there is some ordering in which we search through the successors. Time complexity is OK, stack holds at most all vertices (they must be on the current path).

### My iterative with path in stack

```
PROCEDURE DFS(G, start) IS
    path <- [ start ]
    time <- 1
    start.d, start.color <- time, gray

    WHILE NOT Empty(path) DO
        hasSuccessor <- false
        FOR successor IN path[-1].successors DO
            IF successor.color = white THEN
                hasSuccessor <- true
                successor.d, successor.color <- ++time, gray
                path <- Append(path, successor)
                BREAK
            FI
        OD
        IF NOT hasSuccessor THEN
            lastVertex <- Pop(path)
            lastVertex.f, lastVertex.color <- ++time, black
        FI
    OD
END
```

This approach is similar to the iterative solution from the exercises, but it does not keep the index of the next successor, therefore it always iterates through all of them, which raises the time complexity.

### My iterative solution with iterators

On the other hand, we do not actually have to depend on the representation of the graph. In this case, we just _somehow_ obtain the iterator (which yields all of the succesors) and keep it in the stack.

```
PROCEDURE DFS(G, start) IS
    path <- [ (start, Iterator(start.successors)) ]
    time <- 1
    start.d, start.color <- time, gray

    WHILE NOT Empty(path) DO
        lastVertex, successors <- path[-1]

        IF NOT MoveNext(successors) THEN
            Pop(path)
            lastVertex.f, lastVertex.color <- ++time, black
        ELSE IF successors.Current.color = white THEN
            nextVertex <- successors.Current
            nextVertex.d, nextVertex.color <- ++time, gray
            path <- Append(path, (nextVertex, Iterator(nextVertex.successors)))
        FI
    OD
END
```

( The way we manipulate with the iterators is closest to the C# implementation. Apart from the `Iterator` thing :) In case you tried to implement it in C++, you would more than likely need to change the check, since you would get first successor right at the beginning )

So here we don't keep indices, but the iterators. We can also check existence of other successors easily: by the iterator moving after the last successor.

Closer explanation of the _iterator shenanigans_ follows. In the beginning, either `start` or when pushing new vertex, we are pushing an iterator that points _just before_ the first successor. When populating `lastVertex` and `successors` in the `while`-loop, we take the element from the top of the stack. `MoveNext` returns `true` if there is an element, i.e. successor in this case. If it returns `false` we have nothing to do and we pop the vertex from the stack (also set finishing time and color). If we have successor we check if it has been already visited or not. If has not, we set discovery time and color accordingly, also we add it to stack.

## Implementation

In case you want to play around with the code. At the beginning there is a link to the C# implementation that can be used. It has a basic representation of graph and includes BFS/DFS implementation in classes.

In `Program.cs` you can also find a method that returns graph we used on the seminar.

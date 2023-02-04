---
title: Iterative algorithms via iterators
description: |
  Iterative DFS using iterators.
tags:
- csharp
- graphs
- iterators
- iterative
---

## Introduction

- [Source code used later on.](pathname:///files/ib002/iterative-and-iterators/)

As we have talked on the seminar, iterative approach to implementing DFS is not very intuitive and is a very easy way how to create an incorrect implementation.

On the other hand, we have seen iterative implementation in the exercises and I have also prepared two from which one was similar to recursive implementation without colors from exercises and the other one used features of high-level languages.

## Different implementations

### Recursive DFS implementation from exercises without colors

```ada showLineNumbers
function VisitedDFS(u: Vertex, visited: VertexSet) return VertexSet is
    v: Vertex;
begin
    visited.Union(To_Set(u));

    for v in u.successors loop
        if not Contains(visited, v) then
            visited := visitedDFS(v, Visited);
        end if;
    end loop;

    return visited;
end VisitedDFS;
```

This implementation is correct, does the DFS traversal as it should, however it has one „smallish“ downside and that is the time complexity. The usage of set raises the time complexity, of course it is implementation dependant. However in case of either RB-tree or hash-table implementation, we get look-up in time $\mathcal{O}(n)$ for hash-table in worst-case or $\mathcal{O}(\log n)$ for the other in the worst-case. Both are not ideal compared to checking color on vertex.

### Iterative DFS from the exercises

```ada showLineNumbers
procedure IterDFS(u: Vertex) is
    stack: StateVector;
    i, time: Integer;
    v: Vertex;
begin
    stack.Append(VertexState(u, 0));
    u.color := Gray;
    time := 1;
    u.d := time;

    while not stack.Is_Empty loop
        u := stack.Last_Element.Vertex;
        i := stack.Last_Element.NextIndex;
        stack.Delete_Last;

        if i < u.successors.Length then
            -- search is not finished, is pushed back to stack
            stack.Append(VertexState(u, k + 1));

            v := u.successors.Element(i);
            if v.color = White then
                stack.Append(VertexState(v, 0));
                v.color := Gray;
                time := time + 1;
                v.d := time;
            end if;
        else
            -- u has no other successors, we can finish the search
            time := time + 1;
            u.f := time;
            u.color := Black;
        end if;
    end loop;

end IterDFS;
```

As we can see, there is some ordering in which we search through the successors. Time complexity is OK, stack holds at most all vertices (they must be on the current path).

### My iterative with path in stack

```ada showLineNumbers
procedure DFS(start: Vertex) is
    path: VertexVector;
    time: Integer;
    hasSuccessor: Bool;
    successor: Vertex;
begin
    path.Append(start);
    time := 1;

    start.d := time;
    start.color := Gray;

    while not path.Is_Empty loop
        hasSuccessor := false;

        for successor in path.Last_Element.successors loop
            if successor.color = White then
                hasSuccessor := true;

                successor.d := time + 1;
                successor.color := Gray;
                time := time + 1;

                path.Append(successor);

                exit;
            end if;
        end loop;

        if not hasSuccessor then
            path.Last_Element.f := time + 1;
            path.Last_Element.color := Black;

            time := time + 1;
            path.Delete_Last;
        end if;

    end loop;
end DFS;
```

This approach is similar to the iterative solution from the exercises, but it does not keep the index of the next successor, therefore it always iterates through all of them, which raises the time complexity.

### My iterative solution with iterators

On the other hand, we do not actually have to depend on the representation of the graph. In this case, we just _somehow_ obtain the iterator (which yields all of the succesors) and keep it in the stack.

```ada showLineNumbers
procedure DFS(start: Vertex) is
    path: StateVector;
    time: Integer;
    current: State;
    nextVertex: Vertex;
begin
    path.Append(State(start));
    time := 1;

    start.d := time;
    start.color := Gray;

    while not path.Is_Empty loop
        current := path.Last_Element;

        if not Move_Next(current.successors) then
            path.Delete_Last;

            time := time + 1;
            current.vertex.f := time;

            current.vertex.color := Black;
        else if current.successors.Value.color = white then
            nextVertex := current.successors.Value;

            time := time + 1;
            nextVertex.d := time;

            nextVertex.color := Gray;

            path.Append(State(nextVertex));
        end if;
    end loop;
end DFS;
```


( The way we manipulate with the iterators is closest to the C# implementation. Apart from the `Iterator` thing :) In case you tried to implement it in C++, you would more than likely need to change the check, since you would get first successor right at the beginning )

So here we don't keep indices, but the iterators. We can also check existence of other successors easily: by the iterator moving after the last successor.

Closer explanation of the _iterator shenanigans_ follows. In the beginning, either `start` or when pushing new vertex, we are pushing an iterator that points _just before_ the first successor. When populating `lastVertex` and `successors` in the `while`-loop, we take the element from the top of the stack. `MoveNext` returns `true` if there is an element, i.e. successor in this case. If it returns `false` we have nothing to do and we pop the vertex from the stack (also set finishing time and color). If we have successor we check if it has been already visited or not. If has not, we set discovery time and color accordingly, also we add it to stack.

## Implementation

In case you want to play around with the code. At the beginning there is a link to the C# implementation that can be used. It has a basic representation of graph and includes BFS/DFS implementation in classes.

In `Program.cs` you can also find a method that returns graph we used on the seminar.

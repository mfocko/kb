using System;
using System.Collections.Generic;

namespace graphs {
    class Program {
        static Graph<string> CreateExampleGraph() {
            var vertices = new List<string>() {
                "s", "q", "t", "y", "r", "v", "w", "x", "z", "u"
            };

            var graph = new Graph<string>();

            foreach (var vertex in vertices) {
                graph.AddVertex(vertex);
            }

            graph.AddEdge("s", "v");
            graph.AddEdge("q", "s");
            graph.AddEdge("q", "w");
            graph.AddEdge("q", "t");
            graph.AddEdge("t", "y");
            graph.AddEdge("t", "x");
            graph.AddEdge("y", "q");
            graph.AddEdge("r", "y");
            graph.AddEdge("r", "u");
            graph.AddEdge("v", "w");
            graph.AddEdge("w", "s");
            graph.AddEdge("x", "w");
            graph.AddEdge("x", "z");
            graph.AddEdge("z", "x");
            graph.AddEdge("u", "y");

            return graph;
        }

        static void Main(string[] args) {
            var graph = CreateExampleGraph();

            var dfsTraversal = new DFS<string>(graph).Run();
            foreach (var vertex in graph.Vertices) {
                Console.WriteLine($"{vertex}(d: {dfsTraversal.DiscoveredAt(vertex)}, f: {dfsTraversal.FinishedAt(vertex)})");
            }
        }
    }
}

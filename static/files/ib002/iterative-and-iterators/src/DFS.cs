using System.Collections.Generic;

namespace graphs {
    class DFS<T> where T : notnull {
        private Graph<T> graph;

        private int time;
        private Dictionary<T, int> Discovered = new Dictionary<T, int>();
        private Dictionary<T, int> Finished = new Dictionary<T, int>();
        private Dictionary<T, Color> State = new Dictionary<T, Color>();

        public DFS(Graph<T> graph) {
            this.graph = graph;
        }

        public DFS<T> Run() {
            time = 0;

            foreach (var vertex in graph.Vertices) {
                if (ColorOf(vertex) == Color.White) {
                    RunFrom(vertex, false);
                }
            }

            return this;
        }

        public DFS<T> RunFrom(T start, bool reset = true) {
            var path = new Stack<(T, IEnumerator<T>)>();

            if (reset) {
                time = 0;
            }

            (Discovered[start], State[start]) = (++time, Color.Gray);
            path.Push((start, graph.GetEdgesFrom(start)));

            while (path.Count > 0) {
                var (lastVertex, successors) = path.Peek();

                if (!successors.MoveNext()) {
                    path.Pop();
                    (Finished[lastVertex], State[lastVertex]) = (++time, Color.Black);
                } else if (ColorOf(successors.Current) == Color.White) {
                    var nextVertex = successors.Current;

                    (Discovered[nextVertex], State[nextVertex]) = (++time, Color.Gray);
                    path.Push((nextVertex, graph.GetEdgesFrom(nextVertex)));
                }
            }

            return this;
        }

        private static V GetOrDefault<U, V>(Dictionary<U, V> flags, U u, V defaultValue)
            where U : notnull
            where V : notnull {
            if (flags.TryGetValue(u, out var flag)) {
                return flag;
            }
            return defaultValue;
        }

        public int DiscoveredAt(T u) => GetOrDefault(Discovered, u, 0);
        public int FinishedAt(T u) => GetOrDefault(Finished, u, 0);
        public Color ColorOf(T u) => GetOrDefault(State, u, Color.White);
    }
}

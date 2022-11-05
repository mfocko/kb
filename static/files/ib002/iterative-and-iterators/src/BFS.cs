using System.Collections.Generic;

namespace graphs {
    class BFS<T> where T : notnull {
        private Graph<T> graph;

        private Dictionary<T, int> Distance = new Dictionary<T, int>();
        private Dictionary<T, T?> Parent = new Dictionary<T, T?>();
        private Dictionary<T, Color> State = new Dictionary<T, Color>();

        public BFS(Graph<T> graph) {
            this.graph = graph;
        }

        public BFS<T> Run() {
            foreach (var vertex in graph.Vertices) {
                if (ColorOf(vertex) == Color.White) {
                    RunFrom(vertex);
                }
            }

            return this;
        }

        public BFS<T> RunFrom(T start) {
            State[start] = Color.Gray;
            Distance[start] = 0;
            Parent[start] = default(T);

            var queue = new Queue<T>();
            queue.Enqueue(start);

            while (queue.Count > 0) {
                var u = queue.Dequeue();

                for (var iterator = graph.GetEdgesFrom(u); iterator.MoveNext();) {
                    var v = iterator.Current;

                    if (GetOrDefault(State, v, Color.White) == Color.White) {
                        State[v] = Color.Gray;
                        Distance[v] = Distance[u] + 1;
                        Parent[v] = u;

                        queue.Enqueue(v);
                    }
                }

                State[u] = Color.Black;
            }

            return this;
        }

        private static V GetOrDefault<U, V>(Dictionary<U, V> flags, U u, V defaultValue)
            where U : notnull {
            if (flags.TryGetValue(u, out var flag)) {
                return flag;
            }
            return defaultValue;
        }

        public int DistanceTo(T u) => GetOrDefault(Distance, u, 0);
        public T? ParentOf(T u) => GetOrDefault(Parent, u, default(T));
        public Color ColorOf(T u) => GetOrDefault(State, u, Color.White);
    }
}

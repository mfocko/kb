using System.Collections.Generic;
using System.Linq;

namespace graphs {
    enum Color {
        White, Gray, Black
    }

    class Graph<T> where T : notnull {
        private SortedSet<T> vertices = new SortedSet<T>();
        private SortedDictionary<T, SortedSet<T>> edges = new SortedDictionary<T, SortedSet<T>>();

        public SortedSet<T> Vertices {
            get => vertices;
        }

        public void AddVertex(T u) {
            vertices.Add(u);
            edges.Add(u, new SortedSet<T>());
        }

        public void AddEdge(T u, T v) {
            if (edges.TryGetValue(u, out var edgesFromU)) {
                edgesFromU.Add(v);
            }
        }

        public bool HasEdge(T u, T v) {
            if (edges.TryGetValue(u, out var edgesFromU)) {
                return edgesFromU.Contains(v);
            }

            return false;
        }

        public IEnumerator<T> GetEdgesFrom(T u) {
            if (edges.TryGetValue(u, out var edgesFromU)) {
                return edgesFromU.GetEnumerator();
            }

            return Enumerable.Empty<T>().GetEnumerator();
        }
    }
}

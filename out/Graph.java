import java.util.Collection;
import java.util.HashSet;
import java.util.Set;

public abstract class Graph<N, E> {

    final Set<Node<N>> nodeSet = new HashSet<>();
    final Set<Edge<E>> edgeSet = new HashSet<>();

    public boolean isEmpty() {
        return nodeSet.isEmpty();
    }

    public Collection<Edge<E>> getEdges() {
        return edgeSet;
    }

    public void add(Node<N> node) {
        nodeSet.add(node);
    }

    public void add(Edge<E> edge) {
        edgeSet.add(edge);
    }

    public void add(Collection<Node<N>> nodes) {
        nodes.forEach(this::add);
    }

    public void add(Graph<N, E> graph) {
        for(Node<N> n : graph.nodeSet) {
            this.add(n);
        }
        for(Edge<E> e : graph.edgeSet) {
            this.add(e);
        }
    }

    public void dump(String filename) {
        // TODO
    }

    public void dumpAsDot(String filename) {
        // TODO
    }

}

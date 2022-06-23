import java.util.Iterator;

public class Digraph<N, E> extends Graph<N, E> {
    @Override
    public String generateDotString() {
        StringBuilder dot = new StringBuilder("digraph");
        dot.append('{');
        dot.append('\n');
        for(Edge<E> e : getEdges()) {
            dot.append(generateDotNodeInfo(e, true));
        }
        dot.append('}');
        return dot.toString();
    }


    // TODO: implement iteratorBFS and iteratorDFS
    @Override
    public Iterator<Node<N>> iteratorBFS(String label) {
        return null;
    }

    @Override
    public Iterator<Node<N>> iteratorDFS(String label) {
        return null;
    }
}

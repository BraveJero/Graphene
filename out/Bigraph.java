import javax.swing.plaf.ComponentInputMapUIResource;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Objects;
import java.util.Set;

public class Bigraph<N, E> extends Graph<N, E> {

    @Override
    public void add(Edge<E> edge) {
        super.add(edge);
        super.add(new Edge<>(edge.to, edge.from, edge.data));
    }

    @Override
    public String generateDotString() {
        Set<Pair> pairSet = new HashSet<>();
        StringBuilder dot = new StringBuilder("graph");
        dot.append('{');
        dot.append('\n');
        for(Edge<E> e : getEdges()) {
            Pair p = new Pair(e.from, e.to);
            if(pairSet.contains(p)) continue;
            dot.append(generateDotNodeInfo(e, false));
            pairSet.add(p);
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

    private static class Pair {
        String id1, id2;

        Pair(String id1, String id2) {
            this.id1 = id1;
            this.id2 = id2;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (!(o instanceof Pair pair)) return false;
            return (id1.equals(pair.id1) && id2.equals(pair.id2)) || (id1.equals(pair.id2) && id2.equals(pair.id1));
        }

        @Override
        public int hashCode() {
            if(id1.compareTo(id2) < 0)
                return Objects.hash(id1, id2);
            else
                return Objects.hash(id2, id1);
        }
    }

}

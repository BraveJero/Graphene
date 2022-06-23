import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.*;
import java.io.File;

public abstract class Graph<N, E> {

    private final Set<Node<N>> nodeSet = new HashSet<>();
    private final Set<Edge<E>> edgeSet = new HashSet<>();

    public boolean isEmpty() {
        return nodeSet.isEmpty();
    }

    public Collection<Edge<E>> getEdges() {
        return edgeSet;
    }

    public Collection<Node<N>> getNodes() {
        return nodeSet;
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
        try(FileWriter writer = new FileWriter(filename, false)) {
            writer.append(generateDotString());
        } catch(IOException ex) {
            System.out.println("Error opening file " + filename);
            System.out.println(ex.getMessage());
        }

    }

    public abstract String generateDotString();

    protected String generateDotNodeInfo(Edge<E> e, boolean digraph) {
        String separator = digraph? "->":"--";
        StringBuilder dot = new StringBuilder();
        dot.append('\t');
        dot.append('"');
        N fromData, toData;
        dot.append('\'');
        dot.append(e.from);
        dot.append('\'');
        if((fromData = getNodeData(e.from)) != null) {
            dot.append(": ");
            dot.append(fromData);
        }
        dot.append('"');
        dot.append(separator);
        dot.append('"');
        dot.append('\'');
        dot.append(e.to);
        dot.append('\'');
        if((toData = getNodeData(e.to)) != null) {
            dot.append(": ");
            dot.append(toData);
        }
        dot.append('"');
        E edgeData = e.getData();
        if(edgeData != null) {
            dot.append(String.format("[label=\"%s\"]", edgeData));
        }
        dot.append(';');
        dot.append('\n');
        return dot.toString();
    }

    protected N getNodeData(String label) {
        for(Node<N> n : getNodes()) {
            if(n.label.equals(label)) {
                return n.data;
            }
        }
        return null;
    }

}

import java.util.Objects;

public class Edge<E> {

    final String from, to;
    E data;

    public Edge(String from, String to, E data) {
        this.from = from;
        this.to = to;
        this.data = data;
    }

    public Edge(String from, String to) {
        this.from = from;
        this.to = to;
        this.data = null;
    }

    public E getData() {
        return data;
    }

    public void setData(E data) {
        this.data = data;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Edge<?> edge = (Edge<?>) o;
        return Objects.equals(from, edge.from) && Objects.equals(to, edge.to);
    }

    @Override
    public int hashCode() {
        return Objects.hash(from, to);
    }

}

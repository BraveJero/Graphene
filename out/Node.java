import java.util.Objects;

public class Node<N> {

    final String label;
    N data;

    public Node(String label) {
        this.label = label;
        this.data = null;
    }

    public Node(String label, N data) {
        this.label = label;
        this.data = data;
    }

    public N getData() {
        return data;
    }

    public void setData(N data) {
        this.data = data;
    }


    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Node<?> node = (Node<?>) o;
        return Objects.equals(label, node.label);
    }

    @Override
    public int hashCode() {
        return Objects.hash(label);
    }
}

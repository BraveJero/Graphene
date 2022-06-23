public class Bigraph<N, E> extends Graph<N, E> {

    @Override
    public void add(Edge<E> edge) {
        super.add(edge);
        super.add(new Edge<>(edge.to, edge.from, edge.data));
    }

}

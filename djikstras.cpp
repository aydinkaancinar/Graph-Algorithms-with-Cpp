//
// Created by Kaan Çınar on 8.07.2020.
//
// While creating this program I learnt about Djikstra's Algorithm, C++ vector, random number generator,
// implementing constructors for classes, and how to implement a class in C++ classes.

#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

const int MAX_NODES = 50; //maximum number of nodes
const int MAX_DISTANCE = 10; //maximum cost of edges

//a class that represents an edge
class Edge {
public:
    //constructor
    Edge(int index1 = 0, int index2 = 0, int value = 0):index1(index1), index2(index2), value(value){}
    //returns the value associated to the edge
    int get_value(){return value;}
    //sets the value associated to the edge
    void set_value(int new_value){value = new_value;}
    //returns the first node attached to the edge
    int get_index1(){return index1;}
    //returns the second node attached to the edge
    int get_index2(){return index2;}
    //print function for edge
    void print(){
        cout << "(" << index1 << "," << index2 << ")";
    }
    //destructor
    ~Edge(){}
private:
    int index1; //index of the node attached to the edge
    int index2; //index of the other node attached to the edge
    int value; //value associated to the edge
};

class Node {
public:
    //constructor
    Node(int index = -1, int value = 0):index(index), value(value){}
    //returns the value associated to the node
    int get_value(){return value;}
    //sets the value associated to the node
    void set_value(int new_value){value = new_value;}
    //returns the index associated to the node
    int get_index(){return index;}
    //connects the node x with this node with an edge
    void add_edge(int x, int edge_value = 0){
        edges.push_back(Edge(index, x, edge_value));
        neighbours.push_back(x);
    }
    //if the edge exists, removes the edge that connects this node with node with index2
    // othervise throws an exception
    void remove_edge(int index2){
        if(!adjacent(index2)){
            throw invalid_argument("there is not such an edge");
        }
        for(int i = 0; i < edges.size(); ++i){
            if(edges[i].get_index2() == index2){
                edges.erase(edges.begin()+i);
                return;
            }
        }
    }
    //returns if this node is adjacent to the node with index2
    bool adjacent(int index2){
        for(int i = 0; i < edges.size(); ++i){
            if(edges[i].get_index2() == index2){
                return true;
            }
        }
        return false;
    }
    //returns the degree of this node
    int degree(){
        return edges.size();
    }
    //returns the vector containing the edges connected to this node
    vector<Edge> get_edges(){
        return edges;
    }
    //returns a vector of index of neighbour nodes
    vector<int> get_neighbours(){
        return neighbours;
    }
    //gets the edge connecting this node with node with index2 if edge exists
    // otherwise throws an exception
    Edge get_edge(int index2){
        for(int i = 0; i < edges.size(); ++i){
            if(edges[i].get_index2() == index2){
                return edges[i];
            }
        }
        throw invalid_argument("there is not such an edge");
    }
    //sets the value of the edge connecting this node with the node with index2
    // otherwise throws an exception
    void set_edge_value(int index2, int edge_value){
        if(!adjacent(index2)){
            throw invalid_argument("there is not such an edge");
        }
        for(int i = 0; i < edges.size(); ++i){
            if(edges[i].get_index2() == index2){
                edges[i].set_value(edge_value);
            }
        }
    }
    //prints the node
    void print(){
        cout << "(" << index << ")";
    }
    //destructor
    ~Node(){}
private:
    int index; //index associated to this node
    int value; //value associated to this node
    vector<Edge> edges; //vector containing the edges of this node
    vector<int> neighbours; //vector of indices of neighbours
};

class Graph {
public:
    //constructor
    Graph(int size = 0, int num_edges = 0):size(size), num_edges(num_edges){
        for(int i = 0; i < size; ++i){
            nodes[i] = Node(i);
        }
    }
    //returns the number of vertices in the graph
    int V(){return size;}
    //returns the number of edges in the graph
    int E(){return num_edges;}
    //tests whether there is an edge from node x to node y.
    bool adjacent(int x, int y){
        if(nodes[x].degree() == 0){ //if the adj_list of x is empty, returns false;
            return false;
        } else {
            return nodes[x].adjacent(y);
        }
    }
    //lists all nodes y such that there is an edge from x to y.
    vector<int> neighbors(int x){return nodes[x].get_neighbours();}
    //adds to G the edge from x to y, if it is not there.
    void add(int x, int y){
        if(adjacent(x, y)){ //return if they already share an edge
            return;
        } else {
            num_edges ++;
            nodes[x].add_edge(y);
            nodes[y].add_edge(x);
            return;
        }
    }
    //removes the edge from x to y, if it is there
    void delete_edge(int x, int y) {
        if(adjacent(x, y)){
            nodes[x].remove_edge(y);
            nodes[y].remove_edge(x);
            num_edges--;
        } else { //return if they don't share an edge
            return;
        }
    }
    //returns the value associated with the node x
    int get_node_value(int x){return nodes[x].get_value();}
    //sets the value associated with the node x to a
    void set_node_value(int x, int a){nodes[x].set_value(a);}
    //returns the value associated to the edge (x,y)
    int get_edge_value(int x, int y){
        Edge e = nodes[x].get_edge(y);
        return e.get_value();
    }
    //sets the value associated to the edge (x,y) to v
    void set_edge_value (int x, int y, int v){
        nodes[x].set_edge_value(y, v);
        nodes[y].set_edge_value(x, v);
    }
    //prints the graph
    void print(){
        for (int i = 0; i < size; ++i)
        {
            cout << i << ": ";
            for(auto x : nodes[i].get_neighbours())
                cout << "-> " << x;
            printf("\n");
        }
    }
    //returns the list of nodes
    Node* get_nodes(){
        return nodes;
    }
    //destructor
    ~Graph(){}
private:
    Node nodes[MAX_NODES]; //has the nodes in this graph
    int size; //the number of the nodes in the graph
    int num_edges; //number of edges of the graph
};

//a PriorityQueue implementation for Nodes
class PriorityQueue {
public:
    //constructor, initializes an empty PriorityQueue
    PriorityQueue(){}
    //changes the priority (node value) of queue_element
    void chgPrioirity(int priority, Node queue_element){
        if(!contains(queue_element)){
            throw invalid_argument("there is no such element in queue");
        }
        for(int i = 0; i < queue.size(); ++i){
            //checks if x is same as queue_element
            if(queue[i].get_index() == queue_element.get_index()){
                queue[i].set_value(priority);
                return;
            }
        }
    }
    //removes the top element of the queue.
    Node minPrioirty(){
        if(queue.size() == 0){
            throw invalid_argument("there is no element in the queue");
        }
        int index_top = 0;
        int i;
        for(i = 0; i < queue.size(); ++i){
            if(queue[i].get_value()<queue[index_top].get_value()){
                index_top = i;
            }
        }
        Node temp = queue[index_top];
        queue.erase(queue.begin()+index_top);
        return temp;
    }
    //does the queue contain queue_element.
    bool contains(Node queue_element){
        for(auto x : queue){
            //checks if x is same as queue_element
            if(x.get_index() == queue_element.get_index()){
                return true;
            }
        }
        return false;
    }
    //insert queue_element into queue
    void insert(Node queue_element){queue.push_back(queue_element);}
    //returns the top element of the queue.
    Node top(){
        int index_top = 0;
        int i;
        for(i = 0; i < queue.size(); ++i){
            if(queue[i].get_value()<queue[index_top].get_value()){
                index_top = i;
            }
        }
        return queue[index_top];
    }
    //return the number of queue_elements.
    int size(){return queue.size();}
    //print the queue
    void print(){
        Node n;
        PriorityQueue p = PriorityQueue();
        while(queue.size()!=0) {
            n = minPrioirty();
            n.print();
            cout << " ";
            p.insert(n);
        }
        while(p.size()!=0) {
            insert(p.minPrioirty());
        }
    }
    //destructor
    ~PriorityQueue(){}
private:
    vector<Node> queue;
};

class ShortestPath{
public:
    ShortestPath(Graph graph, int start = 0):graph(graph), start(start){
        open = PriorityQueue();
        int u = start;
        for (int i = 0; i < graph.V(); i++)
            dist[i] = MAX_NODES*MAX_DISTANCE + 1;
        dist[u] = 0;
        Node* nodes = graph.get_nodes();
        nodes[u].set_value(0);
        open.insert(nodes[u]);
        while(closed.size() != graph.V()){
            if(open.size() == 0){
                throw invalid_argument("there is no path from u to w");
            }
            Node n = open.minPrioirty();
            closed.push_back(n.get_index());
            int edge_distance = -1;
            int new_distance = -1;
            vector<int> neighbours = n.get_neighbours();
            for(int i = 0; i < neighbours.size(); ++i) {
                int v = neighbours[i];
                if(!(find(closed.begin(), closed.end(), v) != closed.end())){
                    edge_distance = graph.get_edge_value(n.get_index(), v);
                    new_distance = dist[n.get_index()] + edge_distance;
                    if (new_distance < dist[v])
                        dist[v] = new_distance;
                    path_array[v] = path_array[n.get_index()];
                    path_array[v].push_back(v);
                    nodes[v].set_value(dist[v]);
                    open.insert(nodes[v]);
                }
            }
        }
    }
    //list of vertices in G(V,E).
    Node* vertices(){
        return graph.get_nodes();
    }
    //find shortest path between u-w and returns the sequence of vertices representing shorest path u-v1-v2-…-vn-w.
    void path(int w){
        if(w >= graph.V()){
            throw invalid_argument("the indexes are out of bounds for this graph");
        }
        if(path_array[w].size() == 0){
            throw invalid_argument("501: there is no path from u to w");
        }
        cout << "Path from " << start << " to " << w << ": " << start;
        for(auto x : path_array[w])
            cout << "-> " << x;
        printf("\n");
    }
    //return the path cost associated with the shortest path, returns -1 if there is no path
    int path_size(int w){
        if(w >= graph.V()){
            return -1;
        }
        if(dist[w] == 501){
            return -1;
        }
        return dist[w];
    }
    //returns the start point
    int get_start(){return start;}
    //print the graph
    void print(){graph.print();}
private:
    Graph graph;
    int start;
    int dist[MAX_NODES];
    vector<int> path_array[MAX_NODES];
    PriorityQueue open;
    vector<int> closed;
};

//a function to create a random graph
Graph random_graph_generator(double density, int max_nodes = MAX_NODES, int max_distance = MAX_DISTANCE){
    srand(time(0));
    int size = max_nodes;
    int prob;
    int distance;
    Graph g = Graph(size);
    for(int i = 0; i < size; ++i){
        for(int j = i; j < size; ++j){
            if(i!=j){
                prob = rand() % 100 + 1;
                if(prob < density*100){
                    g.add(i, j);
                    distance = rand() % max_distance + 1;
                    g.set_edge_value(i, j, distance);
                }
            }
        }
    }
    return g;
}

//a function that creates a random graph and calculates the average of the shortest paths to all nodes from node start
double avg_path_length(double density, int max_nodes = MAX_NODES, int max_distance = MAX_DISTANCE, int start = 0){
    Graph g = random_graph_generator(density, max_nodes, max_distance);
    ShortestPath sp = ShortestPath(g, start);
    double sum = 0;
    double distances = 0;
    for(int i = 0; i < MAX_NODES; ++i){
        if(i != start) {
            int dist = sp.path_size(i);
            if(dist != -1){
                sum += dist;
                distances++;
            }
        }
    }
    return sum/distances;
}

int main()
{
    cout << "For density = 0.20: " << avg_path_length(0.20) << endl;
    cout << "For density = 0.40: " << avg_path_length(0.40) << endl;
    // Sample output:
    // For density = 0.20: 5.52083
    // For density = 0.40: 4.02041
    return 0;
}
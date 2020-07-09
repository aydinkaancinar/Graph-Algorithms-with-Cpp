//
// Created by Kaan Çınar on 9.07.2020.
//
// Kruskal's Algorithm demonstration

#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <string>

using namespace std;

const int MAX_NODES = 20; //maximum number of nodes

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
    //"==" operator for Edge
    friend bool operator==(const Edge&, const Edge&);
    //destructor
    ~Edge(){}
private:
    int index1; //index of the node attached to the edge
    int index2; //index of the other node attached to the edge
    int value; //value associated to the edge
};

bool operator==(const Edge& e1, const Edge& e2){return e1.index1 == e2.index1 && e1.index2 == e2.index2;}

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
                for(int i = 0; i < neighbours.size(); ++i){
                    if(neighbours[i] == index2) {
                        neighbours.erase(neighbours.begin()+i);
                        return;
                    }
                }
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
    //"==" operator for Node
    friend bool operator==(const Node&, const Node&);
    //destructor
    ~Node(){}
private:
    int index; //index associated to this node
    int value; //value associated to this node
    vector<Edge> edges; //vector containing the edges of this node
    vector<int> neighbours; //vector of indices of neighbours
};

bool operator==(const Node& n1, const Node& n2){return n1.index == n2.index;}

class Graph {
public:
    //constructor with size and number of edges as possible parameters
    Graph(int size = 0, int num_edges = 0):size(size), num_edges(num_edges){
        for(int i = 0; i < size; ++i){
            nodes[i] = Node(i);
        }
    }
    //constructor for a file input
    Graph(string filename){
        num_edges = 0;
        fstream input("data.txt");
        input >> size;
        for(int i = 0; i < size; ++i){
            nodes[i] = Node(i);
        }
        int n1, n2, cost;
        while (input >> n1 >> n2 >> cost)
        {
            add(n1, n2);
            set_edge_value(n1, n2, cost);
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
    Node* get_nodes(){return nodes;}
    //returns the list of edges
    vector<Edge> get_edges(){
        vector<Edge> edges;
        for(int i  = 0; i < size; ++i){
            for(Edge e: nodes[i].get_edges()){
                edges.push_back(e);
            }
        }
        return edges;
    }
    //checks if graph contains this edge
    bool contains_edge(Edge e){
        vector<Edge> edges = get_edges();
        for(Edge edge: edges){
            if(edge == e)
                return true;
        }
        return false;
    }
    //checks if there is a path between u and v
    bool path(int u, int v){
        return path_helper(u, v, u);
    }
    //returns the total cost of all edges
    int cost(){
        vector<Edge> edges = get_edges();
        int sum = 0;
        for(Edge edge: edges)
            sum += edge.get_value();
        return sum;
    }
    //destructor
    ~Graph(){}
private:
    //path helper, checks so that while the path is searched it doesn't go back
    bool path_helper(int u, int v, int b){
        if(nodes[u].get_neighbours().size() == 0){
            return false;
        }
        else if(nodes[u].adjacent(v)){
            return true;
        }
        else{
            for(int i: nodes[u].get_neighbours()){
                if(i != b) {
                    if (path_helper(i, v, u)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    Node nodes[MAX_NODES]; //has the nodes in this graph
    int size; //the number of the nodes in the graph
    int num_edges; //number of edges of the graph
};

//a PriorityQueue implementation works for edges and nodes
template <typename T>
class PriorityQueue {
public:
    //constructor, initializes an empty PriorityQueue
    PriorityQueue(){}
    //constructor, initializes a PriorityQueue with given vector
    PriorityQueue(vector<T> v){
        for(T element: v){
            insert(element);
        }
    }
    //changes the priority (node value) of queue_element
    void chgPrioirity(int priority, T queue_element){
        if(!contains(queue_element)){
            throw invalid_argument("there is no such element in queue");
        }
        for(int i = 0; i < queue.size(); ++i){
            //checks if x is same as queue_element
            if(queue[i] == queue_element){
                queue[i].set_value(priority);
                return;
            }
        }
    }
    //removes the top element of the queue.
    T minPrioirty(){
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
        T temp = queue[index_top];
        queue.erase(queue.begin()+index_top);
        return temp;
    }
    //does the queue contain queue_element.
    bool contains(T queue_element){
        for(auto x : queue){
            //checks if x is same as queue_element
            if(x == queue_element){
                return true;
            }
        }
        return false;
    }
    //insert queue_element into queue
    void insert(T queue_element){queue.push_back(queue_element);}
    //returns the top element of the queue.
    T top(){
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
        T n;
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
    vector<T> queue;
};

//function that generates a MST for a given graph g
Graph generate_MST(Graph g){
    vector<Edge> edges = g.get_edges();
    PriorityQueue<Edge> p = PriorityQueue<Edge>(g.get_edges());
    Graph mst = Graph(g.V());
    while(p.size() != 0){
        Edge e = p.minPrioirty();
        if(!mst.path(e.get_index1(), e.get_index2())){
            mst.add(e.get_index1(), e.get_index2());
            mst.set_edge_value(e.get_index1(), e.get_index2(), e.get_value());
        }
    }
    return mst;
}

int main()
{
    Graph g = Graph("data.txt"); //the name of the data file should be put here
    Graph mst = generate_MST(g);
    cout << "MST: \n";
    mst.print();
    cout << "MST cost: " << mst.cost() << endl;
    // Sample output:
    //
    // MST:
    // 0: -> 2
    // 1: -> 6-> 17
    // 2: -> 9-> 0
    // 3: -> 12
    // 4: -> 7-> 8-> 15
    // 5: -> 6-> 18-> 16
    // 6: -> 1-> 5
    // 7: -> 4-> 10
    // 8: -> 4-> 9
    // 9: -> 2-> 8-> 12-> 13
    // 10: -> 7
    // 11: -> 12-> 14
    // 12: -> 3-> 11-> 17-> 9
    // 13: -> 9
    // 14: -> 11
    // 15: -> 4-> 19
    // 16: -> 5
    // 17: -> 1-> 12
    // 18: -> 5
    // 19: -> 15
    // MST cost: 60
    return 0;
}
#include <iostream>
#include <vector>
#include <limits>
#include <cstdio>


const double inf = std::numeric_limits<double>::infinity();


class Node;
class Edge;

std::vector<Node*> nodes;
std::vector<Edge*> edges;

void Algo();
std::vector<Node*>* remaining_adjacent(Node*);
Node* extract_smallest(std::vector<Node*>&);
double distance(std::pair<Node*,Node*>);
bool contains(std::vector<Node*>&, Node*);
void Solver(double, double, double);
Node* NODES[14];

class Node {
public:
    Node* prev;
    double dist_from_A;
    Node()
            : prev(NULL), dist_from_A(inf) {
        nodes.push_back(this);
    }
};

int main() {
    double r, g, y;
    std::cout << "Koszt energetyczny polaczen czerwonych:";
    std::cin >> r;
    std::cout << "Koszt energetyczny polaczen zielonych:";
    std::cin >> g;
    std::cout << "Koszt energetyczny polaczen zoltych:";
    std::cin >> y;
    Solver(r, g, y);
    std::cout << "Koszt najkorzystniejszej trasy: " <<  NODES[1]->dist_from_A << std::endl;
    return 0;
}






class Edge {
public:
    std::pair<Node*, Node*>nodes;
    double cost;
    Edge(std::pair<Node*, Node*>nodes, double raw_cost, double cost_coef)
    : nodes(nodes), cost(raw_cost*cost_coef) {
        edges.push_back(this);
    }
    bool connects(std::pair<Node*, Node*>_nodes) {
        return (
                (_nodes.first == this->nodes.first && _nodes.second == this->nodes.second) ||
                (_nodes.first == this->nodes.second && _nodes.second == this->nodes.first)
        );
    }
};

void Solver(double R, double G, double Y) {
    for(unsigned int i=0;i<14;i++){
        NODES[i] = new Node();
    }
    nodes[0]->dist_from_A=0;
    new Edge(std::pair<Node*, Node*>(nodes[ 0], nodes[ 9]),  10.0, G);
    new Edge(std::pair<Node*, Node*>(nodes[ 0], nodes[11]),  2.0,  Y);
    new Edge(std::pair<Node*, Node*>(nodes[ 1], nodes[ 2]),  5.0,  G);
    new Edge(std::pair<Node*, Node*>(nodes[ 1], nodes[ 3]),  5.0,  Y);
    new Edge(std::pair<Node*, Node*>(nodes[ 1], nodes[ 6]),  3.3,  R);
    new Edge(std::pair<Node*, Node*>(nodes[ 2], nodes[ 8]),  2.2,  G);
    new Edge(std::pair<Node*, Node*>(nodes[ 3], nodes[ 4]),  3.5,  Y);
    new Edge(std::pair<Node*, Node*>(nodes[ 4], nodes[ 5]),  3.8,  Y);
    new Edge(std::pair<Node*, Node*>(nodes[ 5], nodes[ 6]),  1.3,  R);
    new Edge(std::pair<Node*, Node*>(nodes[ 5], nodes[13]),  2.0,  R);
    new Edge(std::pair<Node*, Node*>(nodes[ 6], nodes[ 7]),  3.7,  G);
    new Edge(std::pair<Node*, Node*>(nodes[ 7], nodes[ 8]),  1.7,  Y);
    new Edge(std::pair<Node*, Node*>(nodes[ 7], nodes[10]),  4.2,  Y);
    new Edge(std::pair<Node*, Node*>(nodes[ 8], nodes[ 9]),  7.8,  R);
    new Edge(std::pair<Node*, Node*>(nodes[ 9], nodes[10]),  4.5,  G);
    new Edge(std::pair<Node*, Node*>(nodes[10], nodes[13]),  3.5,  G);
    new Edge(std::pair<Node*, Node*>(nodes[10], nodes[11]),  7.5,  Y);
    new Edge(std::pair<Node*, Node*>(nodes[11], nodes[12]),  3.0,  R);
    new Edge(std::pair<Node*, Node*>(nodes[12], nodes[13]),  3.8,  R);

    Algo();
}

void Algo() {
    while(nodes.size() > 0) {
        Node* smallest = extract_smallest(nodes);
        std::vector<Node*>* adjacent_nodes = remaining_adjacent(smallest);
        const int size = adjacent_nodes->size();
        for(unsigned int i = 0; i < size; ++i){
            Node* adjacent = adjacent_nodes->at(i);
            double dist = distance(std::pair<Node*, Node*>(smallest, adjacent)) + smallest->dist_from_A;

            if(dist < adjacent->dist_from_A) {
                adjacent->dist_from_A = dist;
                adjacent->prev = smallest;
            }
        }
        delete adjacent_nodes;
    }
}

Node* extract_smallest(std::vector<Node*>& _nodes) {
    unsigned int size = _nodes.size();
    if (size == 0) return NULL;
    int smallest_pos = 0;
    Node* smallest = nodes.at(0);
    for (unsigned int i=1; i<size;++i){
        Node* current = nodes.at(i);
        if (current->dist_from_A < smallest->dist_from_A) {
            smallest = current;
            smallest_pos = i;
        }
    }
    nodes.erase(nodes.begin() + smallest_pos);
    return smallest;
}

std::vector<Node*>* remaining_adjacent(Node* node) {
    /*std::vector<Node*>**/auto adjacent_nodes = new std::vector<Node*>();
    const unsigned int size = edges.size();
    for(unsigned int i=0;i<size;++i) {
        Edge* edge = edges.at(i);
        Node* adjacent = NULL;
        if (edge->nodes.first == node) {
            adjacent = edge->nodes.second;
        } else if (edge->nodes.second == node) {
            adjacent = edge->nodes.first;
        }
        if (adjacent && contains(nodes, adjacent)) {
            adjacent_nodes->push_back(adjacent);
        }
    }
    return adjacent_nodes;
}

double distance(std::pair<Node*,Node*>nodes) {
    const unsigned int size = edges.size();
    for (int i = 0; i < size; i++) {
        Edge* edge = edges.at(i);
        if (edge->connects(nodes)) {
            return edge->cost;
        }
    }
}

bool contains(std::vector<Node*>& nodes, Node* node) {
    const unsigned int size = nodes.size();
    for(unsigned int i = 0; i < size; i++) {
        if (node == nodes.at(i)) {
            return true;
        }
    }
    return false;
}

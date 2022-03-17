/*
  Rozwiązanie Zadania 1 - II Etap Rekrutacji
  autor: Michał Kozłowski
  data wykonania: 13.03.2022
*/


#include <iostream>
#include <vector>
#include <limits>
#include <cstdio>

using namespace std;

//stała nieskonczona potrzebna do implementacji algorytmu Dijkstry
constexpr double inf = numeric_limits<double>::infinity();

//pre-definicja struktur danych dla krawędzi i punktów
class Node;
class Edge;

//utworzenie wektorów przechowujących strukturę hangaru
vector<Node*> nodes;
vector<Edge*> edges;

//predefinicje funkcji potrzebnych do implementacji algorytmu
void Algo();
vector<Node*>* remaining_adjacent(Node*);
Node* extract_smallest(vector<Node*>&);
double distance(pair<Node*,Node*>);
bool contains(vector<Node*>&, Node*);
void Solver(double, double, double);
Node* NODES[14];

//struktura reprezentująca punkt
class Node {
public:
    Node* prev;
    double dist_from_A;
    Node()
            : prev(NULL), dist_from_A(inf) {
        nodes.push_back(this);
    }
};

//funkcja główna
int main() {
    //utworzenie zmiennych przechowujących koszty energetyczne poszczególnych rodzaji połączeń
    double r, g, y;

    //wczytanie wartości kosztów energetycznych z wejscia standardowego
    cout << "Koszt energetyczny polaczen czerwonych:";
    cin >> r;
    if(r<0){
      cout<<"Ujemne koszty nie sa dozwolone"<<endl;
      return 1;
    }
    cout << "Koszt energetyczny polaczen zielonych:";
    cin >> g;
    if(g<0){
      cout<<"Ujemne koszty nie sa dozwolone"<<endl;
      return 1;
    }
    cout << "Koszt energetyczny polaczen zoltych:";
    cin >> y;
    if(y<0){
      cout<<"Ujemne koszty nie sa dozwolone"<<endl;
      return 1;
    }

    //Obliczenie odległości od punktu początkowego za pomocą Algorytmu Dijkstry
    Solver(r, g, y);

    //Wypisanie rozwiązania na wyjście standardowe
    cout << "Koszt najkorzystniejszej trasy: " <<  NODES[1]->dist_from_A << endl;

    //Zekończenie funkcji głównej programu
    return 0;
}

//Struktura reprezentująca krawędź / połączenie
class Edge {
public:
    pair<Node*, Node*>nodes;
    double cost;
    Edge(pair<Node*, Node*>nodes, double raw_cost, double cost_coef)
    : nodes(nodes), cost(raw_cost*cost_coef) {
        edges.push_back(this);
    }
    bool connects(pair<Node*, Node*>_nodes) {
        return (
                (_nodes.first == this->nodes.first && _nodes.second == this->nodes.second) ||
                (_nodes.first == this->nodes.second && _nodes.second == this->nodes.first)
        );
    }
};

//Funkcja odpowiadająca za zainicjowanie struktury hangaru do pamięci RAM i rozpoczęcia obliczania
void Solver(double R, double G, double Y) {
    //utworzenie 14 struktur reprezentujących skrzyżowania
    for(unsigned int i=0;i<14;i++){
        NODES[i] = new Node();
    }
    //ustawienie punktu początkowego
    nodes[0]->dist_from_A=0;

    //utworzenie krawędzi łączących skrzyżowania
    new Edge(pair<Node*, Node*>(nodes[ 0], nodes[ 9]),  10.0, G);
    new Edge(pair<Node*, Node*>(nodes[ 0], nodes[11]),  2.0,  Y);
    new Edge(pair<Node*, Node*>(nodes[ 1], nodes[ 2]),  5.0,  G);
    new Edge(pair<Node*, Node*>(nodes[ 1], nodes[ 3]),  5.0,  Y);
    new Edge(pair<Node*, Node*>(nodes[ 1], nodes[ 6]),  3.3,  R);
    new Edge(pair<Node*, Node*>(nodes[ 2], nodes[ 8]),  2.2,  G);
    new Edge(pair<Node*, Node*>(nodes[ 3], nodes[ 4]),  3.5,  Y);
    new Edge(pair<Node*, Node*>(nodes[ 4], nodes[ 5]),  3.8,  Y);
    new Edge(pair<Node*, Node*>(nodes[ 5], nodes[ 6]),  1.3,  R);
    new Edge(pair<Node*, Node*>(nodes[ 5], nodes[13]),  2.0,  R);
    new Edge(pair<Node*, Node*>(nodes[ 6], nodes[ 7]),  3.7,  G);
    new Edge(pair<Node*, Node*>(nodes[ 7], nodes[ 8]),  1.7,  Y);
    new Edge(pair<Node*, Node*>(nodes[ 7], nodes[10]),  4.2,  Y);
    new Edge(pair<Node*, Node*>(nodes[ 8], nodes[ 9]),  7.8,  R);
    new Edge(pair<Node*, Node*>(nodes[ 9], nodes[10]),  4.5,  G);
    new Edge(pair<Node*, Node*>(nodes[10], nodes[13]),  3.5,  G);
    new Edge(pair<Node*, Node*>(nodes[10], nodes[11]),  7.5,  Y);
    new Edge(pair<Node*, Node*>(nodes[11], nodes[12]),  3.0,  R);
    new Edge(pair<Node*, Node*>(nodes[12], nodes[13]),  3.8,  R);

    //wywołanie algorytmu Dijkstry
    Algo();
}

//Algorytm Dijkstry
void Algo() {
    while(nodes.size() > 0) {
        Node* smallest = extract_smallest(nodes);
        vector<Node*>* adjacent_nodes = remaining_adjacent(smallest);
        const int size = adjacent_nodes->size();
        for(unsigned int i = 0; i < size; ++i){
            Node* adjacent = adjacent_nodes->at(i);
            double dist = distance(pair<Node*, Node*>(smallest, adjacent)) + smallest->dist_from_A;

            if(dist < adjacent->dist_from_A) {
                adjacent->dist_from_A = dist;
                adjacent->prev = smallest;
            }
        }
        delete adjacent_nodes;
    }
}

// Funkcje wymagane do zaimplementowania algorytmu Dijkstry

Node* extract_smallest(vector<Node*>& _nodes) {
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

vector<Node*>* remaining_adjacent(Node* node) {
    auto adjacent_nodes = new vector<Node*>();
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

double distance(pair<Node*,Node*>nodes) {
    const unsigned int size = edges.size();
    for (int i = 0; i < size; i++) {
        Edge* edge = edges.at(i);
        if (edge->connects(nodes)) {
            return edge->cost;
        }
    }
}

bool contains(vector<Node*>& nodes, Node* node) {
    const unsigned int size = nodes.size();
    for(unsigned int i = 0; i < size; i++) {
        if (node == nodes.at(i)) {
            return true;
        }
    }
    return false;
}

/*
  autor: Michał Kozłowski
  data wykonania: 13.03.2022
*/

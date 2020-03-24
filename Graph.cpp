//
// Created by zyh on 2020/3/24.
//

#include "iostream"
#include <vector>

using namespace std;

class Node;

class Edge {
public:
  Edge(int call, Node *parent, Node *child)
      : call(0), parent(nullptr), child(nullptr) {
    this->call = call;
    this->parent = parent;
    this->child = child;
  }
  void print() {
    if (call > 0) {
      cout << " --" << call << "-- ";
    } else {
      cout << " ---- ";
    }
  }

  bool hasChild() { return child != nullptr; }

  Node *getChild() { return child; }

private:
  Node *parent = nullptr;
  Node *child = nullptr;
  int call;
};

class Node {
public:
  Node() = default;
  void print(int depth = 0) {
    for (int i = 0; i < depth; i++) {
      cout << "\t";
    }
    cout << name << endl;
    for (Edge *e : edges) {
      e->print();
      if (e->hasChild()) {
        e->getChild()->print(depth + 1);
      }
    }
  }

private:
  string name;
  vector<Edge *> edges;
};
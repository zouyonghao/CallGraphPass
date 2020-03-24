//
// Created by zyh on 2020/3/24.
//

#include "iostream"
#include <utility>
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
      // cout << " ---- ";
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
  explicit Node(string name)
      : name(std::move(name)){

        };
  void print(int depth = 0) {
    for (int i = depth; i > 1; i--) {
      cout << " ";
    }
    if (depth > 0) {
      cout << "\\_";
    }
    cout << name << endl;
    for (Edge *e : edges) {
      e->print();
      if (e->hasChild()) {
        e->getChild()->print(depth + 1);
      }
    }
  }
  bool isScanned() const { return scanned; }

  void setScan(bool scan) { this->scanned = scan; }

  static Node *createOrGetFromVector(vector<Node *> &nodes,
                                     const string &functionName) {
    for (Node *node : nodes) {
      if (node->name == functionName) {
        return node;
      }
    }
    Node *node = new Node(functionName);
    nodes.push_back(node);
    return node;
  }

  void addEdge(Node *child) {
    for (Edge* e : edges) {
      if (e->getChild() == child) {
        return;
      }
    }
    edges.push_back(new Edge(0, this, child));
  }

private:
  string name;
  bool scanned = false;
  vector<Edge *> edges;
};
#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <queue>
#include <memory>

class Node {
public:
    Node();
    Node(std::vector<bool> symbol, size_t frequency);  // для мультифайла static
    ~Node();
    static Node* Build(const std::unordered_map<std::vector<bool>, size_t>& frequency);
    bool operator<(const Node& other) const;
    bool IsLeaf() const;
    std::unordered_map<std::vector<bool>, size_t> BFS();

private:
    size_t frequency_;
    std::vector<bool> symbol_;
    Node *left_, *right_;
};

struct Comparator {
    bool operator()(const Node* one, const Node* two) const;
};
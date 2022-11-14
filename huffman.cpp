#include "huffman.h"
#include <iostream>

Node::Node() {
    frequency_ = 0;
    symbol_ = std::vector<bool>();
    left_ = nullptr;
    right_ = nullptr;
}

Node::Node(std::vector<bool> symbol, size_t frequency) {
    frequency_ = frequency;
    symbol_ = symbol;
    left_ = nullptr;
    right_ = nullptr;
}

Node::~Node() {
    delete left_;
    delete right_;
}

bool Node::operator<(const Node& other) const {
    return std::tie(frequency_, symbol_) < std::tie(other.frequency_, other.symbol_);
}

bool Comparator::operator()(const Node* one, const Node* two) const {
    return (*one < *two);
}

Node* Node::Build(const std::unordered_map<std::vector<bool>, size_t>& frequency) {
    std::multiset<Node*, Comparator> q;
    for (auto& [symbol, count] : frequency) {
        Node* cur = new Node(symbol, count);
        q.insert(cur);
    }
    while (q.size() > 1) {
        Node* one = *q.begin();
        q.erase(q.begin());
        Node* two = *q.begin();
        q.erase(q.begin());

        Node* v = new Node(std::min(one->symbol_, two->symbol_), one->frequency_ + two->frequency_);
        v->left_ = one;
        v->right_ = two;
        q.insert(v);
    }
    return *q.begin();
}

bool Node::IsLeaf() const {
    return this->left_ == nullptr && this->right_ == nullptr;
}

// дфс меньше кода, красивее, но лучше бфс, потому что так удобнее возвращать табличку, а в дфс бы пришлось передавать
// по ссылке
std::unordered_map<std::vector<bool>, size_t> Node::BFS() {
    std::queue<std::pair<const Node*, size_t>> q;
    std::unordered_map<std::vector<bool>, size_t> table;
    q.push({this, 0});
    while (!q.empty()) {
        auto [v, len] = q.front();
        q.pop();
        if (v->IsLeaf()) {
            table[v->symbol_] = len;
        } else {
            q.push({v->left_, len + 1});
            q.push({v->right_, len + 1});
        }
    }
    return table;
}

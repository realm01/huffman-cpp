#ifndef __HUFFMAN_H
#define __HUFFMAN_H

#include <string>

struct Node {
  Node* left;
  Node* right;
  Node* parent;
  const char* character;
  int freq;
  Node(char* c = NULL, const unsigned int& freq = 1);
};

struct LL {
  LL* next;
  Node* tree;
  LL(Node* item = NULL);
  void insert(Node* item);
  int* getFreq(const char& c);
  LL* get(const int& i, int curr = 0);
};

class BinaryTree {
  private:
    Node* first;
  public:
    BinaryTree(LL* linkedlist, const unsigned int& size);
};

std::string* compress(std::string* input);

#endif

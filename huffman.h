#ifndef __HUFFMAN_H
#define __HUFFMAN_H

#include <string>
#include <vector>

struct Node {
  Node* left;
  Node* right;
  Node* parent;
  const char* character;
  int freq;
  std::string mapping;
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
    void print(Node const * next = NULL, unsigned int intent = 0);
    void generateMapping(Node* curr_node = NULL, std::string curr_map = "");
};

class Huffman {
  private:
    std::string original;
    std::string* encoded;
    struct Encoding {
      std::vector<std::string>* data;
    };
    Encoding* encoding;
  public:
    std::string* compress(std::string* input);
    std::string* decompress(const std::string* input);
    void setEncoding(const std::vector<std::string>* encoding);
    std::vector<std::string>* getEncoding(void);
    void parseEncoding(const std::string str_enc);
    std::string* getEncoded(void);
};

#endif

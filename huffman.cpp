#include <string>
#include <vector>
#include "huffman.h"

#include <iostream>
#include <iomanip>

#ifndef NULL
#define NULL 0
#endif

Node::Node(char* c, const unsigned int& freq) {
  character = c;
  this->freq = freq;
  parent = NULL;
  mapping = "";
}

Node::~Node(void) {
  delete(character);
  delete(left);
  delete(right);
}

LL::LL(Node* item) {
  next = NULL;
  if(item != NULL)
    tree = item;
  else
    tree = NULL;
}

LL::~LL(void) {
  delete(next);
}

// huffman tree implemented as a binary tree filled in bottom up
BinaryTree::BinaryTree(LL* linkedlist, const unsigned int& size) {
  first = NULL;

  for(unsigned int i = 0; i < size; i += 2) {
    LL* ll_left = linkedlist->get(i);
    LL* ll_right = linkedlist->get(i + 1);

    int freq_left = ll_left->tree->freq;
    int freq_right = 0;
    if(ll_right != NULL)
      freq_right = ll_right->tree->freq;

    Node* n_left = ll_left->tree;
    Node* n_right = NULL;
    if(ll_right != NULL)
      n_right = ll_right->tree;

    if(first == NULL) {
      first = new Node(NULL, freq_left + freq_right);
      first->left = n_right;
      first->right = n_left;
    }else{
      Node* current_node = first;
      while(true) {
        if(current_node->parent == NULL) {
          current_node->parent = new Node(NULL, current_node->freq);
          current_node->parent->right = current_node;
          current_node->parent->left = n_left;
          current_node->parent->freq += freq_left;

          if(n_right == NULL)
            break;

          current_node->parent->parent = new Node(NULL, current_node->parent->freq);
          current_node->parent->parent->right = current_node->parent;
          current_node->parent->parent->left = n_right;
          current_node->parent->parent->freq += freq_right;
          break;
        }else{
          current_node = current_node->parent;
        }
      }
    }
  }

  Node* curr_node = first;
  while(true) {
    if(curr_node->parent != NULL) {
      curr_node = curr_node->parent;
    }else{
      first = curr_node;
      break;
    }
  }
}

BinaryTree::~BinaryTree(void) {
  delete(first);
}

void BinaryTree::print(Node const * next, unsigned int intent) {
  Node const * curr_node = next;
  if(curr_node == NULL)
    curr_node = first;

  if(curr_node->left != NULL)
    print(curr_node->left, ++intent);

  if(curr_node->right != NULL)
    print(curr_node->right, ++intent);

  std::cout << std::setw(intent) << ' ';
  std::cout << curr_node->freq << " : " << curr_node->mapping << std::endl;
}

void BinaryTree::generateMapping(Node* curr_node, std::string curr_map) {
  if(curr_node == NULL)
    curr_node = first;

  // well then lol..
  if(curr_node == NULL)
    return void();

  if(curr_node->left != NULL) {
    generateMapping(curr_node->left, curr_map + '0');
  }

  if(curr_node->right != NULL) {
    generateMapping(curr_node->right, curr_map + '1');
  }

  // left and right Nodes are NULL which means we are
  // at a leaf Node, now assign the mapping to the it

  curr_node->mapping = curr_map;
}

void LL::insert(Node* item) {
  if(tree == NULL && next == NULL)
    tree = item;
  else if(next == NULL)
    next = new LL(item);
  else
    next->insert(item);
}

int* LL::getFreq(const char& c) {
  if(tree == NULL)
    return 0;

  if(*(tree->character) == c) {
    return &(tree->freq);
  }else{
    if(next == NULL)
      return NULL;
    else
      return next->getFreq(c);
  }
}

LL* LL::get(const int& i, const int curr) {
  if(i == curr) {
    return this;
  }else{
    if(next == NULL)
      return NULL;
    else
      return next->get(i, curr + 1);
  }
}

std::string* Huffman::compress(std::string* input) {
  this->original = *input;
  LL* linkedlist = new LL();
  unsigned int ll_size = 0;

  std::string::iterator i = input->begin();
  while(i != input->end()) {
    int* freq = linkedlist->getFreq(*i);
    if(freq == NULL) {
      linkedlist->insert(new Node(new char(*i)));
      ll_size++;
    }else{
      (*freq)++;
    }
    i++;
  }

  // sort linked list
  // I will use bubblesort, because we will have max 256
  Node* t0;

  for(int j = ll_size - 1; j > 0; j--) {
    for(int i = 0; i < j; i++) {
      LL* ll_f = linkedlist->get(i);
      LL* ll_s = linkedlist->get(i+1);
      if(ll_f->tree->freq > ll_s->tree->freq) {
        t0 = ll_f->tree;
        ll_f->tree = ll_s->tree;
        ll_s->tree = t0;
      }
    }
  }

  BinaryTree* bst = new BinaryTree(linkedlist, ll_size);
  bst->generateMapping();

  std::string* encoded = new std::string(input->c_str());

  for(unsigned int i = 0; i < ll_size; i++) {
    LL* ll_n = linkedlist->get(i);
    std::string map = ll_n->tree->mapping;
    const char* c = ll_n->tree->character;

    while(true) {
      size_t pos = encoded->find_first_of(*c);
      if(pos == std::string::npos)
        break;
      else
        encoded->replace(pos, 1, map);
    }
  }

  this->encoded = encoded;

  for(unsigned int i = 0; i < ll_size; i++) {
    this->encoding = new Huffman::Encoding();
    this->encoding->data = new std::vector<std::string>();
    const char* tmp = new char(*(linkedlist->get(i)->tree->character));
    this->encoding->data->push_back(std::string(tmp));
    delete(tmp);
    this->encoding->data->push_back(linkedlist->get(i)->tree->mapping);
  }

  delete(linkedlist);
  delete(bst);

  return encoded;
}

Huffman::~Huffman(void) {
  if(encoded != NULL)
    delete(encoded);
  if(encoding != NULL)
    delete(encoding);
}

Huffman::Encoding::~Encoding(void) {
  if(data != NULL)
    delete(data);
}

std::string* Huffman::decompress(const std::string* input) {

}

void Huffman::setEncoding(const std::vector<std::string>* encoding) {
  // this->encoding = new Huffman::Encoding();
  // this->encoding->data = encoding;
}

std::vector<std::string>* getEncoding(void) {

}

void Huffman::parseEncoding(const std::string str_enc) {

}

std::string* Huffman::getEncoded(void) {
  return encoded;
}

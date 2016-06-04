#include <string>
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

LL::LL(Node* item) {
  next = NULL;
  if(item != NULL)
    tree = item;
  else
    tree = NULL;
}

// huffman tree implemented as a binary tree filled in bottom up
BinaryTree::BinaryTree(LL* linkedlist, const unsigned int& size) {
  first = NULL;

  for(unsigned int i = 0; i < size; i += 2) {
    std::cout << "before get" << std::endl;
    LL* ll_left = linkedlist->get(i);
    LL* ll_right = linkedlist->get(i + 1);
    std::cout << "after get: " << ll_left << " : " << ll_right << std::endl;

    int freq_left = ll_left->tree->freq;
    int freq_right = 0;
    if(ll_right != NULL)
      freq_right = ll_right->tree->freq;

    Node* n_left = ll_left->tree;
    Node* n_right = NULL;
    if(ll_right != NULL)
      n_right = ll_right->tree;

    std::cout << "finished assiging" << std::endl;
    std::cout << freq_left << std::endl;
    std::cout << freq_right << std::endl;
    std::cout << n_left << std::endl;
    std::cout << n_right << std::endl;

    if(first == NULL) {
      std::cout << "first is NULL" << std::endl;
      first = new Node(NULL, freq_left + freq_right);
      first->left = n_left;
      first->right = n_right;
    }else{
      Node* current_node = first;
      while(true) {
        if(current_node->parent == NULL) {
          // assigning to left parent subtree
          // this is always the current node
          current_node->parent = new Node(NULL, current_node->freq);
          current_node->parent->left = current_node;
          current_node->parent->right = n_left;
          current_node->parent->freq += freq_left;
          // assigning to right parent subtree
          // this is always a new node with the caracter and its freq
          if(n_right == NULL)
            break;
          current_node->parent->parent = new Node(NULL, current_node->parent->freq);
          current_node->parent->parent->left = current_node->parent;
          current_node->parent->parent->right = n_right;
          current_node->parent->parent->freq += freq_right;
          break;
        }else{
          current_node = current_node->parent;
        }
      }
    }
  }

  std::cout << "finished building tree, adapting first" << std::endl;

  std::cout << first << std::endl;

  Node* curr_node = first;
  while(true) {
    if(curr_node->parent != NULL) {
      curr_node = curr_node->parent;
      std::cout << curr_node->freq << std::endl;
    }else{
      first = curr_node;
      break;
    }
  }

  std::cout << first << std::endl;
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
    curr_map += '0';
    generateMapping(curr_node->left, curr_map);
  }

  if(curr_node->right != NULL) {
    curr_map += '1';
    generateMapping(curr_node->right, curr_map);
  }

  // left and right Nodes are NULL which means we are
  // at a leaf Node, now assign the mapping to the it

  curr_node->mapping = curr_map;
}

void LL::insert(Node* item) {
  std::cout << "item: " << item->character << " next: " << next << std::endl;
  if(tree == NULL && next == NULL) {
    tree = item;
  }else if(next == NULL) {
    next = new LL(item);
    std::cout << "new next" << std::endl;
  }else{
    next->insert(item);
  }
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

std::string* compress(std::string* input) {
  LL* linkedlist = new LL();
  unsigned int ll_size = 0;

  std::string::iterator i = input->begin();
  while(i != input->end()) {
    std::cout << "creating getfreq call" << std::endl;
    int* freq = linkedlist->getFreq(*i);
    std::cout << "freq: " << freq << std::endl;
    if(freq == NULL) {
      std::cout << "creating insert call" << std::endl;
      linkedlist->insert(new Node(new char(*i)));
      std::cout << "after insert call: " << linkedlist->tree << std::endl;
      ll_size++;
    }else{
      std::cout << "for increasing: " << *i << " : " << *freq << std::endl;
      (*freq)++;
      std::cout << *freq << std::endl;
    }
    i++;
  }

  // sort linked list
  // I will use bubblesort, because we will have max 256
  Node* t0;

  std::cout << "before sort: " << ll_size << std::endl;

  for(unsigned int i = 0; i < ll_size; i++)
    std::cout << linkedlist->get(i) << " : " << linkedlist->get(i)->tree << " : " <<  linkedlist->get(i)->tree->character << " : " <<  linkedlist->get(i)->tree->freq << std::endl;

  std::cout << "======================================================== " << ll_size << std::endl;

  for(int j = ll_size - 1; j > 0; j--) {
    for(int i = 0; i < j; i++) {
      std::cout << "before get" << std::endl;
      LL* ll_f = linkedlist->get(i);
      std::cout << "after get : " << i  << std::endl;
      LL* ll_s = linkedlist->get(i+1);
      std::cout << "after get" << std::endl;
      std::cout << linkedlist << std::endl;
      std::cout << ll_f->tree << std::endl;
      std::cout << ll_s->tree << std::endl;
      if(ll_f->tree->freq > ll_s->tree->freq) {
        std::cout << "after if" << std::endl;
        t0 = ll_f->tree;
        ll_f->tree = ll_s->tree;
        ll_s->tree = t0;
      }
    }
  }

  for(unsigned int i = 0; i < ll_size; i++)
    std::cout << linkedlist->get(i) << " : " << linkedlist->get(i)->tree << " : " <<  linkedlist->get(i)->tree->character << " : " <<  linkedlist->get(i)->tree->freq << std::endl;

  std::cout << "======================================================== BINARY TREE" << std::endl;

  BinaryTree* bst = new BinaryTree(linkedlist, ll_size);
  std::cout << "---------" << std::endl;
  bst->print();
  std::cout << "---------" << std::endl;
  for(unsigned int i = 0; i < ll_size; i++)
    std::cout << linkedlist->get(i) << " : " << linkedlist->get(i)->tree << " : " <<  linkedlist->get(i)->tree->character << " : " <<  linkedlist->get(i)->tree->freq << std::endl;

  bst->generateMapping();
  bst->print();
}

#include <string>
#include "huffman.h"

#include <iostream>

#ifndef NULL
#define NULL 0
#endif

Node::Node(char* c, const unsigned int& freq) {
  character = c;
  this->freq = freq;
  parent = NULL;
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
    unsigned int sum = linkedlist->get(i)->tree->freq + linkedlist->get(i + 1)->tree->freq;
    if(first == NULL) {
      first = new Node(NULL, sum);
      first->left = linkedlist->get(i)->tree;
      first->right = linkedlist->get(i + 1)->tree;
    }else{
      Node* current_node = first;
      while(true) {
        if(current_node->parent == NULL) {
          current_node->parent = new Node(NULL, current_node->freq);
          current_node->parent->left = current_node;
        }else{
          // no right
          if(current_node->parent->right == NULL && current_node->parent->left != NULL) {
            current_node->parent->right = new Node(NULL, sum);
            current_node->parent->freq += sum;
            current_node->parent->right->parent = current_node->parent->right;
            current_node->parent->right->left =  linkedlist->get(i)->tree;
            current_node->parent->right->right = linkedlist->get(i + 1)->tree;
            break;
          // no left
          }else if(current_node->parent->left == NULL && current_node->parent->left == NULL) {
            current_node->parent->left = new Node(NULL, sum);
            current_node->parent->freq += sum;
            current_node->parent->left->parent = current_node->parent->left;
            current_node->parent->left->left =  linkedlist->get(i)->tree;
            current_node->parent->left->right = linkedlist->get(i + 1)->tree;
            break;
          }
        }
        current_node = current_node->parent;
      }
    }
  }
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
  if(i == curr)
    return this;
  else
    return next->get(i, curr + 1);
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
}

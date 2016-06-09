/*
* Created by: Anastassios Martakos
*
* Copyright 2016 Anastassios Martakos
*
* This program is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*/

#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include "huffman.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#ifndef NULL
#define NULL 0
#endif

Node::Node(char* c, const unsigned int& freq) {
  right = NULL;
  left = NULL;
  character = c;
  this->freq = freq;
  parent = NULL;
  mapping = "";
}

Node::~Node(void) {
  if(character != NULL)
    delete(character);
  if(left != NULL)
    delete(left);
  if(right != NULL)
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
  if(next != NULL)
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
  if(first != NULL)
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

Node* BinaryTree::getFirst(void) {
  return first;
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
  // I will use bubblesort, because we will have not so many
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

  encoded = new std::string(input->c_str());

  for(unsigned int i = 0; i < ll_size; i++) {
    LL* ll_n = linkedlist->get(i);
    std::string map = ll_n->tree->mapping;
    const char* c = ll_n->tree->character;

    while(true) {
      size_t pos = encoded->find(*c);
      if(pos == std::string::npos)
        break;
      else
        encoded->replace(pos, 1, map);
    }
  }

  this->encoded = encoded;
  this->encoding = new Huffman::Encoding();
  this->encoding->data = new std::vector<std::string>();

  for(int i = ll_size - 1; i >= 0 ; i--) {
    const char* tmp = new char(*(linkedlist->get(i)->tree->character));
    this->encoding->data->push_back(std::string(tmp));
    delete(tmp);
    std::ostringstream oss;
    oss << linkedlist->get(i)->tree->freq;
    this->encoding->data->push_back(oss.str());
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
  if(encoding == NULL || encoding->data == NULL)
    throw "no encoding set";

  encoded = new std::string();
  LL* linkedlist = new LL();
  unsigned int ll_size = 0;

  std::vector<std::string>::iterator i = encoding->data->end() - 1;
  while(i > encoding->data->begin()) {
    linkedlist->insert(new Node(new char(*((*(i - 1)).c_str())), atoi((*i).c_str())));
    ll_size++;
    i -= 2;
  }

  BinaryTree* bst = new BinaryTree(linkedlist, ll_size);

  Node* curr_node = bst->getFirst();

  std::string::const_iterator curr_symbol = input->begin();
  while(curr_symbol != input->end()) {
    if(*curr_symbol == '0') {
      if(curr_node->left != NULL)
        curr_node = curr_node->left;
    }else if(*curr_symbol == '1') {
      if(curr_node->right != NULL)
        curr_node = curr_node->right;
    }
    if(curr_node->left == NULL && curr_node->right == NULL) {
      const char tmp[] = {*(curr_node->character), '\0'};
      *encoded += std::string(tmp);
      curr_node = bst->getFirst();
    }
    curr_symbol++;
  }

  delete(linkedlist);
  delete(bst);

  return encoded;
}

void Huffman::setEncoding(std::vector<std::string>* encoding) {
  this->encoding = new Huffman::Encoding();
  this->encoding->data = encoding;
}

std::vector<std::string>* Huffman::getEncoding(void) {
  return encoding->data;
}

std::vector<std::string>* Huffman::parseEncoding(const std::string& str_enc) {
  std::string::const_iterator i = str_enc.begin();
  std::vector<std::string>* final = new std::vector<std::string>();

  std::string* tmp = new std::string();
  bool state = false;

  while(i != str_enc.end()) {
    if(!state) {
      const char* t = new char(*i);
      char tt[] = {*t, '\0'};
      final->insert(final->begin(), std::string(tt));
      delete(t);
      state = true;
    }else{
      if(*i != ';') {
        *tmp += *i;
      }else{
        final->insert(final->begin() + 1,std::string(*tmp));
        delete(tmp);
        tmp = new std::string();
        state = false;
      }
    }

    i++;
  }

  if(tmp != NULL)
    delete(tmp);

  return final;
}

std::string* Huffman::getEncoded(void) {
  return encoded;
}

std::string* Huffman::generateHeader(void) {
  if(encoding == NULL)
    return NULL;

  std::string* final = new std::string();

  std::vector<std::string>::iterator i = encoding->data->end() - 1;
  while(i > encoding->data->begin()) {
    const char tmp[] = {*((*(i - 1)).c_str()), '\0'};
    *final += std::string(tmp);
    *final += *i;
    *final += ';';
    i -= 2;
  }

  return final;
}

void Huffman::writeToFile(const std::string& file, const bool write_header) {
  std::string* header = generateHeader();

  std::ofstream outfile;
  outfile.open(file.c_str(), std::ios::out);

  if(write_header)
    outfile << *header << std::endl;
  outfile << *encoded << std::endl;

  outfile.close();

  if(header != NULL)
    delete(header);
}

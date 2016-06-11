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
#include "huffman.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <unordered_map>

#include <stdlib.h>

#include <math.h>

#ifndef NULL
#define NULL 0
#endif

Node::Node(char* c, const unsigned int& freq) {
  right = NULL;
  left = NULL;
  character = c;
  this->freq = freq;
  parent = NULL;
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

  next = NULL;
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
}

void BinaryTree::generateMapping(std::unordered_map<std::string, std::string>* map, Node* curr_node, std::string curr_map) {
  if(map == NULL)
    map = new std::unordered_map<std::string, std::string>();

  if(curr_node == NULL)
    curr_node = first;

  // well then lol..
  if(curr_node == NULL)
    return void();

  if(curr_node->left != NULL) {
    generateMapping(map, curr_node->left, curr_map + '0');
  }

  if(curr_node->right != NULL) {
    generateMapping(map, curr_node->right, curr_map + '1');
  }

  // left and right Nodes are NULL which means we are
  // at a leaf Node, now assign the mapping to the it

  if(curr_node->character != NULL) {
    const char tmp[] = {*(curr_node->character), '\0'};
    map->emplace(std::string(tmp), curr_map);
  }
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
  std::unordered_map<std::string, std::string>* map = new std::unordered_map<std::string, std::string>();
  bst->generateMapping(map);

  encoded = new std::string();

  std::string::iterator curr_symbol = input->begin();
  while(curr_symbol != input->end()) {
    const char tmp[] = {*curr_symbol, '\0'};
    encoded->append(map->at(std::string(tmp)));
    curr_symbol++;
  }

  delete(map);

  this->encoded = encoded;
  this->encoding = new Huffman::Encoding();
  this->encoding->data = new std::vector<std::string>();

  for(int i = ll_size - 1; i >= 0 ; i--) {
    const char* tmp = new char(*(linkedlist->get(i)->tree->character));
    this->encoding->data->push_back(std::string(tmp));
    delete(tmp);
    this->encoding->data->push_back(std::to_string(linkedlist->get(i)->tree->freq));
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
    linkedlist->insert(new Node(new char(*((*(i - 1)).c_str())), std::stoi(*i)));
    ll_size++;
    i -= 2;
  }

  BinaryTree* bst = new BinaryTree(linkedlist, ll_size);

  Node* curr_node = bst->getFirst();

  std::string::const_iterator curr_symbol = input->begin();
  while(curr_symbol < input->end() - overflow) {
    if(*curr_symbol == '0') {
      if(curr_node->left != NULL)
        curr_node = curr_node->left;
    }else if(*curr_symbol == '1') {
      if(curr_node->right != NULL)
        curr_node = curr_node->right;
    }
    if(curr_node->left == NULL && curr_node->right == NULL) {
      if(curr_node->character != NULL) {
        const char tmp[] = {*(curr_node->character), '\0'};
        *encoded += std::string(tmp);
      }
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

  while(i != str_enc.end()) {
    char tt[] = {*i, '\0'};
    final->insert(final->begin(), std::string(tt));

    i++;
    while(true) {
      if(*i == ';')
        break;
      char ttt[] = {*i, '\0'};
      *tmp += std::string(ttt);
      i++;
    }

    final->insert(final->begin() + 1, *tmp);
    tmp = new std::string();

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
  const char* header_c = header->c_str();

  std::fstream outfile;
  outfile.open(file.c_str(), std::ios::out | std::ios::binary);

  unsigned int alloc_bytes = ceil(encoded->size() / 8) + 1;
  unsigned char* bin = new unsigned char[alloc_bytes];

  std::cout << "size: " << encoded->size() << std::endl;
  std::cout << "allocating: " << alloc_bytes << " bytes" << std::endl;
  const char* c_string = encoded->c_str();

  unsigned int overflow = 0;

  for(unsigned int i = 0; i < alloc_bytes; i++) {
    int conv[8];

    for(unsigned int j = 0; j < 8; j++) {
      if((i * 8) + j < encoded->size()) {
        if(c_string[(i * 8) + j] == '1')
          conv[j] = 1;
        else
          conv[j] = 0;
      }else{
        conv[j] = 1;
        overflow++;
      }
    }

    bin[i] =  conv[7] << 7 |
              conv[6] << 6 |
              conv[5] << 5 |
              conv[4] << 4 |
              conv[3] << 3 |
              conv[2] << 2 |
              conv[1] << 1 |
              conv[0] << 0;
  }

  outfile << overflow;

  for(unsigned int m = 0; m < header->size(); m++)
    outfile << header_c[m];

  for(unsigned int n = 0; n < alloc_bytes; n++)
    outfile << bin[n];

  outfile.close();

  /* std::ofstream outfile;
  outfile.open(file.c_str(), std::ios::out);

  if(write_header)
    outfile << *header << std::endl;
  outfile << *encoded << std::endl;

  outfile.close(); */

  if(bin != NULL)
    delete bin;

  if(header != NULL)
    delete(header);
}

void Huffman::writeToStringFile(const char* file) {
  std::fstream outfile;
  outfile.open(file, std::ios::out);

  std::string::iterator i = encoded->begin();
  while(i != encoded->end()) {
    outfile << *i;
    i++;
  }

  outfile.close();
}

void Huffman::setOverflow(unsigned int& overflow) {
  this->overflow = overflow;
}

void Huffman::prepareCompressed(std::string& header, std::string* input, unsigned int& overflow, const char* file) {
  std::ifstream infile;
  infile.open(file, std::ios::in | std::ios::binary | std::ios::ate);

  long size = infile.tellg();
  infile.seekg(0, std::ios::beg);
  char* buff = new char[size];
  infile.read(buff, size);
  infile.close();

  const char tmp[] = {buff[0], '\0'};
  overflow = atoi(tmp);

  unsigned int counter = 1;
  while(true) {
    if(buff[counter] == ';') {
      if(buff[counter + 2] != '0' &&
        buff[counter + 2] != '1' &&
        buff[counter + 2] != '2' &&
        buff[counter + 2] != '3' &&
        buff[counter + 2] != '4' &&
        buff[counter + 2] != '5' &&
        buff[counter + 2] != '6' &&
        buff[counter + 2] != '7' &&
        buff[counter + 2] != '8' &&
        buff[counter + 2] != '9' &&
        buff[counter + 2] != ';')
        break;
    }

    const char tmp[] = {buff[counter], '\0'};
    header += std::string(tmp);
    counter++;
  }

  for(int i = ++counter; i < size; i++) {
    for(int j = 0; j < 8; j++) {
      const char tmp[] = {((buff[i] & (1 << j)) ? '1' : '0'), '\0'};
      *input += std::string(tmp);
    }
  }

  header += ";";

  infile.close();
}

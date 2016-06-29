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
    delete character;
  if(left != NULL)
    delete left;
  if(right != NULL)
    delete right;
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
    delete next;

  next = NULL;
}

// huffman tree implemented as a binary tree filled in bottom up
BinaryTree::BinaryTree(LL* linkedlist, const unsigned int& size) {
  first = NULL;

  std::vector<Node* > tmp_ll;

  for(unsigned int i = 0; i < size; i++)
    tmp_ll.push_back(linkedlist->get(i)->tree);

  while(true) {
    Node* tmp = new Node();
    tmp->freq = tmp_ll[0]->freq + tmp_ll[1]->freq;
    tmp->left = tmp_ll[0];
    tmp->right = tmp_ll[1];
    tmp_ll[0]->parent = tmp;
    tmp_ll[1]->parent = tmp;
    tmp_ll.erase(tmp_ll.begin(), tmp_ll.begin() + 2);
    std::vector<Node* >::iterator i = tmp_ll.begin();
    if(tmp_ll.size() == 0) {
      tmp_ll.push_back(tmp);
      break;
    }
    while(i != tmp_ll.end()) {
      if((*i)->freq == tmp->freq || (*i)->freq > tmp->freq || i == tmp_ll.end() - 1) {
        tmp_ll.insert(i, tmp);
        break;
      }
      i++;
    }
    if(tmp_ll.size() == 1)
      break;
  }

  first = tmp_ll[0];
}

BinaryTree::~BinaryTree(void) {
  if(first != NULL)
    delete first;
}

void BinaryTree::print(Node const * next, unsigned int intent, std::string curr_map) {
  Node const * curr_node = next;
  if(curr_node == NULL)
    curr_node = first;

  if(curr_node->left != NULL)
    print(curr_node->left, ++intent, curr_map + '0');

  if(curr_node->right != NULL)
    print(curr_node->right, ++intent, curr_map + '1');

  if(curr_node->character != NULL) {
    const char tmp[] = { *(curr_node->character), '\0' };
    std::cout << std::setw(intent) << ' ' << tmp << " : " << curr_map << std::endl;
  }
}

void BinaryTree::generateMapping(std::unordered_map<char, std::vector<bool> >* map, Node* curr_node, std::vector<bool> curr_map) {
  if(map == NULL)
    map = new std::unordered_map<char, std::vector<bool> >();

  if(curr_node == NULL)
    curr_node = first;

  // well then lol..
  if(curr_node == NULL)
    return void();

  if(curr_node->left != NULL) {
    curr_map.push_back(false);
    generateMapping(map, curr_node->left, curr_map);
  }

  if(curr_node->right != NULL) {
    curr_map.push_back(true);
    generateMapping(map, curr_node->right, curr_map);
  }

  // left and right Nodes are NULL which means we are
  // at a leaf Node, now assign the mapping to the it

  if(curr_node->character != NULL) {
    map->emplace(*(curr_node->character), curr_map);
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

std::vector<bool>* Huffman::compress(const char* input, const size_t& size) {
  encode = true;
  LL* linkedlist = new LL();
  unsigned int ll_size = 0;

  for(unsigned int i = 0; i < size; i++) {
    int* freq = linkedlist->getFreq(input[i]);
    if(freq == NULL) {
      linkedlist->insert(new Node(new char(input[i])));
      ll_size++;
    }else{
      (*freq)++;
    }
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
  std::unordered_map<char, std::vector<bool> >* map = new std::unordered_map<char, std::vector<bool> >();
  bst->generateMapping(map);

  encoded = (void*)(new std::vector<bool>());

  for(unsigned int i = 0; i < size; i++) {
    ((std::vector<bool>*)encoded)->insert(((std::vector<bool>*)encoded)->end(), map->at(input[i]).begin(), map->at(input[i]).end());
  }

  delete map;

  this->encoding = new Huffman::Encoding();
  this->encoding->data = new std::vector<char>();
  this->encoding->freqs = new std::vector<int>();

  for(int i = ll_size - 1; i >= 0 ; i--) {
    this->encoding->data->push_back(*(linkedlist->get(i)->tree->character));
    this->encoding->freqs->push_back(linkedlist->get(i)->tree->freq);
  }

  delete linkedlist;
  delete bst;

  return (std::vector<bool>*)encoded;
}

Huffman::~Huffman(void) {
  if(encoded != NULL) {
    if(encode)
      delete (std::vector<bool>*)encoded;
    else
      delete (std::string*)encoded;
  }
  if(encoding != NULL)
    delete encoding;
}

Huffman::Encoding::~Encoding(void) {
  if(data != NULL)
    delete data;
    delete freqs;
}

std::string* Huffman::decompress(const std::vector<bool>* input) {
  encode = false;
  if(encoding == NULL || encoding->data == NULL || encoding->freqs == NULL)
    throw "no encoding set";

  encoded = (void*)(new std::string());
  LL* linkedlist = new LL();
  unsigned int ll_size = 0;

  for(unsigned int i = 0; i < encoding->data->size(); i++) {
    linkedlist->insert(new Node(new char(encoding->data->at(i)), encoding->freqs->at(i)));
    ll_size++;
  }

  BinaryTree* bst = new BinaryTree(linkedlist, ll_size);

  Node* curr_node = bst->getFirst();

  for(unsigned int i = 0; i < input->size() - overflow; i++) {
    if(!input->at(i)) {
      if(curr_node->left != NULL)
        curr_node = curr_node->left;
    }else if(input->at(i)) {
      if(curr_node->right != NULL)
        curr_node = curr_node->right;
    }
    if(curr_node->left == NULL && curr_node->right == NULL) {
      if(curr_node->character != NULL) {
        const char tmp[] = {*(curr_node->character), '\0'};
        *((std::string*)encoded) += std::string(tmp);
      }
      curr_node = bst->getFirst();
    }
  }

  delete linkedlist;
  delete bst;

  return (std::string*)encoded;
}

void Huffman::setEncoding(Huffman::Encoding* encoding) {
  if(this->encoding != NULL)
    delete this->encoding;

  this->encoding = encoding;
}

Huffman::Encoding* Huffman::getEncoding(void) {
  return encoding;
}

Huffman::Encoding* Huffman::parseEncoding(std::string& str_enc) {
  Huffman::Encoding* enc = new Huffman::Encoding();
  enc->data = new std::vector<char>();
  enc->freqs = new std::vector<int>();

  std::string* tmp = new std::string();

  std::string::const_iterator i = str_enc.begin();
  while(i != str_enc.end()) {
    enc->data->push_back(*i);

    i++;
      while(true) {
      if(*i == ';')
        break;
      char ttt[] = {*i, '\0'};
      *tmp += std::string(ttt);
      i++;
    }

    enc->freqs->push_back(std::stoi(*tmp));
    delete tmp;
    tmp = new std::string();

    i++;
  }

  if(tmp != NULL)
    delete tmp;

  return enc;
}

void* Huffman::getEncoded(void) {
  return encoded;
}

std::vector<char>* Huffman::generateHeader(void) {
  if(encoding == NULL)
    return NULL;

  std::vector<char>* final = new std::vector<char>();

  for(unsigned int i = 0; i < encoding->data->size(); i++)  {
    final->push_back(encoding->data->at(i));
    std::string s = std::to_string(encoding->freqs->at(i));
    const char* tmp = s.c_str();
    for(unsigned int j = 0; j < s.size(); j++)
      final->push_back(tmp[j]);
    final->push_back(';');
  }

  return final;
}

void Huffman::writeToFile(const char* file, const bool write_header) {
  std::vector<char>* header = generateHeader();

  std::fstream outfile;
  outfile.open(file, std::ios::out | std::ios::binary);

  unsigned int alloc_bytes = ceil(((std::vector<bool>*)encoded)->size() / 8) + 1;
  unsigned char* bin = new unsigned char[alloc_bytes];

  // std::cout << "size: " << encoded->size() << std::endl;
  // std::cout << "allocating: " << alloc_bytes << " bytes" << std::endl;

  // const char* c_string = encoded->c_str();

  unsigned int overflow = 0;

  for(unsigned int i = 0; i < alloc_bytes; i++) {
    int conv[8];

    for(unsigned int j = 0; j < 8; j++) {
      if((i * 8) + j < ((std::vector<bool>*)encoded)->size()) {
        if(((std::vector<bool>*)encoded)->at((i * 8) + j))
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

  for(unsigned int m = 0; m < header->size(); m++) {
    outfile << header->at(m);
  }

  for(unsigned int n = 0; n < alloc_bytes; n++)
    outfile << bin[n];

  outfile.close();

  if(bin != NULL)
    delete bin;

  if(header != NULL)
    delete header;
}

void Huffman::writeToStringFile(const char* file) {
  std::fstream outfile;
  outfile.open(file, std::ios::out);

  std::string::iterator i = ((std::string*)encoded)->begin();
  while(i != ((std::string*)encoded)->end()) {
    outfile << *i;
    i++;
  }

  outfile.close();
}

void Huffman::setOverflow(unsigned int& overflow) {
  this->overflow = overflow;
}

void Huffman::prepareCompressed(std::string& header, std::vector<bool>* input, unsigned int& overflow, const char* file) {
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
      const bool tmp = (buff[i] & (1 << j)) ? true : false;
      input->push_back(tmp);
    }
  }

  header += ";";

  delete buff;

  infile.close();
}

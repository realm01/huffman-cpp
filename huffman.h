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

#ifndef __HUFFMAN_H
#define __HUFFMAN_H

#include <string>
#include <vector>
#include <unordered_map>

struct Node {
  Node* left;
  Node* right;
  Node* parent;
  const char* character;
  int freq;
  Node(char* c = NULL, const unsigned int& freq = 1);
  ~Node(void);
};

struct LL {
  LL* next;
  Node* tree;
  LL(Node* item = NULL);
  ~LL(void);
  void insert(Node* item);
  int* getFreq(const char& c);
  LL* get(const int& i, int curr = 0);
};

class BinaryTree {
  private:
    Node* first;
  public:
    BinaryTree(LL* linkedlist, const unsigned int& size);
    ~BinaryTree(void);
    void print(Node const * next = NULL, unsigned int intent = 0);
    void generateMapping(std::unordered_map<std::string, std::string>* map, Node* curr_node = NULL, std::string curr_map = "");
    Node* getFirst(void);
};

class Huffman {
  private:
    std::string original;
    std::string* encoded;
    struct Encoding {
      std::vector<std::string>* data;
      ~Encoding(void);
    };
    Encoding* encoding;
    std::string* generateHeader(void);
  public:
    ~Huffman(void);
    std::string* compress(std::string* input);
    std::string* decompress(const std::string* input);
    void setEncoding(std::vector<std::string>* encoding);
    std::vector<std::string>* getEncoding(void);
    std::vector<std::string>* parseEncoding(const std::string& str_enc);
    std::string* getEncoded(void);
    void writeToFile(const std::string& file, const bool write_header = true);
};

#endif

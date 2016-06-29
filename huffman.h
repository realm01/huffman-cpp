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
    void print(Node const * next = NULL, unsigned int intent = 0, std::string curr_map = "");
    void generateMapping(std::unordered_map<char, std::vector<bool> >* map, Node* curr_node = NULL, std::vector<bool> curr_map = std::vector<bool>());
    Node* getFirst(void);
};

class Huffman {
  private:
    bool encode;
    void* encoded;          // std::vector<bool>*   or    std::string*
    unsigned int overflow;
    struct Encoding {
      std::vector<char>* data;
      std::vector<int>* freqs;
      ~Encoding(void);
    };
    Encoding* encoding;
    std::vector<char>* generateHeader(void);
  public:
    ~Huffman(void);
    std::vector<bool>* compress(const char* input, const size_t& size);
    std::string* decompress(const std::vector<bool>* input);
    void setEncoding(Huffman::Encoding* encoding);
    Huffman::Encoding* getEncoding(void);
    Huffman::Encoding* parseEncoding(std::string& str_enc);
    void* getEncoded(void);
    void writeToFile(const char* file, const bool write_header = true);
    static void prepareCompressed(std::string& header, std::vector<bool>* input, unsigned int& overflow, const char* file);
    void writeToStringFile(const char* file);
    void setOverflow(unsigned int& overflow);
};

#endif

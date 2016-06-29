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

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "huffman.h"

using namespace std;

void showHelp(void) {
  cout << "Huffman Encoder and Decoder\nCopyright (c) Anastassios Martakos\n\nUSAGE\n-e <input-file> <output-file>       # encode\n-d <input-file> <output-file>       # decode\n" << endl;
}

int main(int argc, char *argv[]) {
  if(argc < 4) {
    showHelp();
    return 1;
  }

  Huffman* hm = new Huffman();

  if(string(argv[1]) == string("-e")) {
    string* input = new string();
    ifstream file;
    file.open(argv[2], ios::in);

    stringstream buffer;
    buffer << file.rdbuf();
    *input = buffer.str();

    if(input->size() == 0)
      return 0;

    input->erase(input->end() - 1, input->end());

    hm->compress(input->c_str(), input->size());
    hm->writeToFile(argv[3]);

    if(input != NULL)
      delete input;
  }else if(string(argv[1]) == string("-d")) {
    string header;
    unsigned int overflow;
    std::vector<bool>* input = new std::vector<bool>();

    Huffman::prepareCompressed(header, input, overflow, argv[2]);

    hm->setOverflow(overflow);
    hm->setEncoding(hm->parseEncoding(header));
    hm->decompress(input);

    hm->writeToStringFile(argv[3]);
    if(input != NULL)
      delete input;
  }else{
    showHelp();
    return 1;
  }

  delete hm;

  return 0;
}

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

  string* input = new string();
  Huffman* hm = new Huffman();

  if(string(argv[1]) == string("-e")) {
    ifstream file;
    file.open(argv[2]);

    stringstream buffer;
    buffer << file.rdbuf();
    *input = buffer.str();

    input->erase(input->end() - 1, input->end());

    hm->compress(input);
    hm->writeToFile(string(argv[3]));
  }else if(string(argv[1]) == string("-d")) {
    ifstream file;
    file.open(argv[2]);

    string header;
    getline(file, header);

    while(!file.eof()) {
      file >> *input;
    }

    file.close();

    hm->setEncoding(hm->parseEncoding(header));
    hm->decompress(input);

    hm->writeToFile(string(argv[3]), false);
  }else{
    showHelp();
    return 1;
  }

  return 0;
}

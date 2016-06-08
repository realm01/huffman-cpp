#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "huffman.h"

using namespace std;

void showHelp(void) {
  cout << "Huffman Encoder and Decoder\nCopyright (c) Anastassios Martakos\n\nUSAGE\n-e <input-file> <output-file>       # encode\n-e <input-file> <output-file>       # decode\n" << endl;
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

    cout << *input;

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

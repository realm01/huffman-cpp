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
#include "huffman.h"

using namespace std;

int main() {
  string* input = new string("this is just a test");
  Huffman* hm = new Huffman();
  hm->compress(input);

  cout << *input << endl;
  cout << *(hm->getEncoded()) << endl;
  hm->writeToFile("/ashscr1/testfile");
  cout << *(hm->decompress(hm->getEncoded())) << endl;

  delete(input);
  delete(hm);


  string* enc = new string("h1;j1;u1;a1;e1;i2;t4;s4; 4;");
  string* in = new string("110111111111110100111010011111110111111010110011111001101111010110");

  Huffman* hf = new Huffman();

  hf->setEncoding(hf->parseEncoding(*enc));
  cout << *(hf->decompress(in)) << endl;

  delete(enc);
  delete(in);

  return 0;
}

// nice explanation
// http://www.cprogramming.com/tutorial/computersciencetheory/huffman.html

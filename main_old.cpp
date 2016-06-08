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

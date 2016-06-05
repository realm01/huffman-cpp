#include <iostream>
#include <string>
#include "huffman.h"

using namespace std;

int main() {
  string* input = new string("this is just a test");
  Huffman* hm = new Huffman();
  hm->compress(input);

  cout << *(hm->getEncoded()) << endl;
  // cout << *(decompressed) << endl;

  return 0;
}

// nice explanation
// http://www.cprogramming.com/tutorial/computersciencetheory/huffman.html

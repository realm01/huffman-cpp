#include <iostream>
#include <string>
#include "huffman.h"

using namespace std;

int main() {
  string* input = new string("aaabbc");

  compress(input);

  return 0;
}

// nice explanation
// http://www.cprogramming.com/tutorial/computersciencetheory/huffman.html

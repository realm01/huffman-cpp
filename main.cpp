#include <iostream>
#include <string>
#include "huffman.h"

using namespace std;

int main() {
  string* input = new string("aaabbc");

  cout << *(compress(input)) << endl;

  return 0;
}

// nice explanation
// http://www.cprogramming.com/tutorial/computersciencetheory/huffman.html

#include <iostream>
#include <string>

using namespace std;

int main() {
  string* s = new string("hello there");

  // cout << *s << endl;
  // s->replace(1, 1, "HUIEHUEHUE");
  // cout << *s << endl;

  cout << s->find(string("there")) << endl;
  // cout << string::npos << endl;

  // int t = int(1);

  return 0;
}

// nice explanation
// http://www.cprogramming.com/tutorial/computersciencetheory/huffman.html

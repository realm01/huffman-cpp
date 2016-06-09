Huffman Encoder Decoder
=======================

Huffman is a compression algorithm used to compress and decompress text files.

Usage
-----
```sh
$ huffman -e <input-file> <output-file>       # encode
          -d <input-file> <output-file>       # decode
```

Compile
-------
Note that I am using c++11 because of the new awesome `std::string` and `std::unordered_map` classes!
```sh
$ g++ -std=c++11 -O3 -Wall main.cpp huffman.cpp -o huffman
```

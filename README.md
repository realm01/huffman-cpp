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
```sh
$ g++ -O3 -Wall main.cpp huffman.cpp -o huffman
```

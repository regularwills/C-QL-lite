## Name: <ins> Will May</ins>

# CQL-Lite Database: A simple SQL-esque data base written in C++

## Features:

- Not Implemented:
  - BTree Data Structure: - Remove capabilites (various remove functions)

<br><br>

- Implemented:

  - BTree Data Structure: - Crux of the Database, where all the information for binary files will be held - BTree acts like a dictionary/map to be used as a Key : Value pair for each node entry

    - BTree low level array functions - Allows the BTree to swap, add, split data amongst the nodes in various ways
      <br><br>

  - String Tokenizer: - Simple tokenizer that takes in a C-String and splits each word up into a token - Token types: Alphanumeric, Digit, Operators, Spaces, Punctuation - Tokenizes by moving through a finite state table to examine and group chars in a C-String - Finite State Tables have various success and fail states: - Fail State: Tokenizer has found a char different than the current group and the token is completed - Success State: Tokenizer is still gathering and adding characters to a current token under the same group
    <br><br>

- Partly implemented:
  - what features have not been implemented

<br><br>

- Bugs
  - Known bugs

<br><br>

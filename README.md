## Name: <ins> Will May</ins>

# C-QL-Lite Database: A SQL-esque data in C++

# ** An In-Memory, Append-Only Simple Data Base **

## Features:

- Not Implemented:
  - BTree Data Structure: - Remove capabilites (various remove functions)

<br><br>

- <strong>Implemented:</strong>

  - BTree Data Structure:
    - Crux of the Database, where all the information for binary files will be held
    - BTree low level array functions - Allows the BTree to swap, add, split data amongst the nodes in various ways
    <br><br>

  - Map and MultiMap:
    - Map and Multimap are wrappers for the btree and also where the SQL commands and data records will be held
    - They use either custom "pair or multi_pair" data type that have Key : Value pairs for their respected btree values
      <br><br>

  - String Tokenizer:
    - Simple tokenizer that takes in a C-String and splits each word up into a token
    - Token types: Alphanumeric, Digit, Operators, Spaces, Punctuation
    - Tokenizes by moving through a finite state table to examine and group chars in a C-String
    - Finite State Tables have various success and fail states: Fail State: Tokenizer has found a char different than the current group and the token is completed. Success State: Tokenizer is still gathering and adding characters to a current token under the same group
      <br><br>

- <strong> Partly implemented:</strong>
  - Parser - SQL command line parser
  - Token presendece and coverting a infix string query to a post fix queue of tokens to pass to Parser

<br><br>

- <strong>TO DO:</strong>
  - get_lookup() function for a btree
  - new shuntingyard with token precedence

<br><br>

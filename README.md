<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CQL-Lite Database - README</title>
</head>

<body>

    <h1 style="text-decoration: underline;">Name: <ins>Will May</ins></h1>

    <h2>CQL-Lite Database: A simple SQL-esque database written in C++</h2>

    <h3>Features:</h3>

    <h4>Not Implemented:</h4>
    <ul>
        <li>BTree Data Structure: Remove capabilities (various remove functions)</li>
    </ul>

    <h4>Implemented:</h4>
    <ul>
        <li>
            <strong>BTree Data Structure:</strong> Crux of the Database, where all the information for binary files will be
            held. BTree acts like a dictionary/map to be used as a Key : Value pair for each node entry
            <ul>
                <li>BTree low level array functions - Allows the BTree to swap, add, split data amongst the nodes in various ways</li>
            </ul>
        </li>
        <li>
            <strong>String Tokenizer:</strong> Simple tokenizer that takes in a C-String and splits each word up into a token
            <ul>
                <li>Token types: Alphanumeric, Digit, Operators, Spaces, Punctuation</li>
                <li>Tokenizes by moving through a finite state table to examine and group chars in a C-String</li>
                <li>Finite State Tables have various success and fail states:</li>
                <ul>
                    <li>Fail State: Tokenizer has found a char different than the current group, and the token is completed</li>
                    <li>Success State: Tokenizer is still gathering and adding characters to a current token under the same group</li>
                </ul>
            </ul>
        </li>
    </ul>

    <h4>Partly Implemented:</h4>
    <ul>
        <li>What features have not been implemented</li>
    </ul>

    <h4>Bugs:</h4>
    <ul>
        <li>Known bugs</li>
    </ul>

</body>

</html>


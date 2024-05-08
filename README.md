# mypython
--Important--
Recursive function is implemented.

--Date Created--
April 26, 2024

--Author--
Zach Pierce and Kamui Morita-Albright

--Purpose & Characteristics--
This program's goal is to simulate python interpreter's functionality on c++. 
It only has a simple functionality of python, such as variable assignment, basic arithmetic operations, if-else functions, user defined functions, recursive functions, and print.
Thus, it does not handle the following:
elif
while/for loops
Lists
Python data structures (lists, arrays, objects)
OOP constructs like class or method calls
Advanced features like I/O, threads, multiple files

When there is an error, the program will continue running to find other errors. And situation-specific error messages will be printed on the console.

--Algorithm and Structure--
The program reads each line from the python file and tokenize them while categorizing each word to different types (e.g. keyword, print, variable), which are then stored in a vector.
Each of these tokenized lines are then stored in a linked list. The program proceeds this process until the end of the input file while keeping each linked list in a vector.
Then, the parser checks the syntax and executes them by iterating through the vector. This evaluation is done in a class "parser". Every user defined function is stored in a different parser, holding onto the tokens inside the functions, which are executed when the function is called.

--Command To Run--
g++ mypython.cpp -o mypython
./mypython <file.py>

----------------------------------------------------
Introduction
----------------------------------------------------
Semantic analyzer handles the following errors
1. Undeclared variables and undefined functions. 
2. Multiply declared variables and multiply defined functions. 
3. Function declaration/call mismatch. 
4. Indexing an array variable with a non-integer type
5. Indexing an array with an out-of-bounds integer literal
6. Type mismatch in assignments.


specifies the line number where there is an error.

----------------------------------------------------
Instruction to run
----------------------------------------------------
    $ cd /path/to/assg2.hooge.rajendran/
    $ make
    $ ./mcc < ./input_file.mC

----------------------------------------------------
Instruction to test
----------------------------------------------------
    $ make
    $ chmod +X inputTests.sh
    $ sh inputTests.sh

----------------------------------------------------
File List
----------------------------------------------------
scanner.l
parser.y
semchk.c
semchk.h 
symtab.c 
symtab.h 
tree.c 
tree.h 
driver.c 

----------------------------------------------------
Other generated files
----------------------------------------------------
y.tab.c
y.tab.h 
lex.yy.c

----------------------------------------------------
Tree
----------------------------------------------------
The Syntax tree is a tree representation of the syntax of the input program. 
It uses an n-ary tree where the internal nodes correspond to non-terminals in the grammar and the leaf nodes represent terminals 
By traversing the tree it is possible to reproduce source code that is equivalent to the original source. 
Some terminals store the corresponding semantic value.

----------------------------------------------------
Symbol Table
----------------------------------------------------
The symbol table has three fields.
they are Name, Type, Scope,Return type,parameter list
Name of the value is stored in the name field of the symbol table.
Type is the data type of the identifier.It can be int,char,void.
Scope can be local and global.

Return type and parameter list is NULL if the enrty corresponds to a variable.
return type holds the return type of the function
parameter list is a single linked list, that stores the type of formal parameters of the function

----------------------------------------------------
Special comments
----------------------------------------------------
Handles both single line comment and multiline comment 
operator precedence is multipicative, additive , relational.
basic data types are int, char, void
variables can be array.


----------------------------------------------------
Bug information
----------------------------------------------------
cannot handle strings
type check for non simple expressions are stated as warnings
line numbers where error occurs are not displayed.

----------------------------------------------------
Extra credit work
----------------------------------------------------

----------------------------------------------------
Work done by
----------------------------------------------------
Narmadha rajendran
Amanda Hooge
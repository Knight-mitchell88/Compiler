# Compiler
Fully functioning PL/0 Compiler. 

To compile & run this code, first download the hw4compiler.c file and input.txt (or any other input files) to the same directory.
Use "gcc -g vm.c" to generate a program file from the .c file.

You can now run the program while passing in your input file with this
command: "./a.out input.txt".  
(or any other input file name).txt

The expected output will be shown directly in the terminal, as well as an elf.txt file containing the VM code.

Grammar used for compiler:

program ::= block "." .
block ::= const-declaration var-declaration procedure-declaration statement.
constdeclaration ::= ["const" ident "=" number {"," ident "=" number} ";"].
var-declaration ::= [ "var "ident {"," ident} “;"].
procedure-declaration ::= { "procedure" ident ";" block ";" }
statement ::= [ ident ":=" expression
| "call" ident
| "begin" statement { ";" statement } "end"
| "if" condition "then" statement
| "while" condition "do" statement
| "read" ident
| "write" expression
| empty ] .
condition ::= "odd" expression
| expression rel-op expression.
rel-op ::= "="|“< >"|"<"|"<="|">"|">=“.
expression ::= [ "+"|"-"] term { ("+"|"-") term}.
term ::= factor {("*"|"/") factor}.
factor ::= ident | number | "(" expression ")“.
number ::= digit {digit}.
ident ::= letter {letter | digit}.
digit ;;= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9“.
letter ::= "a" | "b" | ... | "y" | "z" | "A" | "B" | ... | "Y" | "Z".
Based on Wirth’s definition for EBNF we have the following rule:
[ ] means an optional item.
{ } means repeat 0 or more times.
Terminal symbols are enclosed in quote marks.
A period is used to indicate the end of the definition of a syntactic class.




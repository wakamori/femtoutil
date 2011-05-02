#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include"lexer.h"
#include"ast.h"

using namespace std;


int gettok(){
    static int LastChar = ' ';
    while (isspace(LastChar))
        LastChar = getchar();

    if (isdigit(LastChar)) {   // Number: [0-9]+
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }
    if (LastChar != '\n' && LastChar != EOF) { // identifier: [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr = LastChar;
        while (!isspace((LastChar = getchar())) && LastChar != EOF)
            IdentifierStr += LastChar;

        if (IdentifierStr == "defun") return tok_defun;
        if (IdentifierStr == "if") return tok_if;
        return tok_identifier;
    }

    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF )
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

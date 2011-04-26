#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include"parser.h"
#include"ast.h"
#include"lexer.h"
using namespace std;
static void MainLoop();
static void HandleDefinition();
static void HandleTopLevelExpression();

int main() {

    fprintf(stderr, "ready> ");
    getNextToken();

    // Run the main "interpreter loop" now.
    MainLoop();

    return 0;
}

static void MainLoop() {
    while (1) {
        fprintf(stderr, "ready> ");
        switch (CurTok) {
            case tok_eof:    return;
            case tok_defun:    HandleDefinition(); break;
            default:         HandleTopLevelExpression(); break;
        }
    }
}

static void HandleDefinition() {
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleTopLevelExpression() {
    // Evaluate a top-level expression into an anonymous function.
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}



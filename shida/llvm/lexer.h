#ifndef LEXER
#define LEXER
enum Token { tok_eof, tok_defun, tok_identifier, tok_number, tok_if, };
static std::string IdentifierStr;
static int NumVal;
int gettok();
#endif

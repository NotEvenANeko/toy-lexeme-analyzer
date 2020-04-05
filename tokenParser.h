#include "Buffer.h"
#include <vector>

//Declears
struct TokenPair {
  std::string token;
  std::string ID;
};

class TokenParser {

public :
  //Constructor
  explicit TokenParser(std::string filePath);

private :
  //Private Vars
  Buffer charBuffer;
  std::string KEYS[50] = {
    "int", "if", "while", "do", "float",
    "double", "short", "long", "unsigned",
    "char", "return", "class", "struct",
    "enum", "static", "const", "signed",
    "sizeof", "else", "switch", "case",
    "default", "for", "break", "goto",
    "continue", "auto", "void", "null",
    "nullptr", "typedef"
  };
  std::string OP[50] = {
    "+", "-", "*", "/", "++", "--", "=",
    "+=", "-=", "*=", "/=", "%", "%=",
    "<<", ">>", "<<=", ">>=", "||", "&&",
    "!", "|", "&", "==", "!=", "~", "^",
    "^=", "<", ">", "<=", ">=", "\?",
    ":", ".", "->", "::", "|=", "&="
  };
  std::string DELIMITER[50] = {
    ";", "(", ")", "{", "}", "[", "]", ","
  };
  int whichLine;
  int whichChar;
  int curState;
  char nowCh;
  std::string curLexeme;
  std::vector<TokenPair> tokenSaver;
  //Private methods
  void throwErr();
  bool isDigit(char tmp);
  bool isAlpha(char tmp);
  bool isUnderLine(char tmp);
  void getNextChar();
  bool isKeys(std::string token);
  bool isOps(std::string token);
  bool isDelimiters(char token);
  bool isOpsForChar(char tmp);

public :
  //Public methods
  void LexemeAnalyze();
  void outputResult();
};
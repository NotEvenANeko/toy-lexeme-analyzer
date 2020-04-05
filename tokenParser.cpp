#include "tokenParser.h"

TokenParser::TokenParser(std::string filePath) {
  charBuffer.changeFilePath(filePath);
  whichChar = 0;
  whichLine = 0;
  curState = 1;
  nowCh = '\0';
}

void TokenParser::throwErr() {
  std::cerr << "Unidentfied Lexeme Unit at " << whichLine << ":" << whichChar << "," << nowCh;
  exit(0);
}

bool TokenParser::isDigit(char tmp) {
  return tmp <= '9' && tmp >= '0';
}

bool TokenParser::isAlpha(char tmp) {
  return ('a' <= tmp && tmp <= 'z') || ('A' <= tmp && tmp <= 'Z');
}

bool TokenParser::isUnderLine(char tmp) {
  return tmp == '_';
}

void TokenParser::getNextChar() {
  nowCh = charBuffer.cur();
  while (nowCh == '\t' || nowCh == '\r') {
    if (nowCh == '\t') {
      whichChar += 2;
    }
    charBuffer.next();
    nowCh = charBuffer.cur();
  }
}

bool TokenParser::isKeys(std::string token) {
  for (auto tmp : KEYS) {
    if (tmp == token) return true;
  }
  return false;
}

bool TokenParser::isOps(std::string token) {
  for (auto tmp : OP) {
    if (tmp == token) return true;
  }
  return false;
}

bool TokenParser::isDelimiters(char token) {
  for (auto tmp : DELIMITER) {
    if (tmp[0] == token) return true;
  }
  return false;
}

bool TokenParser::isOpsForChar(char tmp) {
  for (auto op : OP) {
    if (tmp == op[0]) return true;
  }
  return false;
}

void TokenParser::LexemeAnalyze() {
  while (!charBuffer.isEnd) {
    getNextChar();
    switch (curState) {
    case 1:
      if (nowCh == '/') {
        curState = 2;
      }
      else if (isAlpha(nowCh) || isUnderLine(nowCh)) {
        curState = 6;
      }
      else if (isDigit(nowCh)) {
        curState = 11;
      }
      else if (isOpsForChar(nowCh)) {
        curState = 8;
      }
      else if (isDelimiters(nowCh)) {
        curState = 20;
        break;
      }
      else if (nowCh == ' ') {
        charBuffer.next();
        charBuffer.confirmLexeme();
        whichChar++;
        curState = 1;
        break;
      }
      else if (nowCh == '\n') {
        charBuffer.next();
        charBuffer.confirmLexeme();
        whichLine++;
        whichChar = 0;
        curState = 1;
        break;
      }
      else if (nowCh == '\"') {
        curState = 21;
      }
      else if (nowCh == '\'') {
        curState = 24;
      }
      else {
        curState = 1;
      }
      charBuffer.next();
      break;
    case 2:
      if (nowCh == '/') {
        curState = 3;
      }
      else if (nowCh == '*') {
        curState = 4;
      }
      else {
        curState = 8;
      }
      charBuffer.next();
      break;
    case 3:
      if (nowCh == '\n') {
        curState = 1;
      }
      else {
        curState = 3;
      }
      charBuffer.next();
      break;
    case 4:
      if (nowCh == '*') {
        curState = 5;
      }
      else {
        curState = 4;
      }
      charBuffer.next();
      break;
    case 5:
      if (nowCh == '/') {
        curState = 1;
      }
      else {
        curState = 4;
      }
      charBuffer.next();
      break;
    case 6:
      if (isDigit(nowCh) || isAlpha(nowCh) || isUnderLine(nowCh)) {
        curState = 6;
      }
      else {
        curState = 7;
        break;
      }
      charBuffer.next();
      break;
    case 7:
      curLexeme = charBuffer.getLexeme();
      if (isKeys(curLexeme)) {
        tokenSaver.push_back({ curLexeme, "keys" });
      }
      else {
        tokenSaver.push_back({ curLexeme, "id" });
      }
      charBuffer.confirmLexeme();
      curState = 1;
      break;
    case 8:
      curLexeme = charBuffer.getLexeme();
      if (isOps(curLexeme)) {
        charBuffer.next();
        if (!isOps(charBuffer.getLexeme())) charBuffer.pre();
        curState = 9;
      }
      else {
        curState = 9;
      }
      break;
    case 9:
      curLexeme = charBuffer.getLexeme();
      tokenSaver.push_back({ curLexeme, "ops" });
      charBuffer.confirmLexeme();
      curState = 1;
      break;
    case 11:
      if (isDigit(nowCh)) {
        curState = 11;
      }
      else if (nowCh == '.') {
        curState = 14;
      }
      else if (nowCh == 'e' || nowCh == 'E') {
        curState = 17;
        break;
      }
      else if (isDelimiters(nowCh)) {
        curState = 15;
        break;
      }
      else {
        curState = 13;
        break;
      }
      charBuffer.next();
      break;
    case 13:
      throwErr();
      break;
    case 14:
      if (isDelimiters(nowCh)) {
        curState = 15;
        break;
      }
      else if (isDigit(nowCh)) {
        curState = 14;
      }
      else {
        curState = 16;
        break;
      }
      charBuffer.next();
      break;
    case 15:
      curLexeme = charBuffer.getLexeme();
      tokenSaver.push_back({ curLexeme, "number" });
      charBuffer.confirmLexeme();
      curState = 1;
      break;
    case 16:
      throwErr();
      break;
    case 17:
      if (isDigit(nowCh)) {
        curState = 17;
      }
      else if (nowCh == '.') {
        curState = 14;
      }
      else if (isDelimiters(nowCh)) {
        curState = 15;
        break;
      }
      else {
        curState = 19;
        break;
      }
      charBuffer.next();
      break;
    case 19:
      throwErr();
      break;
    case 20:
      charBuffer.next();
      curLexeme = charBuffer.getLexeme();
      tokenSaver.push_back({ curLexeme, "delimiter" });
      charBuffer.confirmLexeme();
      curState = 1;
      break;
    case 21:
      if (nowCh == '\\') {
        curState = 22;
      }
      else if (nowCh == '\"') {
        curState = 23;
        break;
      }
      else {
        curState = 21;
      }
      charBuffer.next();
      break;
    case 22:
      curState = 21;
      charBuffer.next();
      break;
    case 23:
      charBuffer.next();
      curLexeme = charBuffer.getLexeme();
      tokenSaver.push_back({ curLexeme, "string" });
      charBuffer.confirmLexeme();
      curState = 1;
      break;
    case 24:
      if (nowCh == '\\') {
        curState = 25;
      }
      else {
        curState = 26;
      }
      charBuffer.next();
      break;
    case 25:
      curState = 26;
      charBuffer.next();
      break;
    case 26:
      if (nowCh == '\'') {
        curState = 27;
        break;
      }
      else {
        throwErr();
      }
      break;
    case 27:
      charBuffer.next();
      curLexeme = charBuffer.getLexeme();
      tokenSaver.push_back({ curLexeme, "char" });
      charBuffer.confirmLexeme();
      curState = 1;
      break;
    default:
      throwErr();
    }
  }
}

void TokenParser::outputResult() {
  for (auto item : tokenSaver) {
    std::cout << "< " << item.token << " ,  " << item.ID << " >\n";
  }
}

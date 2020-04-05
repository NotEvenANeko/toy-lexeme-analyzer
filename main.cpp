#include "tokenParser.h"

int main() {
  TokenParser test("./source code");
  test.LexemeAnalyze();
  test.outputResult();
  system("pause");
  return 0;
}
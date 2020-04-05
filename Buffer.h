#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cstring>

//Two buffer pair
class Buffer {

public :
  //Constructor & Destructor
  explicit Buffer(std::string filePath);
  explicit Buffer();
  ~Buffer() noexcept;

private :
  //Private Vars
  static const int SIZE = 64; //Size of buffer
  std::string fileStr;  //File path
  std::ifstream fileStream;
  enum class Tag { First, Second }; //Number of current buffer
  Buffer::Tag activitiedBuffer = Tag::First;  //Number of activitied buffer
  char buffer1[SIZE]; //First buffer
  char buffer2[SIZE]; //Second buffer
  char* lexemeBegin = nullptr;  //Start of lexeme
  char* forward = nullptr;  //End of lexeme

public :
  //Public methods
  /*
   * @return Get lexeme between lexemeBegin and forward
   */
  std::string getLexeme();

  /*
   * forward +1
   * @return Get current char
   */
  char next();

  /*
   * @return Get current char
   */
  char cur();

  /*
   * forward -1
   * @return Get current char
   */
  char pre();

  /*
   * Change lexemeBegin if string is a lexeme
   */
  void confirmLexeme();

  /*
   * Change file path
   */
  void changeFilePath(std::string filePath);

  //Public Vars
  bool isEnd = false;

private :
  //Private methods
  /*
   * Fill buffer with chars in filestream
   */
  void read();

};
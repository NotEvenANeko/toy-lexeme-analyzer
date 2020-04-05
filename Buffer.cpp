#include "Buffer.h"

Buffer::Buffer(std::string filePath) {
  fileStr = filePath;

  fileStream.open(fileStr); //Open fstream
  buffer1[SIZE - 1] = EOF;  //Set sentinel
  fileStream.read(buffer1, SIZE - 1); //Read chars

  lexemeBegin = forward = &buffer1[0];  //Get memory address
}

Buffer::Buffer() {
  buffer1[0] = EOF;
  buffer2[0] = EOF;
}

Buffer::~Buffer() noexcept {
  if (fileStream) {
    fileStream.close();
  }
}

std::string Buffer::getLexeme() {
  
  char* current = lexemeBegin; 

  std::stringstream ss;

  while (current != forward) {

    if (*current == EOF) {  //If current is EOF, transfer buffer
      if (activitiedBuffer == Tag::First) {
        current = &buffer1[0];
      }
      else if (activitiedBuffer == Tag::Second) {
        current = &buffer2[0];
      }
    }

    ss << *current;
    if (current == forward) break;
    current++;

  }

  return ss.str();
}

char Buffer::next() {
  char tmp = *forward;
  forward++;

  if (tmp == '\0') {  //'\0' means out of original code or not load yet, end the task
    isEnd = true;
    return '\0';
  }

  if (*forward == EOF && (forward == &buffer1[SIZE - 1] || forward == &buffer2[SIZE - 1])) { //EOF means current buffer has been scanned, load new buffer
    read();
  }
  else if (*forward == EOF && forward != &buffer1[SIZE - 1] && forward != &buffer2[SIZE - 1]) {
    isEnd = true;
  }

  return *forward;
}

char Buffer::cur() {
  return *forward;
}

char Buffer::pre() {
  char tmp = *forward;

  if (forward == &buffer2[0]) { //If forward is at the beginning of the buffer2, which means buffer1 has been loaded and scanned, so go back to the end of buffer1
    forward = &buffer1[SIZE - 1];
    return tmp;
  }
  else if (forward == &buffer1[0] && buffer2[0] != '\0') {  //If forward is at the beginning of the buffer1, which means either buffer2 has been scanned or not loaded yet
    forward = &buffer2[0];
    return tmp;
  }
  else if (forward == &buffer1[0] && buffer2[0] == '\0') {
    return tmp;
  }

  forward--;
  return tmp;

}

void Buffer::confirmLexeme() {
  lexemeBegin = forward;
}

void Buffer::changeFilePath(std::string filePath) {
  fileStr = filePath;

  fileStream.open(fileStr, std::ios::binary); //Open fstream
  buffer1[SIZE - 1] = EOF;  //Set sentinel
  fileStream.read(buffer1, SIZE - 1); //Read chars

  lexemeBegin = forward = &buffer1[0];  //Get memory address
}

void Buffer::read() {
  if (!fileStream) return;

  if (activitiedBuffer == Tag::First) {

    memset(buffer2, -1, sizeof(buffer2));
    buffer2[SIZE - 1] = EOF;
    fileStream.read(buffer2, SIZE - 1);
    activitiedBuffer = Tag::Second;
    forward = &buffer2[0];

  }
  else if (activitiedBuffer == Tag::Second) {

    memset(buffer1, -1, sizeof(buffer1));
    buffer1[SIZE - 1] = EOF;
    fileStream.read(buffer1, SIZE - 1);
    activitiedBuffer = Tag::First;
    forward = &buffer1[0];

  }

}

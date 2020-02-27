/**************************************************************************
  File       [main.cpp]
  Synopsis   [Entanglement & fake QFT]
  Author     [Chilsan Zhang]
  Modify     [2020/02/27]
 **************************************************************************/
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <time.h>

using namespace std;

#define DEBUG

void help_message() {
  cout << "usage: generator_name [ -E | -Q ] [ int number ] <output_file>" << endl
    << "-E means entangle, -Q means QFT" << endl;
}

bool myStr2Int(const string& str, int& num) {

#ifdef DEBUG
  cout << "str2int called" << endl;
#endif

  num = 0;
  size_t i = 0;
  int sign = 1;
  if (str[0] == '-') { sign = -1; i = 1; }
  bool valid = false;
  for (; i < str.size(); ++i) {
    if (isdigit(str[i])) {
      num *= 10;
      num += int(str[i] - '0');
      valid = true;
    }
    else return false;
  }
  num *= sign;
  return valid;
}

void genFile(const int& qbits, const int& format, const string& fileName) {

  cout << "generating file..." << endl;

  fstream file;
  file.open(fileName, ios::out);

  if (!file.is_open()) {
    cout << "Maybe file name typo" << endl;
    return;
  }

  // headers
  file << ".numvars " << qbits << endl << ".variables";
  for (int i = 0; i < qbits; ++i) {
    file << " q" << i;
  }
  file << endl << ".constants ";

  if (format == 0) {
    srand(time(NULL));
    for (int i = 0; i < qbits; ++i) {
      file << (rand() % 2);
    }
  }
  else if (format == 1) {
    for (int i = 0; i < qbits; ++i) {
      file << 0;
    }
  }

  file << endl << ".garbage ";
  for (int i = 0; i < qbits; ++i) {
    file << "-";
  }
  file << endl << ".begin" << endl;

  // QFT
  if (format == 0) {
    for (int i = 0; i < qbits; ++i) {
      file << "h1 q" << i << endl;
      for (int j = i + 1; j < qbits; ++j) {
        file << "z2 q" << j << " q" << i << endl;
      }
    }
  }

  // entangle
  else if (format == 1) {
    file << "h1 q0" << endl;
    for (int i = 1; i < qbits; ++i) {
      file << "t2 q0 q" << i << endl;
    }
  }

  file << ".end";

  file.close();
}

int main(int argc, char* argv[]) {

  if (argc == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
    help_message();
    return 0;
  }

  int qbits;
  if (!myStr2Int(argv[2], qbits)) {
    cout << "Can't convert integer" << endl;
    return 0;
  }
  if (qbits == 0) {
    cout << "Please use a number greater than 0";
    return 0;
  }

#ifdef DEBUG
  cout << "running..." << endl;
#endif

  if (strcmp(argv[1],"-Q") == 0)
    genFile(qbits, 0, argv[3]);
  else if (strcmp(argv[1],"-E") == 0)
    genFile(qbits, 1, argv[3]);

#ifdef DEBUG
  cout << "sucess" << endl;
#endif

  return 0;
}


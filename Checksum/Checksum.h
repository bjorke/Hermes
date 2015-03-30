// ensure this library description is only included once
#ifndef Checksum_h
#define Checksum_h
#include <stdio.h>
#include <string.h>
class Checksum{
  public:
    int generate_verhoeff(const char* num);
    int validate_verhoeff(const char*  num);
    Checksum();
  private:

};

#endif

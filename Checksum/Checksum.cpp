#include <string.h>
#include <stdio.h>

static int generate_verhoeff(const char* num)
{
  int c;
  int len;
  c = 0;
  len = strlen(num);

  for(int i = 0; i < len; i++)
    c = verhoeff_d[c][verhoeff_p[((i + 1) % 8)][num[len - i - 1] - '0']];

  return verhoeff_inv[c];
}

//Validates that an entered number is Verhoeff compliant.
//The check digit must be the last one.
static int validate_verhoeff(const char*  num)
{
  int c;
  int len;
  c = 0;
  len = strlen(num);

  for (int i = 0; i < len; i++)
    c = verhoeff_d[c][verhoeff_p[(i % 8)][num[len - i - 1] - '0']];

  return (c == 0);
}

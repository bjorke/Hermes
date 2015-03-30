#include <string.h>
#include <stdio.h>
// The multiplication table
static int verhoeff_d[10][10]  = {
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
  {1, 2, 3, 4, 0, 6, 7, 8, 9, 5},
  {2, 3, 4, 0, 1, 7, 8, 9, 5, 6},
  {3, 4, 0, 1, 2, 8, 9, 5, 6, 7},
  {4, 0, 1, 2, 3, 9, 5, 6, 7, 8},
  {5, 9, 8, 7, 6, 0, 4, 3, 2, 1},
  {6, 5, 9, 8, 7, 1, 0, 4, 3, 2},
  {7, 6, 5, 9, 8, 2, 1, 0, 4, 3},
  {8, 7, 6, 5, 9, 3, 2, 1, 0, 4},
  {9, 8, 7, 6, 5, 4, 3, 2, 1, 0}
};

// The permutation table
static int verhoeff_p[8][10]= {
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
  {1, 5, 7, 6, 2, 8, 3, 0, 9, 4},
  {5, 8, 0, 3, 7, 9, 6, 1, 4, 2},
  {8, 9, 1, 6, 0, 4, 3, 5, 2, 7},
  {9, 4, 5, 3, 1, 2, 6, 8, 7, 0},
  {4, 2, 8, 6, 5, 7, 3, 9, 0, 1},
  {2, 7, 9, 3, 8, 0, 6, 4, 1, 5},
  {7, 0, 4, 6, 9, 1, 3, 2, 5, 8}
};

//The inverse table
static int verhoeff_inv[] = {0, 4, 3, 2, 1, 5, 6, 7, 8, 9};

//For a given number generates a Verhoeff digit
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
int main(int argc, char *argv[])
{
  char str[30] = "12313212111231321211";
  int retVal = generate_verhoeff(str);
  printf("%d\n",retVal);
  char str2[30] = "123132121112313212115";
  printf("%d\n",validate_verhoeff(str2));
  return 0;

};

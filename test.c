#include <stdio.h>
#include <stdlib.h>

int main() {
  /*  unsigned short a = 0xFFFF;
  unsigned short b = 0x7000;
  short c = 0xFFFF;
  short d = 0x7000;

  printf("a - b = %d / %X \n", (a - b), (a - b));
  printf("c - d = %d / %X \n", (c - d), (c - d));
  //  printf("a - b = %d / %X \n", (a - b), (a - b));
  //  printf("c - d = %d / %X \n", (c - d), (c - d));*/
  /*
  unsigned short a = 0xFFFE;
  unsigned short b = 0x6;
  unsigned short c = a * b;
  short d = 0xFFFE;
  short e = 0x6;
  short f = d * e;
  printf("c = %d / %X \n", c, c);
  printf("d = %d / %X \n", d, d);
  printf("e = %d / %X \n", e, e);
  printf("f = %d / %X \n", f, f);
  */
  unsigned short a = 16;
  unsigned short b = 0x10;
  printf("a = %d / %X \n", a, a);
  printf("b = %d / %X \n", b, b);
  return 0;
}

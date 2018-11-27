// gcc -o shella-easy shella-easy.c -no-pie -fno-pic -mpreferred-stack-boundary=3 -m32 -zexecstack
#include <stdio.h>
#include <stdlib.h>

int main()
{
  char s[0x40]; // [sp+0h] [bp-48h]@1
  unsigned int v5; // [sp+40h] [bp-8h]@1

  setvbuf(stdout, 0, 2, 0x14u);
  setvbuf(stdin, 0, 2, 0x14u);
  v5 = 0xCAFEBABE;
  printf("Yeah I'll have a %p with a side of fries thanks\n", &s);
  gets(s);
  if ( v5 != 0xDEADBEEF )
    exit(0);
  return 0;
}

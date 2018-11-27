// gcc -o ehh ehh.c -m32
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int val;

int main(int argc, const char **argv)
{
  char buf[0x20-4]; // [sp+0h] [bp-20h]@1
  int *v5; // [sp+1Ch] [bp-4h]@1

  v5 = &argc;
  setvbuf(stdout, 0, 2, 0x14u);
  setvbuf(stdin, 0, 2, 0x14u);
  printf(">Input interesting text here< %p\n", &val);
  read(0, buf, 0x18u);
  printf(buf);
  if ( val == 0x18 )
    system("/bin/cat ./flag");
  return 0;
}

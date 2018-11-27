// gcc -o canary canary.c -m32 -no-pie -fno-pic -mpreferred-stack-boundary=2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int nextind, devrand;
char *password;
int cans[2];

void initCanary(char *s)
{
  memset(s, 0, 0x28u);
  read(devrand, s + 0x28, 4u);
  *((unsigned int *)s + 11) = nextind;
  cans[nextind] = *((unsigned int *)s + 10);
  nextind++;
}

void checkCanary(int *a1)
{
  if ( *(unsigned int *)(a1 + 10) != cans[*(unsigned int *)(a1 + 11)] )
  {
    puts("---------------------- HEY NO STACK SMASHING! --------------------");
    exit(1);
  }
}

void checkPass(char *s1, char *s2)
{

  if ( !memcmp(s1, s2, 0x20u) )
  {
    puts("*unlocks door*\nYou're cool, c'mon in");
    system("/bin/cat ./flag");
  }
  else
  {
    puts("Yeah right! Scram");
  }
}

void doCanary(char *buf)
{
  initCanary(buf);
  read(0, buf, 0x1A4u);
  checkCanary((int *)buf);
}

void main(int argc, const char **argv, const char **envp)
{
  char s2[0x30]; // [sp+0h] [bp-34h]@1
  int fd; // [sp+30h] [bp-4h]@1

  setvbuf(stdout, 0, 2, 0x14u);
  setvbuf(stdin, 0, 2, 0x14u);
  devrand = open("/dev/urandom", 0);
  password = malloc(0x20u);
  printf("*slides open window*\nPassword? ");
  doCanary(s2);
  fd = open("./password", 0);
  read(fd, password, 0x21u);
  checkPass(password, s2);
}

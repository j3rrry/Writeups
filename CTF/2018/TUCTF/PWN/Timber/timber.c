//  gcc -o timber timber.c -m32 -fstack-protector-all -no-pie -mpreferred-stack-boundary=2
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int devrand;
char *ttype;
int tage;

void date()
{
  system("/bin/cat ./flag");
  return;
}

void getName(void *a1)
{
  printf("Welcome to Timber\nThe world's largest lumberjack dating site\nPlease enter your name: ");
  memset(a1, 0, 0x18u);
  read(0, a1, 0x64u);
  return;
}

void clearStdin()
{
  char v2; // [sp+3h] [bp-9h]@2

  do
    v2 = getchar();
  while ( v2 != '\n' && v2 != -1 );
  return;
}

unsigned int getRand(unsigned int a1)
{
  unsigned int buf; // [sp+4h] [bp-Ch]@1

  buf = 0;
  read(devrand, &buf, 4u);
  buf %= a1;
  return buf;
}

void genMatch()
{
  tage = getRand(5000);
  switch ( getRand(10) )
  {
    case 0:
      ttype = "Sugar Maple";
      break;
    case 1:
      ttype = "Bonsai";
      break;
    case 2:
      ttype = "American Beech";
      break;
    case 3:
      ttype = "American Elm";
      break;
    case 4:
      ttype = "Black Cherry";
      break;
    case 5:
      ttype = "American sycamore";
      break;
    case 6:
      ttype = "White Spruce";
      break;
    case 7:
      ttype = "Eastern Pine";
      break;
    case 8:
      ttype = "Northern Oak";
      break;
    case 9:
      ttype = "Shagbark Hickory";
      break;
    case 0xA:
      ttype = "Swamp Birch";
      break;
    default:
      ttype = "Unkown";
      break;
  }
  return;
}

void doStuff()
{
  char v2; // [sp+3h] [bp-51h]@2
  char format[0x50-0x38]; // [sp+4h] [bp-50h]@1
  char buf[0x30]; // [sp+1Ch] [bp-38h]@5

  getName(format);
  printf("Alright ");
  printf(format);
  puts("Let's find you a match!");
  puts("Options:\n l: Swipe Left\n r: Swipe Right\n s: Super Swipe\n");
  do
  {
    genMatch();
    printf("%s age %d? ", ttype, tage);
    scanf("%c", &v2);
    clearStdin();
  }
  while ( (v2 != 'r' || getRand(10)) && v2 != 's' );
  puts("+Match Found!");
  printf("----------------- %s ----------------\n", ttype);
  printf("[%s] So, are you a tree hugger or what.\n", ttype);
  read(0, buf, 0x64u);
  printf("[%s] Pff, lumberjacks are all the same.\n", ttype);
  return;
}

int main(int argc, const char **argv, const char **envp)
{

  setvbuf(stdin, NULL, _IONBF, 0x14u);
  setvbuf(stdout, NULL, _IONBF, 0x14u);
  devrand = open("/dev/urandom", 0);
  doStuff();
  close(devrand);
  return 0;
}

